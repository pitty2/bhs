#include <bhs/dbg.h>
#include <bhs/data.h>

/*	TODO:
	value:
		string und list auf pointer umstellen.
		DONE: 22:08.2009
	designator:
		put- und get-(value)methoden implementieren
 */

char	*data_cause[] = {
	"DATA okay",
	"DATA malloc() failed!",
	"DATA syscall failed!",
	"DATA wrong designator!",
	"DATA not in list!",
	"DATA hirarchie out of band!",
	"DATA conversion error!",
	"DATA ciurcuit!",
	"DATA no method!",
	"DATA wrong identifier!"
};

/*
	-----------------------------------------------------------------------
 */
#if	defined(__SUNPRO_C)
data_des_t	_data_designator = {
	.des_name	= "DATA_",
	.des_init	= data_init,
	.des_new	= data_new,
	.des_fini	= _data_fini,
	.des_drop	= _data_drop,
	.des_cpy	= _data_cpy,
	.des_cmp	= _data_cmp,
	.des_prn	= _data_prn
};

#elif	defined(__GNUC__)
data_des_t	_data_designator = {
	des_name	: "DATA_",
	des_init	: data_init,
	des_new		: data_new,
	des_fini	: _data_fini,
	des_drop	: _data_drop,
	des_cpy		: _data_cpy,
	des_cmp		: _data_cmp,
	des_prn		: _data_prn
};

#else
#error("used compiler is not yet supported!")
#endif /* compiler */

/*
	-----------------------------------------------------------------------
 */
int
data_size(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p,
	leng_t		*sz
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);
	FNC_CHKOUTP(rc, sz);

	*sz = (leng_t)sizeof(*p);
	FNC_RETURN(rc);
}

int
data_clear_refby(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*p
) {
	int		 rc = FNC_OKAY;
	data_link_t	*lnk = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	while((lnk = (data_link_t *)DLL_FIRST(DREF(p))) != NULL) {
		data_assoc_t	*asp = NULL;

		TRY(data_link_2assoc, _door, sp, lnk, &asp);
		TRY(data_assoc_drop, _door, sp, asp);
	}
_door:
	FNC_RETURN(rc);
}
/*
	-----------------------------------------------------------------------
 */
int
data_init(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*ident,
	data_t		*p
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	if(ident != NULL) {
		FNC_CHKINP(rc, ident);
		CHK_DES(rc, ident, DATA_IDENT_DESIGNATOR);
	}
	memset(p, 0, sizeof(*p));
	dll_init(DREF(p));
	DES(p) = DATA_DESIGNATOR;
	DID(p) = ident;
	FNC_RETURN(rc);
}

int
data_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_id_t	*ident,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	if((p = MALLOC(sizeof(*p))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "MALLOC(%llu) failed!", (leng_t)sizeof(*p));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	TRY(data_init, _door, sp, ident, p);
	DFREE(p) = FREE_FNC;

_door:
	if((rc != FNC_OKAY) && (p != NULL)) {
		FREE(p);
		p = NULL;
	}
	*pp = p;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
_data_fini(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);

	if(DLL_ISEMPTY(DREF(dp)) != TRUE) {
		 goto _door;
	}
	if(DID(dp) != NULL) {
		TRY(data_id_drop, _door, sp, DID(dp));
		DID(dp) = NULL;
	}
	if(SEQU(dp) != NULL) {
		TRY(data_sequ_drop, _door, sp, SEQU(dp));
		SEQU(dp) = NULL;
	}
_door:
	FNC_RETURN(rc);
}

int
_data_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);

	if(DLL_ISEMPTY(DREF(dp)) != TRUE) {
		goto _door;
	}
	TRY(_data_fini, _door, sp, dp);
	if(DFREE(dp) != NULL) {
		DFREE(dp)(dp);
	}
_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
_data_cpy(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*src,
	data_t		**dst
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_id_t	*id = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, dst);

	if(DID(src) != NULL) {
		if(DID(src)->data_free != NULL) {
			TRY(data_id_cpy, FNC_RET, sp, DID(src), &id);
		} else {
			id = DID(src);
		}
	}
	TRY(data_new, _door, sp, id, &dp);
	id = NULL;
	DES(dp) = DES(src);
	TRY(data_sequ_inherit, _door, sp, src, dp);

_door:
	if(rc != FNC_OKAY) {
		if(id != NULL) {
			data_drop(NEXT_FNCRC, sp, id);
			id = NULL;
		}
		if(dp != NULL) {
			data_drop(NEXT_FNCRC, sp, dp);
			dp = NULL;
		}
	}
	*dst = dp;
FNC_RET:
	FNC_RETURN(rc);
}

int
_data_cmp(
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
	FNC_CHKOUTP(rc, pp);

	if(p1 == p2) {
		code = DATA_EQ;
		goto FNC_EXIT;
	}
	if(DES(p1) != DES(p2)) {
		code = DATA_NE;
	}
FNC_EXIT:
	*pp = code;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_hdr_2cstr(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	char		**pp
) {
	int		 rc = FNC_OKAY;
	char		*msg = NULL;
	char		*id_str = NULL;
	char		*sequ_str = NULL;
	char		*des_str = NULL;
	char		*cstr = NULL;
	leng_t		 refcnt = 0ULL;
	leng_t		 len = 0ULL;
	int		 sysrc = 0;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);
	FNC_CHKOUTP(rc, pp);

	if((msg = CALLOC(1, FNC_MSG_SIZE +1)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (FNC_MSG_SIZE +1));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}	

	if(DES(dp) != NULL) {
		des_str = DES(dp)->des_name;
	}
	if(DID(dp) != NULL) {
		id_str = DCSTR(DID(dp));
	}
	refcnt = dll_count(DREF(dp));

	if(DATA_DBG(sp) <= DATA_SHORT_DBG) {
		if(DES(dp) == DATA_LIST_DESIGNATOR) {
			len = dll_count(DLST(dp));
		} else if(DES(dp) == DATA_STRING_DESIGNATOR) {
			len = STR_LEN(DSTR(dp));
		} else {
			len = sizeof(data_value_t);
		}
		sysrc = snprintf(msg, FNC_MSG_SIZE,
			"%s [%s] (%llu/%llu)",
			TSTR(id_str), TSTR(des_str), refcnt, len);

	} else if(DATA_DBG(sp) >= DATA_SEQU_DBG) {
		TRY(data_sequ_2cstr, _door, sp, SEQU(dp), &sequ_str);
		if(DES(dp) == DATA_LIST_DESIGNATOR) {
			len = dll_count(DLST(dp));
		} else if(DES(dp) == DATA_STRING_DESIGNATOR) {
			len = STR_LEN(DSTR(dp));
		} else {
			len = sizeof(data_value_t);
		}
		sysrc = snprintf(msg, FNC_MSG_SIZE,
			"%s/%s[%s] (%llu/%llu)",
			TSTR(id_str), TSTR(sequ_str), TSTR(des_str), refcnt, len);
	}
	if(sysrc < 0) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_SYSCALL);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "snprintf() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	
	if((cstr = STRDUP(msg)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "STRDUP() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	}
_door:
	if(msg != NULL) {
		FREE(msg);
		msg = NULL;
	}
	if(sequ_str != NULL) {
		FREE(sequ_str);
		sequ_str = NULL;
	}
	*pp = cstr;
	FNC_RETURN(rc);
}

int
data_refby_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;
	char		*hdr_str = NULL;
	data_link_t	*lnk = NULL;
	data_t		*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);

	PRNN(n); PRNO("data_refby : (%lld) {\n", dll_count(DREF(dp)));
	n++;
	DLL_WALK(lnk, data_link_t, DREF(dp)) {
		TRY(data_link_2parent, _door, sp, lnk, &p);
		TRY(data_hdr_2cstr, _door, sp, p, &hdr_str);
		PRNN(n); PRNO("%s\n", TSTR(hdr_str))
		FREE(hdr_str);
		hdr_str = NULL;
	}
_door:
	n--;
	PRNN(n); PRNO("}\n");
	FNC_RETURN(rc);
}

int
data_hdr_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;
	char		*hdr_str = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);

	TRY(data_hdr_2cstr, _door, sp, dp, &hdr_str);

	if(DATA_DBG(sp) <= DATA_SHORT_DBG) {
		PRNN(n); PRNO("%s {\n", TSTR(hdr_str));
	} else if(DATA_DBG(sp) == DATA_ADDR_DBG) {
		PRNN(n); PRNO("(0x%p) %s {\n", dp, TSTR(hdr_str));
	} else if(DATA_DBG(sp) >= DATA_LONG_DBG) {
		PRNN(n); PRNO("(0x%p) %s {\n", dp, TSTR(hdr_str));
		TRY(data_refby_prn, _door, sp, dp, out, n+1);
	} else {
		PRNN(n); PRNO("%s {\n", TSTR(hdr_str));
	}
	
_door:
	if(hdr_str != NULL) {
		FREE(hdr_str);
		hdr_str = NULL;
	}
	FNC_RETURN(rc);
}

int
_data_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);

	if(DATA_DBG(sp) == DATA_NONE_DBG) {
		goto _door;
	}
	TRY(data_hdr_prn, _door, sp, dp, out, n);
	PRNN(n); PRNO("}\n");
_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_fini(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);

	if(DES_FINI(dp) != NULL) {
		TRY(DES_FINI(dp), _door, sp, dp);
	} else {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_METHOD);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "DES_FINI() not found!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	} 
_door:
	FNC_RETURN(rc);
}

int
data_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);

	if(DES_DROP(dp) != NULL) {
		TRY(DES_DROP(dp), _door, sp, dp);
	} else {
		rc= FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_METHOD);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "DES_DROP() not found!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	}
_door:
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_cpy(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*src,
	data_t		**dst
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, dst);

	if(DES_CPY(src) != NULL) {
		TRY(DES_CPY(src), _door, sp, src, &dp);
	} else {
		rc= FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_METHOD);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "DES_CPY() not found!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	} 
_door:
	*dst = dp;
	FNC_RETURN(rc);
}

int
data_cmp(
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
	FNC_CHKOUTP(rc, pp);

	if(DES_CMP(p1) != NULL) {
		TRY(DES_CMP(p1), _door, sp, p1, p2, &code);
	} else {
		rc= FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_METHOD);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "DES_CMP() not found!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	} 
_door:
	*pp = code;
	FNC_RETURN(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
data_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	data_t		*dp,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, dp);

	if(DES_PRN(dp) != NULL) {
		TRY(DES_PRN(dp), _door, sp, dp, out, n);
	} else {
		rc= FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(DATA_CAUSE_METHOD);
		FNCRC_SYSERR(0);
		FNC_TXT(fncrc, "DES_PRN() not found!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	}
_door:
	FNC_RETURN(rc);
}

