#ifndef __BHS_COMMON_FNC_H__
#define __BHS_COMMON_FNC_H__
/*
	function reporting faramework
 */

#include <bhs/dbg.h>
#include <bhs/dll.h>
#include <bhs/error.h>
#include <bhs/compat.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern	char	*fnc_cause[];
#define	FNC_CAUSE_OKAY		fnc_cause[0]
#define	FNC_CAUSE_INPARAM	fnc_cause[1]
#define	FNC_CAUSE_OUTPARAM	fnc_cause[2]
#define	FNC_CAUSE_INOUTPARAM	fnc_cause[3]
#define	FNC_CAUSE_NOMEM		fnc_cause[4]
#define	FNC_CAUSE_SYSCALL	fnc_cause[5]
#define	FNC_CAUSE_NOTINLIST	fnc_cause[6]

enum fnc_result_value_e {
	FNC_OKAY = 0,
	FNC_NOFNCRC = 10000,
	FNC_NOTICE,
	FNC_WARNING,
	FNC_ERROR,
	FNC_FATAL
};
typedef enum fnc_result_value_e fnc_result_t;

#define	FNC_MSG_SIZE	254
#define	FNC_STACK_SIZE	100
struct fnc_msg {
	dll_t	_msg_lst;
	char	msg_text[FNC_MSG_SIZE +1];
};
typedef struct fnc_msg fncmsg_t;

struct fnc_return_code_s {
	FILE		*fnc_err;			/* file-handle for error-message */
	int		 fnc_dbg;			/* debug-level */
	fnc_result_t	 fnc_result;
	char		*fnc_cause;
	int		 fnc_syserr;			/* content of errno, if any */
	int		 fnc_stack_cnt;			/* counter of elements are actually in stackframe-buffer */
	void		*fnc_stack[FNC_STACK_SIZE];	/* stackframe-buffer */
	dll_t		 fnc_msghdr;
	void		(*fnc_free)(void *);
};
typedef struct fnc_return_code_s fncrc_t;

#define FNCRC_RESULT(v)	fncrc->fnc_result = v
#define FNCRC_CAUSE(v)	fncrc->fnc_cause = v
#define FNCRC_SYSERR(v)	fncrc->fnc_syserr = v

#define IN(...)		__VA_ARGS__
#define OUT(...)	__VA_ARGS__
#define INOUT(...)	__VA_ARGS__
#define FNC_ARGS(...)	(fncrc_t *fncrc, __VA_ARGS__)
#define FNC_PARAMS(...)	(fncrc, __VA_ARGS__)


#ifdef FNC_CALL_TRACE

#define FNC_TXT(ack, ...)	 				\
	fnc_set_text(ack, __FILE__,(char *)__func__,__LINE__,	\
		__VA_ARGS__, NULL);

#define	FNC_ATXT(_ack, _cstr)					\
	fnc_call_info(_ack, _cstr, "()",			\
		__FILE__,(char *)__func__,__LINE__);

#define FNC_ERR(fnc)						\
	fnc_call_info(fncrc, fnc, "()",				\
		__FILE__,(char *)__func__,__LINE__);

#define TRY(fnc, _LBL_, ...)					\
do {								\
	int	_rc = 0;					\
	if((_rc = fnc(fncrc, __VA_ARGS__)) != FNC_OKAY) {	\
		fnc_call_info(fncrc, #fnc, #__VA_ARGS__,	\
			__FILE__,(char *)__func__,__LINE__);	\
		rc = _rc;					\
		goto _LBL_;					\
	}							\
	rc = _rc;						\
} while (0)

#define TRY_RETURN(fnc, ...)					\
do {								\
	int	_rc = 0;					\
	if((_rc = fnc(fncrc, __VA_ARGS__)) != FNC_OKAY) {	\
		fnc_call_info(fncrc, #fnc, #__VA_ARGS__,	\
			__FILE__,(char *)__func__,__LINE__);	\
		return(_rc);					\
	}							\
	rc = _rc;						\
} while (0)

#define YY_TRY(fnc, ...)					\
do {								\
	int	_rc = 0;					\
	if((_rc = fnc(fncrc, __VA_ARGS__)) != FNC_OKAY) {	\
		fnc_call_info(fncrc, #fnc, #__VA_ARGS__,	\
			__FILE__,(char *)__func__,__LINE__);	\
		YYABORT;					\
	}							\
} while (0)

#else
#define	FNC_TXT(ack, ...)
#define	FNC_ATXT(_ack, _cstr)
#define FNC_ERR(fnc)

#define TRY(fnc, _LBL_, ...)					\
do {								\
	int	_rc = 0;					\
	if((_rc = fnc(fncrc, __VA_ARGS__)) != FNC_OKAY) {	\
		rc = _rc;					\
		goto _LBL_;					\
	}							\
	rc = _rc;						\
} while (0)

#define TRY_RETURN(fnc, ...)					\
do {								\
	int	_rc = 0;					\
	if((_rc = fnc(fncrc, __VA_ARGS__)) != FNC_OKAY) {	\
		return(_rc);					\
	}							\
	rc = _rc;						\
} while (0)

#define YY_TRY(fnc, ...)					\
do {								\
	int	_rc = 0;					\
	if((_rc = fnc(fncrc, __VA_ARGS__)) != FNC_OKAY) {	\
		YYABORT;					\
	}							\
} while (0)

#endif

#ifdef FNC_PARAM_CHK

#define FNCRC_CHECK 								\
if (fncrc == NULL) {								\
	PRNE(stderr, "fncrc-parameter not set!");				\
	return(FNC_NOFNCRC);							\
} else {									\
	if(fncrc->fnc_dbg > 0) {						\
		DBGOUT(fncrc->fnc_err, "enter:");				\
		DBGOUT(fncrc->fnc_err, "      (fncrc=0x%p)", fncrc);		\
	}									\
}

#define FNC_CHKINP(rc, _p)							\
if (_p == NULL) {								\
	rc = FNC_ERROR;								\
	FNCRC_RESULT(rc);							\
	FNCRC_CAUSE(FNC_CAUSE_INPARAM);						\
	FNCRC_SYSERR(0);							\
	FNC_TXT(fncrc,"In-parameter(%s) == NULL", #_p);				\
	fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE); \
	return(rc);								\
} else {									\
	if(fncrc->fnc_dbg > 0) {						\
		DBGOUT(fncrc->fnc_err, "      (%s=0x%p)", #_p, _p);		\
	}									\
}

#define FNC_CHKINL(rc, _l)							\
	if(fncrc->fnc_dbg > 0) {						\
		DBGOUT(fncrc->fnc_err, "      (%s=%lld)", #_l, _l);		\
	}

#define FNC_CHKOUTP(_rc, _pp)							\
if (_pp == NULL) {								\
	_rc = FNC_ERROR;							\
	FNCRC_RESULT(_rc);							\
	FNCRC_CAUSE(FNC_CAUSE_OUTPARAM);					\
	FNCRC_SYSERR(0);							\
	FNC_TXT(fncrc,"Out-parameter(%s) == NULL", #_pp);			\
	fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);	\
	return(_rc);								\
} else {									\
	if(fncrc->fnc_dbg > 0) {						\
		DBGOUT(fncrc->fnc_err, "      (%s=0x%p)", #_pp, _pp);		\
	}									\
}

#define FNC_CHKINOUTP(_rc, _pp)							\
if (_pp == NULL) {								\
	_rc = FNC_ERROR;							\
	FNCRC_RESULT(_rc);							\
	FNCRC_CAUSE(FNC_CAUSE_INOUTPARAM);					\
	FNCRC_SYSERR(0);							\
	FNC_TXT(fncrc,"InOut-parameter(%s) == NULL", #_pp);			\
	fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE); \
	return(_rc);								\
} else {									\
	if(fncrc->fnc_dbg > 0) {						\
		DBGOUT(fncrc->fnc_err, "      (%s=0x%p)", #_pp, _pp);		\
	}									\
}

#define FNC_RETURN(_rc)								\
	if(fncrc->fnc_dbg > 0) {						\
		DBGOUT(fncrc->fnc_err,"return(%d)", _rc);			\
	}									\
	return(_rc)

#else

#define FNCRC_CHECK
#define FNC_CHKINP(_rc, _p) 
#define FNC_CHKINL(_rc, _l) 
#define FNC_CHKOUTP(_rc, _pp)
#define FNC_CHKINOUTP(_rc, _p)
#define FNC_RETURN(_rc)								\
	return(_rc)

#endif

extern	int	 fnc_init(fncrc_t *fncrc, FILE *out);
extern	int	 fnc_new(FILE *out, fncrc_t **pp);
extern	int	 fnc_drop(fncrc_t *p);
extern	char	*fnc_result_2str(fnc_result_t rc);
extern	void	 fnc_prn(fncrc_t *p, FILE *out, leng_t n);
extern	int	 fnc_amend_txt(fncrc_t *fncrc, char *str);

extern	int	 fnc_chkoutp(fncrc_t *fncrc, void **pp);
extern	int	 fnc_chkinp(fncrc_t *fncrc, void *p);
extern	int	 fnc_clear(fncrc_t *p);
extern	char	*fnc_addr_2name(void *fnc_addr);
extern	int	 fnc_set_text(fncrc_t *,char *, char *, int, char *,...);
extern	int	 fnc_call_info(fncrc_t *,char *, char *, char *, char *, int);

extern	int	 fnc_set_dbg(fncrc_t *, int);

#ifdef  __cplusplus
}
#endif

#endif /* __BHS_COMMON_FNC_H__ */
