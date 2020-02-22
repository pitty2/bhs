#include "aml.h"

DECLARE_DATA_IDENT(div_op_id,		"/");
DECLARE_DATA_FUNC(div_op_fnc,		&div_op_id,		div_fnc);
/*
	-----------------------------------------------------------------------
 */
int
comp_str_split(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p1,	/* String */
	data_t		*p2,	/* Search */
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	char		*string = NULL;
	char		*search = NULL;
	char		*token = NULL;

	data_link_t	*lnk = NULL;
	data_t		*val = NULL;
	data_t		*elm = NULL;
	leng_t		 len = 0ULL;

	
	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p1);
	FNC_CHKINP(rc, p2);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, p1, DATA_STRING_DESIGNATOR);
	CHK_DES(rc, p2, DATA_STRING_DESIGNATOR);

	TRY(data_str_2cstr, _door, sp, p1, &string);
	TRY(data_str_2cstr, _door, sp, p2, &search);

	if((token = strtok(string, search)) != NULL) {

		TRY(data_lst_new, _door, sp, ARRAY, &dp);
		TRY(data_sequ_inherit, _door, sp, p1, dp);

		TRY(data_lst_new, _door, sp, VALUE, &val);
		TRY(data_sequ_inherit, _door, sp, p1, val);

		len = strlen(token);
		TRY(data_str_new, _door, sp, STRING, len, token, &elm);
		TRY(data_lst_ins_last, _door, sp, val, elm, &lnk);
		elm = NULL;
		TRY(data_lst_ins_last, _door, sp, dp, val, &lnk);
		val = NULL;

		while((token = strtok(NULL, search)) != NULL) {
			TRY(data_lst_new, _door, sp, VALUE, &val);
			TRY(data_sequ_inherit, _door, sp, p1, val);
			len = strlen(token);
			TRY(data_str_new, _door, sp, STRING, len,token, &elm);
			TRY(data_lst_ins_last, _door, sp, val, elm, &lnk);
			elm = NULL;
			TRY(data_lst_ins_last, _door, sp, dp, val, &lnk);
			val = NULL;
		}
	}

_door:
	if(string != NULL) {
		FREE(string);
	}
	if(search != NULL) {
		FREE(search);
	}
	if(rc != FNC_OKAY) {
		if(dp != NULL) {
			data_drop(NEXT_FNCRC, sp, dp);
			dp = NULL;
		}
		if(elm != NULL) {
			data_drop(NEXT_FNCRC, sp, elm);
			elm = NULL;
		}
	}
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
div_fnc(				/* divide (/) */
	fncrc_t		*fncrc,		/* function reporting faramework */
	sess_t		*sp,		/* session descriptor */
	comp_t		*cp,		/* compiler descriptor */
	leng_t		 argc,		/* argument count */
	data_link_t	*argv,		/* argument list */
	data_t		**pp		/* result */
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*lnk2 = NULL;
	data_link_t	*lnk1 = NULL;
	data_t		*arg1 = NULL;
	data_t		*arg2 = NULL;
	leng_t		 argc1 = 0ULL;
	leng_t		 argc2 = 0ULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	TRY(data_lst_next,	_door, sp, argv, &lnk1);
	TRY(data_cnt_by_lnk,	_door, sp, lnk1, &argc1);
	TRY(aml_eval,		_door, sp, cp, argc1, lnk1, &arg1);
	TRY(data_lst_next,	_door, sp, lnk1, &lnk2);
	TRY(data_cnt_by_lnk,	_door, sp, lnk2, &argc2);
	TRY(aml_eval,		_door, sp, cp, argc2, lnk2, &arg2);
	if(
		(comp_id_cmp(DID(arg1), NUMERIC) == DATA_EQ) 
	&&	(comp_id_cmp(DID(arg2), NUMERIC) == DATA_EQ)
	) {
		if(DNUM(arg2) == 0LL) {
			TRY(error_fnc, _door, sp, cp, argc, argv, "division by zero not allowed", &dp);
		} else {
			number_t n = 0LL;

			n = (number_t)(DNUM(arg1) / DNUM(arg2));
			TRY(data_num_new, _door, sp, NUMERIC, n, &dp);
		}

	} else if(
		(comp_id_cmp(DID(arg1), STRING) == DATA_EQ) 
	&&	(comp_id_cmp(DID(arg2), STRING) == DATA_EQ)
	) {
		TRY(comp_str_split, _door, sp, arg1, arg2, &dp);
		if(dp == NULL) {
			TRY(error_fnc, _door, sp, cp, argc, argv, "arg1 not containts arg2", &dp);
		} 

	} else {
		TRY(error_fnc, _door, sp, cp, argc, argv, "not allowed", &dp);
	}

_door:
	if(arg1 != NULL) {
		if(DLL_ISEMPTY(&arg1->data_refby)) {
			if(arg1 == arg2) {
				arg2 = NULL;
			}
			data_drop(NEXT_FNCRC, sp, arg1);
		}
	}
	if(arg2 != NULL) {
		if(DLL_ISEMPTY(&arg2->data_refby)) {
			data_drop(NEXT_FNCRC, sp, arg2);
		}
	}
	*pp = dp;
	FNC_RETURN(rc);
}
/*
	-----------------------------------------------------------------------
 */
