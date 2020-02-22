#include <bhs/data.h>

/*
	-----------------------------------------------------------------------
 */
int
data_des_get(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_des_t	**dsg
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, dsg);

	*dsg = DES(dp);
	FNC_RETURN(rc);
}

int
data_des_set(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_des_t	*dsg
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, dsg);

	DES(dp) = dsg;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_des_2cstr(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_des_t	*p,
	char		**cstr
) {
	int		 rc = FNC_OKAY;
	char		*s = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKOUTP(rc, cstr);

	if(
		(p == DATA_DESIGNATOR) ||
		(p == DATA_IDENT_DESIGNATOR) ||
		(p == DATA_LIST_DESIGNATOR) ||
		(p == DATA_STRING_DESIGNATOR) ||
		(p == DATA_NUMBER_DESIGNATOR) ||
		(p == DATA_BOOL_DESIGNATOR) ||
		(p == DATA_FUNC_DESIGNATOR) ||
		(p == DATA_FILE_DESIGNATOR)
	) {
		if((s = STRDUP(p->des_name)) == NULL) {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(DATA_CAUSE_NOMEM);
			FNCRC_SYSERR(errno);
			FNC_TXT(fncrc, "STRDUP() failed!");
		}
	} else {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_DESIGNATOR);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "[%s] unknown data_designator!", TSTR(p->des_name));
	}
	*cstr = s;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_des_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_des_t	*p,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	PRNN(n); PRNO("[%s]", TSTR((char *)p->des_name));
	FNC_RETURN(rc);
}


