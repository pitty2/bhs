#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef Linux
#include <unistd.h>
#endif
#include <bhs.h>

void
usage(void) {
	fprintf(stderr, "usage : [-d] [-m] [-r] [-n] -s \n");
}

int
main(argc, argv) 
	int	 argc;
	char	*argv[];
{
	int	 c, rc;
extern	char	*optarg;
extern	int	 optind;
extern	int	 optopt;

	int	 errflg = 0;
	int	 dat_debug = 0;
	int	 fnc_debug = 0;
	int	 mem_debug = 0;

	char	*elcnt = NULL;
	char	*elsz = NULL;
	fncrc_t	 ack;
	fncrc_t	*fncrc = &ack;

	while((c = getopt(argc, argv, "dmrs:n:")) != EOF) {
		switch(c) {
		case 'd' :
			dat_debug++;
			break;
		case 'm' :
			mem_debug++;
			break;
		case 'n' :
			elcnt = optarg;
			break;
		case 'r' :
			fnc_debug++;
			break;
		case 's' :
			elsz = optarg;
			break;
		case ':' :
			errflg++;
			break;
		case '?' :
			errflg++;
			break;
		}
	}
	if(errflg > 0) {
		usage();
		exit(2);
	}

	MEM_DLVL(mem_debug);
	fnc_init(fncrc, stderr);
	fnc_set_dbg(fncrc, fnc_debug);
{
	arr_t	*aptr = NULL;
	leng_t	 cnt = 0LL;
	leng_t	 sz = 0LL;

	if(elsz != NULL) {
		sz = atoll(elsz);
	}
	if(elcnt != NULL) {
		cnt = atoll(elcnt);
	}
	

	TRY(arr_new, _door, sz, cnt, &aptr);
	if(dat_debug > 0) {
		arr_prn(aptr, stdout, 0);
	}
	TRY(arr_drop, _door, aptr);
}

_door:
	if((fncrc->fnc_dbg > 0) || (fncrc->fnc_result != FNC_OKAY)) {
		fnc_prn(fncrc, stderr, 0);
	}
	MEM_STAT(stderr);
	return(rc);
}
