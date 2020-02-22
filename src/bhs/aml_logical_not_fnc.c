#include "aml.h"

DECLARE_DATA_IDENT(logical_not_op_id,	"!");
DECLARE_DATA_FUNC(logical_not_op_fnc,	&logical_not_op_id,	logical_not_fnc);
/*
	-----------------------------------------------------------------------
 */
int
logical_not_fnc(			/* (!) */
	fncrc_t		*fncrc,		/* function reporting faramework */
	sess_t		*sp,		/* session descriptor */
	comp_t		*cp,		/* compiler descriptor */
	leng_t		 argc,		/* argument count */
	data_link_t	*argv,		/* argument list */
	data_t		**pp		/* result */
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*lnk1 = NULL;
	data_t		*arg1 = NULL;
	leng_t		 argc1 = 0ULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	TRY(data_lst_next,	_door, sp, argv, &lnk1);
	TRY(data_cnt_by_lnk,	_door, sp, lnk1, &argc1);
	TRY(aml_eval,		_door, sp, cp, argc1, lnk1, &arg1);

	if(comp_id_cmp(DID(arg1), BOOLEAN) == DATA_EQ) {
		bool_t	b;

		b = (DBOOL(arg1) == TRUE) ? FALSE : TRUE;
		TRY(data_bool_new, _door, sp, BOOLEAN, b, &dp);
	} else {
		TRY(error_fnc, _door, sp, cp, argc, argv, "not allowed!", &dp);
	}
_door:
	if(arg1 != NULL) {
		if(DLL_ISEMPTY(&arg1->data_refby)) {
			data_drop(NEXT_FNCRC, sp, arg1);
		}
	}
	*pp = dp;
	FNC_RETURN(rc);
}
/*
	-----------------------------------------------------------------------
 */
