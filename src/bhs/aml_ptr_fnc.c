#include "aml.h"

DECLARE_DATA_IDENT(pointer_op_id,	"->");
DECLARE_DATA_FUNC(pointer_op_fnc,	&pointer_op_id,		ptr_fnc);
/*
	------------------------------------------------------------------------
 */
int
ptr_fnc(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	leng_t		 argc,		/* argument count */
	data_link_t	*argv,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, argv);
	FNC_CHKOUTP(rc, pp);

	*pp = dp;
	FNC_RETURN(rc);
}
/*
	-----------------------------------------------------------------------
 */
