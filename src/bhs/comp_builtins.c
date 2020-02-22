#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/utsname.h>
#include <bhs/compile.h>

DECLARE_DATA_IDENT(builtin_id,			"builtin");

/*
	------------------------------------------------------------------------
 */
DECLARE_DATA_IDENT(env_builtin_id,			"builtin getenv()");
DECLARE_DATA_FUNC(env_builtin,	&env_builtin_id,	env_builtin_fnc);

int
env_builtin_fnc(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	leng_t		 argc,
	data_link_t	*argv,
	data_t		**pp

) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	char		*cstr = NULL;
	char		*vstr = NULL;
	data_t		*fnc = NULL;
	data_link_t	*lnk1 = NULL;
	data_t		*arg1 = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2child,	FNC_ABORT, sp, argv, &fnc);
	TRY(data_lst_next,	FNC_ABORT, sp, argv, &lnk1);
	TRY(data_link_2child,	FNC_ABORT, sp, lnk1, &arg1);
	TRY(data_str_2cstr,	FNC_ABORT, sp, arg1, &cstr);

	if((vstr = getenv((const char*)cstr)) == NULL) {
		TRY(comp_env_error, FNC_ABORT, sp, cp, arg1, &dp);
	} else {
		leng_t	 len = 0ULL;

		len = strlen(vstr);
		TRY(data_str_new, FNC_ABORT, sp, STRING, len, vstr, &dp);
	}

FNC_ABORT:
	if(cstr != NULL) {
		FREE(cstr);
	}
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
DECLARE_DATA_IDENT(date_builtin_id,			"builtin strftime()");
DECLARE_DATA_FUNC(date_builtin,	&date_builtin_id,	date_builtin_fnc);

int
date_builtin_fnc(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	leng_t		 argc,
	data_link_t	*argv,
	data_t		**pp

) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	char		*cstr = NULL;
	char		*vstr = NULL;
	data_t		*fnc = NULL;
	data_link_t	*lnk1 = NULL;
	data_t		*arg1 = NULL;

	time_t		*secs = NULL;
	struct tm	*ltime = NULL;
	leng_t		 len = 0ULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2child,	FNC_ABORT, sp, argv, &fnc);
	TRY(data_lst_next,	FNC_ABORT, sp, argv, &lnk1);
	TRY(data_link_2child,	FNC_ABORT, sp, lnk1, &arg1);
	TRY(data_str_2cstr,	FNC_ABORT, sp, arg1, &cstr);

	if((secs = CALLOC(1, sizeof(*secs))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto FNC_ABORT;
	}
	if(time(secs) == (time_t)(-1)) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_SYSCALL);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "time() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto FNC_ABORT;
	}
	if((ltime = CALLOC(1, sizeof(*ltime))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto FNC_ABORT;
	}
	if(localtime_r(secs, ltime) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_SYSCALL);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "localtime_r() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto FNC_ABORT;
	}
	if((vstr = CALLOC(1, (DATE_STR_LEN +1))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto FNC_ABORT;
	}
	if(strftime(vstr, DATE_STR_LEN, cstr, ltime) == 0) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_SYSCALL);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "strftime(vstr=0x%p, max=%d, cstr=0x%p, ltime=0x%p) failed!",
			vstr, DATE_STR_LEN, cstr, ltime);
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto FNC_ABORT;
	}
	
	len = strlen(vstr);
	TRY(data_str_new, FNC_ABORT, sp, STRING, len, vstr, &dp);

FNC_ABORT:
	if(ltime != NULL) {
		FREE(ltime);
	}
	if(secs != NULL) {
		FREE(secs);
	}
	if(cstr != NULL) {
		FREE(cstr);
	}
	if(vstr != NULL) {
		FREE(vstr);
	}
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
DECLARE_DATA_IDENT(uname_builtin_id,			"builtin uname()");
DECLARE_DATA_FUNC(uname_builtin, &uname_builtin_id,	uname_builtin_fnc);

int
uname_builtin_fnc(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	leng_t		 argc,
	data_link_t	*argv,
	data_t		**pp

) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*lnk = NULL;
	data_t		*lst = NULL;
	data_t		*arg = NULL;
	struct utsname	*ptr = NULL;
	data_t		*asgn = NULL;
	char		*msg = NULL;
	int		 sysrc = 0;

	char		*cstr = NULL;
	char		*ustr = NULL;
	leng_t		 len = 0ULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	if((ptr = CALLOC(1, sizeof(*ptr))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)sizeof(*ptr));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto FNC_ABORT;
	}
	if(uname(ptr) < 0) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_SYSCALL);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "uname() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto FNC_ABORT;
	}
	TRY(data_link_2parent, FNC_ABORT, sp, argv, &lst);
	argc = dll_count(DLST(lst));
	TRY(data_lst_next, FNC_ABORT, sp, argv, &lnk);
	TRY(data_link_2child, FNC_ABORT, sp, lnk, &arg);
	cstr = DCSTR(arg);
	len = strlen(cstr);

	if(strcmp(cstr, "sysname") == 0) {
		ustr = ptr->sysname;
	} else if(strcmp(cstr, "nodename") == 0) {
		ustr = ptr->nodename;
	} else if(strcmp(cstr, "release") == 0) {
		ustr = ptr->release;
	} else if(strcmp(cstr, "version") == 0) {
		ustr = ptr->version;
	} else if(strcmp(cstr, "machine") == 0) {
		ustr = ptr->machine;
	} else {
		if((msg = CALLOC(1, COMP_MSG_LEN +1)) == NULL) {
			rc = FNC_FATAL;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(FNC_CAUSE_NOMEM);
			FNCRC_SYSERR(errno);
			FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (COMP_MSG_LEN +1));
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto FNC_ABORT;
		}	
		sysrc = snprintf(msg, COMP_MSG_LEN,
			"%s:%lld: builtin error, uname() : \"%s\" invalid qualifier!",
			TSTR(SEQU(arg)->name), arg->data_sequ->num,
			TSTR(cstr));
		if(sysrc < 0) {
			rc = FNC_FATAL;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(FNC_CAUSE_SYSCALL);
			FNCRC_SYSERR(errno);
			FNC_TXT(fncrc, "snprintf() failed!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto FNC_ABORT;
		}
		FPRINTF(sp->sess_err, "%s\n", msg);
		TRY(comp_err_new, FNC_ABORT, sp, msg, &dp);
		goto FNC_EXIT;
	}
	TRY(data_str_new, FNC_ABORT,sp, STRING, len, ustr, &dp);

FNC_ABORT:
	if(rc != FNC_OKAY) {
		if(dp != NULL) {
			data_drop(sess_next_fncrc(sp), sp, dp);
			dp = NULL;
		}
		if(asgn != NULL) {
			data_drop(sess_next_fncrc(sp), sp, asgn);
			asgn = NULL;
		}
	}
	if(msg != NULL) {
		FREE(msg);
		msg = NULL;
	}
	if(ptr != NULL) {
		FREE(ptr);
		ptr = NULL;
	}
FNC_EXIT:
	*pp = dp;
	FNC_RETURN(rc);
}
/*
	------------------------------------------------------------------------
 */
DECLARE_DATA_IDENT(len_builtin_id,			"builtin len()");
DECLARE_DATA_FUNC(len_builtin,	&len_builtin_id,	len_builtin_fnc);

int
len_builtin_fnc(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	leng_t		 argc,
	data_link_t	*argv,
	data_t		**pp

) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_t		*fnc = NULL;
	data_link_t	*lnk = NULL;
	data_t		*arg = NULL;
	leng_t		 _argc = 0ULL;
	leng_t		 num = 0LL;
	int		 sysrc = 0;
	char		*msg = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2child,	FNC_ABORT, sp, argv, &fnc);
	TRY(data_lst_next,	FNC_ABORT, sp, argv, &lnk);
	TRY(data_cnt_by_lnk,	FNC_ABORT, sp, lnk, &_argc);
	TRY(cp->comp_eval,	FNC_ABORT, sp, cp, _argc, lnk, &arg);

	if(comp_id_cmp(DID(arg), BOOLEAN) == DATA_EQ) {
		num = sizeof(bool_t);

	} else if(comp_id_cmp(DID(arg), NUMERIC) == DATA_EQ) {
		num = sizeof(number_t);

	} else if(comp_id_cmp(DID(arg), STRING) == DATA_EQ) {
		TRY(data_str_len, FNC_ABORT, sp, arg, &num);

	} else if(comp_id_cmp(DID(arg), ARRAY) == DATA_EQ) {
		TRY(data_lst_cnt, FNC_ABORT, sp, arg, &num);

	} else if(comp_id_cmp(DID(arg), STRUCT) == DATA_EQ) {
		TRY(data_lst_cnt, FNC_ABORT, sp, arg, &num);

	} else {
		if((msg = CALLOC(1, COMP_MSG_LEN +1)) == NULL) {
			rc = FNC_FATAL;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(FNC_CAUSE_NOMEM);
			FNCRC_SYSERR(errno);
			FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (COMP_MSG_LEN +1));
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto FNC_ABORT;
		}	
		sysrc = snprintf(msg, COMP_MSG_LEN,
			"%s:%lld: builtin error, len() : \"%s\" invalid object!",
			TSTR(SEQU(arg)->name), arg->data_sequ->num,
			TSTR(DES(arg)->des_name));
		if(sysrc < 0) {
			rc = FNC_FATAL;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(FNC_CAUSE_SYSCALL);
			FNCRC_SYSERR(errno);
			FNC_TXT(fncrc, "snprintf() failed!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto FNC_ABORT;
		}
		FPRINTF(sp->sess_err, "%s\n", msg);
		TRY(comp_err_new, FNC_ABORT, sp, msg, &dp);
		goto FNC_EXIT;
	}
	TRY(data_num_new, FNC_ABORT, sp, NUMERIC, num, &dp);

FNC_ABORT:
	if(msg != NULL) {
		FREE(msg);
		msg = NULL;
	}
	if(rc != FNC_OKAY) {
		if(dp != NULL) {
			data_drop(sess_next_fncrc(sp), sp, dp);
			dp = NULL;
		}
	}
	if(arg != NULL) {
		if(DLL_ISEMPTY(&arg->data_refby)) {
			data_drop(sess_next_fncrc(sp), sp, arg);
			arg = NULL;
		}
	}
FNC_EXIT:
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
DECLARE_DATA_IDENT(pwd_builtin_id,			"builtin pwd()");
DECLARE_DATA_FUNC(pwd_builtin,	&pwd_builtin_id,	pwd_builtin_fnc);

int
pwd_builtin_fnc(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	leng_t		 argc,
	data_link_t	*argv,
	data_t		**pp

) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	char		*ptr = NULL;
	char		*buf = NULL;
	leng_t		 len = 0ULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	if((buf = CALLOC(1, PATH_MAX +1)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)(PATH_MAX +1));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto FNC_ABORT;
	}
	if((ptr = getcwd(buf, PATH_MAX)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_SYSCALL);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "getcwd(%p, %llu) failed!", buf, (leng_t)(PATH_MAX +1));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto FNC_ABORT;
	}
	len = strlen(ptr);
	TRY(data_str_new, FNC_ABORT, sp, STRING, len, ptr, &dp);

FNC_ABORT:
	if(buf != NULL) {
		FREE(buf);
		buf = NULL;
	}
	*pp = dp;
	FNC_RETURN(rc);
}
