#ifndef __BHS_COMMON_COMPAT_H__
#define __BHS_COMMON_COMPAT_H__

#include <stdio.h>
#include <stdarg.h>

#ifdef	SunOS
#include <thread.h>
#include <synch.h>
#endif

#ifdef	OpenBSD
#include <pthread.h>
#endif

#ifdef	Linux
#define _GNU_SOURCE
#include <dlfcn.h>	/* dladdr, dlsym, ... */
#include <pthread.h>
#include <execinfo.h>	/* backtrace, ... */
#include <mcheck.h>	/* mtrace, muntrace */
#endif

#ifdef  __cplusplus
extern "C" {
#endif

#if	defined( SunOS )
#define	BHS_MUTEX_INIT	DEFAULTMUTEX
typedef	mutex_t			bhs_mutex_t;
#define	bhs_mutex_lock(mx)	mutex_lock(mx)
#define	bhs_mutex_unlock(mx)	mutex_unlock(mx)
#define	bhs_mutex_destroy(mx)	mutex_destroy(mx)

#elif	defined( OpenBSD )
#define	BHS_MUTEX_INIT	PTHREAD_MUTEX_INITIALIZER
typedef	pthread_mutex_t		bhs_mutex_t;
#define bhs_mutex_lock(mx)	pthread_mutex_lock(mx)
#define bhs_mutex_unlock(mx)	pthread_mutex_unlock(mx)
#define	bhs_mutex_destroy(mx)	pthread_mutex_destroy(mx)

#elif	defined( Linux )
#define	BHS_MUTEX_INIT	PTHREAD_MUTEX_INITIALIZER
typedef	pthread_mutex_t		bhs_mutex_t;
#define bhs_mutex_lock(mx)	pthread_mutex_lock(mx)
#define bhs_mutex_unlock(mx)	pthread_mutex_unlock(mx)
#define	bhs_mutex_destroy(mx)	pthread_mutex_destroy(mx)

#define bhs_uuid_generate(out)			uuid_generate(out)
#define bhs_uuid_generate_random(out)		uuid_generate_random(out)
#define bhs_uuid_generate_time(out)		uuid_generate_time(out)
#define bhs_uuid_generate_time_safe(out)	uuid_generate_time_safe(out)
#define bhs_uuid_clear(uu)			uuid_clear(uu)
#define bhs_uuid_compare(p1, p2)		uuid_compare(p1, p2)
#define bhs_uuid_copy(dst, src)			uuid_copy(dst, src)
#define bhs_uuid_is_null(uu)			uuid_is_null(uu)
#define bhs_uuid_time(uu, tt)			uuid_time(uu, tt)
#define bhs_uuid_parse(in, uu)			uuid_parse(in, uu)
#define bhs_uuid_unparse(uu, out)		uuid_unparse(uu, out)
#define bhs_uuid_unparse_upper(uu, out)		uuid_unparse_upper(uu, out)
#define bhs_uuid_unparse_lower(uu, out)		uuid_unparse_lower(uu, out)

#else
#error("used operating system is not yet supported!")
#endif

extern	void	 bhs_mutex_init(bhs_mutex_t *mx, void *arg);
extern	void	 bhs_fprintf(FILE *out, char *format, ...);

extern	int	 bhs_backtrace(void **buffer, int size);
extern	char   **bhs_backtrace_symbols(void *const *buffer, int size);

#ifdef  __cplusplus
}
#endif

#endif /* __BHS_COMMON_COMPAT_H__ */
