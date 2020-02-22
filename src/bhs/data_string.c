#include <bhs/dbg.h>
#include <bhs/data.h>

/*
 	----------------------------------------------------------------------
 */
#if	defined(__SUNPRO_C)
data_des_t	_data_string_designator = {
	.des_name	= "DATA_STRING",
	.des_init	= data_str_init,
	.des_new	= data_str_new,
	.des_fini	= data_str_fini,
	.des_drop	= data_str_drop,
	.des_cpy	= data_str_cpy,
	.des_cmp	= data_str_cmp,
	.des_prn	= data_str_prn
};

#elif	defined(__GNUC__)
data_des_t	_data_string_designator = {
	 des_name	: "DATA_STRING",
	 des_init	: data_str_init,
	 des_new	: data_str_new,
	 des_fini	: data_str_fini,
	 des_drop	: data_str_drop,
	 des_cpy	: data_str_cpy,
	 des_cmp	: data_str_cmp,
	 des_prn	: data_str_prn
};

#else
#error("used compiler is not yet supported!")
#endif /* compiler */

/*
 	----------------------------------------------------------------------
 */
int
data_str_init(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_id_t	*ident,
	leng_t		 len,
	char		*word
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKINP(rc, word);

	TRY(data_init, _door, sp, ident, p);
	TRY(str_init, _door, DSTR(p), len, (byte_t *)word);
	DES(p) = DATA_STRING_DESIGNATOR;

_door:
	FNC_RETURN(rc);
}

int
data_str_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*ident,
	leng_t		 len,
	char		*word,
	data_t		**pp
) {
	int	 rc = FNC_OKAY;
	data_t	*dp = NULL;
	str_t	*str = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, word);
	FNC_CHKOUTP(rc, pp);

	TRY(data_new, _door, sp, ident, &dp);
	TRY(str_new, _door, len, (byte_t *)word, &str);
	DSTR(dp) = str;
	DES(dp) = DATA_STRING_DESIGNATOR;

_door:
	if(rc != FNC_OKAY) {
		if(dp != NULL) {
			FREE(dp);
			dp = NULL;
		}
	}
	*pp = dp;
	FNC_RETURN(rc);
}

/*
 	----------------------------------------------------------------------
 */
int
data_str_fini(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_STRING_DESIGNATOR);

	if(DLL_ISEMPTY(&p->data_refby) != TRUE) {
		goto _door;
	}
	TRY(str_fini, _door, DSTR(p));

_door:
	FNC_RETURN(rc);
}

int
data_str_drop(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_STRING_DESIGNATOR);

	if(DLL_ISEMPTY(&p->data_refby) != TRUE) {
		goto _door;
	}
	if(p->data_free != NULL) {
		TRY(str_drop, _door, DSTR(p));
		DSTR(p) = NULL;
		TRY(_data_drop, _door, sp, p);
	}

_door:
	FNC_RETURN(rc);
}

/*
 	----------------------------------------------------------------------
 */
int
data_str_cpy(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*src,
	data_t		**dst
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;
	leng_t		 len = 0LL;
	char		*cstr = NULL;
	str_t		*str = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, dst);
	CHK_DES(rc, src, DATA_STRING_DESIGNATOR);

	len = str_len(DSTR(src));
	cstr = DCSTR(src);

	TRY(_data_cpy, _door, sp, src, &p);
	TRY(str_new, _door, len, (byte_t *)cstr, &str);
	DSTR(p) = str;

_door:
	if(rc != FNC_OKAY) {
		if(p != NULL) {
			_data_drop(NEXT_FNCRC, sp, p);
			p = NULL;
		}
	}
	*dst = p;
	FNC_RETURN(rc);
}

int
data_str_cmp(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p1,
	data_t		*p2,
	data_code_t	*pp
) {
	int		 rc = FNC_OKAY;
	data_code_t	 code = DATA_NIL;
	int		 i;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p1);
	FNC_CHKINP(rc, p2);
	FNC_CHKOUTP(rc, code);
	CHK_DES(rc, p1, DATA_STRING_DESIGNATOR);

	if(p1 == p2) {
		code = DATA_EQ;
		goto _door;
	}
	if(DES(p1) != DES(p2)) {
		code = DATA_NE;
		goto _door;
	}
	i = str_cmp(DSTR(p1), DSTR(p2));
	code = (i > 0) ? DATA_GT : (i < 0) ? DATA_LT : DATA_EQ;

_door:
	*pp = code;
	FNC_RETURN(rc);
}

/*
 	----------------------------------------------------------------------
 */
int
data_str_prn(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p,
	FILE	*out,
	leng_t	 n
) {
	int	 rc = FNC_OKAY;
	char	*cstr = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_STRING_DESIGNATOR);

	TRY(data_hdr_prn, _door, sp, p, out, n);
	cstr = DCSTR(p);
	n++;
	PRNN(n); PRNO("\"%s\"\n", TSTR(cstr));
	if(DATA_DBG(sp) >= DATA_LONG_DBG) {
		PRNN(n); PRNO("data_free = 0x%p\n", p->data_free);
	}
	n--;
	PRNN(n); PRNO("}\n");

_door:
	FNC_RETURN(rc);
}
