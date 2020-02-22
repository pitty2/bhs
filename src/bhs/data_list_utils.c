#include <bhs/data.h>

/*
	-----------------------------------------------------------------------
 */
int
data_lst_ins_first(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*list,
	data_t		*elem,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_assoc_t	*asp = NULL;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, list);
	FNC_CHKINP(rc, elem);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);

	TRY(data_assoc_new, _door, sp, elem, &asp);
	dll_insert_next(DLST(list), &asp->assoc_link[PARENT].link_entry);
	asp->assoc_link[PARENT].link_owner = list;
	lnk = &asp->assoc_link[PARENT];

_door:
	*pp = lnk;
	FNC_RETURN(rc);
}

int
data_lst_ins_last(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*list,
	data_t		*elem,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_assoc_t	*asp = NULL;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, list);
	FNC_CHKINP(rc, elem);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);

	TRY(data_assoc_new, _door, sp, elem, &asp);
	dll_insert_prev(DLST(list), &asp->assoc_link[PARENT].link_entry);
	asp->assoc_link[PARENT].link_owner = list;
	lnk = &asp->assoc_link[PARENT];

_door:
	*pp = lnk;
	FNC_RETURN(rc);
}

int
data_lst_remove(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*elem = NULL;
	data_assoc_t	*asp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2child, _door, sp, lnk, &elem);
	TRY(data_link_2assoc, _door, sp, lnk, &asp);
	TRY(data_assoc_drop, _door, sp, asp);

_door:
	*pp = elem;
	FNC_RETURN(rc);
}

int
data_lst_ins_prev(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	data_t		*elem,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_assoc_t	*asp = NULL;
	data_link_t	*nlnk = NULL;
	data_t		*list = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);
	FNC_CHKINP(rc, elem);
	FNC_CHKOUTP(rc, pp);

	if((list = lnk->link_owner) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "link_owner == NULL!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);
	TRY(data_assoc_new, _door, sp, elem, &asp);
	dll_insert_prev(&lnk->link_entry, &asp->assoc_link[PARENT].link_entry);
	asp->assoc_link[PARENT].link_owner = lnk->link_owner;
	nlnk = &asp->assoc_link[PARENT];

_door:
	*pp = nlnk;
	FNC_RETURN(rc);
}

int
data_lst_ins_next(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	data_t		*elem,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_assoc_t	*asp = NULL;
	data_link_t	*nlnk = NULL;
	data_t		*list = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);
	FNC_CHKINP(rc, elem);
	FNC_CHKOUTP(rc, pp);

	if((list = lnk->link_owner) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "link_owner == NULL!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);
	TRY(data_assoc_new, _door, sp, elem, &asp);
	dll_insert_next(&lnk->link_entry, &asp->assoc_link[PARENT].link_entry);
	asp->assoc_link[PARENT].link_owner = lnk->link_owner;
	nlnk = &asp->assoc_link[PARENT];

_door:
	*pp = nlnk;
	FNC_RETURN(rc);
}

int
data_lst_first(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*list,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, list);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);
	lnk = (data_link_t *)DLL_FIRST(DLST(list));
	*pp = lnk;
	FNC_RETURN(rc);
}

int
data_lst_last(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*list,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, list);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);
	lnk = (data_link_t *)DLL_LAST(DLST(list));
	*pp = lnk;
	FNC_RETURN(rc);
}

int
data_lst_prev(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*nlnk = NULL;
	data_t		*list = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);
	FNC_CHKOUTP(rc, pp);

	if((list = lnk->link_owner) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "link_owner == NULL!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);
	nlnk = (data_link_t *)DLL_PREV(&lnk->link_entry);
	if(DLL_ISHEAD(&nlnk->link_entry)) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "link runs into head!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		nlnk = NULL;
	}
_door:
	*pp = nlnk;
	FNC_RETURN(rc);
}

int
data_lst_next(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*nlnk = NULL;
	data_t		*list = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);
	FNC_CHKOUTP(rc, pp);

	if((list = lnk->link_owner) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "link_owner == NULL!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);

	nlnk = (data_link_t *)DLL_NEXT(&lnk->link_entry);
	if(DLL_ISHEAD(&nlnk->link_entry)) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "link runs into head!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		nlnk = NULL;
	}
	*pp = nlnk;
_door:
	FNC_RETURN(rc);
}

int
data_lst_nth(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*list,
	leng_t		 pos,  /* 1 ... */
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;
	leng_t		 cnt = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, list);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);

	cnt = 1LL;
	DLL_WALK(lnk, data_link_t, DLST(list)) {
		if(pos == cnt) {
			goto _door;
		}
		cnt++;
	}
	lnk = NULL;
_door:
	*pp = lnk;
	FNC_RETURN(rc);
}

int
data_lst_cnt(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*list,
	leng_t		*pp
) {
	int		 rc = FNC_OKAY;
	leng_t		 cnt = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, list);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);

	cnt = dll_count(DLST(list));
	*pp = cnt;
	FNC_RETURN(rc);
}

