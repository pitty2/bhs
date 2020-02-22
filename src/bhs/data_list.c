#include <bhs/data.h>

/*
	-----------------------------------------------------------------------
 */
#if	defined(__SUNPRO_C)
data_des_t	_data_list_designator = {
	.des_name	= "DATA_LIST",
	.des_init	= data_lst_init,
	.des_new	= data_lst_new,
	.des_fini	= data_lst_fini,
	.des_drop	= data_lst_drop,
	.des_cpy	= data_lst_cpy,
	.des_cmp	= data_lst_cmp,
	.des_prn	= data_lst_prn
};

#elif	defined(__GNUC__)
data_des_t	_data_list_designator = {
	des_name	: "DATA_LIST",
	des_init	: data_lst_init,
	des_new		: data_lst_new,
	des_fini	: data_lst_fini,
	des_drop	: data_lst_drop,
	des_cpy		: data_lst_cpy,
	des_cmp		: data_lst_cmp,
	des_prn		: data_lst_prn
};

#else
#error("used compiler is not yet supported!")
#endif /* compiler */

/*
	-----------------------------------------------------------------------
 */
/*
 * Liefert den Positionsindex des Elements(dp) innerhalt der Liste(lt) zururck.
 * Wenn nicht in Liste(lst) enthalten, dann Index(*pp) == 0.
 */
static int
_data_lst_isin_pos(
	fncrc_t		*fncrc,	/* Functions return Element */
	sess_t		*sp,	/* Session */
	data_t		*lst,	/* Zu durchsuchende Liste */
	data_t		*dp,	/* Zu findendes Element */
	leng_t		*pp	/* Returnvalue, Index */
) {
	int		 rc = FNC_OKAY;
	leng_t		 pos = 0LL;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lst);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, lst, DATA_LIST_DESIGNATOR);

	DLL_WALK(lnk, data_link_t, DLST(lst)) {
		data_t	*cp = NULL;

		TRY(data_link_2child, _door, sp, lnk, &cp);
		if(cp == dp) {
			pos++;
			goto _door;
		}
		pos++;
	}
	pos = 0LL;	/* Nicht gefunden */

_door:
	if(rc != FNC_OKAY) {
		pos = 0LL;
	}
	*pp = pos;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_lst_init(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*ident,
	data_t		*p
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	TRY(data_init, _door, sp, ident, p);
	DES(p) = DATA_LIST_DESIGNATOR;
	dll_init(DLST(p));

_door:
	FNC_RETURN(rc);
}

int
data_lst_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*ident,
	data_t		**dp
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;
	dll_t		*list = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, dp);
	if(ident != NULL) {
		FNC_CHKINP(rc, ident);
	}

	TRY(data_new, _door, sp, ident, &p);
	DES(p) = DATA_LIST_DESIGNATOR;
	if((list = CALLOC(1, sizeof(*list))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)sizeof(*list));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	DLST(p) = list;
	dll_init(DLST(p));

_door:
	if(rc != FNC_OKAY) {
		if(p != NULL) {
			_data_drop(NEXT_FNCRC, sp, p);
			p = NULL;
		}
	}
	*dp = p;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_lst_is_circ(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*cp,
	data_assoc_t	*asp,
	bool_t		*b
) {
	int		 rc = FNC_OKAY;
	bool_t		 is = FALSE;
	data_t		*pp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, asp);
	FNC_CHKOUTP(rc, b);

	TRY(data_assoc_2parent, _door, sp, asp, &pp);
	if(cp == pp) {	/* Short-Circuit ? */
		is = TRUE;
		if(SESS_DBG(sp) >= SESS_SHORT_DBG) {
			DBGOUT(sp->sess_err, "Short-Circuit");
		}
	} else {
	/*
		Suche nach liste, die "unter" der aktuellen liste ist und
		die aktuelle liate enthaelt.
	 */
		data_link_t	*lnk = NULL;
		data_t		*dp = NULL;
		data_assoc_t	*_asp = NULL;

		DLL_WALK(lnk, data_link_t, DLST(cp)) {
			TRY(data_link_2child, _door, sp, lnk, &dp);
			if(DES(dp) != DATA_LIST_DESIGNATOR) {
				continue;
			}
			TRY(data_link_2assoc, _door, sp, lnk, &_asp);
			if(asp == _asp) {
				is = TRUE;
				break;
			}
			TRY(data_lst_is_circ, _next, sp, dp, asp, &is);
_next:
			if(rc != FNC_OKAY) {
				is = FALSE;
				goto _door;
			}
			if(is == TRUE) {
				if(SESS_DBG(sp) >= SESS_SHORT_DBG) {
					DBGOUT(sp->sess_err, "Long-Circuit");
				}
				break;
			}
		}
	}

_door:
	*b = is;
	FNC_RETURN(rc);
}

int
data_lst_fini(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p
) {
	int		 rc = FNC_OKAY;
	
	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, p);
	CHK_DES(rc, p, DATA_LIST_DESIGNATOR);

	if(DLL_ISEMPTY(DREF(p)) != TRUE) {
		goto _door;
	}
_door:
	FNC_RETURN(rc);
}

int
data_lst_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;
	bool_t		 is = FALSE;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, p);
	CHK_DES(rc, p, DATA_LIST_DESIGNATOR);

	/*
	 *	Suche nach Circuits, und loesche sie.
	 */
_again:
	DLL_WALK(lnk, data_link_t, DREF(p)) {
		data_assoc_t	*asp = NULL;

		TRY(data_link_2assoc, FNC_RET, sp, lnk, &asp);
		TRY(data_lst_is_circ, FNC_RET, sp, p, asp, &is);
		if(is == TRUE) {
			TRY(data_assoc_drop, FNC_RET, sp, asp);
			goto _again;
		}
	}

	/*
	 *	Wenn Daten-Liste nicht mehr referenziert wird,
	 *	loeschen.
	 */
	if(DLL_ISEMPTY(DREF(p)) == TRUE) {
		while((lnk = (data_link_t *)DLL_FIRST(DLST(p))) != NULL) {
			data_t	*dp = NULL;

			TRY(data_lst_remove, FNC_RET, sp, lnk, &dp);
			if(DES_DROP(dp) != NULL) {
				TRY(DES_DROP(dp), _door, sp, dp);
			} else {
				rc = FNC_ERROR;
				FNCRC_RESULT(rc);
				FNCRC_CAUSE(DATA_CAUSE_METHOD);
				FNCRC_SYSERR(0);
				FNC_TXT(fncrc, "DES_DROP() not found!");
				fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
				goto _door;
			}
		}
		FREE(DLST(p));
		DLST(p) = NULL;
		TRY(_data_drop, FNC_RET, sp, p);
	}

_door:

FNC_RET:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_lst_cpy(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*src,
	data_t		**dst
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;
	data_link_t	*lnk = NULL;
	leng_t		 cpos = 0LL;
	data_circ_t	*ci = NULL;
	leng_t		 dist = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, dst);
	CHK_DES(rc, src, DATA_LIST_DESIGNATOR);

	/*
	 *	Suche nach Circuits im Source-Objekt.
	 */
	TRY(data_stack_lookup, _door, sp, SESS_STACK(sp), src, &ci);
	if(ci != NULL) {		// Circuit gefunden ?
		dist = ci->ci_dist;
		FREE(ci);
		ci = NULL;
		if(dist > 0LL) {	// Short-Circuit ?
			goto _door;
		}
	}

	TRY(_data_cpy, _cleanup, sp, src, &p);
	if((DLST(p) = CALLOC(1, sizeof(dll_t))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)sizeof(dll_t));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _cleanup;
	}
	dll_init(DLST(p));

	DLL_WALK(lnk, data_link_t, DLST(src)) {
		data_t		*cp = NULL;
		data_t		*pp = NULL;
		data_link_t	*ign = NULL;
		leng_t		 ipos = 0LL;

		cpos++;
		TRY(data_link_2child, _cleanup, sp, lnk, &cp);
		TRY(data_link_2parent, _cleanup, sp, lnk, &pp);

		/*
		 *	Erkenne Short-Circuit
		 */
		if(cp == pp) {
			TRY(data_lst_ins_last, _cleanup, sp, p, p, &ign);
			continue;
		}

		/*
		 *	Erkenne Mehrfach Eintraege des selben Elements in Liste
		 */
		TRY(_data_lst_isin_pos, _cleanup, sp, src, cp, &ipos);
		if(ipos < cpos && ipos > 0LL) {
			TRY(data_lst_nth, _cleanup, sp, p, ipos, &ign);
			if(ign != NULL) {
				data_t	*ip = NULL;

				TRY(data_link_2child, _cleanup, sp, ign, &ip);
				TRY(data_lst_ins_last, _cleanup, sp, p, ip, &ign);
			}
			continue;
		}

		if(DES_CPY(cp) != NULL) {
			TRY(data_stack_push, _cleanup, sp, SESS_STACK_PTR(sp), lnk);
			TRY(DES_CPY(cp), _next, sp, cp, &pp);
			TRY(data_stack_pop, _cleanup, sp, SESS_STACK_PTR(sp), &ign);
_next:
			if(rc != FNC_OKAY) {
				TRY(data_stack_pop, _cleanup, sp, SESS_STACK_PTR(sp), &ign);
				goto _cleanup;
			}
			if(pp != NULL) {
				TRY(data_lst_ins_last, _cleanup, sp, p, pp, &ign);
			} else {
				PRNW(sp->sess_err, "pp == NULL");
			}
		} else {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(DATA_CAUSE_METHOD);
			FNCRC_SYSERR(0);
			FNC_TXT(fncrc, "DES_CPY() not found!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _cleanup;
		}
	}

_cleanup:
	if(rc != FNC_OKAY) {
		if(p != NULL) {
			data_drop(NEXT_FNCRC, sp, p);
			p = NULL;
		}
	}
	*dst = p;

_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_lst_cmp(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t 		*p1,
	data_t 		*p2,
	data_code_t	*pp
) {
	int		 rc = FNC_OKAY;
	data_code_t	 code = DATA_NIL;
	data_circ_t	*ci1 = NULL;
	data_circ_t	*ci2 = NULL;
	data_link_t	*lnk1 = NULL;
	data_link_t	*lnk2 = NULL;
	leng_t		 cnt1 = 0LL;
	leng_t		 cnt2 = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p1);
	FNC_CHKINP(rc, p2);
	FNC_CHKOUTP(rc, code);
	CHK_DES(rc, p1, DATA_LIST_DESIGNATOR);

	if(p1 == p2) {
		code = DATA_EQ;
		goto _cleanup;
	}
	if(DES(p1) != DES(p2)) {
		code = DATA_NE;
		goto _cleanup;
	}
	/*
	 * 	Vergleiche Listen-Laenge.
	 */
	cnt1 = dll_count(DLST(p1));
	cnt2 = dll_count(DLST(p2));
	if(cnt1 > cnt2) {
		code = DATA_GT;
		goto _cleanup;
	} else if(cnt1 < cnt2) {
		code = DATA_LT;
		goto _cleanup;
	}

	/*
	 *	Circuit Erkennung
	 *	Wenn Circuit erkannt, dann ist der Vergleich zu beeden,
	 *	sonst Endlos-Schleife!
	 */
	TRY(data_stack_lookup, _door, sp, SESS_STACK(sp), p1, &ci1);
	TRY(data_stack_lookup, _door, sp, SESS_AUX_STACK(sp), p2, &ci2);
	if((ci1 != NULL) && (ci2 != NULL)) {
		if(ci1->ci_dist == ci2->ci_dist) {
			code = DATA_EQ;
		} else {
			code = (ci1->ci_dist > ci2->ci_dist) ? DATA_GT : DATA_LT;
		}
		goto _cleanup;
	} else if((ci1 != NULL) && (ci2 == NULL)) {
		code = DATA_GT;
		goto _cleanup;
	} else if((ci1 == NULL) && (ci2 != NULL)) {
		code = DATA_LT;
		goto _cleanup;
	}

	/*
	 *	Okay, kein Circuit erkannt, weiter gehts!
	 */
	DLL_WALK_DOUBLE(lnk1, lnk2, data_link_t, DLST(p1), DLST(p2)) {
		data_t		*dp1 = NULL;
		data_t		*dp2 = NULL;
		data_link_t	*ign = NULL;

		TRY(data_link_2child, _cleanup, sp, lnk1, &dp1);
		TRY(data_link_2child, _cleanup, sp, lnk2, &dp2);

		if(DES_CMP(dp1) == NULL) {
			rc= FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(DATA_CAUSE_METHOD);
			FNCRC_SYSERR(0);
			FNC_TXT(fncrc, "DES_CMP(dp1) not found!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _cleanup;
		}

		TRY(data_stack_push, _cleanup, sp, SESS_STACK_PTR(sp), lnk1);
		TRY(data_stack_push, _cleanup, sp, SESS_AUX_STACK_PTR(sp), lnk2);
		TRY(DES_CMP(dp1),   _next, sp, dp1, dp2, &code);
		TRY(data_stack_pop, _cleanup, sp, SESS_STACK_PTR(sp), &ign);
		TRY(data_stack_pop, _cleanup, sp, SESS_AUX_STACK_PTR(sp), &ign);
_next:
		if(rc != FNC_OKAY) {
			TRY(data_stack_pop, _cleanup, sp, SESS_STACK_PTR(sp), &ign);
			TRY(data_stack_pop, _cleanup, sp, SESS_AUX_STACK_PTR(sp), &ign);
			goto _cleanup;
		}
		if(code != DATA_EQ) {
			break;
		}
	}
_cleanup:
	*pp = code;

_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_lst_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;
	data_circ_t	*ci = NULL;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	CHK_DES(rc, dp, DATA_LIST_DESIGNATOR);

	TRY(data_hdr_prn, _door, sp, dp, out, n);
	TRY(data_stack_lookup, _door, sp, SESS_STACK(sp), dp, &ci);
	n++;
	if(ci != NULL) {	// Zylus gefunden ?
		data_circ_prn(fncrc, sp, ci, out, n);
		FREE(ci);
		ci = NULL;
	} else DLL_WALK(lnk, data_link_t, DLST(dp)) {
		data_t		*cp = NULL;
		data_link_t	*ign = NULL;

		TRY(data_link_2child, _door, sp, lnk, &cp);
		if(DES_PRN(cp) == NULL) {
			rc= FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(DATA_CAUSE_METHOD);
			FNCRC_SYSERR(0);
			FNC_TXT(fncrc, "DES_PRN(cp) not found!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _door;
		}
		TRY(data_stack_push,	_door, sp, SESS_STACK_PTR(sp), lnk);
		TRY(DES_PRN(cp),	_next, sp, cp, out, n);
		TRY(data_stack_pop,	_door, sp, SESS_STACK_PTR(sp), &ign);
_next:
		if(rc != FNC_OKAY) {
			TRY(data_stack_pop, _door, sp, SESS_STACK_PTR(sp), &ign);
			FNC_ATXT(fncrc, "DES_PRN");
			goto _door;
		}
	}
	if(DATA_DBG(sp) >= DATA_LONG_DBG) {
		PRNN(n); PRNO("data_free = 0x%p\n", DFREE(dp));
	}
	n--;
	PRNN(n); PRNO("}\n");

_door:
	FNC_RETURN(rc);
}
