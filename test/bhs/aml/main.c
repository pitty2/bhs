#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef Linux
#include <unistd.h>
#endif
#include <bhs.h>

void
usage(void) {
	fprintf(stderr, "usage : -f <inputfile> [-n <searchname>] [-c[c...]] [-d[d...]] [-m[m...]] [-l[l...]] [-y[y...]] [-s[s...]] [-r[r...]]\n");
	fprintf(stderr, "        <inputfile>  ::= filename\n");
	fprintf(stderr, "        <serachname> ::= name[->name...]\n");
	fprintf(stderr, "        -c           ::= compile debug-level\n");
	fprintf(stderr, "        -d           ::= data-store debug-level\n");
	fprintf(stderr, "        -m           ::= memory debug-level\n");
	fprintf(stderr, "        -l           ::= lexical scanner debug-level\n");
	fprintf(stderr, "        -y           ::= grammar parser debug-level\n");
	fprintf(stderr, "        -s           ::= session debug-level\n");
	fprintf(stderr, "        -r           ::= function-framework debug-level\n");
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

	int	 fnc_debug = 0;
	int	 sess_debug = 0;
	int	 data_debug = 0;
	int	 comp_dbg = 0;
	int	 yy_debug = 0;
	int	 lex_debug = 0;
	int	 mem_debug = 0;
	char	*filename = NULL;
	char	*searchname = NULL;

	fncrc_t	 ack;
	fncrc_t	*fncrc = &ack;
	sess_t	*sp = NULL;
	comp_t	*cp = NULL;
	data_t	*tree = NULL;

	data_t	*val = NULL;
	data_t	*stree = NULL;

	int	 errflg = 0;

	fnc_init(fncrc, stderr);

	if(argc == 1) {
		fprintf(stderr, "%s : illegal parameter count!\n", argv[0]);
		usage();
		exit(2);
	}

	while((c = getopt(argc, argv, "cdmlyrsf:n:")) != EOF) {
		switch(c) {
		case 'c' :
			comp_dbg++;
			break;
		case 'd' :
			data_debug++;
			break;
		case 'l' :
			lex_debug++;
			break;
		case 'm' :
			mem_debug++;
			break;
		case 'y' :
			yy_debug++;
			break;
		case 's' :
			sess_debug++;
			break;
		case 'r' :
			fnc_debug++;
			break;
		case 'f' :
			filename = optarg;
			break;
		case 'n' :
			searchname = optarg;
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

	fnc_set_dbg(fncrc, fnc_debug);

	TRY(sess_new, _door, stdin, stdout, stderr, &sp);
	sess_setdbg(fncrc, sp, sess_debug);
	sess_set_data_dbg(fncrc, sp, data_debug);

	TRY(comp_new, _door, sp, &aml_comp, &cp);
	comp_set_dbg(fncrc, sp, cp, comp_dbg);
	comp_set_lex_dbg(fncrc, sp, cp, lex_debug);
	comp_set_yacc_dbg(fncrc, sp, cp, yy_debug);

	TRY(comp_exec, _door, sp, cp, filename, &tree);
	if(tree != NULL) {
		if(sp->sess_data_dbg > 0) {
			TRY(data_prn, _door, sp, tree, sp->sess_out, 0);
		}
		if(searchname != NULL) {
			TRY(data_by_ident, _door, sp, tree, SYMTREE, &stree);
			TRY(bhs_value_by_symbol, _door, sp, stree, searchname, &val);
			if(sp->sess_data_dbg > 0) {
				TRY(data_prn, _door, sp, val, sp->sess_err, 0);
			}
		}
	}

_door:
	if(cp != NULL) {
		if(cp->comp_dbg > 0) {
			comp_prn(fncrc, sp, cp, stderr, 0);
		}
		if((rc = comp_drop(fncrc, sp, cp)) != FNC_OKAY) {
			FNC_ERR("comp_drop");
		}
		cp = NULL;
	}
	if(tree != NULL) {
		if((rc = data_drop(fncrc, sp, tree)) != FNC_OKAY) {
			FNC_ERR("data_drop");
		}
	}

	if(sp != NULL) {
		if(sp->sess_dbg != 0) {
			sess_prn(fncrc, sp, stderr, 0);
		}
		sess_drop(fncrc, sp);
	}
	if((fncrc->fnc_dbg > 0) || (fncrc->fnc_result != FNC_OKAY)) {
		fnc_prn(fncrc, stderr, 0);
	}
	fnc_clear(fncrc);
	MEM_STAT(stderr);
	return(rc);
}
