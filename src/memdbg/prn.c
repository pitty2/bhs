#include <stdio.h>
#include <errno.h>

int
prn_ferror(FILE *fd) {
	int	 rc = 0;

	if(fd == NULL) {
		rc = (-1);
	} else {
		rc = ferror(fd);
	}
	return(rc);
}

