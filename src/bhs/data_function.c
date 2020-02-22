#if defined( SunOS ) || defined( Linux )
#ifdef Linux
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
#endif
#include <bhs/data.h>

/*
	-----------------------------------------------------------------------
 */
#if	defined(__SUNPRO_C)
data_des_t	_data_function_designator = {
	.des_name	= "DATA_FUNCTION",
	.des_init	= data_fnc_init,
	.des_new	= data_fnc_new,
	.des_fini	= data_fnc_fini,
	.des_drop	= data_fnc_drop,
	.des_cpy	= data_fnc_cpy,
	.des_cmp	= data_fnc_cmp,
	.des_prn	= data_fnc_prn
};

#elif	defined(__GNUC__)
data_des_t	_data_function_designator = {
	 des_name	: "DATA_FUNCTION",
	 des_init	: data_fnc_init,
	 des_new	: data_fnc_new,
	 des_fini	: data_fnc_fini,
	 des_drop	: data_fnc_drop,
	 des_cpy	: data_fnc_cpy,
	 des_cmp	: data_fnc_cmp,
	 des_prn	: data_fnc_prn
};

#else
#error("used compiler is not yet supported!")
#endif /* compiler */

/*
	-----------------------------------------------------------------------
 */
int
data_fnc_init(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	data_id_t	*ident,
	data_fnc	 fnc
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);

	if(ident != NULL) {
		FNC_CHKINP(rc, ident);
	}
	TRY(data_init, _door, sp, ident, dp);
	DES(dp) = DATA_FUNC_DESIGNATOR;
	DFUNC(dp) = fnc;
_door:
	FNC_RETURN(rc);
}

int
data_fnc_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*ident,
	data_fnc	 fnc,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	if(ident != NULL) {
		FNC_CHKINP(rc, ident);
	}
	TRY(data_new, _door, sp, ident, &dp);
	DES(dp) = DATA_FUNC_DESIGNATOR;
	DFUNC(dp) = fnc;
_door:
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_fnc_fini(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_FUNC_DESIGNATOR);

	if(DLL_ISEMPTY(DREF(p)) != TRUE) {
		goto _door;
	}
_door:
	FNC_RETURN(rc);
}

int
data_fnc_drop(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_FUNC_DESIGNATOR);

	if(DLL_ISEMPTY(DREF(p)) != TRUE) {
		goto _door;
	}
	if(DFREE(p) != NULL) {
		TRY(data_fnc_fini, _door, sp, p);
		TRY(_data_drop, _door, sp, p);
	}
_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_fnc_cpy(
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
	CHK_DES(rc, src, DATA_FUNC_DESIGNATOR);

	TRY(_data_cpy, _door, sp, src, &p);
	DFUNC(p) = DFUNC(src);
_door:
	*dst = p;
	FNC_RETURN(rc);
}

int
data_fnc_cmp(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p1,
	data_t		*p2,
	data_code_t	*pp
) {
	int		 rc = FNC_OKAY;
	data_code_t	 code = DATA_NE;
	
	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	CHK_DES(rc, p1, DATA_FUNC_DESIGNATOR);

	if(p1 == p2) {
		code = DATA_EQ;
		goto _door;
	}
	if(DES(p1) != DES(p2)) {
		code = DATA_NE;
		goto _door;
	} 
	code = (DFUNC(p1) == DFUNC(p2)) ? DATA_EQ : DATA_NE;
_door:
	*pp = code;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_fnc_prn(
	fncrc_t	*fncrc,
	sess_t	*sp,
	data_t	*p,
	FILE	*out,
	leng_t	 n
) {
	int	 rc = FNC_OKAY;
	int	 flg = 0;

#if defined( SunOS ) || defined ( Linux )
	Dl_info	*fnc_info;
#endif

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	CHK_DES(rc, p, DATA_FUNC_DESIGNATOR);

#if defined( SunOS ) || defined ( Linux )
	if((fnc_info = CALLOC(1, sizeof(*fnc_info))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", sizeof(*fnc_info));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}	

	flg = dladdr((void *)DFUNC(p), fnc_info);
#endif

	TRY(data_hdr_prn, _door, sp, p, out, n);
	n++;
#if defined( SunOS ) || defined ( Linux )
	PRNN(n); PRNO("%s (0x%p)\n",
			TSTR(fnc_info->dli_sname), DFUNC(p));
#else
	PRNN(n); PRNO(" (0x%p)\n",
			DFUNC(p));
#endif
	if(DATA_DBG(sp) >= DATA_LONG_DBG) {
		PRNN(n); PRNO("data_free = 0x%p\n", DFREE(p));
	}
	n--;
	PRNN(n); PRNO("}\n");
_door:
#if defined( SunOS ) || defined ( Linux )
	if(fnc_info != NULL) {
		FREE(fnc_info);
	}
#endif
	FNC_RETURN(rc);
}

