#ifdef Linux
#define _GNU_SOURCE
#include <link.h>
#endif
#include <dlfcn.h>
#include <bhs/compile.h>

char	*comp_cause[] = {
	"COMP okay",
	"COMP malloc() failed!",
	"COMP syscall failed!",
	"COMP wrong designator!",
	"COMP not in list!",
	"COMP hirarchie out of band!",
	"COMP conversion error!",
	"COMP identifier error!",
	"COMP syntax error!",
	"COMP symbol error!"
};

/*
	------------------------------------------------------------------------
 */
int
scne_init(
	fncrc_t		*fncrc,
	sess_t		*sp,
	scne_t		*p
) {
	int		 rc = FNC_OKAY;
	data_t		*lst = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	p->scne_dbg = 0;
	TRY(data_lst_new, _door, sp, NULL, &lst);
	p->scne_file_stack = lst;
	TRY(data_lst_new, _door, sp, NULL, &lst);
	p->scne_file_save = lst;

_door:
	if(rc != FNC_OKAY) {
		if(p->scne_file_stack != NULL) {
			data_drop(NEXT_FNCRC, sp, p->scne_file_stack);
			p->scne_file_stack = NULL;
		}
	}
	FNC_RETURN(rc);
}

int
scne_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	scne_t		**pp
) {
	int		 rc = FNC_OKAY;
	scne_t		*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	if((p = CALLOC(1, sizeof(*p))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, sizeof(%llu)) failed!",(leng_t)sizeof(*p));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	TRY(scne_init, _door, sp, p);

_door:
	if((rc != FNC_OKAY) && (p != NULL)) {
		FREE(p);
		p = NULL;
	}
	*pp = p;
	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
scne_fini(
	fncrc_t		*fncrc,
	sess_t		*sp,
	scne_t		*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	if(p->scne_file != NULL) {
		TRY(data_drop, _door, sp, p->scne_file);
		p->scne_file = NULL;
	}
	TRY(data_drop, _door, sp, p->scne_file_stack);
	TRY(data_drop, _door, sp, p->scne_file_save);

_door:
	FNC_RETURN(rc);
}

int
scne_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	scne_t		*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	TRY(scne_fini, _door, sp, p);
_door:
	FREE(p);
	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
#define PRN_FUNC(p, name) \
	dladdr((void *)p->name, &fnc_info); \
	PRNN(n); PRNO("%-25s = %-25s(0x%p)\n", #name, TSTR(fnc_info.dli_sname), p->name); 

int
scne_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	scne_t		*p,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;
	Dl_info		 fnc_info;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	PRNN(n); PRNO("scne_t -> (%p) {\n", p);
	n++;
		PRNN(n); PRNO("scne_dbg  = %d\n", p->scne_dbg);
		PRNN(n); PRNO("scne_file = {\n");
		n++;
			if(p->scne_file != NULL) {
				data_prn(fncrc, sp, p->scne_file, out, n);
			} else {
				PRNN(n); PRNO("{nil}\n");
			}
		n--;
		PRNN(n); PRNO("}\n");

		PRNN(n); PRNO("scne_file_stack = {\n");
		n++;
			data_prn(fncrc, sp, p->scne_file_stack, out, n);
		n--;
		PRNN(n); PRNO("}\n");

		PRNN(n); PRNO("scne_file_save = {\n");
		n++;
			data_prn(fncrc, sp, p->scne_file_save, out, n);
		n--;
		PRNN(n); PRNO("}\n");

		PRN_FUNC(p, scne_restart);
		PRN_FUNC(p, scne_get_current_buffer);
		PRN_FUNC(p, scne_switch_to_buffer);
		PRN_FUNC(p, scne_create_buffer);
		PRN_FUNC(p, scne_delete_buffer);
		PRN_FUNC(p, scne_flush_buffer);
		PRN_FUNC(p, scne_push_buffer_state);
		PRN_FUNC(p, scne_pop_buffer_state);
		PRN_FUNC(p, snce_scan_buffer);
		PRN_FUNC(p, snce_scan_string);
		PRN_FUNC(p, snce_scan_bytes);
		PRN_FUNC(p, scne_alloc);
		PRN_FUNC(p, scne_realloc);
		PRN_FUNC(p, scne_free);
		PRN_FUNC(p, scne_init);
		PRN_FUNC(p, scne_init_extra);
		PRN_FUNC(p, scne_fini);
		PRN_FUNC(p, scne_get_extra);
		PRN_FUNC(p, scne_set_extra);
		PRN_FUNC(p, scne_get_debug);
		PRN_FUNC(p, scne_set_debug);
		PRN_FUNC(p, scne_get_in);
		PRN_FUNC(p, scne_set_in);
		PRN_FUNC(p, scne_get_out);
		PRN_FUNC(p, scne_set_out);
		PRN_FUNC(p, scne_get_leng);
		PRN_FUNC(p, scne_get_text);
		PRN_FUNC(p, scne_get_lineno);
		PRN_FUNC(p, scne_set_lineno);
		PRN_FUNC(p, scne_get_lval);
		PRN_FUNC(p, scne_set_lval);

	n--;
	PRNN(n); PRNO("}\n");

	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
parser_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	parser_t	**pp
) {
	int		 rc = FNC_OKAY;;
	parser_t	*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKOUTP(rc, pp);

	if((p = CALLOC(1, sizeof(*p))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!",(leng_t)sizeof(*p));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	p->parser_dbg = 0;
	p->parser_errors = 0;
	p->parser_errortxt = NULL;
	p->parser_fatal = NULL;

	TRY(data_lst_new, _door, sp, SYMTREE, &p->parser_symtree);
	TRY(data_lst_new, _door, sp, REFTREE, &p->parser_reftree);
	TRY(data_lst_new, _door, sp, EXPRTREE, &p->parser_exptree);
	TRY(data_lst_new, _door, sp, RESTREE, &p->parser_restree);
	dll_init(&p->parser_assoc_lst);

_door:
	if(rc != FNC_OKAY) {
		if(p->parser_tree != NULL) {
			data_drop(NEXT_FNCRC, sp, p->parser_tree);
			p->parser_tree = NULL;
		}
		if(p->parser_symtree != NULL) {
			data_drop(NEXT_FNCRC, sp, p->parser_symtree);
			p->parser_symtree = NULL;
		}
		if(p->parser_reftree != NULL) {
			data_drop(NEXT_FNCRC, sp, p->parser_reftree);
			p->parser_reftree = NULL;
		}
		if(p->parser_exptree != NULL) {
			data_drop(NEXT_FNCRC, sp, p->parser_exptree);
			p->parser_exptree = NULL;
		}
		if(p->parser_restree != NULL) {
			data_drop(NEXT_FNCRC, sp, p->parser_restree);
			p->parser_restree = NULL;
		}
		FREE(p);
		p = NULL;
	}
	*pp = p;
	FNC_RETURN(rc);
}

int
parser_drop(
	fncrc_t		*fncrc,
	sess_t		*sp,
	parser_t	*p
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	if(p->parser_errortxt != NULL) {
		TRY(data_drop, _door, sp, p->parser_errortxt);
		p->parser_errortxt = NULL;
	}
	if(p->parser_tree != NULL) {
		TRY(data_drop, _door, sp, p->parser_tree);
		p->parser_tree = NULL;
	}
	if(p->parser_symtree != NULL) {
		TRY(data_drop, _door, sp, p->parser_symtree);
		p->parser_symtree = NULL;
	}
	if(p->parser_reftree != NULL) {
		TRY(data_drop, _door, sp, p->parser_reftree);
		p->parser_reftree = NULL;
	}
	if(p->parser_exptree != NULL) {
		TRY(data_drop, _door, sp, p->parser_exptree);
		p->parser_exptree = NULL;
	}
	if(p->parser_restree != NULL) {
		TRY(data_drop, _door, sp, p->parser_restree);
		p->parser_restree = NULL;
	}
	FREE(p);

_door:
	FNC_RETURN(rc);
}

int
parser_prn(
	fncrc_t		*fncrc,
	sess_t		*sp,
	parser_t	*p,
	FILE		*out,
	leng_t		 n
) {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	PRNN(n); PRNO("parser_t -> (%p) {\n", p);
	n++;
		PRNN(n); PRNO("parser_dbg       = %d\n", p->parser_dbg);
		PRNN(n); PRNO("parser_errors    = %d\n", p->parser_errors);
		PRNN(n); PRNO("parser_warnings  = %d\n", p->parser_warnings);
		PRNN(n); PRNO("parser_errortxt  = %p\n", p->parser_errortxt);
		PRNN(n); PRNO("parser_fatal     = 0x%p\n", p->parser_fatal);
		PRNN(n); PRNO("parser_tree      = 0x%p {\n", p->parser_tree);
			if(p->parser_tree != NULL) {
				data_prn(fncrc, sp, p->parser_tree, out, n+1);
			}
		PRNN(n); PRNO("}\n");
		PRNN(n); PRNO("parser_symtree   = 0x%p {\n", p->parser_symtree);
			if(p->parser_symtree != NULL) {
				data_prn(fncrc, sp, p->parser_symtree, out, n+1);
			}
		PRNN(n); PRNO("}\n");
		PRNN(n); PRNO("parser_reftree   = 0x%p {\n", p->parser_reftree);
			if(p->parser_reftree != NULL) {
				data_prn(fncrc, sp, p->parser_reftree, out, n+1);
			}
		PRNN(n); PRNO("}\n");
		PRNN(n); PRNO("parser_exptree   = 0x%p {\n", p->parser_exptree);
			if(p->parser_exptree != NULL) {
				data_prn(fncrc, sp, p->parser_exptree, out, n+1);
			}
		PRNN(n); PRNO("}\n");
		PRNN(n); PRNO("parser_restree   = 0x%p {\n", p->parser_restree);
			if(p->parser_restree != NULL) {
				data_prn(fncrc, sp, p->parser_restree, out, n+1);
			}
		PRNN(n); PRNO("}\n");
	n--;
	PRNN(n); PRNO("}\n");

	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
comp_new(
	fncrc_t	*fncrc,
	sess_t	*sp,
const	comp_t	*tp,
	comp_t	**pp
) {
	int	 rc = FNC_OKAY;
	comp_t	*p = NULL;
	char	*func_name = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, tp);	 /* Template */
	FNC_CHKOUTP(rc, pp);	/* compiler control block */

	if((p = CALLOC(1, sizeof(*p))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!",(leng_t)sizeof(*p));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}

	/*
	 * copy data from template.
	*/
	if((p->comp_name = STRDUP(tp->comp_name)) == NULL) {
		FREE(p);
		p = NULL;
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(COMP_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "STRDUP() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	TRY(sess_new, _door, sp->sess_in, sp->sess_out, sp->sess_err, &p->comp_sp);
	p->comp_scanner = tp->comp_scanner;
	p->comp_scanner_init = tp->comp_scanner_init;
	p->comp_scanner_init_extra = tp->comp_scanner_init_extra;
	p->comp_scanner_fini = tp->comp_scanner_fini;
	p->comp_parser = tp->comp_parser;
	p->comp_eval = tp->comp_eval;
	p->comp_gen = tp->comp_gen;

	if((rc = scne_new(fncrc, sp, &p->comp_scanner_ext)) != FNC_OKAY) {
		FNC_ERR("scne_new");
		FREE(p);
		FREE(p->comp_name);
		p = NULL;
		goto _door;
	}

	/*
	 * Initialisiere Scanner
	 */
	if((rc = p->comp_scanner_init(&p->comp_scanner_db)) != FNC_OKAY) {
		func_name = fnc_addr_2name((void *)p->comp_scanner_init);
		FNC_ERR(func_name);
		FREE(func_name);
		func_name = NULL;
		FREE(p);
		FREE(p->comp_name);
		scne_drop(fncrc, sp, p->comp_scanner_ext);
		p = NULL;
		goto _door;
	}
	if((rc = p->comp_scanner(NULL, fncrc, sp, p, p->comp_scanner_db)) != FNC_OKAY) {
		func_name = fnc_addr_2name((void *)p->comp_scanner);
		FNC_ERR(func_name);
		FREE(func_name);
		func_name = NULL;
		FREE(p);
		FREE(p->comp_name);
		scne_drop(fncrc, sp, p->comp_scanner_ext);
		p = NULL;
		goto _door;
	}

	if((rc = p->comp_scanner_init_extra(p, &p->comp_scanner_db)) != 0) {
		func_name = fnc_addr_2name((void *)p->comp_scanner_init_extra);
		FNC_ERR(func_name);
		FREE(func_name);
		func_name = NULL;
		FREE(p);
		FREE(p->comp_name);
		scne_drop(fncrc, sp, p->comp_scanner_ext);
		p = NULL;
		goto _door;
	}

	if((rc = parser_new(fncrc, sp, &p->comp_parser_ext)) != FNC_OKAY) {
		FNC_ERR("parser_new");
		FREE(p);
		FREE(p->comp_name);
		scne_drop(fncrc, sp, p->comp_scanner_ext);
		p = NULL;
		goto _door;
	}

	p->comp_free_fnc = FREE_FNC;
_door:
	*pp = p;
	FNC_RETURN(rc);
}

int
comp_drop(
	fncrc_t	*fncrc,
	sess_t	*sp,
	comp_t	*p
) {
	int	 rc = FNC_OKAY;
	int	 _rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	if(COMP_DBG(p) == COMP_FULL_DBG) {
		comp_prn(NEXT_FNCRC, sp, p, sp->sess_err, 0);
	}
	if(p->comp_name != NULL) {
		FREE(p->comp_name);
	}
	if(p->comp_scanner_db != NULL) {
		_rc = p->comp_scanner_fini(p->comp_scanner_db);
		p->comp_scanner_db = NULL;
	}

	if(p->comp_scanner_ext != NULL) {
		TRY(scne_drop, _door, sp, p->comp_scanner_ext);
		p->comp_scanner_ext = NULL;
	}

	if(p->comp_parser_ext != NULL) {
		TRY(parser_drop, _door, sp, p->comp_parser_ext);
		p->comp_parser_ext = NULL;
	}
	TRY(sess_drop, _door, p->comp_sp);
	if(p->comp_free_fnc != NULL) {
		p->comp_free_fnc(p);
	}
_door:
	FNC_RETURN(rc);
}

int
comp_prn(
	fncrc_t	*fncrc,
	sess_t	*sp,
	comp_t	*p,
	FILE	*out,
	leng_t	 n
) {
	int	 rc = FNC_OKAY;
	Dl_info	 fnc_info;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	PRNN(n); PRNO("comp_t -> (%p) {\n", p);
	n++;
	if(p != NULL) {
		PRNN(n); PRNO("comp_name               = %s\n", TSTR(p->comp_name));
		PRNN(n); PRNO("comp_dbg                = %d\n", p->comp_dbg);
		PRNN(n); PRNO("comp_sp                 = 0x%p\n", p->comp_sp);
		sess_prn(fncrc, p->comp_sp, out, n+1);

		PRN_FUNC(p, comp_scanner);
		PRN_FUNC(p, comp_scanner_init);
		PRN_FUNC(p, comp_scanner_fini);
		PRN_FUNC(p, comp_scanner_init_extra);
		PRNN(n); PRNO("comp_scanner_db         = 0x%p\n", p->comp_scanner_db);
		PRNN(n); PRNO("comp_scanner_ext        = 0x%p\n", p->comp_scanner_ext);


		PRN_FUNC(p, comp_parser);
		PRNN(n); PRNO("comp_parser_ext         = 0x%p\n", p->comp_parser_ext);

		PRN_FUNC(p, comp_eval);
		PRN_FUNC(p, comp_gen);
		PRNN(n); PRNO("comp_tree               = 0x%p\n", p->comp_tree);

		PRNN(n); PRNO("comp_scanner_ext        = {\n");
		n++;
		scne_prn(fncrc, sp, p->comp_scanner_ext, out, n);
		n--;
		PRNN(n); PRNO("}\n");
		PRNN(n); PRNO("comp_parser_ext         = {\n");
		n++;
		parser_prn(fncrc, sp, p->comp_parser_ext, out, n);
		n--;
		PRNN(n); PRNO("}\n");
	}
	n--;
	PRNN(n); PRNO("}\n");

	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
comp_exec(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	char		*fn,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*in = NULL;
	data_t		*p = NULL;
	data_link_t	*lnk = NULL;
	int		 _rc = FNC_OKAY;
	char		*func_name = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, fn);
	FNC_CHKOUTP(rc, pp);

	TRY(data_file_new, _door, sp, NULL, fn, &in);
	TRY(data_file_open, _next, sp, in, "r");
_next:
	if (rc != FNC_OKAY) {
		data_file_drop(fncrc, sp, in);
		in = NULL;
		goto _door;
	}
	cp->comp_scanner_ext->scne_file = in;
	cp->comp_parser(fncrc, sp, cp, cp->comp_scanner_db);
	if(fncrc->fnc_result != FNC_OKAY) {
		func_name = fnc_addr_2name((void *)cp->comp_parser);
		FNC_ERR(func_name);
		FREE(func_name);
		func_name = NULL;
		goto _door;
	}
	if(COMP_DBG(cp) >= COMP_FULL_DBG) {
		data_prn(NEXT_FNCRC, sp, COMP_PTREE(cp), sp->sess_err, 0);
	}

	TRY(comp_resolve, _door, sp, cp);
	if(COMP_ERRORS(cp) == 0) {
		if((rc = cp->comp_gen(fncrc, sp, cp)) != FNC_OKAY) {
			func_name = fnc_addr_2name((void *)cp->comp_gen);
			FNC_ERR(func_name);
			FREE(func_name);
			func_name = NULL;
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _door;
		}
	} else {
		rc = fncrc->fnc_result;
		FNC_ERR("comp_resolve");
		goto _door;
	}
	TRY(data_lst_new, _door, sp, RESTREE, &p);

	if(COMP_SYMTREE(cp) != NULL) {
		TRY(data_lst_ins_last, _door, sp, p, COMP_SYMTREE(cp), &lnk);
		COMP_SYMTREE(cp) = NULL;
	}
	if(COMP_REFTREE(cp) != NULL) {
		TRY(data_lst_ins_last, _door, sp, p, COMP_REFTREE(cp), &lnk);
		COMP_REFTREE(cp) = NULL;
	}
	if(COMP_PTREE(cp) != NULL) {
		TRY(data_lst_ins_last, _door, sp, p, COMP_PTREE(cp), &lnk);
		COMP_PTREE(cp) = NULL;
	}

_door:
	in = cp->comp_scanner_ext->scne_file;
	if(rc != FNC_OKAY) {
		if(COMP_DBG(cp) >= COMP_FULL_DBG) {
			if((_rc = parser_prn(NEXT_FNCRC, sp, COMP_YACCEXT(cp), sp->sess_err, 0)) != FNC_OKAY) {
				PRNE(sp->sess_err, "parser_prn");
			}
		}
		if((_rc = parser_drop(NEXT_FNCRC, sp, COMP_YACCEXT(cp))) != FNC_OKAY) {
			PRNE(sp->sess_err, "parser_drop");
		}
		COMP_YACCEXT(cp) = NULL;

		if((rc = cp->comp_scanner_fini(cp->comp_scanner_db)) != 0) {
			FNC_ATXT(fncrc, "comp_scanner_fini");
		}
		cp->comp_scanner_db = NULL;

		if(COMP_DBG(cp) >= COMP_FULL_DBG) {
			if((_rc = scne_prn(NEXT_FNCRC, sp, COMP_LEXEXT(cp), sp->sess_err, 0)) != FNC_OKAY) {
				PRNE(sp->sess_err, "parser_prn");
			}
		}
		if((_rc = scne_drop(NEXT_FNCRC, sp, COMP_LEXEXT(cp))) != FNC_OKAY) {
			PRNE(sp->sess_err, "scne_drop");
		}
		COMP_LEXEXT(cp) = NULL;

		if(p != NULL) {
			if((_rc = data_drop(NEXT_FNCRC, sp, p)) != FNC_OKAY) {
				PRNE(sp->sess_err, "data_drop");
			}
			p = NULL;
		}
	} else {
		if(in != NULL) {
			if((_rc = data_file_drop(NEXT_FNCRC, sp, in)) != FNC_OKAY) {
				PRNE(sp->sess_err, "data_file_drop");
			}
			cp->comp_scanner_ext->scne_file = NULL;
		}
	}
	*pp = p;
	FNC_RETURN(rc);
}

int
comp_set_dbg(
	fncrc_t	*fncrc,
	sess_t	*sp,
	comp_t	*cp,
	int	 dbg
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);

	COMP_DBG(cp) = dbg;
	if(COMP_DBG(cp) == COMP_FULL_DBG) {
		if((rc = comp_prn(fncrc, sp, cp, sp->sess_err, 0)) != FNC_OKAY) {
			FNC_ATXT(fncrc, "comp_prn");
		}
	}
	FNC_RETURN(rc);
}

int
comp_set_lex_dbg(
	fncrc_t	*fncrc,
	sess_t	*sp,
	comp_t	*cp,
	int	 dbg
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);

	COMP_LEXDBG(cp) = dbg;
	FNC_RETURN(rc);
}

int
comp_set_yacc_dbg(
	fncrc_t	*fncrc,
	sess_t	*sp,
	comp_t	*cp,
	int	 dbg
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);

	COMP_YACCDBG(cp) = dbg;
	FNC_RETURN(rc);
}

void
comp_init(comp_t *p) {
}

void
comp_fini(comp_t *p) {
}
/*
	------------------------------------------------------------------------
 */
DECLARE_DATA_IDENT(aml_id,		"aml");
DECLARE_DATA_IDENT(symtree_id,		"symbol-tree");
DECLARE_DATA_IDENT(reftree_id,		"reference-tree");
DECLARE_DATA_IDENT(exprtree_id,		"expression-tree");
DECLARE_DATA_IDENT(restree_id,		"result-tree");

DECLARE_DATA_IDENT(error_id,		"error");
DECLARE_DATA_IDENT(errtxt_id,		"error-description");
DECLARE_DATA_IDENT(octet_id,		"octet");

DECLARE_DATA_IDENT(assign_id,		"assign");
DECLARE_DATA_IDENT(value_id,		"value");

DECLARE_DATA_IDENT(ident_id,		"identifier");
DECLARE_DATA_IDENT(name_id,		"name");
DECLARE_DATA_IDENT(normalized_id,	"normalized-symbol");
DECLARE_DATA_IDENT(symbol_id,		"symbol");
DECLARE_DATA_IDENT(obj_identifier_id,	"object-identifier");
DECLARE_DATA_IDENT(reference_id,	"reference");
DECLARE_DATA_IDENT(array_index_id,	"array-index");

DECLARE_DATA_IDENT(expr_id,		"expr");
DECLARE_DATA_IDENT(struct_id,		"struct");
DECLARE_DATA_IDENT(array_id,		"array");
DECLARE_DATA_IDENT(string_id,		"string");
DECLARE_DATA_IDENT(number_id,		"number");
DECLARE_DATA_IDENT(numeric_id,		"numeric");
DECLARE_DATA_IDENT(boolean_id,		"boolean");

DECLARE_DATA_IDENT(any_id,		"any");

