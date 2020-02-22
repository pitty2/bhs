#include <bhs/data.h>

/*
 	-------------------------------------------------------------------------
 */
int
data_str_len(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p,
	leng_t	*pp
) {
	int	 rc = FNC_OKAY;
	leng_t	 len = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, p, DATA_STRING_DESIGNATOR);

	len = str_len(DSTR(p));
	*pp = len;
	FNC_RETURN(rc);
}

int
data_str_cat(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p1,
	data_t	*p2,
	data_t	**pp
) {
	int	 rc = FNC_OKAY;
	data_t	*p = NULL;
	str_t	*s = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p1);
	FNC_CHKINP(rc, p2);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, p1, DATA_STRING_DESIGNATOR);
	CHK_DES(rc, p2, DATA_STRING_DESIGNATOR);

	TRY(str_cat, _door, DSTR(p1), DSTR(p2), &s);

_door:
	*pp = p;
	FNC_RETURN(rc);
}

int
data_strn_cpy(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*src,
	leng_t	 n,
	data_t	**dst
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;
	data_id_t	*id = NULL;
	leng_t		 len = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, dst);
	CHK_DES(rc, src, DATA_STRING_DESIGNATOR);

	TRY(data_str_len, _door, sp, src, &len);
	if(n > len) {
		n = len;
	}

	TRY(data_id_cpy, _door, sp, DID(src), &id);
	TRY(data_str_new, _door, sp, id, n, DCSTR(src), &p);
	id = NULL;

_door:
	if(id != NULL) {
		data_id_drop(NEXT_FNCRC, sp, id);
	}
	*dst = p;
	FNC_RETURN(rc);
}

/*
 	-------------------------------------------------------------------------
 */
int
data_str_split(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p1,	/* String */
	data_t		*p2,	/* Search */
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	char		*string = NULL;
	char		*search = NULL;
	char		*token = NULL;

	data_link_t	*lnk = NULL;
	data_t		*elm = NULL;
	leng_t		 len = 0ULL;

	
	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p1);
	FNC_CHKINP(rc, p2);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, p1, DATA_STRING_DESIGNATOR);
	CHK_DES(rc, p2, DATA_STRING_DESIGNATOR);

	TRY(data_str_2cstr, _door, sp, p1, &string);
	TRY(data_str_2cstr, _door, sp, p2, &search);

	if((token = strtok(string, search)) != NULL) {

		TRY(data_lst_new, _door, sp, NULL, &dp);
		TRY(data_sequ_inherit, _door, sp, p1, dp);

		len = strlen(token);
		TRY(data_str_new, _door, sp, NULL, len, token, &elm);
		TRY(data_lst_ins_last, _door, sp, dp, elm, &lnk);
		elm = NULL;

		while((token = strtok(NULL, search)) != NULL) {
			len = strlen(token);
			TRY(data_str_new, _door, sp, NULL, len,token, &elm);
			TRY(data_lst_ins_last, _door, sp, dp, elm, &lnk);
			elm = NULL;
		}
	}

_door:
	if(string != NULL) {
		FREE(string);
	}
	if(search != NULL) {
		FREE(search);
	}
	if(rc != FNC_OKAY) {
		if(dp != NULL) {
			data_drop(NEXT_FNCRC, sp, dp);
			dp = NULL;
		}
		if(elm != NULL) {
			data_drop(NEXT_FNCRC, sp, elm);
			elm = NULL;
		}
	}
	*pp = dp;
	FNC_RETURN(rc);
}

