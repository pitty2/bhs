#include <bhs/data.h>
#include <bhs/data_conv.h>

int
data_num_2data_str(		/* Wandelt NUMBER (long long) in Dezimal-Sting */
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*src,
	data_t	**pp
) {
	int	 rc = FNC_OKAY;
	data_t	*str = NULL;
	char	 numstr[128];
	leng_t	 len = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, src, DATA_NUMBER_DESIGNATOR);

	if((rc = snprintf(&numstr[0], 127, "%lld", DNUM(src))) < 0) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_CONVERSION);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "snprintf() failed!");
		goto _door;
	}
	len = strlen(numstr);
	TRY(data_str_new, FNC_RET, sp, NULL, len, numstr, &str);

_door:
	*pp = str;

FNC_RET:
	FNC_RETURN(rc);
}

int
data_str_2data_num(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*src,
	leng_t		 base,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	number_t	 value = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, src, DATA_STRING_DESIGNATOR);

	TRY(str_2num, FNC_RET, DSTR(src), base, &value);
	TRY(data_num_new, FNC_RET, sp, NULL, value, &dp);
	*pp = dp;

FNC_RET:
	FNC_RETURN(rc);
}

int
data_str_2cstr(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*src,
	char		**pp
) {
	int		 rc = FNC_OKAY;
	char		*cstr = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, src, DATA_STRING_DESIGNATOR);

	TRY(str_2cstr, FNC_RET, DSTR(src), &cstr);
	*pp = cstr;

FNC_RET:
	FNC_RETURN(rc);
}

