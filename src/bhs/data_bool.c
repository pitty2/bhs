#include <bhs/data.h>
/*
 *	-----------------------------------------------------------------------
 */
#if	defined(__SUNPRO_C)
data_des_t	_data_boolean_designator = {
	.des_name	= "DATA_BOOLEAN",
	.des_init	= data_bool_init,
	.des_new	= data_bool_new,
	.des_fini	= data_bool_fini,
	.des_drop	= data_bool_drop,
	.des_cpy	= data_bool_cpy,
	.des_cmp	= data_bool_cmp,
	.des_prn	= data_bool_prn
};

#elif	defined(__GNUC__)
data_des_t	_data_boolean_designator = {
	 des_name	: "DATA_BOOLEAN",
	 des_init	: data_bool_init,
	 des_new	: data_bool_new,
	 des_fini	: data_bool_fini,
	 des_drop	: data_bool_drop,
	 des_cpy	: data_bool_cpy,
	 des_cmp	: data_bool_cmp,
	 des_prn	: data_bool_prn
};

#else
#error("used compiler is not yet supported!")
#endif /* compiler */

/*
	-----------------------------------------------------------------------
 */
int
data_bool_init(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_id_t	*ident,
	bool_t		 val
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);

	TRY(data_init, _door, sp, ident, dp);
	DES(dp) = DATA_BOOL_DESIGNATOR;
	DBOOL(dp) = val;
_door:
	FNC_RETURN(rc);
}

int
data_bool_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*ident,
	bool_t		 val,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	TRY(data_new, _door, sp, ident, &dp);
	DES(dp) = DATA_BOOL_DESIGNATOR;
	DBOOL(dp) = val;
_door:
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_bool_fini(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_BOOL_DESIGNATOR);

	if(DLL_ISEMPTY(DREF(p)) != TRUE) {
		goto _door;
	}
_door:
	FNC_RETURN(rc);
}

int
data_bool_drop(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_BOOL_DESIGNATOR);

	if(DLL_ISEMPTY(DREF(p)) != TRUE) {
		goto _door;
	}
	if(DFREE(p) != NULL) {
		TRY(data_bool_fini, _door, sp, p);
		TRY(_data_drop, _door, sp, p);
	}
_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_bool_cpy(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*src,
	data_t	**dst
) {
	int	 rc = FNC_OKAY;
	data_t	*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, dst);
	CHK_DES(rc, src, DATA_BOOL_DESIGNATOR);

	TRY(_data_cpy, _door, sp, src, &p);
	DBOOL(p) = DBOOL(src);
_door:
	*dst = p;
	FNC_RETURN(rc);
}

int
data_bool_cmp(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t 		*p1,
	data_t 		*p2,
	data_code_t	*pp
) {
	int		 rc = FNC_OKAY;
	data_code_t	 code = DATA_NIL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p1);
	FNC_CHKINP(rc, p2);
	FNC_CHKOUTP(rc, code);
	CHK_DES(rc, p1, DATA_BOOL_DESIGNATOR);

	if(p1 == p2) {
		code = DATA_EQ;
		goto _door;
	}
	if(DES(p1) != DES(p2)) {
		code = DATA_NE;
		goto _door;
	}
	code = (DBOOL(p1) == DBOOL(p2)) ? DATA_EQ : DATA_NE;
_door:
	*pp = code;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_bool_prn(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p,
	FILE	*out,
	leng_t	 n
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_BOOL_DESIGNATOR);

	TRY(data_hdr_prn, _door, sp, p, out, n);
	n++;
	PRNN(n); PRNO("%s\n", DBOOL(p) == TRUE ? "TRUE" : "FALSE" );
	if(DATA_DBG(sp) >= DATA_LONG_DBG) {
		PRNN(n); PRNO("data_free = 0x%p\n", DFREE(p));
	}
	n--;
	PRNN(n); PRNO("}\n");
_door:
	FNC_RETURN(rc);
}
