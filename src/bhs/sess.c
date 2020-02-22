#include <bhs/mem.h>
#include <bhs/sess.h>

char	*sess_cause[] = {
	"Sess okay",
	"Sess malloc() failed!",
	"Sess systemcall failed!"
};

static
dll_t	sessions = {
		&sessions,
		&sessions,
		&sessions
	};

/*
	------------------------------------------------------------------------
 */
int
sess_new(
	fncrc_t	*fncrc,
	FILE	*in,
	FILE	*out,
	FILE	*err,
	sess_t	**sp
) {
	int	 rc = FNC_OKAY;
	int	 j;
	sess_t	*p = NULL;

	FNCRC_CHECK;
	FNC_CHKOUTP(rc, sp);

	if((p = CALLOC(1, sizeof(*p))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(SESS_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)sizeof(*p));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	dll_init(&p->sess_entry);
	p->sess_id = sp;
	p->sess_in = in;
	p->sess_out = out;
	p->sess_err = err;
	p->sess_fncrc_cnt = 1;
	for(j = 0; j < SESS_MAX_FNCRC_CNT; j++) {
		rc = fnc_init(&p->sess_fncrc[j], out);
	}
	p->sess_free = FREE_FNC;
	*sp = p;
_door:
	FNC_RETURN(rc);
}

int
sess_drop(
	fncrc_t *fncrc,
	sess_t *p
) {
	int	 rc = FNC_OKAY;
	int	 j;

	FNCRC_CHECK;
	FNC_CHKINP(rc, p);

	for(j = 0; j < SESS_MAX_FNCRC_CNT; j++) {
		rc = fnc_clear(&p->sess_fncrc[j]);
	}
	if(p->sess_free != NULL) {
		p->sess_free(p);
	}
	FNC_RETURN(rc);
}

int
sess_setdbg(
	fncrc_t *fncrc,
	sess_t *p,
	int lvl
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, p);

	p->sess_dbg = lvl;
	FNC_RETURN(rc);
}

int
sess_set_data_dbg(
	fncrc_t *fncrc,
	sess_t *p,
	int lvl
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, p);

	p->sess_data_dbg = lvl;
	FNC_RETURN(rc);
}

fncrc_t *
sess_next_fncrc(
	sess_t *sp
) {
	fncrc_t *fp = NULL;

	if(sp->sess_fncrc_cnt < SESS_MAX_FNCRC_CNT) {
		fp = &sp->sess_fncrc[sp->sess_fncrc_cnt++];
	} else {
		fp = &sp->sess_fncrc[0];
	}
	return(fp);
}

int
sess_prn(
	fncrc_t *fncrc,
	sess_t *p,
	FILE *out,
	leng_t n
) {
	int	 rc = FNC_OKAY;
	int	 j;

	FNCRC_CHECK;

	if(out == NULL) {
		goto _door;
	}
	if(p == NULL) {
		PRNN(n); PRNO("sess_t (nil) -> { nil }\n");
		goto _door;
	}
	PRNN(n); PRNO("sess_t (%p) -> {\n", p);
	n++;
	PRNN(n); PRNO("sess_id            = 0x%p\n", p->sess_id);
	PRNN(n); PRNO("sess_in            = 0x%p\n", p->sess_in);
	PRNN(n); PRNO("sess_out           = 0x%p\n", p->sess_out);
	PRNN(n); PRNO("sess_err           = 0x%p\n", p->sess_err);
	PRNN(n); PRNO("sess_dbg           = %d\n", p->sess_dbg);
	PRNN(n); PRNO("sess_data_dbg      = %d\n", p->sess_data_dbg);
	PRNN(n); PRNO("sess_data_stack[0] = 0x%p\n", p->sess_data_stack[0]);
	PRNN(n); PRNO("sess_data_stack[1] = 0x%p\n", p->sess_data_stack[1]);
	PRNN(n); PRNO("sess_walk_fnc      = 0x%p\n", p->sess_walk_fnc);
	PRNN(n); PRNO("sess_data          = 0x%p\n", p->sess_extra_data);
	PRNN(n); PRNO("sess_fncrc         [\n");
	n++;
	for(j = 0; j < SESS_MAX_FNCRC_CNT; j++) {
		PRNN(n); PRNO("sess_fncrc[%d]     = 0x%p\n", j, &p->sess_fncrc[j]);
		fnc_prn(&p->sess_fncrc[j], out, n);
	}
	n--;
	PRNN(n); PRNO("]\n");
	n--;
	PRNN(n); PRNO("}\n");
_door:
	FNC_RETURN(rc);
}
