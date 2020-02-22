#include <bhs/data.h>


/*
	-----------------------------------------------------------------------
 */
dll_t	data_id_list = {
	&data_id_list,
	&data_id_list,
	&data_id_list
};

/*
 	----------------------------------------------------------------------
 */
#if	defined(__SUNPRO_C)
data_des_t	_data_id_designator = {
	.des_name	= "DATA_IDENTIFIER",
	.des_init	= NULL,
	.des_new	= data_id_new,
	.des_fini	= NULL,
	.des_drop	= data_id_drop,
	.des_cpy	= data_id_cpy,
	.des_cmp	= data_id_cmp,
	.des_prn	= data_id_prn
};

#elif	defined(__GNUC__)
data_des_t	_data_id_designator = {
	 des_name	: "DATA_IDENTIFIER",
	 des_init	: NULL,
	 des_new	: data_id_new,
	 des_fini	: NULL,
	 des_drop	: data_id_drop,
	 des_cpy	: data_id_cpy,
	 des_cmp	: data_id_cmp,
	 des_prn	: data_id_prn
};

#else
#error("used compiler is not yet supported!")
#endif /* compiler */
/*
	-----------------------------------------------------------------------
 */
int
data_id_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	char		*name,
	data_id_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_id_t	*p = NULL;
	leng_t		 len = 0ULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, name);
	FNC_CHKOUTP(rc, pp);

	len = strlen(name);
	TRY(data_str_new, _door, sp, NULL, len, name, &p);
	DES(p) = DATA_IDENT_DESIGNATOR;
	*pp = p;

_door:
	FNC_RETURN(rc);
}

int
data_id_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*p
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_IDENT_DESIGNATOR);

	if(DFREE(p) != NULL) {
		TRY(str_drop, _door, DSTR(p));
		DFREE(p)(p);
	}

_door:
	FNC_RETURN(rc);
}

int
data_id_cpy(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*src,
	data_id_t	**dst
) {
	int		 rc = FNC_OKAY;
	data_id_t	*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, dst);
	CHK_DES(rc, src, DATA_IDENT_DESIGNATOR);

	if(src->data_free != NULL) {
		TRY(data_id_new, _door, sp, DCSTR(src), &p);
	} else {
		p = src;
	}

_door:
	*dst = p;
	FNC_RETURN(rc);
}

int
data_id_cmp(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t 	*p1,
	data_id_t 	*p2,
	data_code_t	*pp
) {
	int		 rc = FNC_OKAY;
	data_code_t	 code = DATA_NIL;
	int		 str_code = 0;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p1);
	FNC_CHKINP(rc, p2);
	FNC_CHKOUTP(rc, code);
	CHK_DES(rc, p1, DATA_IDENT_DESIGNATOR);
	CHK_DES(rc, p2, DATA_IDENT_DESIGNATOR);

	if(p1 == p2) {
		code = DATA_EQ;
		goto _door;
		
	}
	str_code = str_cmp(DSTR(p1), DSTR(p2));
	code = (str_code == 0) ? DATA_EQ : (str_code > 0) ? DATA_GT : DATA_LT;
_door:
	*pp = code;
	FNC_RETURN(rc);
}

int
data_id_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*p,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);

	if(p != NULL) {
		PRNN(n); PRNO("%s", TSTR(DCSTR(p)));
	} else {
		PRNN(n); PRNO("{nil}");
	}
	FNC_RETURN(rc);
}

int
data_id_set(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_id_t	*id
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKINP(rc, id);
	CHK_DES(rc, id, DATA_IDENT_DESIGNATOR);

	if(DID(p) != NULL) {
		TRY(data_id_drop, _door, sp, DID(p));
		DID(p) = NULL;
	}
	DID(p) = id;

_door:
	FNC_RETURN(rc);
}

int
data_id_get(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_id_t	**pp
) {
	int		 rc = FNC_OKAY;
	data_id_t	*id = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKOUTP(rc, pp);

	id = DID(p);
	*pp = id;
	FNC_RETURN(rc);
}

int
data_id_2cstr(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*p,
	char		**cstr
) {
	int		 rc = FNC_OKAY;
	char		*s = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKOUTP(rc, cstr);
	CHK_DES(rc, p, DATA_IDENT_DESIGNATOR);

	TRY(str_2cstr, _door, DSTR(p), &s);

_door:
	*cstr = s;
	FNC_RETURN(rc);
}


