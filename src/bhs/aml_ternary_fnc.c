#include "aml.h"

DECLARE_DATA_IDENT(ternary_id,			"ternary");
DECLARE_DATA_FUNC(ternary__fnc,	&ternary_id,	ternary_fnc);
/*
	-----------------------------------------------------------------------
 */
int
ternary_fnc(				/* ternary ( (expr) ? expr : expr ) */
	fncrc_t		*fncrc,		/* function reporting faramework */
	sess_t		*sp,		/* session descriptor */
	comp_t		*cp,		/* compiler descriptor */
	leng_t		 argc,		/* argument count */
	data_link_t	*argv,		/* argument list */
	data_t		**pp		/* result */
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*expr = NULL;
	data_t		*bres = NULL;
	data_t		*arg = NULL;
	data_link_t	*lnk1 = NULL;
	data_link_t	*lnk2 = NULL;
	leng_t		 argc1 = 0ULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	TRY(data_lst_next, _door, sp, argv, &expr);
	TRY(data_cnt_by_lnk, _door, sp, expr, &argc1);
	TRY(data_lst_next, _door, sp, expr, &lnk1);
	TRY(data_lst_next, _door, sp, lnk1, &lnk2);

	TRY(aml_eval, _door, sp, cp, argc1, expr, &bres);

	if(comp_id_cmp(DID(bres), BOOLEAN) == DATA_EQ) {
		if(DBOOL(bres) == TRUE) {
			TRY(data_cnt_by_lnk, _door, sp, lnk1, &argc1);
			TRY(aml_eval, _door, sp, cp, argc1, lnk1, &arg);
		} else {
			TRY(data_cnt_by_lnk, _door, sp, lnk2, &argc1);
			TRY(aml_eval, _door, sp, cp, argc1, lnk2, &arg);
		}
		if(DLL_ISEMPTY(DREF(arg)) == TRUE) {
			dp = arg;
		} else {
			TRY(data_cpy, _door, sp, arg, &dp);
		}
	} else {
		leng_t		 _argc = 0ULL;
		data_t		*_expr = NULL;
		data_link_t	*_argv = NULL;

		TRY(data_link_2child, _door, sp, expr, &_expr);
		TRY(data_lst_cnt, _door, sp, _expr, &_argc);
		TRY(data_lst_first, _door, sp, _expr, &_argv);

		if(COMP_DBG(cp) >= COMP_DATA_DBG) {
			data_prn(NEXT_FNCRC, sp, _expr, sp->sess_err, 0);
		}

		TRY(error_fnc, _door, sp, cp, _argc, _argv, "produce not BOOLEAN", &dp);
	}

_door:
	if(bres != NULL) {
		data_drop(NEXT_FNCRC, sp, bres);
	}
	*pp = dp;
	FNC_RETURN(rc);
}
/*
	-----------------------------------------------------------------------
 */
