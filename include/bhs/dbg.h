#ifndef __BHS_COMMON_DBG_H__
#define __BHS_COMMON_DBG_H__

#include <stdio.h>
#include <bhs/prn.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum bhs_dbglvl_e {
	BHS_NODEBUG = 0,
	BHS_MINIMAL,
	BHS_MEDIUM,
	BHS_FULL
}dbglvl_t;

#define DBGOUT(out, ...)							\
if (prn_ferror(out) == 0 ) {							\
	fprintf (out, "DBG ");							\
	fprintf (out, "%-20s (+%-4d) %20s : ", __FILE__, __LINE__, __func__);	\
	fprintf (out, __VA_ARGS__);						\
	fprintf (out, "\n");							\
	fflush(out);								\
} 

#ifdef  __cplusplus
}
#endif

#endif /* __BHS_COMMON_DBG_H__ */
