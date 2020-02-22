#include <bhs/compile.h>

DECLARE_DATA_IDENT(preproc_id,		"pre-processor");
DECLARE_DATA_IDENT(include_id,		"include");
DECLARE_DATA_IDENT(filename_id,		"filename");

/*
	------------------------------------------------------------------------
 */
int
comp_inc_open(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_t		*fn,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	int		 lno = 0;
	data_t		*scne_file = NULL;
	data_t		*stack = NULL;
	data_t		*save = NULL;
	data_t		*dp = NULL;

	char		*msg = NULL;
	data_link_t	*ign = NULL;
	int		 is_open = 0;
	int		 sysrc = 0;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, fn);
	FNC_CHKOUTP(rc, pp);
	CHK_DES(rc, fn, DATA_FILE_DESIGNATOR);

	lno = COMP_SCNE(cp)->scne_get_lineno(COMP_SCANNER(cp));
	stack = SCNE_STACK(cp);
	save = SCNE_SAVE(cp);

	DLL_WALK(scne_file, data_t, &stack) {
		if(strcmp(DFILE(fn)->fd_name, DFILE(scne_file)->fd_name) == 0) {
			is_open++;
			break;
		}
	}
	scne_file = SCNE_FILE(cp);
	if(is_open == 0) {
		if(strcmp(DFILE(fn)->fd_name, DFILE(scne_file)->fd_name) == 0) {
			is_open++;
		}
	}

	if((msg = CALLOC(1, FNC_MSG_SIZE +1)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (FNC_MSG_SIZE +1));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}

	if(is_open != 0) {
		COMP_WARNINGS(cp)++;
		sysrc = snprintf(&msg[0], FNC_MSG_SIZE,
			"%s:%d: include %s: %s",
			TSTR(FDES_NAME(cp)), lno,
			TSTR(DFILE(fn)->fd_name), "already open!"
			);
		if(sysrc < 0) {
			rc = FNC_FATAL;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_SYSCALL);
			FNCRC_SYSERR(errno);
			FNC_TXT(fncrc, "snprintf() failed!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _door;
		}
		PRNW(sp->sess_err, "%s\n", msg);
		TRY(comp_err_new, _door, sp, msg, &dp);
		TRY(data_drop, _door, sp, fn);
		COMP_ERRTXT(cp) = dp;
		TRY(data_lst_new, _door, sp, ERROR, &dp);
		goto _door;
	}

	TRY(data_file_open, _next1, sp, fn, "r");
_next1:
	if(rc != FNC_OKAY) {
		COMP_ERRORS(cp)++;
		sysrc = snprintf(&msg[0], FNC_MSG_SIZE,
			"%s:%d: include %s :%s",
			TSTR(FDES_NAME(cp)), lno,
			TSTR(DFILE(fn)->fd_name), strerror(fncrc->fnc_syserr)
			);
		if(sysrc < 0) {
			rc = FNC_FATAL;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_SYSCALL);
			FNCRC_SYSERR(errno);
			FNC_TXT(fncrc, "snprintf() failed!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _door;
		}
		PRNE(sp->sess_err, "%s\n", msg);
		fnc_clear(fncrc);

		TRY(comp_err_new, _door, sp, msg, &dp);
		TRY(data_drop, _door, sp, fn);
		COMP_ERRTXT(cp) = dp;
		TRY(data_lst_new, _door, sp, ERROR, &dp);
	} else {
		DFILE(scne_file)->fd_buffer =
			COMP_SCNE(cp)->scne_get_current_buffer(COMP_SCANNER(cp));
		DFILE(scne_file)->fd_lineno =
			COMP_SCNE(cp)->scne_get_lineno(COMP_SCANNER(cp));

		TRY(data_lst_ins_first, _door, sp, stack, scne_file, &ign);

		COMP_SCNE(cp)->scne_set_in(DFILE(fn)->fd_hdl ,COMP_SCANNER(cp));
		COMP_SCNE(cp)->scne_set_out(sp->sess_out ,COMP_SCANNER(cp));
		DFILE(fn)->fd_lineno = 1;
		DFILE(fn)->fd_buffer =
			COMP_SCNE(cp)->scne_create_buffer(
				DFILE(fn)->fd_hdl, YY_BUF_SIZE, COMP_SCANNER(cp));
		COMP_SCNE(cp)->scne_switch_to_buffer(
				DFILE(fn)->fd_buffer, COMP_SCANNER(cp));
		cp->comp_scanner_ext->scne_file = fn;
		dp = fn;
	}
_door:
	if(msg != NULL) {
		FREE(msg);
		msg = NULL;
	}
	*pp = dp;
	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
comp_inc_eof(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*lnk = NULL;
	data_t		*stack = NULL;
	data_t		*save = NULL;
	scne_t		*scnep = NULL;
	yyscan_t	 scanner = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);

	scnep = cp->comp_scanner_ext;
	scanner	= cp->comp_scanner_db;
	dp = SCNE_FILE(cp);	
	stack = SCNE_STACK(cp);
	save = SCNE_SAVE(cp);

	if(DLL_ISEMPTY(DLST(stack))) {
		TRY(data_lst_ins_last, _door, sp, save, dp, &lnk);
	} else {
		scnep->scne_delete_buffer(scnep->scne_get_current_buffer(scanner), scanner);
		DFILE(dp)->fd_buffer = NULL;
		TRY(data_file_close, _door, sp, dp);
		TRY(data_lst_ins_last, _door, sp, save, dp, &lnk);
		scnep->scne_file = NULL;

		TRY(data_lst_first, _door, sp, stack, &lnk);
		TRY(data_lst_remove, _door, sp, lnk, &dp);
		scnep->scne_file = dp;

		scnep->scne_set_in(DFILE(dp)->fd_hdl, scanner);
		scnep->scne_set_out(sp->sess_out, scanner);
		scnep->scne_switch_to_buffer(DFILE(dp)->fd_buffer, scanner);

		rc = 1;
	}

_door:
	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
comp_prep_inc(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	leng_t		 argc,
	data_link_t	*argv,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*edp = NULL;
	data_t		*dp = NULL;
	data_t		*arg = NULL;
	data_t		*fn = NULL;
	char		*msg = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2child, _door, sp, argv, &arg);
	TRY(cp->comp_eval, _door, sp, cp, argc, argv, &edp);
	if(comp_id_cmp(DID(edp), STRING) == DATA_EQ) {
		char	*name = NULL;

		name = DCSTR(edp);
		TRY(data_file_new, _door, sp, INCLUDE, name, &fn);
		TRY(data_sequ_inherit, _door, sp, arg, fn);
		TRY(comp_inc_open, _door, sp, cp, fn, &dp);

	} else {
		int	 sysrc = 0;

		COMP_WARNINGS(cp)++;
		if((msg = CALLOC(1, COMP_MSG_LEN +1)) == NULL) {
			rc = FNC_FATAL;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_NOMEM);
			FNCRC_SYSERR(errno);
			FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (COMP_MSG_LEN +1));
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _door;
		}

		sysrc = snprintf(msg, COMP_MSG_LEN,
			"%s:%lld: include error, expression \"%s\" %s!",
			TSTR(SEQU(arg)->name), arg->data_sequ->num,
			TSTR(DCSTR(DID(arg))),
			"produces not an string constant!");

		if(sysrc < 0) {
			rc = FNC_FATAL;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(COMP_CAUSE_SYSCALL);
			FNCRC_SYSERR(errno);
			FNC_TXT(fncrc, "snprintf() failed!");
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _door;
		}

		FPRINTF(sp->sess_err, "%s\n", msg);
		TRY(comp_err_new, _door, sp, msg, &dp);
		COMP_ERRTXT(cp) = dp;
	}

_door:
	if(edp != NULL) {
		data_drop(NEXT_FNCRC, sp, edp);
		edp = NULL;
	}
	if(rc != FNC_OKAY) {
		if(dp != NULL) {
			if(dp == fn) {
				fn = NULL;
			}
			data_drop(NEXT_FNCRC, sp, dp);
			dp = NULL;
		}
		if(fn != NULL) {
			data_drop(NEXT_FNCRC, sp, fn);
			fn = NULL;
		}
	}
	if(msg != NULL) {
		FREE(msg);
		msg = NULL;
	}
	*pp = dp;
	FNC_RETURN(rc);
}
