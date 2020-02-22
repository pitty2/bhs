#include <bhs/dbg.h>
#include <bhs/data.h>

int
data_sequ_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	leng_t		 num,
	char		*name,
	data_sequ_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_sequ_t	*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	if((dp = CALLOC(1, sizeof(*dp))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)sizeof(*dp));
		goto _door;
	}
	dp->num = num;
	dp->name = name;
_door:
	*pp = dp;
	FNC_RETURN(rc);
}

int
data_sequ_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_sequ_t	*p
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	p->name = NULL;
	FREE(p);
	FNC_RETURN(rc);
}

int
data_sequ_cpy(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_sequ_t	*src,
	data_sequ_t	**dst
) {
	int		 rc = FNC_OKAY;
	data_sequ_t	*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, dst);

	TRY(data_sequ_new, _door, sp, src->num, src->name, &dp);

_door:
	*dst = dp;
	FNC_RETURN(rc);
}

int
data_sequ_set(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_sequ_t	*sequ
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKINP(rc, sequ);

	if(p->data_sequ != NULL) {
		TRY(data_sequ_drop, _door, sp, p->data_sequ);
		p->data_sequ = NULL;;
	}
	p->data_sequ = sequ;

_door:
	FNC_RETURN(rc);
}

int
data_sequ_inherit(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*src,
	data_t		*dst
) {
	int		 rc = FNC_OKAY;
	data_sequ_t	*sequ = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKINP(rc, dst);

	if(SEQU(src) == NULL) {
		goto _door;
	}
	TRY(data_sequ_cpy, _door, sp, SEQU(src), &sequ);
	TRY(data_sequ_set, _next, sp, dst, sequ);
_next:
	if(rc != FNC_OKAY) {
		data_sequ_drop(NEXT_FNCRC, sp, sequ);
		goto _door;
	}

_door:
	FNC_RETURN(rc);
}

int
data_sequ_get(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_sequ_t	**pp
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);


	*pp = dp->data_sequ;
	FNC_RETURN(rc);
}

int
data_sequ_2cstr(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_sequ_t	*sequ,
	char		**pp
) {
	int		 rc = FNC_OKAY;
	char		 msg[FNC_MSG_SIZE +1];
	char		*cstr = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	if(sequ == NULL) {
		snprintf(msg, FNC_MSG_SIZE, "({nil}:{nil})");
	} else {
		snprintf(msg, FNC_MSG_SIZE, "(%s:(%llu))",
			TSTR(sequ->name), sequ->num);
	}
	if((cstr = STRDUP(msg)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "STRDUP() failed!");
	}
	*pp = cstr;
	FNC_RETURN(rc);
}

int
data_sequ_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_sequ_t	*p,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;

	if(out == NULL) {
		goto _door;
	}
	if(DATA_DBG(sp) >= DATA_LONG_DBG) {
		if(p == NULL) {
			PRNN(n); PRNO("data_sequ_t -> (0x%p) {}\n", p);
			goto _door;
		}
		PRNN(n); PRNO("data_sequ_t -> (0x%p) {\n", p);
		n++;
		PRNN(n); PRNO("num  = %llu\n", p->num);
		PRNN(n); PRNO("name = %s\n", TSTR(p->name));
		n--;
		PRNN(n); PRNO("}\n");
	}
_door:
	FNC_RETURN(rc);
}
