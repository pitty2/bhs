#include <stdlib.h>
#include <strings.h>
#include <stdarg.h>
#include <errno.h>
#include <_memdbg.h>

void
bhs_mutex_init(bhs_mutex_t *mx, void *arg) {
	pthread_mutex_init(mx, arg);
}

/*
	TODO:
		inmplentiere %p ... OS-unabhaengig!
 */
void
bhs_fprintf(FILE *out, char *format, ...) {
	int	 error = 0;
#if (0)
	int	 pos = 0;
	char	*ptr = NULL;
#endif
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
