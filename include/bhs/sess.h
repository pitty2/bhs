#ifndef __BHS_COMMON_SESS_H__
#define __BHS_COMMON_SESS_H__

#include <stdio.h>
#include <bhs/dll.h>
#include <bhs/fncrc.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern	char	*sess_cause[];
#define	SESS_CAUSE_OKAY		sess_cause[0]
#define	SESS_CAUSE_NOMEM	sess_cause[1]
#define	SESS_CAUSE_SYSCALL	sess_cause[2]

#define SESS_MAX_FNCRC_CNT	10

struct	sess_s {
	dll_t		 sess_entry;
	void		*sess_id;
	FILE		*sess_in;
	FILE		*sess_out;
	FILE		*sess_err;
	int		 sess_dbg;
	int		 sess_data_dbg;
	void		*sess_data_stack[2];
	int		(*sess_walk_fnc)();
	void		*sess_extra_data;
	leng_t		 sess_fncrc_cnt;
	fncrc_t		 sess_fncrc[SESS_MAX_FNCRC_CNT +1];

	void		(*sess_free)(void *);
};
typedef struct sess_s sess_t;

#define	SESS_DBG(p)	((p)->sess_dbg)	
#define SESS_SHORT_DBG	1
#define SESS_PARAM_DBG	2
#define SESS_DATA_DBG	3
#define SESS_CIRC_DBG	4
#define SESS_FNCRC(p)	(&(p)->sess_fncrc[0])

#define NEXT_FNCRC	sess_next_fncrc(sp)

extern	int	 sess_new(
			fncrc_t	*fncrc,
			FILE	*in,
			FILE	*out,
			FILE	*err,
			sess_t	**sp
			);
extern	int	 sess_drop(
			fncrc_t	*fncrc,
			sess_t	*sp
			);
extern	int	 sess_setdbg(
			fncrc_t	*fncrc,
			sess_t	*sp,
			int	 lvl
			);
extern	int	 sess_set_data_dbg(
			fncrc_t	*fncrc,
			sess_t	*sp,
			int	 lvl
			);
extern	fncrc_t	*sess_next_fncrc(sess_t *sp);
extern	int	 sess_prn(
			fncrc_t	*fncrc,
			sess_t	*sp,
			FILE	*out,
			leng_t	 n
			);




#ifdef  __cplusplus
}
#endif

#endif /* __BHS_COMMON_SESS_H__ */
