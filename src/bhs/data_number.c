#include <bhs/data.h>
/*
 *	-----------------------------------------------------------------------
 */
#if	defined(__SUNPRO_C)
data_des_t	_data_number_designator = {
	.des_name	= "DATA_NUMBER",
	.des_init	= data_num_init,
	.des_new	= data_num_new,
	.des_fini	= data_num_fini,
	.des_drop	= data_num_drop,
	.des_cpy	= data_num_cpy,
	.des_cmp	= data_num_cmp,
	.des_prn	= data_num_prn
};

#elif	defined(__GNUC__)
data_des_t	_data_number_designator = {
	 des_name	: "DATA_NUMBER",
	 des_init	: data_num_init,
	 des_new	: data_num_new,
	 des_fini	: data_num_fini,
	 des_drop	: data_num_drop,
	 des_cpy	: data_num_cpy,
	 des_cmp	: data_num_cmp,
	 des_prn	: data_num_prn
};

#else
#error("used compiler is not yet supported!")
#endif /* compiler */

/*
	-----------------------------------------------------------------------
 */
int
data_num_init(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_id_t	*ident,
	number_t	 number
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);

	TRY(data_init, _door, sp, ident, dp);
	DES(dp) = DATA_NUMBER_DESIGNATOR;
	DNUM(dp) = number;

_door:
	FNC_RETURN(rc);
}

int
data_num_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*ident,
	number_t	 number,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	TRY(data_new, _door, sp, ident, &dp);
	DES(dp) = DATA_NUMBER_DESIGNATOR;
	DNUM(dp) = number;

_door:
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_num_fini(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_NUMBER_DESIGNATOR);

	if(DLL_ISEMPTY(&p->data_refby) != TRUE) {
		goto _door;
	}
_door:
	FNC_RETURN(rc);
}

int
data_num_drop(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_NUMBER_DESIGNATOR);

	if(DLL_ISEMPTY(&p->data_refby) != TRUE) {
		goto _door;
	}
	if(p->data_free != NULL) {
		TRY(data_num_fini, _door, sp, p);
		TRY(_data_drop, _door, sp, p);
	}

_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_num_cpy(
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
	CHK_DES(rc, src, DATA_NUMBER_DESIGNATOR);

	TRY(_data_cpy, _door, sp, src, &p);
	DNUM(p) = DNUM(src);

_door:
	*dst = p;
	FNC_RETURN(rc);
}

int
data_num_cmp(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p1,
	data_t		*p2,
	data_code_t	*pp
) {
	int		 rc = FNC_OKAY;
	data_code_t	 code = DATA_NIL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p1);
	FNC_CHKINP(rc, p2);
	FNC_CHKOUTP(rc, code);
	CHK_DES(rc, p1, DATA_NUMBER_DESIGNATOR);

	if(p1 == p2) {
		code = DATA_EQ;
		goto _door;
	}
	if(DES(p1) != DES(p2)) {
		code = DATA_NE;
		goto _door;
	}
	code = (DNUM(p1) == DNUM(p2)) ? DATA_EQ :
		(DNUM(p1) > DNUM(p2)) ?  DATA_GT : DATA_LT;

_door:
	*pp = code;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_num_prn(
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
	CHK_DES(rc, p, DATA_NUMBER_DESIGNATOR);

	TRY(data_hdr_prn, _door, sp, p, out, n);
	n++;
	PRNN(n); PRNO("dec : %lld\n", DNUM(p));
	if(DATA_DBG(sp) >= DATA_SHORT_DBG) {
		PRNN(n); PRNO("hex : 0x%llx\n", DNUM(p));
		PRNN(n); PRNO("oct : O_%llo\n", DNUM(p));
	}
	if(DATA_DBG(sp) >= DATA_LONG_DBG) {
		PRNN(n); PRNO("data_free = 0x%p\n", DFREE(p));
	}
	n--;
	PRNN(n); PRNO("}\n");

_door:
	FNC_RETURN(rc);
}

