#include <bhs/data.h>

/*
	-----------------------------------------------------------------------
 */
dll_t	data_assoc_list = {
	&data_assoc_list,
	&data_assoc_list,
	&data_assoc_list
};

/*
	-----------------------------------------------------------------------
 */
int
data_assoc_init(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_assoc_t	*p,
	data_t		*child
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKINP(rc, child);


	TRY(data_link_init, _door, sp, NULL, p, &p->assoc_link[PARENT]);
	TRY(data_link_init, _door, sp, child, p, &p->assoc_link[CHILD]);
	dll_insert_prev(DREF(child), &p->assoc_link[CHILD].link_entry);

	dll_init(&p->assoc_entry);
	dll_insert_prev(&data_assoc_list, &p->assoc_entry);

_door:
	FNC_RETURN(rc);
}

int
data_assoc_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*child,
	data_assoc_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_assoc_t	*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, child);
	FNC_CHKOUTP(rc, pp);

	if((p = CALLOC(1, sizeof(*p))) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!",(leng_t)sizeof(*p));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	TRY(data_assoc_init, _door, sp, p, child);

_door:
	if((rc != FNC_OKAY) && (p != NULL)) {
		FREE(p);
		p = NULL;
	}
	*pp = p;
	FNC_RETURN(rc);
}

int
data_assoc_fini(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_assoc_t	*p
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	if(p->assoc_entry.head != NULL) {
		dll_remove(&p->assoc_entry);
		p->assoc_entry.head = NULL;
	}

	TRY(data_link_fini, _door, sp, &p->assoc_link[PARENT]);
	TRY(data_link_fini, _door, sp, &p->assoc_link[CHILD]);

_door:
	FNC_RETURN(rc);
}

int
data_assoc_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_assoc_t	*p
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	TRY(data_assoc_fini, _door, sp, p);
	FREE(p);

_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_assoc_2parent(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_assoc_t	*asp,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, asp);
	FNC_CHKOUTP(rc, pp);

	if((dp = asp->assoc_link[PARENT].link_owner) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "asp->assoc_link[PARENT].link_owner == NULL");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	}
	*pp = dp;
	FNC_RETURN(rc);
}

int
data_assoc_2child(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_assoc_t	*asp,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, asp);
	FNC_CHKOUTP(rc, pp);

	if((dp = asp->assoc_link[CHILD].link_owner) == NULL) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOTINLIST);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "asp->assoc_link[CHILD].link_owner == NULL");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	}
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
lookup_child(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*lst,
	data_t		*search,
volatile	leng_t		*dist,
	data_assoc_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_assoc_t	*asp = NULL;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, lst);
	FNC_CHKINP(rc, search);
	FNC_CHKINOUTP(rc, dist);
	FNC_CHKOUTP(rc, pp);

	if(lst->data_designator != DATA_LIST_DESIGNATOR) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_DESIGNATOR);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "wrong designator");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	DLL_WALK(lnk, data_link_t, DLST(lst)) {
		data_t		*child = NULL;
		data_t		*parent = NULL;

		asp = lnk->link_assoc;
#if (0)
		if(asp->assoc_loop == search) {
			goto _door;
		}
#endif
		parent = asp->assoc_link[PARENT].link_owner;
		child = asp->assoc_link[CHILD].link_owner;
		if(parent == child) {
			goto _door;
		}
		if(*dist > 0LL) {
			if(parent == search) {	
				goto _door;
			}
		}
		*dist++;
		TRY(lookup_child, _abort, sp, child, search, dist, &asp);

		if(asp != NULL) {
			goto _door;
		}
	}
_abort:
	*dist--;
	asp = NULL;

_door:
	*pp = asp;
	FNC_RETURN(rc);
}

data_assoc_t *
lookup_parent(
	data_t		*dp,
	data_t		*child,
	leng_t		*dist
) {
	data_assoc_t	*asp = NULL;
	data_link_t	*lnk = NULL;

	if(DES(dp) != DATA_LIST_DESIGNATOR) {
		goto _door;
	}
	DLL_WALK(lnk, data_link_t, DREF(dp)) {
		data_t		*parent = NULL;

		asp = lnk->link_assoc;
#if (0)
		if(asp->assoc_loop == child) {
			goto _door;
		}
#endif
		parent = asp->assoc_link[PARENT].link_owner;
		if(parent == child) {	
			goto _door;
		}
		dist++;
		if((asp = lookup_parent(parent, child, dist)) != NULL) {
			goto _door;
		}
	}
	dist--;
	asp = NULL;
_door:
	return(asp);
}


int
data_assoc_lookup(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_assoc_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_assoc_t	*asp = NULL;
	dll_t		*lst = &data_assoc_list;
	data_assoc_t	*el = NULL;
	leng_t		 dist = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKOUTP(rc, pp);

	DLL_WALK(el, data_assoc_t, lst) {
		if(el->assoc_link[PARENT].link_owner == p) {
			TRY(lookup_child, _next1, sp, p, p, &dist, &asp);
_next1:
			if(rc != FNC_OKAY) {
				asp = NULL;
				goto _door;
			}
			if(asp != NULL) {
				goto _door;
			}
		}
	}
	asp = NULL;
_door:
	*pp = asp;
	FNC_RETURN(rc);
}
/*
	-----------------------------------------------------------------------
 */
int
data_assoc_2cstr(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_assoc_t	*asp,
	char		**cstr
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk0 = NULL;
	data_link_t	*lnk1 = NULL;
	char		*lnk0_str = NULL;
	char		*lnk1_str = NULL;
	char		*msg = NULL;
	char		*s = NULL;
	int		 sysrc = 0;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, cstr);

	if((msg = CALLOC(1, FNC_MSG_SIZE +1)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (FNC_MSG_SIZE +1));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _cleanup;
	}	
	if(asp != NULL) {
		lnk0 = &asp->assoc_link[PARENT];
		lnk1 = &asp->assoc_link[CHILD];
	}
	TRY(data_link_2cstr, _cleanup, sp, lnk0, &lnk0_str);
	TRY(data_link_2cstr, _cleanup, sp, lnk1, &lnk1_str);

	sysrc = snprintf(&msg[0], FNC_MSG_SIZE,
		"PARENT %s <--> CHILD  %s",
		TSTR(lnk0_str), TSTR(lnk1_str)
		);
	if(sysrc < 0) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_SYSCALL);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "snprintf() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _cleanup;
	}

	if((s = STRDUP(msg)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "STRDUP() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _cleanup;
	}
_cleanup:
	if(msg != NULL) {
		FREE(msg);
	}
	if(lnk0_str != NULL) {
		FREE(lnk0_str);
	}
	if(lnk1_str != NULL) {
		FREE(lnk1_str);
	}
	*cstr = s;
	FNC_RETURN(rc);
}

int
data_assoc_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_assoc_t	*asp,
	FILE		*out,
	leng_t		 n
) {
	int	 rc = FNC_OKAY;
	char	*ass_str = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);

	TRY(data_assoc_2cstr, _door, sp, asp, &ass_str);
	PRNN(n); PRNO("%s\n", TSTR(ass_str));
	FREE(ass_str);

_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_assoc_list_prn(
	fncrc_t	*fncrc,
	sess_t	*sp,
	FILE	*out,
	leng_t	 n
) {
	int		 rc = FNC_OKAY;
	dll_t		*p = &data_assoc_list;
	data_assoc_t	*el = NULL;

	FNCRC_CHECK;

	if(out != NULL) {
		PRNN(n); PRNO("(0x%p) [%s] : {\n", p, "data_assoc_list");
		n++;
		DLL_WALK(el, data_assoc_t, p) {
			data_assoc_prn(fncrc, sp, el, out, n);
		}
		n--;
		PRNN(n); PRNO("}\n");
	}
	FNC_RETURN(rc);
}

