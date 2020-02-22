#include <bhs/dbg.h>
#include <bhs/data.h>

/*
	-----------------------------------------------------------------------
 */
int
data_link_init(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*owp,
	data_assoc_t	*asp,
	data_link_t	*lnk
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);

	dll_init(&lnk->link_entry);
	lnk->link_entry.head = NULL;
	lnk->link_owner = owp;
	lnk->link_assoc = asp;
	FNC_RETURN(rc);
}

int
data_link_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*owp,
	data_assoc_t	*asp,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	if((lnk = CALLOC(1, sizeof(*lnk))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!",(leng_t)sizeof(*lnk));
		goto _door;
	}
	TRY(data_link_init, _next1, sp, owp, asp, lnk);
_next1:
	if(rc != FNC_OKAY) {
		FNC_ATXT(fncrc, "data_link_init");
		FREE(lnk);
		lnk = NULL;
		goto _door;
	}
_door:
	*pp = lnk;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_link_fini(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);

	if(lnk->link_entry.head != NULL) {
		dll_remove(&lnk->link_entry);
		lnk->link_entry.head = NULL;
	}
	FNC_RETURN(rc);
}

int
data_link_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);

	TRY(data_link_fini, _door, sp, lnk);
	FREE(lnk);

_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_link_points_parent(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	bool_t		*pp
) {
	int		 rc = FNC_OKAY;
	data_t		*parent = NULL;
	bool_t		 b = FALSE;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);
	FNC_CHKOUTP(rc, pp);

	if((parent = lnk->link_owner) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "link_owner == NULL!");
		goto _door;
	}
	if((byte_t *)&lnk->link_entry == (byte_t *)&lnk->link_assoc->assoc_link[PARENT]) {
		b = TRUE;
	}
_door:
	*pp = b;
	FNC_RETURN(rc);
}

int
data_link_points_child(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	bool_t		*pp
) {
	int		 rc = FNC_OKAY;
	data_t		*child = NULL;
	bool_t		 b = FALSE;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);
	FNC_CHKOUTP(rc, pp);

	if((child = lnk->link_owner) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "link_owner == NULL!");
		goto _door;
	}
	if((byte_t *)&lnk->link_entry == (byte_t *)&lnk->link_assoc->assoc_link[CHILD]) {
		b = TRUE;
	}
_door:
	*pp = b;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_link_2plink(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_assoc_t	*asp = NULL;
	data_link_t	*plnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	if(lnk == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "lnk == NULL");
		goto _door;
	}
	if((asp = lnk->link_assoc) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "lnk->link_assoc == NULL");
		goto _door;
	}
	plnk = &asp->assoc_link[PARENT];
_door:
	*pp = plnk;
	FNC_RETURN(rc);
}

int
data_link_2parent(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*plnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2plink, _door, sp, lnk, &plnk);
	if((dp = plnk->link_owner) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "lnk->link_owner == NULL");
		goto _door;
	}

_door:
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_link_2clink(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	data_link_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_assoc_t	*asp = NULL;
	data_link_t	*clnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	if(lnk == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "lnk == NULL");
		goto _door;
	}
	if((asp = lnk->link_assoc) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "lnk->link_assoc == NULL");
		goto _door;
	}
	clnk = &asp->assoc_link[CHILD];
_door:
	*pp = clnk;
	FNC_RETURN(rc);
}

int
data_link_2child(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*clnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2clink, _door, sp, lnk, &clnk);
	if((dp = clnk->link_owner) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "lnk->link_owner == NULL");
		goto _door;
	}

_door:
	*pp = dp;
	FNC_RETURN(rc);
}
/*
	-----------------------------------------------------------------------
 */
int
data_link_2assoc(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	data_assoc_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_assoc_t	*asp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	if(lnk == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "lnk == NULL!");
		goto _door;
	}
	if((asp = lnk->link_assoc) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "lnk->link_assoc == NULL");
		goto _door;
	}
_door:
	*pp = asp;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_link_2cstr(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	char		**cstr
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	char		*id_str = NULL;
	char		*des_str = NULL;
	char		*sequ_name = NULL;
	leng_t		 sequ_num = 0LL;
	char		 msg[FNC_MSG_SIZE +1];
	char		*s = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lnk);
	FNC_CHKOUTP(rc, cstr);

	if((dp = lnk->link_owner) != NULL) {
		if(DID(dp) != NULL) {
			TRY(data_id_2cstr, _door, sp, DID(dp), &id_str);
		}
		des_str = DES(dp)->des_name;
		if(dp->data_sequ != NULL) {
			sequ_name = SEQU(dp)->name;
			sequ_num = SEQU(dp)->num;
		}
	}

	if(DATA_DBG(sp) == DATA_ADDR_DBG) {
		snprintf(&msg[0], FNC_MSG_SIZE,
			"(0x%p) %s(%s:%llu) [%s]",
			lnk, TSTR(id_str), TSTR(sequ_name), sequ_num, TSTR(des_str));

	} else if(DATA_DBG(sp) == DATA_LONG_DBG) {
		snprintf(&msg[0], FNC_MSG_SIZE,
			"lnk=(0x%p), owner=(0x%p) %s [%s]",
			lnk, lnk->link_owner, TSTR(id_str), TSTR(des_str));

	} else if(DATA_DBG(sp) >= DATA_DUMP_DBG) {
		snprintf(&msg[0], FNC_MSG_SIZE,
			"lnk=(0x%p), dll=(n 0x%p,p 0x%p,h 0x%p), owner=(0x%p) %s [%s], assoc=(0x%p)",
			lnk,
			lnk->link_entry.next,
			lnk->link_entry.prev,
			lnk->link_entry.head,
			lnk->link_owner,
			TSTR(id_str), TSTR(des_str), lnk->link_assoc);

	} else {
		snprintf(&msg[0], FNC_MSG_SIZE,
			"%-15s [%s]", TSTR(id_str), TSTR(des_str));
	}
	if((s = STRDUP(msg)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "STRDUP() failed!");
		goto _door;
	}
_door:
	if(id_str != NULL) {
		FREE(id_str);
	}
	*cstr = s;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_link_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_link_t	*lnk,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;
	char		*lnk_str = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);

	TRY(data_link_2cstr, _next1, sp, lnk, &lnk_str);
	if(rc != FNC_OKAY) {
_next1:
		PRNN(n); PRNO("{nil} [{nil}]\n");
		goto _door;
	}
	PRNN(n); PRNO("%s\n", TSTR(lnk_str));
	FREE(lnk_str);

_door:
	FNC_RETURN(rc);
}

