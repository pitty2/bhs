#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef Linux
#include <unistd.h>
#endif
#include <bhs.h>

char	*prg_name = NULL;

void
usage(void) {
	fprintf(stderr, "usage : %s [-m] [-s] [-d]\n", prg_name);
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

	int	 mem_debug = 0;
	int	 sess_debug = 0;
	int	 data_debug = 0;
	int	 errflg = 0;

	fncrc_t	 ack;
	sess_t	*sp = NULL;


	prg_name = argv[0];

#if (0)
	if(argc == 1) {
		fprintf(stderr, "%s : illegal parameter count!\n", argv[0]);
		usage();
		exit(2);
	}
#endif

	while((c = getopt(argc, argv, "msd")) != EOF) {
		switch(c) {
		case 'm' :
			mem_debug++;
			break;
		case 's' :
			sess_debug++;
			break;
		case 'd' :
			data_debug++;
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

	fnc_init(&ack, stderr);
	rc = sess_new(&ack, stdin, stdout, stderr, &sp);
	MEM_DLVL(mem_debug);
	rc = sess_setdbg(&ack, sp, sess_debug);
	rc = sess_set_data_dbg(&ack, sp, data_debug);
{
	data_link_t	*lnk = NULL;

	data_id_t	*data_id = NULL;
	data_sequ_t	*data_sequ = NULL;
	data_t		*data = NULL;

	data_id_t	*str_id = NULL;
	data_sequ_t	*str_sequ = NULL;
	data_t		*str = NULL;

	data_id_t	*num_id = NULL;
	data_sequ_t	*num_sequ = NULL;
	data_t		*num = NULL;

	data_id_t	*bool_id = NULL;
	data_sequ_t	*bool_sequ = NULL;
	data_t		*bool = NULL;

	data_id_t	*fnc_id = NULL;
	data_sequ_t	*fnc_sequ = NULL;
	data_t		*fnc = NULL;
	data_t		*file = NULL;

	data_t		*str2 = NULL;
	data_t		*num2 = NULL;
	data_t		*bool2 = NULL;
	data_t		*fnc2 = NULL;

	data_t		*lst2 = NULL;
	
#if (0)
	CALL(&ack, _door, rc, data_id_new, (&ack, NULL, "Test-Data", &data_id));
#endif

	if((rc = data_id_new(&ack, sp, "Test-Data", &data_id)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_sequ_new(&ack, sp, __LINE__, __FILE__, &data_sequ)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_lst_new(&ack, sp, data_id, &data)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_sequ_set(&ack, sp, data, data_sequ)) != FNC_OKAY) {
		goto _door;
	}


	if((rc = data_id_new(&ack, sp, "Test-String", &str_id)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_sequ_new(&ack, sp, __LINE__, __FILE__, &str_sequ)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_str_new(&ack, sp, str_id, 6, "String", &str)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_sequ_set(&ack, sp, str, str_sequ)) != FNC_OKAY) {
		goto _door;
	}

	if((rc = data_id_new(&ack, sp, "Test-Number", &num_id)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_sequ_new(&ack, sp, __LINE__, __FILE__, &num_sequ)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_num_new(&ack, sp, num_id, 0x0ffffff, &num)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_sequ_set(&ack, sp, num, num_sequ)) != FNC_OKAY) {
		goto _door;
	}

	if((rc = data_id_new(&ack, sp, "Test-Boolean", &bool_id)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_sequ_new(&ack, sp, __LINE__, __FILE__, &bool_sequ)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_bool_new(&ack, sp, bool_id, FALSE, &bool)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_sequ_set(&ack, sp, bool, bool_sequ)) != FNC_OKAY) {
		goto _door;
	}

	if((rc = data_id_new(&ack, sp, "Test-Function", &fnc_id)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_sequ_new(&ack, sp, __LINE__, __FILE__, &fnc_sequ)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_fnc_new(&ack, sp, fnc_id, data_bool_new, &fnc)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_sequ_set(&ack, sp, fnc, fnc_sequ)) != FNC_OKAY) {
		goto _door;
	}


	if((rc = data_file_new(&ack, sp, NULL, "MyFile", &file)) != FNC_OKAY) {
		goto _door;
	}



	if((rc = data_lst_ins_first(&ack, sp, data, data, &lnk)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_lst_ins_last(&ack, sp, data, str, &lnk)) != FNC_OKAY) {
		goto _door;
	}
#if (1)
	if((rc = data_str_cpy(&ack, sp, str, &str2)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_lst_ins_last(&ack, sp, data, str2, &lnk)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_num_cpy(&ack, sp, num, &num2)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_lst_ins_last(&ack, sp, data, num2, &lnk)) != FNC_OKAY) {
		goto _door;
	}
#endif
	if((rc = data_lst_ins_last(&ack, sp, data, num, &lnk)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_lst_ins_last(&ack, sp, data, bool, &lnk)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_lst_ins_last(&ack, sp, data, fnc, &lnk)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_lst_ins_last(&ack, sp, data, file, &lnk)) != FNC_OKAY) {
		goto _door;
	}

	if((rc = data_lst_ins_last(&ack, sp, data, str, &lnk)) != FNC_OKAY) {
		goto _door;
	}


	if((rc = data_cpy(&ack, sp, data, &lst2)) != FNC_OKAY) {
		goto _door;
	}



	if((rc = data_prn(&ack, sp, data, sp->sess_out, 0)) != FNC_OKAY) {
		goto _door;
	}
	if((rc = data_prn(&ack, sp, lst2, sp->sess_out, 0)) != FNC_OKAY) {
		goto _door;
	}



	if((rc = data_drop(&ack, sp, data)) != FNC_OKAY) {
		FNC_ATXT(&ack, "data_drop(data)");
		goto _door;
	}
	if((rc = data_drop(&ack, sp, lst2)) != FNC_OKAY) {
		FNC_ATXT(&ack, "data_drop(lst2)");
		goto _door;
	}
}

_door:
	if((rc = sess_drop(&ack, sp)) != FNC_OKAY) {
		FNC_ATXT(&ack, "sess_drop");
	}
	fnc_prn(&ack, stderr, 0);
	MEM_STAT(stderr);
	return(rc);
}
