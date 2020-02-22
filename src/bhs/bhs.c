#include <bhs.h>

int
bhs_value_by_symbol(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*tree,
	char	*sym,
	data_t	**pp
) {
	int	 rc = FNC_OKAY;
	data_t	*nn = NULL;
	data_t	*val = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, tree);
	FNC_CHKINP(rc, sym);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_cstr_2normalized, FNC_RET, sp, sym, &nn);
	TRY(comp_vcont_by_nsym, _door, sp, nn, tree, &val);

_door:
	data_drop(NEXT_FNCRC, sp, nn);
	*pp = val;
FNC_RET:
	FNC_RETURN(rc);
}

int
bhs_value_2cstr(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*val,
	char	**pp
) {
	int	 rc = FNC_OKAY;
	char	*cstr = NULL;
	data_t	*nstr = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, val);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, FNC_RET, sp, val, VALUE);
	if(comp_id_cmp(DID(val), REFERENCE) == DATA_EQ) {
		TRY(comp_ref_2cstr, _door, sp, val, &cstr);

	} else if(comp_id_cmp(DID(val), NUMERIC) == DATA_EQ) {
		TRY(data_num_2data_str, _door, sp, val, &nstr);
		TRY(data_str_2cstr, _door, sp, nstr, &cstr);

	} else if(comp_id_cmp(DID(val), STRING) == DATA_EQ) {
		TRY(data_str_2cstr, _door, sp, val, &cstr);

	} else {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_CONVERSION);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "\"%s\" is not an scalar data-identifier!",
			TSTR(DCSTR(DID(val))));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	}
_door:
	if(nstr != NULL) {
		data_drop(NEXT_FNCRC, sp, nstr);
	}
	*pp = cstr;

FNC_RET:
	FNC_RETURN(rc);
}

int
bhs_post_load(void) {
	return 0;
}

int
bhs_pre_unload(void) {
	return 0;
}

