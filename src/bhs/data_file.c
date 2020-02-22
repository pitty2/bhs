#include <bhs/data.h>

/*
	-----------------------------------------------------------------------
 */
#if	defined(__SUNPRO_C)
data_des_t	_data_file_designator = {
	.des_name	= "DATA_FILE",
	.des_init	= data_file_init,
	.des_new	= data_file_new,
	.des_fini	= data_file_fini,
	.des_drop	= data_file_drop,
	.des_cpy	= data_file_cpy,
	.des_cmp	= data_file_cmp,
	.des_prn	= data_file_prn
};

#elif	defined(__GNUC__)
data_des_t	_data_file_designator = {
	 des_name	: "DATA_FILE",
	 des_init	: data_file_init,
	 des_new	: data_file_new,
	 des_fini	: data_file_fini,
	 des_drop	: data_file_drop,
	 des_cpy	: data_file_cpy,
	 des_cmp	: data_file_cmp,
	 des_prn	: data_file_prn
};

#else
#error("used compiler is not yet supported!")
#endif /* compiler */

/*
	-----------------------------------------------------------------------
 */
int
data_file_init(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	data_id_t	*ident,
	char		*name
) {
	int		 rc = FNC_OKAY;
	fdes_t		*fp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKINP(rc, name);

	TRY(data_init, _door, sp, ident, p);
	TRY(fdes_new, _door, name, &fp);
	DES(p) = DATA_FILE_DESIGNATOR;
	DFILE(p) = fp;

_door:
	FNC_RETURN(rc);
}

int
data_file_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*id,
	char		*name,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;
	fdes_t		*fp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, name);
	FNC_CHKOUTP(rc, pp);

	TRY(data_new, _door, sp, id, &p);
	TRY(fdes_new, _door, name, &fp);
	DES(p) = DATA_FILE_DESIGNATOR;
	DFILE(p) = fp;

_door:
	if(rc != FNC_OKAY) {
		if(p != NULL) {
			_data_drop(NEXT_FNCRC, sp, p);	
			p = NULL;
		}
	}
	*pp = p;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_file_fini(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_FILE_DESIGNATOR);

	if(DLL_ISEMPTY(DREF(p)) != TRUE) {
		goto _door;
	}
	TRY(fdes_drop, _door, DFILE(p));
	DFILE(p) = NULL;
	TRY(_data_fini, _door, sp, p);

_door:
	FNC_RETURN(rc);
}

int
data_file_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_FILE_DESIGNATOR);

	if(DLL_ISEMPTY(DREF(p)) != TRUE) {
		goto _door;
	}
	TRY(fdes_drop, _door, DFILE(p));
	DFILE(p) = NULL;
	TRY(_data_drop, _door, sp, p);

_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_file_cpy(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*src,
	data_t	**dst
) {
	int	 rc = FNC_OKAY;
	data_t	*p = NULL;
	fdes_t	*fp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, dst);
	CHK_DES(rc, src, DATA_FILE_DESIGNATOR);

	TRY(_data_cpy, _door, sp, src, &p);
	TRY(fdes_cpy, _door, DFILE(src), &fp);
	DFILE(p) = fp;

_door:
	if(rc != FNC_OKAY) {
		if(p != NULL) {
			data_drop(NEXT_FNCRC, sp, p);
			p = NULL;
		}
	}
	*dst = p;
	FNC_RETURN(rc);
}

int
data_file_cmp(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p1,
	data_t		*p2,
	data_code_t	*pp
) {
	int		 rc = FNC_OKAY;
	data_code_t	 code = DATA_NIL;
	int		 c = 0;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p1);
	FNC_CHKINP(rc, p2);
	FNC_CHKOUTP(rc, code);
	CHK_DES(rc, p1, DATA_FILE_DESIGNATOR);

	if(p1 == p2) {
		code = DATA_EQ;
		goto _door;
	}
	if(DES(p1) != DES(p2)) {
		code = DATA_NE;
		goto _door;
	}
	TRY(fdes_cmp, _door, DFILE(p1), DFILE(p2), &c);
	code = (c == 0) ? DATA_EQ : DATA_NE;

_door:
	*pp = code;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_file_prn(
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
	CHK_DES(rc, p, DATA_FILE_DESIGNATOR);

	TRY(data_hdr_prn, _door, sp, p, out, n);
	n++;
	PRNN(n); PRNO("fd_name   : %s\n",	TSTR(DFILE(p)->fd_name));
	PRNN(n); PRNO("fd_hdl    : 0x%p\n",	DFILE(p)->fd_hdl);
	PRNN(n); PRNO("fd_mode   : %s\n",	TSTR(DFILE(p)->fd_mode));
	PRNN(n); PRNO("fd_lineno : %d\n",	DFILE(p)->fd_lineno);
	PRNN(n); PRNO("fd_buffer : 0x%p\n",	DFILE(p)->fd_buffer);
	if(DATA_DBG(sp) >= DATA_LONG_DBG) {
		PRNN(n); PRNO("data_free = 0x%p\n", DFREE(p));
	}
	n--;
	PRNN(n); PRNO("}\n");

_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_file_open(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p,
	char	*mode
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKINP(rc, mode);
	CHK_DES(rc, p, DATA_FILE_DESIGNATOR);

	TRY(fdes_open, FNC_RET, mode, DFILE(p));

FNC_RET:
	FNC_RETURN(rc);
}

int
data_file_close(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_FILE_DESIGNATOR);

	TRY(fdes_close, FNC_RET, DFILE(p));

FNC_RET:
	FNC_RETURN(rc);
}
