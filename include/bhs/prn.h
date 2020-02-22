#ifndef __BHS_COMMON_PRN_H__
#define __BHS_COMMON_PRN_H__

#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define TSTR(p)	((p == NULL) ? "{nil}" : p)
#define TVAL(p)	((p == NULL) ? "{0}" : p)

#if	defined(__SUNPRO_C)
#define FPRINTF(out, ...)			\
if (prn_ferror(out) == 0) {			\
	fprintf (out, __VA_ARGS__);		\
}

#define PRNN(__n__) 				\
if (prn_ferror(out) == 0) {			\
	unsigned long	 __cnt__ = __n__;	\
						\
	while(__cnt__ > 0) {			\
		fprintf(out, "\t");		\
		__cnt__--;			\
	}					\
	fflush(out);				\
}

#define PRNO(...)				\
if (prn_ferror(out) == 0) {			\
	fprintf (out, __VA_ARGS__);		\
	fflush(out);				\
}

#elif	defined(__GNUC__)
#define FPRINTF(out, format, args...)		\
if (prn_ferror(out) == 0) {			\
	fprintf (out, format , ## args);	\
}

#define PRNN(__n__) 				\
if (prn_ferror(out) == 0) {			\
	unsigned long	 __cnt__ = __n__;	\
						\
	while(__cnt__ > 0) {			\
		fprintf(out, "\t");		\
		__cnt__--;			\
	}					\
	fflush(out);				\
}

#define PRNO(format, args...)			\
if (prn_ferror(out) == 0) {			\
	fprintf (out, format , ## args);	\
	fflush(out);				\
}

#else
#error("used compiler is not yet supported!")
#endif /* compiler */

int	 prn_ferror(FILE *fd);

#ifdef  __cplusplus
}
#endif

#endif /* __BHS_COMMON_PRN_H__ */
