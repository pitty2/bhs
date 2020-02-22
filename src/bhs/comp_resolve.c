#include <bhs/compile.h>

/*
	-----------------------------------------------------------------------
 */
int
comp_lookup_symtree(
	fncrc_t	*fncrc,
	sess_t	*sp,
	comp_t	*cp,
	data_t	*nn,
	data_t	**pp
) {
	int	 rc = FNC_OKAY;
	data_t	*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, nn);	/* normalisierter name */
	FNC_CHKOUTP(rc, pp);
	
	TRY(data_lookup, FNC_RET, sp, COMP_SYMTREE(cp), nn, &dp);
	*pp = dp;

FNC_RET:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
comp_ins_symtree(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_t		*name,		/* NAME */
	data_t		*nn		/* NORMALIZED_NAME */
) {
	int		 rc = FNC_OKAY;
	data_t		*en = NULL;
	data_link_t	*ign = NULL;
	data_t		*err = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, name);
	FNC_CHKINP(rc, nn);

	TRY(comp_chk_id, _door, sp, name, NAME);
	TRY(comp_chk_id, _door, sp, nn, NORMALIZED);
	TRY(comp_lookup_symtree, _door, sp, cp, nn, &en);
	if(en != NULL) { /* name already stored in nametree ? */
		TRY(comp_name_error, _door, sp, cp, en, nn, &err);
		TRY(data_lst_ins_first, _door, sp, name, err, &ign);
	} else {
		data_t	*stree = COMP_SYMTREE(cp);

		TRY(data_lst_ins_last, _door, sp, stree, nn, &ign);
		TRY(data_lst_ins_first, _door, sp, name, nn, &ign);
	}
_door:
	if(rc != FNC_OKAY) {
		if(err != NULL) {
			data_drop(NEXT_FNCRC, sp, err);
		}
	}
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
comp_symbol_from_name(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*dp,		/* name */
	data_t	**pp
) {
	int	 rc = FNC_OKAY;
	data_t	*sym = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, FNC_RET, sp, dp, NAME);
	TRY(data_by_ident, FNC_RET, sp, dp, SYMBOL, &sym);
	if(sym == NULL) {
		TRY(data_by_ident, FNC_RET, sp, dp, OBJ_IDENTIFIER, &sym);
		if(sym == NULL) {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_IDENTIFIER);
			FNCRC_SYSERR(0);
			FNC_TXT(fncrc, "No SYMBOL or OBJ_IDENTIFIER found in name!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		}
	}

	*pp = sym;

FNC_RET:
	FNC_RETURN(rc);
}

int
comp_normalize_name(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,		/* name */
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*nn = NULL;	/* normalized name */
	data_t		*asgn = NULL;
	data_t		*struc = NULL;
	data_t		*val = NULL;
	data_t		*name = NULL;
	data_t		*p = NULL;
	data_t		*sym = NULL;
	data_link_t	*ign = NULL;
	leng_t		 cnt = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, FNC_RET, sp, dp, NAME);
	TRY(data_lst_new, FNC_RET, sp, NORMALIZED, &nn);
	TRY(data_sequ_inherit, _door, sp, dp, nn);

	p = dp;
	while(rc == FNC_OKAY) {
		TRY(data_parent_by_ident, _door, sp, p, ASSIGN, &asgn);
		if(asgn == NULL) {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_HIRARCHIE);
			FNCRC_SYSERR(0);
			FNC_TXT(fncrc, "No ASSIGN found for NAME!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _door;
		}
		TRY(data_by_ident, _door, sp, asgn, NAME, &name);
		if(name == NULL) {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_IDENTIFIER);
			FNCRC_SYSERR(0);
			FNC_TXT(fncrc, "No NAME found in ASSIGN!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _door;
		}
		TRY(comp_symbol_from_name, _door, sp, name, &sym);
		TRY(data_lst_ins_first, _door, sp, nn, sym, &ign);
		TRY(data_parent_by_ident, _door, sp, asgn, STRUCT, &struc);
		if(struc == NULL) {
			break;
		}
		TRY(data_parent_by_ident, _door, sp, struc, VALUE, &val);
		if(val == NULL) {
			break;
		}
		p = val;
	}
	if(rc != FNC_OKAY) {
		goto _door;
	}

	TRY(data_lst_cnt, _door, sp, nn, &cnt);
	if(cnt == 0LL) {
		data_drop(NEXT_FNCRC, sp, nn);
		nn = NULL;
	} 

_door:
	if(rc != FNC_OKAY) {
		if(nn != NULL) {
			data_drop(NEXT_FNCRC, sp, nn);
			nn = NULL;
		}
	}
	*pp = nn;

FNC_RET:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
_comp_setup_symtree(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_t		*dp		/* actual parser tree segment */
) {
	int		 rc = FNC_OKAY;
	data_t		*name = NULL;	/* name */
	data_t		*nn = NULL;	/* normalized name */

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, dp);

	TRY(comp_chk_id, FNC_RET, sp, dp, ANY);

	if(comp_id_cmp(DID(dp), NAME) == DATA_EQ) {
		name = dp ;

		TRY(comp_normalize_name, FNC_RET, sp, name, &nn);
		if(nn == NULL) { /* NAME not in ASSIGN */
			goto _door;
		}
		TRY(comp_ins_symtree, _next1, sp, cp, name, nn);
_next1:
		if(rc != FNC_OKAY) {
			data_drop(NEXT_FNCRC, sp, nn);
			nn = NULL;
			goto _door;
		}

	} else if(DES(dp) == DATA_LIST_DESIGNATOR) {
		data_link_t	*lnk = NULL;
		data_circ_t	*ci = NULL;
		int		 _rc = FNC_OKAY;

		TRY(data_stack_lookup, FNC_RET, sp, SESS_STACK(sp), dp, &ci);
		if(ci != NULL) {	// Zykus gefunden ?
			PRNW(sp->sess_err, "%s: cycle detected, recursion canceled!", __func__);
			data_circ_prn(fncrc, sp, ci, sp->sess_err, 0);
			FREE(ci);
			ci = NULL;

		} else DLL_WALK(lnk, data_link_t, DLST(dp)) {
			data_t		*_dp = NULL;
			data_link_t	*ign = NULL;

			TRY(data_link_2child, FNC_RET, sp, lnk, &_dp);
			TRY(data_stack_push, FNC_RET, sp, SESS_STACK_PTR(sp), lnk);
			_rc = _comp_setup_symtree(fncrc, sp, cp, _dp);
			TRY(data_stack_pop, FNC_RET, sp, SESS_STACK_PTR(sp), &ign);

			if(_rc != FNC_OKAY) {
				rc = _rc;
				FNC_ATXT(fncrc, "_comp_setup_symtree");
				goto _door;
			}
		}
	}

_door:	
FNC_RET:
	FNC_RETURN(rc);
}

int
comp_setup_symtree(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);

	if(COMP_DBG(cp) >= COMP_SHORT_DBG) {
		DBGOUT(sp->sess_err, "enter(cp=0x%p)", cp);
	}
	TRY(_comp_setup_symtree, _door, sp, cp, COMP_PTREE(cp));

_door:
	if(COMP_DBG(cp) >= COMP_SHORT_DBG) {
		DBGOUT(sp->sess_err, "return(%d)", rc);
	}
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
comp_ref_2vlnk(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_link_t	*reflnk,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*ref = NULL;
	data_t		*nn = NULL;
	data_t		*name = NULL;
	data_t		*sym = NULL;
	data_link_t	*lnk = NULL;
	data_link_t	*out = NULL;
	data_t		*err = NULL;
	data_circ_t	*ci = NULL;
	int		 _rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, reflnk);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2child, FNC_RET, sp, reflnk, &ref);
	TRY(data_stack_lookup, FNC_RET, sp, SESS_STACK(sp), ref, &ci);
	if(ci != NULL) {	// Zylus gefunden ?
		if(COMP_DBG(cp) >= COMP_CIRC_DBG ) {
			DBGOUT(sp->sess_err, "CIRCUIT detected, cancel current opertation!");
			data_circ_prn(fncrc, sp, ci, sp->sess_err, 0);
		}
		FREE(ci);
		ci = NULL;
		goto _door;
	}
	TRY(data_stack_push, FNC_RET, sp, SESS_STACK_PTR(sp), reflnk);
	TRY(data_get_first, _pop, sp, ref, &nn);
	TRY(comp_lookup_symtree, _pop, sp, cp, nn, &sym);
	if(sym == NULL) {
		TRY(comp_ref_error, _pop, sp, cp, ref, &err);
		TRY(data_lst_ins_first, _pop, sp, ref, err, &out);
		goto _pop;
	}

	TRY(data_parent_by_ident, _pop, sp, sym, NAME, &name);
	if(name == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_HIRARCHIE);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "No NAME found for Symbol(normalized-name)!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _pop;
	}

	TRY(datalnk_parent_by_ident, _pop, sp, name, ASSIGN, &lnk);
	TRY(data_lst_next, _pop, sp, lnk, &out);

_pop:
	TRY(data_stack_pop, _door, sp, SESS_STACK_PTR(sp), &lnk);

_door:
	if(_rc != FNC_OKAY) {
		rc = _rc;
	}
	*pp = out;

FNC_RET:
	FNC_RETURN(rc);
}

int
comp_res_arr(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_t		*ref,
	data_link_t	*inp,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*vlnk = NULL;
	data_t		*val = NULL;
	data_link_t	*vclnk = NULL;
	data_t		*vcont = NULL;
	data_link_t	*out = NULL;

	data_link_t	*lnk = NULL;
	data_t		*arr_idx = NULL;
	leng_t		 arr_idx_len = 0ULL;
	leng_t		 arr_idx_cnt = 0ULL;

	data_t		*err = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, ref);
	FNC_CHKINP(rc, inp);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2child, FNC_RET, sp, inp, &val);
	TRY(data_get_first, FNC_RET, sp, val, &vcont);
	if(comp_id_cmp(DID(vcont), ARRAY) != DATA_EQ) {
		out = inp;	// OKay, kein Array.
		goto _door;
	}
	TRY(data_by_ident, FNC_RET, sp, ref, ARRAY_INDEX, &arr_idx);
	if(arr_idx == NULL) {
		out = inp;	// Okay, ganzes Array.
		goto _door;
	}

	arr_idx_len = dll_count(DLST(arr_idx));
	arr_idx_cnt = 1ULL;
	DLL_WALK(lnk, data_link_t, DLST(arr_idx)) {
		data_t		*num = NULL;
		number_t	 i = 0LL;

		TRY(data_link_2child, FNC_RET, sp, lnk, &num);
		i = DNUM(num) +1;
		TRY(data_lst_nth, FNC_RET, sp, vcont, i, &vlnk);
		if(vlnk == NULL) {
			TRY(comp_arrref_error, FNC_RET, sp, cp, ref, arr_idx, &err);
			TRY(data_lst_ins_first, FNC_RET, sp, ref, err, &out);
			goto _door;
		}
		out = vlnk;
		if(arr_idx_cnt == arr_idx_len) {
			break;
		}
		arr_idx_cnt++;
		TRY(data_link_2child, FNC_RET, sp, vlnk, &val);
		TRY(data_lst_first, FNC_RET, sp, val, &vclnk);
		TRY(data_link_2child, FNC_RET, sp, vclnk, &vcont);
		if(comp_id_cmp(DID(vcont), ARRAY) == DATA_EQ) {
			continue;
		}
		if(comp_id_cmp(DID(vcont), REFERENCE) == DATA_EQ) {
			TRY(comp_ref_2vlnk, FNC_RET, sp, cp, vclnk, &vlnk);
			TRY(data_link_2child, FNC_RET, sp, vlnk, &val);
			TRY(data_get_first, FNC_RET, sp, val, &vcont);
		}
	}
_door:
	*pp = out;

FNC_RET:
	FNC_RETURN(rc);
}

int
comp_resref(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_link_t	*reflnk,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*ref = NULL;
	data_link_t	*vlnk = NULL;
	data_t		*val = NULL;
	data_link_t	*vclnk = NULL;
	data_t		*vcont = NULL;
	data_link_t	*out = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, reflnk);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2child,	_door, sp, reflnk, &ref);
	TRY(comp_chk_id,	_door, sp, ref, REFERENCE);
	TRY(comp_ref_2vlnk,	_door, sp, cp, reflnk, &vlnk);
	TRY(comp_res_arr,	_door, sp, cp, ref, vlnk, &out);
	TRY(data_link_2child,	_door, sp, out, &val);
	TRY(data_lst_first,	_door, sp, val, &vclnk);
	TRY(data_link_2child,	_door, sp, vclnk, &vcont);
	if(comp_id_cmp(DID(vcont), REFERENCE) == DATA_EQ) {
		TRY(comp_ref_2vlnk, _door, sp, cp, vclnk, &out);
	}
	
	*pp = out;

_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
comp_link_refs(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_link_t	*lnk
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*vlnk = NULL;
	data_link_t	*blnk = NULL;
	data_t		*val = NULL;
	data_t		*vcont = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, lnk);

	TRY(data_link_2child, _door, sp, lnk, &dp);
	if(comp_id_cmp(DID(dp), REFERENCE) != DATA_EQ) {
		goto _door;
	}

	TRY(comp_resref, _door, sp, cp, lnk, &vlnk);
	TRY(data_link_2child, _door, sp, vlnk, &val);
	if(comp_id_cmp(DID(val), ERROR) == DATA_EQ) {
		vcont = val;
	} else {
		TRY(data_get_first, _door, sp, val, &vcont);
	}
	DLL_WALK(blnk, data_link_t, DREF(dp)) {
		data_t		*xx = NULL;
		data_link_t	*plnk = NULL;
		data_link_t	*ign = NULL;
		data_assoc_t	*asp = NULL;

		TRY(data_link_2parent, _door, sp, blnk, &xx);
		if(comp_id_cmp(DID(xx), REFTREE) == DATA_EQ) {
			continue;
		}

		TRY(data_link_2plink, _door, sp, blnk, &plnk);
		if(vcont == dp) { // Short-Circuit ?
			TRY(data_lst_ins_prev, _door, sp, plnk, val, &ign);
		} else {
			TRY(data_lst_ins_prev, _door, sp, plnk, vcont, &ign);
		}

		TRY(data_link_2assoc, _door, sp, plnk, &asp);
		TRY(data_assoc_drop, _door, sp, asp);
		break;
	}
	
_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
comp_resolve(
	fncrc_t	*fncrc,
	sess_t	*sp,
	comp_t	*cp
) {
	int		 rc = FNC_OKAY;
	data_t		*tree = NULL;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);

	TRY(comp_setup_symtree, _door, sp, cp);
	tree = COMP_REFTREE(cp);
	DLL_WALK(lnk, data_link_t, DLST(tree)) {
		TRY(comp_link_refs, _door, sp, cp, lnk);
	}
	TRY(data_drop, _door, sp, tree);
	COMP_REFTREE(cp) = NULL;

_door:
	FNC_RETURN(rc);
}
