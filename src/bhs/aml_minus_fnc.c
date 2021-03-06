#include "aml.h"

DECLARE_DATA_IDENT(minus_op_id,		"-");
DECLARE_DATA_FUNC(minus_op_fnc,		&minus_op_id,		minus_fnc);
/*
	-----------------------------------------------------------------------
 */
int
minus_fnc(				/* minus (-) */
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
		number_t n = 0LL;

		n =  (number_t)(DNUM(arg1) - DNUM(arg2));
		TRY(data_num_new, _door, sp, NUMERIC, n, &dp);

	} else if(
		(comp_id_cmp(DID(arg1), STRING) == DATA_EQ) 
	&&	(comp_id_cmp(DID(arg2), NUMERIC) == DATA_EQ)
	) {
		leng_t	 n = 0LL;
		leng_t	 len = 0LL;

		TRY(data_str_len, _door, sp, arg1, &n);
		if(n <= DNUM(arg2)) {
			TRY(error_fnc, _door, sp, cp, argc, argv, "produce zero-lenght string", &dp);
			goto _door;
		}

		len = (n - DNUM(arg2));
		TRY(data_strn_cpy, _door, sp, arg1, len, &dp);
		
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
