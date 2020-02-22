#include <bhs/data.h>

int
data_stack_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_stack_t	**stack
) {
	int		 rc = FNC_OKAY;
	data_stack_t	*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINOUTP(rc, sp);
	FNC_CHKINOUTP(rc, stack);

	if((p = CALLOC(1, sizeof(*p))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)sizeof(*p));
		goto _door;
	}
	dll_init(p);
_door:
	*stack = p;
	FNC_RETURN(rc);
}

int
data_stack_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_stack_t	**stack
) {
	int		 rc = FNC_OKAY;
	data_link_t	*elm = NULL;

	FNCRC_CHECK;
	FNC_CHKINOUTP(rc, sp);

	if(*stack == NULL) {
		goto FNC_RET;
	}

	do {
		TRY(data_stack_pop, _door, sp, stack, &elm);
	} while(elm != NULL);

_door:
	FREE(*stack);
	*stack = NULL;

FNC_RET:
	FNC_RETURN(rc);
}

int
data_stack_push(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_stack_t	**stack,
	data_link_t	*elm
) {
	int		 rc = FNC_OKAY;
	stack_elm_t	*item = NULL;

	FNCRC_CHECK;
	FNC_CHKINOUTP(rc, sp);
	FNC_CHKINOUTP(rc, stack);
	FNC_CHKINP(rc, elm);

	if(*stack == NULL) {
		TRY(data_stack_new, _door, sp, stack);
	}
	if((item = CALLOC(1, sizeof(*item))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)sizeof(*item));
		goto _door;
	}
	item->elm = elm;
	dll_insert_next(*stack, &item->entry);

_door:
	FNC_RETURN(rc);
}

int
data_stack_pop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_stack_t	**stack,
	data_link_t	**elm
) {
	int		 rc = FNC_OKAY;
	stack_elm_t	*item = NULL;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINOUTP(rc, sp);
	FNC_CHKINOUTP(rc, stack);
	FNC_CHKOUTP(rc, elm);

	if(*stack != NULL) {
		if((item = (void *)DLL_FIRST(*stack)) != NULL) {
			dll_remove(&item->entry);
			lnk = item->elm;
			FREE(item);
		}
	}
	*elm = lnk;

	if(DLL_ISEMPTY((dll_t *)*stack) == TRUE) {
		FREE(*stack);
		*stack = NULL;
	}
	FNC_RETURN(rc);
}

int
data_stack_lookup(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_stack_t	*stack,
	data_t		*search,
	data_circ_t	**pp
) {
	int		 rc = FNC_OKAY;
	stack_elm_t	*item = NULL;
	data_t		*parent = NULL;
	data_circ_t	*ci = NULL;
	leng_t		 dist = 0LL;

	FNCRC_CHECK;
	FNC_CHKINOUTP(rc, sp);
	FNC_CHKINP(rc, search);
	FNC_CHKOUTP(rc, pp);

	if(stack == NULL) {
		goto _door;
	}
	DLL_WALK(item, stack_elm_t, stack) {
		TRY(data_link_2parent, _door, sp, item->elm, &parent);
		if(parent == search) {
			data_assoc_t	*asp = NULL;

			TRY(data_link_2assoc, _door, sp, item->elm, &asp);
			TRY(data_circ_new, _next1, sp, asp, search, dist, &ci);
			if(rc != FNC_OKAY) {
_next1:
				ci = NULL;
				goto _door;
			}
			ci->ci_stack = stack;
			goto _door;
		}
		dist++;
	}
_door:
	*pp = ci;
	FNC_RETURN(rc);
}

int
data_stack_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_stack_t	*stack,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;
	stack_elm_t	*item = NULL;
	data_assoc_t	*asp = NULL;

	FNCRC_CHECK;
	FNC_CHKINOUTP(rc, sp);

	PRNN(n); PRNO("data_stack : {\n");
	if(stack != NULL) {
	n++;
	DLL_WALK(item, stack_elm_t, stack) {
		TRY(data_link_2assoc, _door, sp, item->elm, &asp);
		TRY(data_assoc_prn, _door, sp, asp, out, n);
	}
	n--;
	}
	PRNN(n); PRNO("}\n");

_door:
	FNC_RETURN(rc);
}
