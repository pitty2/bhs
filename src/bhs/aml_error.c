#include "aml.h"

/*
	------------------------------------------------------------------------
 */
int
error_fnc(	// Produce/Make an error-message for eval-functions
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	leng_t		 argc,
	data_link_t	*argv,
	char		*errtxt,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	char		*msg = NULL;
	data_t		*op = NULL;
	data_t		*fnc = NULL;
	data_link_t	*lnk1 = NULL;
	data_link_t	*lnk2 = NULL;
	data_t		*arg1 = NULL;
	data_t		*arg2 = NULL;
	int		 sysrc = 0;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKINP(rc, errtxt);
	FNC_CHKOUTP(rc, pp);

	TRY(data_link_2child, _door, sp, argv, &op);
	TRY(data_get_first, _door, sp, op, &fnc);
	TRY(data_lst_next, _door, sp, argv, &lnk1);
	TRY(data_link_2child, _door, sp, lnk1, &arg1);

	if((msg = CALLOC(1, COMP_MSG_LEN +1)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (COMP_MSG_LEN +1));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}

	if(argc == 2) {
		sysrc = snprintf(msg, COMP_MSG_LEN,
			"%s:%lld: expression error, operator \"%s %s\" %s!",
			TSTR(SEQU(op)->name), op->data_sequ->num,
			TSTR(DCSTR(DID(fnc))),
			TSTR(DCSTR(DID(arg1))),
			TSTR(errtxt));
	} else if (argc == 3) {
		TRY(data_lst_next, _door, sp, lnk1, &lnk2);
		TRY(data_link_2child, _door, sp, lnk2, &arg2);

		sysrc = snprintf(msg, COMP_MSG_LEN,
			"%s:%lld: expression error, \"%s %s %s\" %s!",
			TSTR(SEQU(op)->name), op->data_sequ->num,
			TSTR(DCSTR(DID(arg1))),
			TSTR(DCSTR(DID(fnc))),
			TSTR(DCSTR(DID(arg2))),
			TSTR(errtxt));
	} else {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_HIRARCHIE);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "Wrong argument count %llu", argc);
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}

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

_door:
	if(msg != NULL) {
		FREE(msg);
	}
	*pp = dp;
	FNC_RETURN(rc);
}
/*
	------------------------------------------------------------------------
 */

