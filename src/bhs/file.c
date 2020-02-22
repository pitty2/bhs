#include <bhs/mem.h>
#include <bhs/file.h>


char	*file_cause[] = {
	"FILE okay",
	"FILE mode!",
	"FILE malloc() failed!",
	"FILE systemcall failed!"
};

/*
	------------------------------------------------------------------------
 */
int
fdes_init(
	fncrc_t	*fncrc,
	char	*name,
	fdes_t	*fp
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, name);
	FNC_CHKINP(rc, fp);

	if((fp->fd_name = STRDUP(name)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FILE_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "STRDUP(%d) failed!",strlen(name));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	fp->fd_hdl = NULL;
	fp->fd_mode = NULL;
	fp->fd_lineno = 0;
	fp->fd_buffer = NULL;

_door:
	FNC_RETURN(rc);
}

int
fdes_new(
	fncrc_t	*fncrc,
	char	*name,
	fdes_t	**pp
) {
	int	 rc = FNC_OKAY;
	fdes_t	*fp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, name);
	FNC_CHKOUTP(rc, pp);

	if((fp = CALLOC(1, sizeof(*fp))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FILE_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)sizeof(*fp));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	TRY(fdes_init, _next1, name, fp);
_next1:
	if(rc != FNC_OKAY) {
		FREE(fp);
		fp = NULL;
		FNC_ATXT(fncrc, "fdes_init");
	}

_door:
	*pp = fp;
	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
fdes_fini(
	fncrc_t	*fncrc,
	fdes_t	*fp
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, fp);

	if(fp->fd_name != NULL) {
		FREE(fp->fd_name);
		fp->fd_name = NULL;
	}
	if(fp->fd_hdl != NULL) {
		fclose(fp->fd_hdl);
		fp->fd_hdl = NULL;
	}
	if(fp->fd_mode != NULL) {
		FREE(fp->fd_mode);
		fp->fd_mode = NULL;
	}
	
	fp->fd_lineno = 0;

	if(fp->fd_buffer != NULL) {
#if (0)
		FREE(fp->fd_buffer);
#endif
		fp->fd_buffer = NULL;
	}
	FNC_RETURN(rc);
}

int
fdes_drop(
	fncrc_t	*fncrc,
	fdes_t	*fp
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, fp);

	TRY(fdes_fini, _door, fp);
	FREE(fp);
_door:
	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
fdes_cpy(
	fncrc_t	*fncrc,
	fdes_t	*fp,
	fdes_t	**pp
) {
	int	 rc = FNC_OKAY;
	fdes_t	*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, fp);
	FNC_CHKOUTP(rc, pp);

	TRY(fdes_new, _door, fp->fd_name, &p);
	if(fp->fd_hdl != NULL && fp->fd_mode != NULL) {
		TRY(fdes_open, _next1, fp->fd_mode, p);
_next1:
		if(rc != FNC_OKAY) {
			FNC_ATXT(fncrc, "fdes_new");
			fdes_drop(fncrc, p);
			p = NULL;
			goto _door;
		}
	}

_door:
	*pp = p;
	FNC_RETURN(rc);
}

int
fdes_cmp(
	fncrc_t	*fncrc,
	fdes_t	*fp1,
	fdes_t	*fp2,
	int	*code
) {
	int	 rc = FNC_OKAY;
	int	 c = 0;

	FNCRC_CHECK;
	FNC_CHKINP(rc, fp1);
	FNC_CHKOUTP(rc, fp2);

	if((c = strcmp(TSTR(fp1->fd_name), TSTR(fp2->fd_name))) == 0) {
		c = strcmp(TSTR(fp1->fd_mode), TSTR(fp2->fd_mode));
	}
	*code = c;
	FNC_RETURN(rc);
}


/*
	------------------------------------------------------------------------
 */
int
fdes_prn(
	fncrc_t	*fncrc,
	fdes_t	*fp,
	FILE	*out,
	leng_t	 n
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, fp);

	PRNN(n); PRNO("name   : %s\n", fp->fd_name);
	PRNN(n); PRNO("hdl    : 0x%p\n", fp->fd_hdl);
	PRNN(n); PRNO("mode   : %s\n", fp->fd_mode);
	PRNN(n); PRNO("lineno : %d\n", fp->fd_lineno);
	PRNN(n); PRNO("buffer : 0x%p\n", fp->fd_buffer);

	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
fdes_open(
	fncrc_t	*fncrc,
	char	*mode,
	fdes_t	*fp
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, mode);
	FNC_CHKINP(rc, fp);

	if((fp->fd_hdl = fopen(fp->fd_name, mode)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FILE_CAUSE_SYSCALL);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "fopen(%s, %s) failed!",
			TSTR(fp->fd_name), TSTR(mode));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	if((fp->fd_mode = STRDUP(mode)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FILE_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "STRDUP(%d) failed!",strlen(mode));
		fclose(fp->fd_hdl);
		fp->fd_hdl = NULL;
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	
_door:
	FNC_RETURN(rc);
}

int
fdes_close(
	fncrc_t	*fncrc,
	fdes_t	*fp
) {
	int	 rc = FNC_OKAY;
	int	 frc = 0;

	FNCRC_CHECK;
	FNC_CHKINP(rc, fp);

	if((frc = fclose(fp->fd_hdl)) != 0) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FILE_CAUSE_SYSCALL);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "fclose(%s) failed!", TSTR(fp->fd_name));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
	}
	fp->fd_hdl = NULL;
	if(fp->fd_mode != NULL) {
		FREE(fp->fd_mode);
		fp->fd_mode = NULL;
	}
	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
fdes_set_buffer(
	fncrc_t	*fncrc,
	void	*buffer,
	fdes_t	*fp
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, buffer);
	FNC_CHKINP(rc, fp);

	fp->fd_buffer = buffer;
	FNC_RETURN(rc);
}
