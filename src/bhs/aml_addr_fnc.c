#include "aml.h"

DECLARE_DATA_IDENT(addr_op_id,		"&()");
DECLARE_DATA_FUNC(addr_op_fnc,		&addr_op_id,		addr_fnc);
/*
	-----------------------------------------------------------------------
 */
int
addr_fnc(				/* address of (&) */
	fncrc_t		*fncrc,		/* function reporting faramework */
	sess_t		*sp,		/* session descriptor */
	comp_t		*cp,		/* compiler descriptor */
	leng_t		 argc,		/* argument count */
	data_link_t	*argv,		/* argument list */
	data_t		**pp		/* result */
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;
	data_link_t	*lnk1 = NULL;
	data_t		*arg1 = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINL(rc, argc);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	TRY(data_lst_next,	_door, sp, argv, &lnk1);
	TRY(data_link_2child,	_door, sp, lnk1, &arg1);
	if(
		(comp_id_cmp(DID(arg1), REFERENCE) == DATA_EQ)
	) {
		dp = arg1;

	} else {
		TRY(error_fnc, _door, sp, cp, argc, argv, "not allowed!", &dp);
	}

_door:
	*pp = dp;
	FNC_RETURN(rc);
}
/*
	-----------------------------------------------------------------------
 */
