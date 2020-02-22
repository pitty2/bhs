#include <stdarg.h>
#include <strings.h>
#include <bhs/compile.h>

/*
	------------------------------------------------------------------------
 */
data_code_t
comp_id_cmp(
	data_id_t	*p1,
	data_id_t	*p2
) {
	data_code_t	 code = DATA_NE;

	if((p1 == NULL) || (p2 == NULL)) {
		code = DATA_NIL;
		goto _door;
	}

	if(p1 == p2) {
		code = DATA_EQ;
		goto _door;
	}

	if(strcmp(DCSTR(p1), DCSTR(p2)) == 0) {
		code = DATA_EQ;
	}
_door:
	return(code);
}

int
comp_chk_id(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_id_t	*id
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKINP(rc, id);

	if(DID(p) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_IDENTIFIER);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "identifier == NULL, \"%s\" expected!",
			TSTR(DCSTR(id)));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	if(comp_id_cmp(id, ANY) != DATA_EQ) {
		if(comp_id_cmp(DID(p), id) != DATA_EQ) {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_IDENTIFIER);
			FNCRC_SYSERR(0);
			FNC_TXT(fncrc, "identifier == \"%s\", \"%s\" expected!",
				TSTR(DCSTR(DID(p))), TSTR(DCSTR(id)));
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _door;
		}
	}
	
_door:
	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
comp_varg_2normalized(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		**pp,
	...
) {
	int		 rc = FNC_OKAY;
	data_t		*nsym = NULL;
	data_t		*sym = NULL;
	data_link_t	*ign = NULL;
	char		*name = NULL;
	va_list		 ap;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	va_start(ap, pp);

	TRY(data_lst_new, _door, sp, NORMALIZED, &nsym);
	while((name = va_arg(ap, char *)) != NULL) {
		leng_t	 len = 0ULL;

		len = strlen(name);
		TRY(data_str_new, _door, sp, SYMBOL, len, name, &sym);
		TRY(data_lst_ins_last, _door, sp, nsym, sym, &ign);
		sym = NULL;
	}

_door:
	va_end(ap);
	if(rc != FNC_OKAY) {
		if(sym != NULL) {
			data_drop(NEXT_FNCRC, sp, sym);
			sym = NULL;
		}
		if(nsym != NULL) {
			data_drop(NEXT_FNCRC, sp, nsym);
			nsym = NULL;
		}
	}
	*pp = nsym;
	FNC_RETURN(rc);
}

int
comp_cstr_2normalized(
	fncrc_t		*fncrc,
	sess_t		*sp,
	char		*cstr,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*nsym = NULL;
	data_t		*elm = NULL;
	data_link_t	*lnk = NULL;
	char		*token = NULL;
	leng_t		 len = 0ULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cstr);
	FNC_CHKOUTP(rc, pp);

	TRY(data_lst_new, _door, sp, NORMALIZED, &nsym);
	if((token = strtok(cstr, "->")) != NULL) {

		len = strlen(token);
		TRY(data_str_new, _door, sp, SYMBOL, len, token, &elm);
		TRY(data_lst_ins_last, _door, sp, nsym, elm, &lnk);
		elm = NULL;

		while((token = strtok(NULL, "->")) != NULL) {
			len = strlen(token);
			TRY(data_str_new, _door, sp, SYMBOL, len, token, &elm);
			TRY(data_lst_ins_last, _door, sp, nsym, elm, &lnk);
			elm = NULL;
		}
	}

_door:
	if(rc != FNC_OKAY) {
		if(elm != NULL) {
			data_drop(NEXT_FNCRC, sp, elm);
			elm = NULL;
		}
		if(nsym != NULL) {
			data_drop(NEXT_FNCRC, sp, nsym);
			nsym = NULL;
		}
	}
	*pp = nsym;
	FNC_RETURN(rc);
}

