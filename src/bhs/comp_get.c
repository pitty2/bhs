#include <bhs/compile.h>

int
comp_assign_2name(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, dp, ASSIGN);
	TRY(data_by_ident, _door, sp, dp, NAME, &p);
	*pp = p;

_door:
	FNC_RETURN(rc);
}

int
comp_assign_2value(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, dp, ASSIGN);
	TRY(data_by_ident, _door, sp, dp, VALUE, &p);
	*pp = p;

_door:
	FNC_RETURN(rc);
}

int
comp_name_2assign(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, dp, NAME);
	TRY(data_parent_by_ident, _door, sp, dp, ASSIGN, &p);
	*pp = p;

_door:
	FNC_RETURN(rc);
}

int
comp_value_2assign(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, dp, VALUE);
	TRY(data_parent_by_ident, _door, sp, dp, ASSIGN, &p);
	*pp = p;

_door:
	FNC_RETURN(rc);
}

int
comp_name_2value(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;
	data_t		*vp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, dp, NAME);
	TRY(comp_name_2assign, _door, sp, dp, &p);
	TRY(comp_assign_2value, _door, sp, p, &vp);
	*pp = vp;
_door:
	FNC_RETURN(rc);
}


int
comp_value_2content(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, dp, VALUE);
	TRY(data_get_first, _door, sp, dp, &p);
	*pp = p;

_door:
	FNC_RETURN(rc);
}

int
comp_value_type(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_id_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_value_2content, _door, sp, dp, &p);
	*pp = DID(p);

_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
comp_vcont_by_nsym(		// VALUE-content by NORMALIZRD-Symbol
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*nsym,
	data_t		*sym_tree,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*entry = NULL;
	data_t		*name = NULL;
	data_t		*value = NULL;
	data_t		*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, nsym);
	FNC_CHKINP(rc, sym_tree);
	FNC_CHKOUTP(rc, pp);

	TRY(comp_chk_id, _door, sp, nsym, NORMALIZED);
	TRY(comp_chk_id, _door, sp, sym_tree, SYMTREE);

	TRY(data_lookup, _door, sp, sym_tree, nsym, &entry);
	if(entry == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "Symbol not found in list!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}

	TRY(data_parent_by_ident, _door, sp, entry, NAME, &name);
	if(name == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "name not found in symbol-entry!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}

	TRY(comp_name_2value, _door, sp, name, &value);
	TRY(data_get_first, _door, sp, value, &dp);

	*pp = dp;
_door:
	FNC_RETURN(rc);
}

