#include <bhs/compile.h>

/*
	-----------------------------------------------------------------------
 */
int
comp_err_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	char		*msg,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*err = NULL;
	data_t		*errtxt = NULL;
	data_link_t	*lnk = NULL;
	leng_t		 len = 0ULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, msg);
	FNC_CHKOUTP(rc, pp);

	len = strlen(msg);
	TRY(data_lst_new, _door, sp, ERROR, &err);
	TRY(data_str_new, _door, sp, ERRTXT, len, msg, &errtxt);
	TRY(data_lst_ins_last, _door, sp, err, errtxt, &lnk);
	errtxt = NULL;

_door:
	if(rc != FNC_OKAY) {
		if(err != NULL) {
			data_drop(NEXT_FNCRC, sp, err);
			err = NULL;
		}
		if(errtxt != NULL) {
			data_drop(NEXT_FNCRC, sp, errtxt);
			errtxt = NULL;
		}
	}
	*pp = err;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
comp_name_error(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_t		*p1,	/* normalized-name from symtree */
	data_t		*p2,	/* normalized-name from ptree */
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;
	data_t		*sym = NULL;
	data_t		*err = NULL;

	char		*cstr = NULL;
	char		 msg[COMP_MSG_LEN];
	data_link_t	*ign = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, p1);
	FNC_CHKINP(rc, p2);
	FNC_CHKOUTP(rc, pp);

	TRY(data_lst_last, _door, sp, p2, &lnk);
	TRY(data_link_2child, _door, sp, lnk, &sym);
	TRY(data_str_2cstr, _door, sp, sym, &cstr);

	snprintf(msg, COMP_MSG_LEN -1,
		"%s:%lld: symbol error, name \"%s\" already defined in file %s:%lld!",
		TSTR(p2->data_sequ->name),
		p2->data_sequ->num,
		TSTR(cstr),
		TSTR(p1->data_sequ->name),
		p1->data_sequ->num
	);
	FREE(cstr);
	FPRINTF(sp->sess_err, "%s\n", msg);

	TRY(comp_err_new, _door, sp, msg, &err);
	TRY(data_lst_ins_last, _door, sp, err, p2, &ign);

_door:
	if(rc != FNC_OKAY) {
		if(err != NULL) {
			data_drop(NEXT_FNCRC, sp, err);
			err = NULL;
		}
	}
	COMP_ERRORS(cp)++;
	FNCRC_RESULT(FNC_ERROR);
	FNCRC_CAUSE(COMP_CAUSE_SYMBOL);
	FNCRC_SYSERR(0);
	FNC_TXT(fncrc, "%s", msg);
	*pp = err;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
comp_nsym_2cstr(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*nsym,
	char		**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;
	char		*str = NULL;
	char		*cstr = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, nsym);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, nsym, NORMALIZED);
	DLL_WALK(lnk, data_link_t, DLST(nsym)) {
		data_t	*sym = NULL;
		leng_t	 len = 0LL;

		TRY(data_link_2child, _door, sp, lnk, &sym);
		TRY(data_str_2cstr, _door, sp, sym, &str);
		if(cstr == NULL) {
			cstr = str;
		} else {
			char	*p = NULL;

			len = strlen(cstr) + strlen(str);
			if((p = REALLOC(cstr, len +3)) == NULL) {
				rc = FNC_ERROR;
				FNCRC_RESULT(rc);
				FNCRC_CAUSE(COMP_CAUSE_NOMEM);
				FNCRC_SYSERR(errno);
				FNC_TXT(fncrc, "REALLOC() failed!");
				fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
				goto _door;
			}
			cstr = p;
			strcat(cstr, "->");
			strcat(cstr, str);
		}
		str = NULL;
	}

_door:
	if(rc != FNC_OKAY) {
		if(str != NULL) {
			FREE(str);
		}
		if(cstr != NULL) {
			FREE(cstr);
			cstr = NULL;
		}
	}
	*pp = cstr;
	FNC_RETURN(rc);
}

int
comp_ref_2cstr(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*ref,
	char		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*nn = NULL;
	char		*cstr = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, ref);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, ref, REFERENCE);
	TRY(data_get_first, _door, sp, ref, &nn);
	TRY(comp_nsym_2cstr, _door, sp, nn, &cstr);

	*pp = cstr;
_door:
	FNC_RETURN(rc);
}

int
comp_idx_2cstr(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*idx,
	char		**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;
	char		*cstr = NULL;
	char		 buf[128];

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, idx);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, idx, ARRAY_INDEX);
	DLL_WALK(lnk, data_link_t, DLST(idx)) {
		data_t		*num = NULL;

		TRY(data_link_2child, _door, sp, lnk, &num);
		if(snprintf(&buf[0], 127, "%lld", DNUM(num)) < 0) {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_CONVERSION);
			FNCRC_SYSERR(errno);
			FNC_TXT(fncrc, "snprintf() failed!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _door;
		}

		if(cstr == NULL) {
			if((cstr = STRDUP(&buf[0])) == NULL) {
				rc = FNC_ERROR;
				FNCRC_RESULT(rc);
				FNCRC_CAUSE(COMP_CAUSE_NOMEM);
				FNCRC_SYSERR(errno);
				FNC_TXT(fncrc, "STRDUP() failed!");
				fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
				goto _door;
			}
		} else {
			char	*p = NULL;
			int	 len = 0;

			len = strlen(cstr) + strlen(&buf[0]);
			if((p = REALLOC(cstr, len +3)) == NULL) {
				rc = FNC_ERROR;
				FNCRC_RESULT(rc);
				FNCRC_CAUSE(COMP_CAUSE_NOMEM);
				FNCRC_SYSERR(errno);
				FNC_TXT(fncrc, "REALLOC() failed!");
				fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
				goto _door;
			}
			cstr = p;
			strcat(cstr, ", ");
			strcat(cstr, &buf[0]);
		}
	}

_door:
	if((rc != FNC_OKAY) && (cstr != NULL)) {
		FREE(cstr);
		cstr = NULL;
	}
	*pp = cstr;
	FNC_RETURN(rc);
}

int
comp_ref_error(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_t		*ref,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	char		 msg[COMP_MSG_LEN + 1];
	char		*str = NULL;
	data_t		*err = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, ref);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, ref, REFERENCE);
	TRY(comp_ref_2cstr, _door, sp, ref, &str);
	snprintf(&msg[0], COMP_MSG_LEN,
		"%s:%lld: reference error, symbol \"%s\" not found!",
		TSTR(SEQU(ref)->name), SEQU(ref)->num, TSTR(str));

	FREE(str);

	FPRINTF(sp->sess_err, "%s\n", msg);

	TRY(comp_err_new, _door, sp, msg, &err);

_door:
	COMP_ERRORS(cp)++;
	FNCRC_RESULT(FNC_ERROR);
	FNCRC_CAUSE(COMP_CAUSE_SYMBOL);
	FNCRC_SYSERR(0);
	FNC_TXT(fncrc, "%s", msg);
	*pp = err;
	FNC_RETURN(rc);
}

int
comp_arrref_error(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_t		*ref,
	data_t		*arr_idx,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	char		 msg[COMP_MSG_LEN + 1];
	char		*str = NULL;
	char		*idx_str = NULL;
	data_t		*err = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, ref);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, ref, REFERENCE);
	TRY(comp_ref_2cstr, _door, sp, ref, &str);
	TRY(comp_idx_2cstr, _door, sp, arr_idx, &idx_str);
	snprintf(&msg[0], COMP_MSG_LEN,
		"%s:%lld: reference error, array-element \"%s[%s]\" not found!",
		TSTR(SEQU(ref)->name), SEQU(ref)->num, TSTR(str), TSTR(idx_str));

	FREE(str);
	FREE(idx_str);
	FPRINTF(sp->sess_err, "%s\n", msg);
	TRY(comp_err_new, _door, sp, msg, &err);

_door:
	COMP_ERRORS(cp)++;
	FNCRC_RESULT(FNC_ERROR);
	FNCRC_CAUSE(COMP_CAUSE_SYMBOL);
	FNCRC_SYSERR(0);
	FNC_TXT(fncrc, "%s", msg);
	*pp = err;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
comp_env_error(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_t		*vname,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	char		*vstr = NULL;
	char		 msg[COMP_MSG_LEN + 1];
	data_t		*err = NULL;

	FNCRC_CHECK;

	TRY(data_str_2cstr, _door, sp, vname, &vstr);
	snprintf(&msg[0], COMP_MSG_LEN,
		"%s:%lld: buitlin error, env(%s) \"%s\" not set!",
		TSTR(SEQU(vname)->name), SEQU(vname)->num, TSTR(vstr), TSTR(vstr));
	FREE(vstr);
	FPRINTF(sp->sess_err, "%s\n", msg);
	TRY(comp_err_new, _door, sp, msg, &err);

_door:
	COMP_ERRORS(cp)++;
	*pp = err;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
comp_get_error(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*nsym,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	char		*cstr = NULL;
	char		 msg[COMP_MSG_LEN + 1];
	data_t		*err = NULL;

	FNCRC_CHECK;

	TRY(comp_nsym_2cstr, _door, sp, nsym, &cstr);
	snprintf(&msg[0], COMP_MSG_LEN,
		"name  error, symbol \"%s\" not found!",
		TSTR(cstr));
	FREE(cstr);
	FPRINTF(sp->sess_err, "%s\n", msg);
	TRY(comp_err_new, _door, sp, msg, &err);

_door:
	*pp = err;
	FNC_RETURN(rc);
}
