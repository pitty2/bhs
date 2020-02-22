#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <bhs/compat.h>

void
bhs_mutex_init(bhs_mutex_t *mx, void *arg) {
#if	defined(SunOS)
	mutex_init(mx, USYNC_THREAD, arg);

#elif	defined(OpenBSD)
	pthread_mutex_init(mx, arg);

#elif	defined(Linux)
	pthread_mutex_init(mx, arg);
#else
#error("used operating system is not yet supported!")
#endif
}

int
bhs_backtrace(void **buffer, int size) {
#if	defined(SunOS)
	return(0)

#elif	defined(OpenBSD)
	return(0)

#elif	defined(Linux)
	int	rc;

#if (1)
	rc = backtrace(buffer, size);
#endif

	return(rc);
#else
#error("used operating system is not yet supported!")
#endif
}

char **
bhs_backtrace_symbols(void *const *buffer, int size) {
#if	defined(SunOS)
	return(NULL)

#elif	defined(OpenBSD)
	return(NULL)

#elif	defined(Linux)
	return(backtrace_symbols(buffer, size));
#else
#error("used operating system is not yet supported!")
#endif
}

/*
	TODO:
		inmplentiere %p ... OS-unabhaengig!
 */
void
bhs_fprintf(FILE *out, char *format, ...) {
	int	 error = 0;
	va_list	 ap;

	if(out == NULL) {
		goto _door;
	}
	if((error = ferror(out)) != 0) {
		if(error == EINTR) {
			clearerr(out);
		} else {
			goto _door;
		}
	}
	if(format == NULL) {
		goto _door;
	}

	va_start(ap, format);
	vfprintf(out, format, ap);
	va_end(ap);
	fflush(out);
_door:
	return;
}
