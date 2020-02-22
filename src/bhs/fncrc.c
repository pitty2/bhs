#ifdef Linux
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
#include <stdarg.h>
#include <string.h>
#include <bhs/fncrc.h>
#include <bhs/mem.h>

char	*fnc_cause[] = {
	"FNC okay",
	"FNC Invalid In-Parameter!",
	"FNC Invalid Out-Parameter!",
	"FNC Invalid InOut-Parameter!",
	"FNC malloc() failed!",
	"FNC Systemcall failed!",
	"FNC reference/pointer not in list!"
};

/*
	-----------------------------------------------------------------------
 */
int
fnc_chkoutp(fncrc_t *fncrc, void **pp) {
	int	 rc = FNC_OKAY;

	if(pp == NULL) {
		rc = FNC_FATAL;
		fncrc->fnc_result = rc;
		fncrc->fnc_cause = FNC_CAUSE_OUTPARAM;
		FNCRC_SYSERR(0);
	}
	return(rc);
}

int
fnc_chkinp(fncrc_t *fncrc, void *p) {
	int	 rc = FNC_OKAY;

	if(p == NULL) {
		rc = FNC_FATAL;
		fncrc->fnc_result = rc;
		fncrc->fnc_cause = FNC_CAUSE_INPARAM;
		FNCRC_SYSERR(0);
	}
	return(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
fnc_init(
	fncrc_t	*fncrc,
	FILE	*out
) {
	int	 rc = FNC_OKAY;

	if(fncrc == NULL) {
		PRNE(stderr, "fncrc-parameter not set!");
		abort();
	}

	memset(fncrc, 0, sizeof(*fncrc));
	fncrc->fnc_err = out;
	FNCRC_RESULT(rc);
	FNCRC_CAUSE(FNC_CAUSE_OKAY);
	FNCRC_SYSERR(0);
	dll_init(&fncrc->fnc_msghdr);

	return(rc);
}

int
fnc_new(
	FILE	*out,
	fncrc_t	**pp
) {
	int	 rc = FNC_OKAY;
	fncrc_t	*p = NULL;

	if((p = CALLOC(1, sizeof(fncrc_t))) == NULL) {
		rc = FNC_FATAL;
		goto _door;
	}
	fnc_init(p, out);
	p->fnc_free = FREE_FNC;
_door:
	*pp = p;
	return(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
fnc_clear(
	fncrc_t *p
) {
	int	 rc = FNC_OKAY;
	fncmsg_t	*msg = NULL;

	p->fnc_dbg = 0;
	p->fnc_result = FNC_OKAY;
	p->fnc_cause = FNC_CAUSE_OKAY;
	p->fnc_syserr = 0;
	while((msg = (fncmsg_t *)DLL_FIRST(&p->fnc_msghdr)) != NULL) {
		dll_remove((dll_t *)msg);
		FREE(msg);
	}
	return(rc);
}

int
fnc_drop(
	fncrc_t	*p
) {
	int	 rc = FNC_OKAY;
	fncmsg_t	*msg = NULL;

	while((msg = (fncmsg_t *)DLL_FIRST(&p->fnc_msghdr)) != NULL) {
		dll_remove((dll_t *)msg);
		FREE(msg);
	}
	if(p->fnc_free != NULL) {
		p->fnc_free(p);
	}
	return(rc);
}

/*
	-----------------------------------------------------------------------
 */
char *
fnc_result_2str(
	fnc_result_t	i
) {
	switch(i) {
		case FNC_OKAY :
			return("FNC_OKAY");
		case FNC_NOFNCRC :
			return("FNC_NOFNCRC");
		case FNC_NOTICE :
			return("FNC_NOTICE");
		case FNC_WARNING :
			return("FNC_WARNING");
		case FNC_ERROR :
			return("FNC_ERROR");
		case FNC_FATAL :
			return("FNC_FATAL");
	}
	return("unkown FNC_RESULT");
}

void
fnc_prn(
	fncrc_t	*p,
	FILE	*out,
	leng_t	 n
){
	char	**strings;
	int	j;
	fncmsg_t	*fmsg = NULL;

	if(p == NULL) {
		return;
	}
	PRNN(n); PRNO("fncrc_t -> %p {\n", p);
	n++;
	PRNN(n); PRNO("fnc_dbg       = %d\n", p->fnc_dbg);
	PRNN(n); PRNO("fnc_result    = %d : %s\n", p->fnc_result, fnc_result_2str(p->fnc_result));
	PRNN(n); PRNO("fnc_cause     = %s\n", TSTR(p->fnc_cause));
	PRNN(n); PRNO("fnc_syserr    = %d : %s\n", p->fnc_syserr,
		(p->fnc_syserr == 0) ? "OKAY": strerror(p->fnc_syserr));

	PRNN(n); PRNO("fnc_messages  = %lld\n", dll_count(&p->fnc_msghdr));
	n++;
	DLL_WALK(fmsg, fncmsg_t, &p->fnc_msghdr) {
		PRNN(n); PRNO("%s\n", TSTR(&fmsg->msg_text[0]));
	}
	n--;

	PRNN(n); PRNO("fnc_stack_cnt = %d\n", p->fnc_stack_cnt);
	PRNN(n); PRNO("fnc_stack_backtrace =\n");

	strings = backtrace_symbols(p->fnc_stack, p->fnc_stack_cnt);
	if (strings == NULL) {
		perror("backtrace_symbols");
		goto _door;
	}
	for (j = 0; j < p->fnc_stack_cnt; j++) {
		PRNN(n); PRNO("%s\n", strings[j]);
	}
	free(strings);

_door:
	n--;
	PRNN(n); PRNO("}\n");
}

/*
	-----------------------------------------------------------------------
 */
char *
fnc_addr_2name(
	void	*fnc_addr
) {
	int	 rc = 0;
	char	*name = NULL;
	Dl_info	 fnc_info;

	rc = dladdr(fnc_addr, &fnc_info);
	name = STRDUP((char *)fnc_info.dli_sname);

	return(name);
}

/*
	-----------------------------------------------------------------------
 */
int
fnc_set_text(
	fncrc_t	*fncrc,
	char	*file,
	char	*func,
	int	 line,
	char	*format,
	...
) {
	int	 rc = FNC_OKAY;
	char	 msg[FNC_MSG_SIZE +1];
	FILE	*out = NULL;
	fncmsg_t *fmsg = NULL;
	va_list	 ap;

	va_start(ap, format);
	vsnprintf(&msg[0], FNC_MSG_SIZE, format, ap);
	va_end(ap);

	out = fncrc->fnc_err;
	if((fmsg = CALLOC(1, sizeof(*fmsg))) == NULL) {
		PRNE(out, "%s.%s: msg = CALLOC(1, sizeof(*msg)) == NULL", __FILE__, __func__);
		abort();
	}
	dll_init(&fmsg->_msg_lst);
	dll_insert_prev(&fncrc->fnc_msghdr, &fmsg->_msg_lst);
	snprintf(&fmsg->msg_text[0], FNC_MSG_SIZE,
		"%-15s(+%-4d) %15s: %s",
		TSTR(file), line, TSTR(func), &msg[0]);
	
	return(rc);
}

int
fnc_amend_txt(
	fncrc_t	*fncrc,
	char	*str
) {
	int	 rc = FNC_OKAY;

	if(fncrc == NULL) {
		goto _door;
	}
	fncmsg_t *fmsg = NULL;

	if((fmsg = CALLOC(1, sizeof(*fmsg))) == NULL) {
		PRNE(fncrc->fnc_err, "%s.%s: msg = CALLOC(1, sizeof(*msg)) == NULL", __FILE__, __func__);
		abort();
	}
	dll_init(&fmsg->_msg_lst);
	dll_insert_prev(&fncrc->fnc_msghdr, &fmsg->_msg_lst);

	if((rc = snprintf(&fmsg->msg_text[0], FNC_MSG_SIZE, "%s", str)) < 0) {
		PRNE(fncrc->fnc_err, "%s.%s: snprintf failed!", __FILE__, __func__);
		goto _door;
	}
_door:
	return(rc);
}

int
fnc_call_info(
	fncrc_t	*fncrc,
	char	*called,
	char	*args,
	char	*file,
	char	*func,
	int	 line
) {
	int	 rc = FNC_OKAY;
	fncmsg_t *fmsg = NULL;

	if((fmsg = CALLOC(1, sizeof(*fmsg))) == NULL) {
		PRNE(fncrc->fnc_err, "%s.%s: msg = CALLOC(1, sizeof(*msg)) == NULL", __FILE__, __func__);
		abort();
	}
	dll_init(&fmsg->_msg_lst);
	dll_insert_prev(&fncrc->fnc_msghdr, &fmsg->_msg_lst);

	if((rc = snprintf(&fmsg->msg_text[0], FNC_MSG_SIZE,
		"%-15s(+%-4d) %15s: %s = %s%s!",
		TSTR(file),line,TSTR(func),
		TSTR(fnc_result_2str(fncrc->fnc_result)),
		TSTR(called), TSTR(args))) < 0) {
		PRNE(fncrc->fnc_err, "%s.%s: snprintf failed!", __FILE__, __func__);
		goto _door;
	}

_door:
	return(rc);
}

/*
	-----------------------------------------------------------------------
 */
int
fnc_set_dbg(
	fncrc_t	*fncrc,
	int	 dbg
) {
	int	 rc = FNC_OKAY;

	fncrc->fnc_dbg = dbg;
	return(rc);
}
