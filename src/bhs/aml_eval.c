#include "aml.h"

/*
	Precedence-Tabell der Opeatoren
	------------------------------------------------------------------------
 */
typedef	data_t	*op_t;

/*			hight, ... low */
static op_t prec_0[] = { PTR, NULL };
static op_t prec_1[] = { U_PLUS, U_MINUS, NULL };
static op_t prec_2[] = { L_NOT, NOT, NULL };
static op_t prec_3[] = { DEREF, ADDR, NULL };
static op_t prec_4[] = { MUL, DIV, MOD, NULL };
static op_t prec_5[] = { PLUS, MINUS, NULL };
static op_t prec_6[] = { SHL, SHR, NULL };
static op_t prec_7[] = { LT, LE, GT, GE, EQ, NE, NULL };
static op_t prec_8[] = { AND, XOR, OR, NULL };
static op_t prec_9[] = { L_AND, L_OR, NULL };

op_t	*prec_tbl[] = {
	&prec_0[0],	/* hight */
	&prec_1[0],
	&prec_2[0],
	&prec_3[0],
	&prec_4[0],
	&prec_5[0],
	&prec_6[0],
	&prec_7[0],
	&prec_8[0],
	&prec_9[0],	/* low */
	NULL
};

static int
is_in(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*op,
	op_t		*prec,
	int		*pp
) {
	int		 rc = 0;
	op_t		*p = NULL;
	int		 n = 0;
	

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, op);
	FNC_CHKINP(rc, prec);
	FNC_CHKINOUTP(rc, pp);

	for (p = prec, n = 1; *p != NULL; p++, n++) {
		if(comp_id_cmp(DID(op), *p) == DATA_EQ) {
			goto _door;
		}
	}	
	n = 0;
_door:
	*pp = n;
	FNC_RETURN(rc);
}

int
aml_reduce_prec(
	fncrc_t	*fncrc,
	sess_t	*sp,
	comp_t	*cp,
	data_t	*expr
) {
	int		 rc = FNC_OKAY;
	op_t		**pp = NULL;
	leng_t		 cnt = 0L;
	data_link_t	*lnk = NULL;
	data_t		*op = NULL;

	data_t		*sub = NULL;
	data_t		*tp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, expr);

	if(COMP_DBG(cp) >= COMP_SHORT_DBG) {
		DBGOUT(sp->sess_err, "enter(expr=0x%p)", expr);
	}
	if(COMP_DBG(cp) >= COMP_PARAM_DBG) {
		DBGOUT(sp->sess_err, "DID(%s(0x%p))", TSTR(DCSTR(DID(expr))), expr);
	}
	if(COMP_DBG(cp) >= COMP_DATA_DBG) {
		data_prn(NEXT_FNCRC, sp, expr, sp->sess_err, 0);
	}

	if (
		(comp_id_cmp(DID(expr), UNARY_EXPR) == DATA_EQ)
	) {
		goto FNC_EXIT;
	}
	if (
		(comp_id_cmp(DID(expr), EXPR) != DATA_EQ)
	&&	(comp_id_cmp(DID(expr), SUB_EXPR) != DATA_EQ)
	&&	(comp_id_cmp(DID(expr), BOOL_EXPR) != DATA_EQ)
	) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_IDENTIFIER);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "EXPR, SUB_EXPR or BOOL_EXPR expected!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto FNC_EXIT;
	}
	for(pp = &prec_tbl[0]; *pp != NULL; pp++) {
NEXT_PREC:
		TRY(data_lst_cnt, FNC_RET, sp, expr, &cnt);
		if(cnt <= 3) {
			break;
		}
		DLL_WALK(lnk, data_link_t, DLST(expr)) {
			data_t		*e = NULL;
			data_link_t	*ign = NULL;
			int		 cnt = 0;

			TRY(data_link_2child, FNC_RET, sp, lnk, &e);
			if(
				(comp_id_cmp(DID(e), EXPR_OP) == DATA_EQ)
			||	(comp_id_cmp(DID(e), BOOL_OP) == DATA_EQ)
			) {
				data_link_t	*left = NULL;
				data_link_t	*right = NULL;

				TRY(data_lst_prev,  FNC_RET, sp, lnk, &left);
				TRY(data_lst_next,  FNC_RET, sp, lnk, &right);
				TRY(data_get_first, FNC_RET, sp, e, &op);
				TRY(is_in,          FNC_RET, sp, op, *pp, &cnt);
				if(cnt != 0) {
					TRY(data_lst_new,      FNC_RET, sp, SUB_EXPR, &sub);
					TRY(data_lst_ins_prev, FNC_EXIT, sp, left, sub, &ign);
					TRY(data_lst_remove,   FNC_RET, sp, lnk, &tp);
					TRY(data_lst_ins_last, FNC_EXIT, sp, sub, tp, &ign);
					tp = NULL;
					TRY(data_lst_remove,   FNC_RET, sp, left, &tp);
					TRY(data_lst_ins_last, FNC_EXIT, sp, sub, tp, &ign);
					tp = NULL;
					TRY(data_lst_remove,   FNC_RET, sp, right, &tp);
					TRY(data_lst_ins_last, FNC_EXIT, sp, sub, tp, &ign);
					tp = NULL;
					sub = NULL;

					goto NEXT_PREC;
				}
			}
		}
	}

	TRY(data_lst_cnt, FNC_EXIT, sp, expr, &cnt);
	if(cnt == 3) {
		data_link_t	*ign = NULL;

		TRY(data_lst_nth,     FNC_RET, sp, expr, 2, &lnk);
		TRY(data_link_2child, FNC_RET, sp, lnk, &op);
		if(
			(comp_id_cmp(DID(op), EXPR_OP) == DATA_EQ)
		||	(comp_id_cmp(DID(op), BOOL_OP) == DATA_EQ)
		) {
			TRY(data_lst_remove,    FNC_RET, sp, lnk, &op);
			TRY(data_lst_ins_first, FNC_EXIT, sp, expr, op, &ign);
			op = NULL;
		}
	}
	
FNC_EXIT:
	if(op != NULL) {
		data_drop(NEXT_FNCRC, sp, op);
	}
	if(sub != NULL) {
		data_drop(NEXT_FNCRC, sp, sub);
	}
	if(tp != NULL) {
		data_drop(NEXT_FNCRC, sp, tp);
	}
	if(COMP_DBG(cp) >= COMP_DATA_DBG) {
		data_prn(NEXT_FNCRC, sp, expr, sp->sess_err, 0);
	}
	if(COMP_DBG(cp) >= COMP_PARAM_DBG) {
		DBGOUT(sp->sess_err, "DID(expr) = %s", TSTR(DCSTR(DID(expr))));
	}
	if(COMP_DBG(cp) >= COMP_SHORT_DBG) {
		DBGOUT(sp->sess_err, "return(%d)", rc);
	}
FNC_RET:
	FNC_RETURN(rc);
}

/*
	--------------------------------------------------------------------------
 */
int
aml_eval(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	leng_t		 argc,
	data_link_t	*argv,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_t		*p = NULL;

	data_circ_t	*ci = NULL;
	data_link_t	*ign = NULL;
	data_link_t	*_lnk = NULL;
	int		 _rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2child,	_door, sp, argv, &dp);
	TRY(comp_chk_id,	_door, sp, dp, ANY);

	if(COMP_DBG(cp) >= COMP_PARAM_DBG) {
		DBGOUT(sp->sess_err, "DID(%s(0x%p))", DCSTR(DID(dp)), dp);
	}
	if(COMP_DBG(cp) >= COMP_DATA_DBG) {
		data_prn(NEXT_FNCRC, sp, dp, sp->sess_err, 0);
	}

/* Circuit - Check */
	TRY(data_stack_lookup, _door, sp, SESS_STACK(sp), dp, &ci);
	if(ci != NULL) {
		FREE(ci);
		ci = NULL;
		if(COMP_DBG(cp) >= COMP_CIRC_DBG) {
			DBGOUT(sp->sess_err, "Circuit detected, cancel recursion!");
			data_circ_prn(fncrc, sp, ci, sp->sess_err, 0);
		}
		p = dp;
		goto _door;
	}
	TRY(data_stack_push, _door, sp, SESS_STACK_PTR(sp), argv);

	if(comp_id_cmp(DID(dp), INCLUDE) == DATA_EQ) {
		p = dp;

	} else if(comp_id_cmp(DID(dp), ASSIGN) == DATA_EQ) {
		leng_t	 _argc = 0ULL;

		TRY(datalnk_by_ident, _pop, sp, dp, VALUE, &_lnk);
		TRY(data_cnt_by_lnk,  _pop, sp, _lnk, &_argc);
		TRY(aml_eval,         _pop, sp, cp, _argc, _lnk, &p);

	} else if(comp_id_cmp(DID(dp), VALUE) == DATA_EQ) {
		data_t	*rdp = NULL;
		leng_t	 _argc = 0ULL;

		TRY(data_lst_first,  _pop, sp, dp, &_lnk);
		TRY(data_cnt_by_lnk, _pop, sp, _lnk, &_argc);
		TRY(aml_eval,        _pop, sp, cp, _argc, _lnk, &p);
		if(DLL_ISEMPTY(DREF(p)) == TRUE) {
		/*
		 *	Wenn Ergebnis da, und noch keinem Value
		 *	zugeordnet, dann aktuellem Value zuordnen.
		 */
			if(comp_id_cmp(DID(p), ERROR) != DATA_EQ) {
				TRY(data_lst_remove, _pop, sp, _lnk, &rdp);
				TRY(data_drop,       _pop, sp, rdp);
			}
			TRY(data_lst_ins_first, _pop, sp, dp, p, &ign);
			ign = NULL;
		}

	} else if(comp_id_cmp(DID(dp), BUILTIN) == DATA_EQ) {
		data_link_t	*argv = NULL;
		data_t		*fnc = NULL;
		leng_t		 argc = 0ULL;

		TRY(data_lst_cnt,   _pop, sp, dp, &argc);
		TRY(data_lst_first, _pop, sp, dp, &argv);
		TRY(data_get_first, _pop, sp, dp, &fnc);

		if(DES(fnc) != DATA_FUNC_DESIGNATOR) {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_DESIGNATOR);
			FNCRC_SYSERR(0);
			FNC_TXT(fncrc, "DATA_FUNC_DESIGNATOR expected!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _pop;
		}
		TRY(DFUNC(fnc), _pop, sp, cp, argc, argv, &p);

	} else if(comp_id_cmp(DID(dp), REFERENCE) == DATA_EQ) {
		data_link_t	*vlnk = NULL;
		leng_t		 _argc = 0ULL;

		TRY(comp_resref, _pop, sp, cp, argv, &vlnk);
		TRY(data_cnt_by_lnk, _pop, sp, vlnk, &_argc);
		TRY(aml_eval, _pop, sp, cp, _argc, vlnk, &p);

	} else if(comp_id_cmp(DID(dp), STRUCT) == DATA_EQ) {
		p = dp;
		DLL_WALK(_lnk, data_link_t, DLST(dp)) {
			leng_t	 _argc = 0ULL;
			data_t	*xx = NULL;

			TRY(data_cnt_by_lnk, _pop, sp, _lnk, &_argc);
			TRY(aml_eval,        _pop, sp, cp, _argc, _lnk, &xx);
			xx = NULL;
		}

	} else if(comp_id_cmp(DID(dp), ARRAY) == DATA_EQ) {
		p = dp;
		DLL_WALK(_lnk, data_link_t, DLST(dp)) {
			leng_t	 _argc = 0ULL;
			data_t	*xx = NULL;

			TRY(data_cnt_by_lnk, _pop, sp, _lnk, &_argc);
			TRY(aml_eval,        _pop, sp, cp, _argc, _lnk, &xx);
			xx = NULL;
		}

	} else if(
		(comp_id_cmp(DID(dp), EXPR) == DATA_EQ)
	||	(comp_id_cmp(DID(dp), SUB_EXPR) == DATA_EQ)
	||	(comp_id_cmp(DID(dp), BOOL_EXPR) == DATA_EQ)
	||	(comp_id_cmp(DID(dp), UNARY_EXPR) == DATA_EQ)
	) {
		data_link_t	*argv = NULL;
		data_t		*op = NULL;
		data_t		*fnc = NULL;
		leng_t		 argc = 0ULL;

		TRY(aml_reduce_prec,  _pop, sp, cp, dp);
		TRY(data_lst_cnt,     _pop, sp, dp, &argc);
		TRY(data_lst_first,   _pop, sp, dp, &argv);
		TRY(data_link_2child, _pop, sp, argv, &op);
		TRY(data_get_first,   _pop, sp, op, &fnc);

		if(DES(fnc) != DATA_FUNC_DESIGNATOR) {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_DESIGNATOR);
			FNCRC_SYSERR(0);
			FNC_TXT(fncrc, "DATA_FUNC_DESIGNATOR expected!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _pop;
		}
		TRY(DFUNC(fnc), _pop, sp, cp, argc, argv, &p);

	} else if(comp_id_cmp(DID(dp), TERNARY) == DATA_EQ) {
		data_link_t	*argv = NULL;
		data_t		*fnc = NULL;
		leng_t		 argc = 0ULL;

		TRY(data_lst_cnt,     _pop, sp, dp, &argc);
		TRY(data_lst_first,   _pop, sp, dp, &argv);
		TRY(data_link_2child, _pop, sp, argv, &fnc);

		if(DES(fnc) != DATA_FUNC_DESIGNATOR) {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_DESIGNATOR);
			FNCRC_SYSERR(0);
			FNC_TXT(fncrc, "DATA_FUNC_DESIGNATOR expected!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _pop;
		}
		TRY(DFUNC(fnc), _pop, sp, cp, argc, argv, &p);

	} else if(comp_id_cmp(DID(dp), NUMBER) == DATA_EQ) {
		TRY(data_by_ident, _pop, sp, dp, NUMERIC, &p);

	} else if(
		(comp_id_cmp(DID(dp), STRING) == DATA_EQ) 
	||	(comp_id_cmp(DID(dp), NUMERIC) == DATA_EQ) 
	||	(comp_id_cmp(DID(dp), BOOLEAN) == DATA_EQ) 
	) {
		p = dp;

	} else if(comp_id_cmp(DID(dp), ERROR) == DATA_EQ) {
		p = dp;

	} else {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_HIRARCHIE);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "unable to evaluate \"%s\"!", TSTR(DCSTR(DID(dp))));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		p = NULL;
	}

_pop:
	TRY(data_stack_pop, _door, sp, SESS_STACK_PTR(sp), &ign);
	ign = NULL;
_door:
	if(_rc != FNC_OKAY) {
		rc = _rc;
	}
	if(rc != FNC_OKAY) {
		if(p != NULL) {
			data_drop(NEXT_FNCRC, sp, p);
			p = NULL;
		}
	}
	if(COMP_DBG(cp) >= COMP_DATA_DBG) {
		if(p != NULL) {
			data_prn(NEXT_FNCRC, sp, p, sp->sess_err, 0);
		}
	}
	if(COMP_DBG(cp) >= COMP_PARAM_DBG) {
		if(p != NULL) {
			DBGOUT(sp->sess_err, "DID(%s(0x%p))", TSTR(DCSTR(DID(p))), p);
		}
	}
	*pp = p;
	FNC_RETURN(rc);
}

int
aml_gen(
	fncrc_t	*fncrc,
	sess_t	*sp,
	comp_t	*cp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*argv = NULL;
	data_t		*lst = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);

	lst = COMP_PTREE(cp);

	DLL_WALK(argv, data_link_t, DLST(lst)) {
		leng_t	 argc = 0ULL;
		data_t	*p = NULL;
	
		TRY(data_cnt_by_lnk, _door, sp, argv, &argc);
		TRY(aml_eval, _door, sp, cp, argc, argv, &p);
		p = NULL;
	}

_door:
	FNC_RETURN(rc);
}
