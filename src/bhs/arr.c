#include <bhs/arr.h>

char	*arr_cause[] = {
	"Array okay",
	"Array pointer across boundaries!",
	"Array element size!",
	"Array malloc() failed!",
	"Array systemcall failed!"
};

/*
	-----------------------------------------------------------------------
 */
int
arr_init(
	fncrc_t	*fncrc,
	arr_t	*aptr,
	leng_t	 elsz,
	leng_t	 len
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, aptr);
	FNC_CHKINL(rc, elsz);
	FNC_CHKINL(rc, len);

	if((aptr->elsz = elsz) == 0) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(ARR_CAUSE_ELEMENT_SIZE);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "element size == 0!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	if(len == 0) {
		goto _door;
	}
	if((aptr->base = (byte_t *)CALLOC(len, elsz)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(ARR_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(%llu, %llu) failed!", (leng_t)len, (leng_t)elsz);
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	aptr->next = aptr->base;
	aptr->halt = aptr->base + (elsz * len);
_door:
	FNC_RETURN(rc);
}

int
arr_new(
	fncrc_t	*fncrc,
	leng_t	 elsz,
	leng_t	 len,
	arr_t	**pp
) {
	int	 rc = FNC_OKAY;
	arr_t	*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINL(rc, elsz);
	FNC_CHKINL(rc, len);
	FNC_CHKINP(rc, pp);

	if((p = (arr_t *)CALLOC(1, sizeof(*p))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(ARR_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)sizeof(*p));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	TRY(arr_init, _door, p, elsz, len);

_door:
	if((rc != FNC_OKAY) && (p != NULL)) {
		FREE(p);
		p = NULL;
	}
	*pp = p;
	FNC_RETURN(rc);
}

int
arr_fini(
	fncrc_t	*fncrc,
	arr_t	*aptr
) {
	int	rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, aptr);

	if(aptr->base != NULL) {
		FREE(aptr->base);
		aptr->base = NULL;
		aptr->next = NULL;
		aptr->halt = NULL;
	}
	FNC_RETURN(rc);
}

int
arr_drop(
	fncrc_t	*fncrc,
	arr_t	*aptr
) {
	int	rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, aptr);

	TRY(arr_fini, _door, aptr);
	FREE(aptr);

_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
arr_set(
	fncrc_t	*fncrc,
	arr_t	*aptr,
	leng_t	 elcnt,
	void	*word
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, aptr);
	FNC_CHKINP(rc, word);

	if(elcnt < 1) {
		TRY(arr_fini, _door, aptr);
	} else {
		if(elcnt > arr_count(aptr)) {
			TRY(arr_resize, _door, aptr, elcnt);
		}
		memset(aptr->base, 0, (aptr->halt - aptr->base));
		memmove(aptr->base, word, (aptr->elsz * elcnt));
		aptr->next = aptr->base + (aptr->elsz * elcnt);
	}
_door:
	FNC_RETURN(rc);
}

int
arr_resize(
	fncrc_t	*fncrc,
	arr_t	*aptr,
	leng_t	 elcnt
) {
	int	 rc = FNC_OKAY;
	leng_t	 ocnt;

	FNCRC_CHECK;
	FNC_CHKINP(rc, aptr);
	FNC_CHKINL(rc, elcnt);

	if(elcnt < 1) {
		TRY(arr_fini, _door, aptr);
	} else {
		ocnt = arr_count(aptr);
		aptr->base = (byte_t *)REALLOC(aptr->base, elcnt * aptr->elsz);
		if(aptr->base != NULL) {
			if(ocnt > elcnt) {
				ocnt = elcnt;
			}
			aptr->halt = aptr->base + (aptr->elsz * elcnt);
			aptr->next = aptr->base + (aptr->elsz * ocnt);
			if(aptr->next > aptr->halt) {
				aptr->next = aptr->halt;
			}
		} else {
			rc = FNC_FATAL;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(ARR_CAUSE_NOMEM);
			FNCRC_SYSERR(errno);
			FNC_TXT(fncrc, "REALLOC(0x%p, %llu * %llu) failed!", 
				aptr->base, (leng_t)elcnt, (leng_t)aptr->elsz);
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		}
	}
_door:
	FNC_RETURN(rc);
}

int
arr_insert(
	fncrc_t	*fncrc,
	arr_t	*aptr,
	void	*ielm,
	leng_t	 idx
) {
	int	 rc = FNC_OKAY;
	byte_t	*rptr = NULL;
	long	 msz;

	FNCRC_CHECK;
	FNC_CHKINP(rc, aptr);
	FNC_CHKINP(rc, ielm);

	if(aptr->next == aptr->halt) {
		TRY(arr_resize, FNC_RET, aptr, arr_size(aptr) + 1);
	}
	TRY(arr_elm_ref, FNC_RET, aptr, idx, &rptr);
	if((msz = aptr->next - rptr) > 0) {
		memmove(rptr + aptr->elsz, rptr, msz);
	}
	memmove(rptr, ielm, aptr->elsz);
	aptr->next += aptr->elsz;

FNC_RET:
	FNC_RETURN(rc);
}

int
arr_append(
	fncrc_t	*fncrc,
	arr_t	*aptr,
	void	*ielm
) {
	int	 rc = FNC_OKAY;
	byte_t	*rptr = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, aptr);
	FNC_CHKINP(rc, ielm);

	if(aptr->next == aptr->halt) {
		TRY(arr_resize, FNC_RET, aptr, arr_size(aptr) + 1);
	}
	rptr = aptr->next;
	memmove(rptr, ielm, aptr->elsz);
	aptr->next += aptr->elsz;

FNC_RET:
	FNC_RETURN(rc);
}

int
arr_remove(
	fncrc_t	*fncrc,
	arr_t	*aptr,
	leng_t	 idx,
	void	**pp
) {
	int	 rc = FNC_OKAY;
	byte_t	*rptr = NULL;
	byte_t	*elm = NULL;
	leng_t msz;

	FNCRC_CHECK;
	FNC_CHKINP(rc, aptr);
	FNC_CHKOUTP(rc, pp);

	if((aptr->base == aptr->next) || (aptr->base == NULL)) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(ARR_CAUSE_BOUNDARIES);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "base == next-pointer or base-pointer == NULL!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	TRY(arr_elm_ref, FNC_RET, aptr, idx, &rptr);

	if(rptr == aptr->next) {
		goto _door;
	}
	if((elm = (byte_t *)MALLOC(aptr->elsz)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(ARR_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "MALLOC(%llu) failed!", (leng_t)aptr->elsz);
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	memmove(elm, rptr, aptr->elsz);
	if((msz = aptr->next - rptr) > 1) {
		memmove(rptr, rptr + aptr->elsz, msz);
	}
	aptr->next -= aptr->elsz;
	memset(aptr->next, 0x00, aptr->elsz);

_door:
	*pp = elm;
FNC_RET:
	FNC_RETURN(rc);
}

int
arr_elm_idx(
	fncrc_t	*fncrc,
	arr_t	*aptr,
	void	*ref,
	leng_t	*idx
) {
	int	 rc = FNC_OKAY;
	byte_t	*rptr = ref;

	FNCRC_CHECK;
	FNC_CHKINP(rc, aptr);
	FNC_CHKINP(rc, ref);
	FNC_CHKOUTP(rc, idx);

	if((rptr < aptr->base) || (rptr > aptr->next)) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(ARR_CAUSE_BOUNDARIES);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "array reference-pointer out of boundaries!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	} else {
		*idx = rptr - aptr->base;
		*idx = *idx / aptr->elsz;
	}
	FNC_RETURN(rc);
}

int
arr_elm_ref(
	fncrc_t	*fncrc,
	arr_t	*aptr,
	leng_t	 idx,
	byte_t	**pp
) {
	int	 rc = FNC_OKAY;
	byte_t	*rptr = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, aptr);
	FNC_CHKOUTP(rc, pp);

	if(idx > arr_count(aptr)) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(ARR_CAUSE_BOUNDARIES);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "array index out of boundaries!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	} else {
		if(aptr->next == aptr->base) {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(ARR_CAUSE_BOUNDARIES);
			FNCRC_SYSERR(0);
			FNC_TXT(fncrc, "arr_elm_ref array is empty!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		} else {
			rptr = aptr->base + (idx * aptr->elsz);
		}
	}
	*pp = rptr;
	FNC_RETURN(rc);
}

void *
arr_first(arr_t *aptr) {
	byte_t	*rptr = NULL;

	if(aptr->base < aptr->next) {
		rptr = aptr->base;
	}
	return(rptr);
}

void *
arr_last(arr_t *aptr) {
	byte_t	*rptr = NULL;

	if(aptr->next > aptr->base) {
		rptr = aptr->next - aptr->elsz;
	}
	return(rptr);
}

void *
arr_next(arr_t *aptr, void *ref) {
	byte_t	*rptr = ref;

	rptr += aptr->elsz;

	if((rptr >= aptr->next) || (rptr < aptr->base)) {
		rptr = NULL;
	}
	return(rptr);
}

leng_t
arr_count(arr_t *aptr) {
	leng_t cnt = 0;

	cnt = aptr->next - aptr->base;
	cnt = cnt / aptr->elsz;
	return(cnt);
}

leng_t
arr_size(arr_t *aptr) {
	leng_t cnt = 0;

	cnt = aptr->halt - aptr->base;
	cnt = cnt / aptr->elsz;
	return(cnt);
}

leng_t
arr_avail(arr_t *aptr) {
	leng_t cnt = 0;

	cnt = (aptr->halt - aptr->next) / aptr->elsz;
	return(cnt);
}

void
arr_prn(arr_t *aptr, FILE *out, leng_t n) {

#ifdef _LP64
	PRNN(n); PRNO("(arr_t *)%p[%lu] {", aptr, sizeof(arr_t));
#else
	PRNN(n); PRNO("(arr_t *)%p[%u] {", aptr, sizeof(arr_t));
#endif
	if(aptr != NULL) {
		PRNO("\n"); n++;
		PRNN(n); PRNO("elsz: %llu,\n", aptr->elsz);
		PRNN(n); PRNO("base: 0x%p,\n", aptr->base);
		PRNN(n); PRNO("next: 0x%p,\n", aptr->next);
		PRNN(n); PRNO("halt: 0x%p\n", aptr->halt);
		n--; PRNN(n);
	}
	PRNO("}");
}
