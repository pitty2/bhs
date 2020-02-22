#include <bhs/data.h>

int
data_circ_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_assoc_t	*origin,
	data_t		*cref,
	leng_t		 dist,
	data_circ_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_circ_t	*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, origin);
	FNC_CHKINP(rc, cref);
	FNC_CHKOUTP(rc, pp);

	if((p = CALLOC(1, sizeof(*p))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)sizeof(*p));
		goto _door;
	}
	p->ci_origin = origin;
	p->ci_cref = cref;
	p->ci_dist = dist;

_door:
	*pp = p;
	FNC_RETURN(rc);
}

int
data_circ_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_circ_t	*ci
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, ci);

	if(ci->ci_stack != NULL) {
		TRY(data_stack_drop, _door, sp, &ci->ci_stack);
	}

_door:
	FREE(ci);
	FNC_RETURN(rc);
}

int
data_circ_cmp(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_circ_t	*ci1,
	data_circ_t	*ci2,
	data_code_t	*pp
) {
	int		 rc = FNC_OKAY;
	data_code_t	 code = DATA_NE;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	if((ci1 == NULL) && (ci2 == NULL)) {
		code = DATA_NIL;
	} else if((ci1 != NULL) && (ci2 != NULL)) {
		if(ci1->ci_dist == ci2->ci_dist) {
			code = DATA_EQ;
		} else if(ci1->ci_dist > ci2->ci_dist) {
			code = DATA_GT;
		} else {
			code = DATA_LT;
		}
	} else {
		code = DATA_NE;
	}

	*pp = code;
	FNC_RETURN(rc);
}

int
data_circ_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_circ_t	*ci,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;
	data_assoc_t	*asp = NULL;
	data_link_t	*lnk0 = NULL;
	data_link_t	*lnk1 = NULL;
	char		*lnk0_str = NULL;
	char		*lnk1_str = NULL;
	leng_t		 dist = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);

	if(ci == NULL) {
		goto _door;
	}

	if((asp = ci->ci_origin) != NULL) {
		lnk0 = &asp->assoc_link[PARENT];
		lnk1 = &asp->assoc_link[CHILD];
	}
	TRY(data_link_2cstr, _door, sp, lnk0, &lnk0_str);
	TRY(data_link_2cstr, _door, sp, lnk1, &lnk1_str);
	dist = ci->ci_dist;


	PRNN(n); PRNO("data_circ : {\n");
	n++;
	PRNN(n); PRNO("%s", TSTR(lnk0_str));
		 PRNO(" <=(%llu)=> ", dist);
		 PRNO("%s\n", TSTR(lnk1_str));
	data_stack_prn(fncrc, sp, ci->ci_stack, out, n);
	n--;
	PRNN(n); PRNO("}\n");

_door:
	if(lnk0_str != NULL) {
		FREE(lnk0_str);
	}
	if(lnk1_str != NULL) {
		FREE(lnk1_str);
	}
	FNC_RETURN(rc);
}
