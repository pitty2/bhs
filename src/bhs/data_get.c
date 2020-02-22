#include <bhs/data.h>

/*
	------------------------------------------------------------------------
 */
int
datalnk_by_address(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_t		*addr,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;
	data_t		*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKINP(rc, addr);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, p, DATA_LIST_DESIGNATOR);

	DLL_WALK(lnk, data_link_t, DLST(p)) {
		TRY(data_link_2child, _door, sp, lnk, &dp);
		if(dp == addr) {
			goto _door;
		}
	}
	lnk = NULL;

_door:
	if(rc != FNC_OKAY) {
		lnk = NULL;
	}
	*pp = lnk;
	FNC_RETURN(rc);
}

int
datalnk_parent_by_address(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_t		*addr,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;
	data_t		*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKINP(rc, addr);
	FNC_CHKOUTP(rc, pp);

	DLL_WALK(lnk, data_link_t, DREF(p)) {
		TRY(data_link_2parent, _door, sp, lnk, &dp);
		if(dp == addr) {
			goto _door;
		}
	}
	lnk = NULL;

_door:
	if(rc != FNC_OKAY) {
		lnk = NULL;
	}
	*pp = lnk;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
datalnk_by_ident(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_id_t	*id,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;
	data_t		*dp = NULL;
	data_code_t	 code = DATA_NIL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, pp);
	FNC_CHKINP(rc, id);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, p, DATA_LIST_DESIGNATOR);

	DLL_WALK(lnk, data_link_t, DLST(p)) {
		TRY(data_link_2child, _door, sp, lnk, &dp);
		TRY(data_id_cmp, _door, sp, DID(dp), id, &code);
		if(code == DATA_EQ) {
			goto _door;
		}
	}
	lnk = NULL;

_door:
	if(rc != FNC_OKAY) {
		lnk = NULL;
	}
	*pp = lnk;
	FNC_RETURN(rc);
}

int
datalnk_parent_by_ident(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_id_t	*id,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*blnk = NULL;
	data_link_t	*lnk = NULL;
	data_t		*dp = NULL;
	data_code_t	 code = DATA_NIL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, pp);
	FNC_CHKINP(rc, id);
	FNC_CHKOUTP(rc, pp);

	DLL_WALK(blnk, data_link_t, DREF(p)) {
		TRY(data_link_2parent, _door, sp, blnk, &dp);
		TRY(data_id_cmp, _door, sp, DID(dp), id, &code);
		if(code == DATA_EQ) {
			TRY(data_link_2plink, _door, sp, blnk, &lnk);
			break;
		}
	}

_door:
	if(rc != FNC_OKAY) {
		lnk = NULL;
	}
	*pp = lnk;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_by_ident(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_id_t	*id,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;
	data_t		*dp = NULL;
	data_code_t	 code = DATA_NIL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, pp);
	FNC_CHKINP(rc, id);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, p, DATA_LIST_DESIGNATOR);

	DLL_WALK(lnk, data_link_t, DLST(p)) {
		TRY(data_link_2child, _door, sp, lnk, &dp);
		TRY(data_id_cmp, _door, sp, DID(dp), id, &code);
		if(code == DATA_EQ) {
			goto _door;
		}
	}
	dp = NULL;

_door:
	if(rc != FNC_OKAY) {
		dp = NULL;
	}
	*pp = dp;
	FNC_RETURN(rc);
}

int
data_parent_by_ident(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_id_t	*id,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*blnk = NULL;
	data_t		*dp = NULL;
	data_code_t	 code = DATA_NIL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, pp);
	FNC_CHKINP(rc, id);
	FNC_CHKOUTP(rc, pp);

	DLL_WALK(blnk, data_link_t, DREF(p)) {
		TRY(data_link_2parent, _door, sp, blnk, &dp);
		TRY(data_id_cmp, _door, sp, DID(dp), id, &code);
		if(code == DATA_EQ) {
			goto _door;
		}
	}
	dp = NULL;

_door:
	if(rc != FNC_OKAY) {
		dp = NULL;
	}
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_lookup(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*list,	/* Objekt, das durchsucht werden soll */
	data_t		*p,	/* Objekt nach dem gesucht wird */
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*lnk = NULL;
	data_code_t	 code = DATA_NIL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, list);
	FNC_CHKINP(rc, p);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);

	DLL_WALK(lnk, data_link_t, DLST(list)) {
		TRY(data_link_2child, _door, sp, lnk, &dp);
		TRY(data_cmp, _door, sp, dp, p, &code);
		if(code == DATA_EQ) {
			goto _door;
		}
	}
	dp = NULL;

_door:
	if(rc != FNC_OKAY) {
		dp = NULL;
	}
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_get_first(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*list,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, list);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);

	TRY(data_lst_first, _door, sp, list, &lnk);
	TRY(data_link_2child, _door, sp, lnk, &dp);

_door:
	*pp = dp;
	FNC_RETURN(rc);
}

int
data_get_last(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*list,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, list);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, list, DATA_LIST_DESIGNATOR);

	TRY(data_lst_last, _door, sp, list, &lnk);
	TRY(data_link_2child, _door, sp, lnk, &dp);

_door:
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_cnt_by_lnk(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	leng_t		*pp
) {
	int		 rc = FNC_OKAY;
	leng_t		 cnt = 0ULL;
	data_t		*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2child, _door, sp, lnk, &dp);
	if(DES(dp) == DATA_LIST_DESIGNATOR) {
		TRY(data_lst_cnt, _door, sp, dp, &cnt);
	} else {
		cnt = 1ULL;
	}

_door:
	*pp = cnt;
	FNC_RETURN(rc);
}
