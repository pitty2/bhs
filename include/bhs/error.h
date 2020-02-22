#ifndef __BHS_COMMON_ERROR_H__
#define __BHS_COMMON_ERROR_H__

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <bhs/prn.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define PRNW(out, ...)					\
if (prn_ferror(out) == 0 ) {				\
	fprintf (out, "WARNING ");			\
	fprintf (out, "%-15s (+%-4d) ) %15s : ", __FILE__, __LINE__,__func__);	\
	fprintf (out, __VA_ARGS__);			\
	fprintf (out, "\n");				\
	fflush(out);					\
} 

#define PRNE(out, ...)					\
if (prn_ferror(out) == 0 ) {				\
	fprintf (out, "+++ERROR+++ ");			\
	fprintf (out, "%-15s (+%-4d) %15s : ", __FILE__, __LINE__,__func__);	\
	fprintf (out, __VA_ARGS__);			\
	fprintf (out, "\n");				\
	fflush(out);					\
} 

#define PABORT(...)					\
if (prn_ferror(stderr) == 0 ) {				\
	fprintf (stderr, "+++ERROR+++ in ");		\
	fprintf (stderr, "%s +%d ", __FILE__, __LINE__);\
	fprintf (stderr, "%s : ", __func__);		\
	fprintf (stderr, __VA_ARGS__);			\
	fprintf (stderr, "\n");				\
	fflush(stderr);					\
}							\
abort()

#ifdef  __cplusplus
}
#endif

#endif /* __BHS_COMMON_ERROR_H__ */
