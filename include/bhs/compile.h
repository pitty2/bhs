#ifndef __BHS_COMPILE_H__
#define __BHS_COMPILE_H__

#include <bhs/data.h>
#include <bhs/data_conv.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern	char	*comp_cause[];
#define	COMP_CAUSE_OKAY		comp_cause[0]
#define	COMP_CAUSE_NOMEM	comp_cause[1]
#define	COMP_CAUSE_SYSCALL	comp_cause[2]
#define	COMP_CAUSE_DESIGNATOR	comp_cause[3]
#define	COMP_CAUSE_NOTINLIST	comp_cause[4]
#define	COMP_CAUSE_HIRARCHIE	comp_cause[5]
#define	COMP_CAUSE_CONVERSION	comp_cause[6]
#define	COMP_CAUSE_IDENTIFIER	comp_cause[7]
#define	COMP_CAUSE_SYNTAX	comp_cause[8]
#define	COMP_CAUSE_SYMBOL	comp_cause[9]

typedef struct comp_s comp_t;

extern	comp_t	aml_comp;	/* Definition for AML-Language Parser/Printer */

extern int comp_new(fncrc_t *fncrc, sess_t *sp, const comp_t *tp, comp_t **pp);
extern int comp_drop(fncrc_t *fncrc, sess_t *sp, comp_t *p);
extern int comp_prn(fncrc_t *fncrc, sess_t *sp, comp_t *p, FILE *out, leng_t n);
extern int comp_exec(fncrc_t *fncrc, sess_t *sp, comp_t *cp, char *fn, data_t **pp);
extern int comp_resolve(fncrc_t *fncrc, sess_t *sp, comp_t *cp);

extern int comp_set_dbg(fncrc_t *fncrc, sess_t *sp, comp_t *cp, int dbg);
extern int comp_set_lex_dbg(fncrc_t *fncrc, sess_t *sp, comp_t *cp, int dbg);
extern int comp_set_yacc_dbg(fncrc_t *fncrc, sess_t *sp, comp_t *cp, int dbg);



/*
	------------------------------------------------------------------------
 */

#define COMP_PRN(sp, ...)					\
if (prn_ferror(sp->sess_err) == 0)  {				\
	fprintf (sp->sess_err, "%s +%d ", __FILE__, __LINE__);	\
	fprintf (sp->sess_err, "%s :", __func__);		\
	fprintf (sp->sess_err, __VA_ARGS__);			\
	fprintf (sp->sess_err, "\n");				\
	fflush(sp->sess_err);					\
} 

/*
	------------------------------------------------------------------------
 */
#if (1)

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE {
	data_t	*dp;
} YYSTYPE;
#define yystype YYSTYPE
#define	YYSTYPE_IS_DECLARED 1
#endif

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

#ifndef YY_BUF_SIZE
#define YY_BUF_SIZE 16384
#endif

#ifndef YY_TYPEDEF_YY_BUFFER_STATE
#define YY_TYPEDEF_YY_BUFFER_STATE
typedef struct yy_buffer_state *YY_BUFFER_STATE;
#endif

#ifndef YY_TYPEDEF_YY_SIZE_T
#define YY_TYPEDEF_YY_SIZE_T
typedef size_t yy_size_t;
#endif

#ifndef YY_EXTRA_TYPE
#define YY_EXTRA_TYPE comp_t *
#endif

#endif

struct scn_keytab_s {
	char	*name;
	int	 token;
};
typedef struct scn_keytab_s scn_key_t;


typedef struct scanner_extra_s scne_t;
struct scanner_extra_s {
	int	 scne_dbg;
	data_t	*scne_file;
	data_t	*scne_file_stack;
	data_t	*scne_file_save;

	void		 (*scne_restart)(FILE*, yyscan_t);
	void		*(*scne_get_current_buffer)(yyscan_t);
	void		 (*scne_switch_to_buffer)(YY_BUFFER_STATE, yyscan_t);
	YY_BUFFER_STATE	 (*scne_create_buffer)(FILE*, int, yyscan_t);
	void		 (*scne_delete_buffer)(YY_BUFFER_STATE, yyscan_t);
	void		 (*scne_flush_buffer)(YY_BUFFER_STATE, yyscan_t);
	void		 (*scne_push_buffer_state)(YY_BUFFER_STATE, yyscan_t);
	void		 (*scne_pop_buffer_state)(yyscan_t);

	YY_BUFFER_STATE	 (*snce_scan_buffer)(char*, yy_size_t, yyscan_t);
	YY_BUFFER_STATE	 (*snce_scan_string)(const char*, yyscan_t);
	YY_BUFFER_STATE	 (*snce_scan_bytes)(const char*, int, yyscan_t);

	void		*(*scne_alloc)(yy_size_t, yyscan_t);
	void		*(*scne_realloc)(void*, yy_size_t, yyscan_t);
	void		 (*scne_free)(void*, yyscan_t);

	int		 (*scne_init)(yyscan_t*);
	int		 (*scne_init_extra)(YY_EXTRA_TYPE, yyscan_t*);
	int		 (*scne_fini)(yyscan_t);
	YY_EXTRA_TYPE	 (*scne_get_extra)(yyscan_t);
	void		 (*scne_set_extra)(YY_EXTRA_TYPE, yyscan_t);

	int		 (*scne_get_debug)(yyscan_t);
	void		 (*scne_set_debug)(int, yyscan_t);

	FILE		*(*scne_get_in)(yyscan_t);
	void		 (*scne_set_in)(FILE*, yyscan_t);
	FILE		*(*scne_get_out)(yyscan_t);
	void		 (*scne_set_out)(FILE*, yyscan_t);

	int		 (*scne_get_leng)(yyscan_t);
	char		*(*scne_get_text)(yyscan_t);
	int		 (*scne_get_lineno)(yyscan_t);
	void		 (*scne_set_lineno)(int, yyscan_t);

	YYSTYPE		*(*scne_get_lval)(yyscan_t);
	void		 (*scne_set_lval)(YYSTYPE*, yyscan_t);

};

#define	SCNE_RESTART(cp)	((p)->comp_scanner_ext->scne_restart)
#define	SCNE_CURRENT_BUFFER(cp)	((p)->comp_scanner_ext->scne_get_current_buffer)
#define	SCNE_TO_BUFFER(cp)	((p)->comp_scanner_ext->scne_switch_to_buffer)
#define	SCNE_CREATE_BUFFER(cp)	((p)->comp_scanner_ext->scne_create_buffer)
#define	SCNE_DELETE_BUFFER(cp)	((p)->comp_scanner_ext->scne_delete_buffer)
#define	SCNE_FLUSH_BUFFER(cp)	((p)->comp_scanner_ext->scne_flush_buffer)
#define	SCNE_PUSH_BUFFER(cp)	((p)->comp_scanner_ext->scne_push_buffer_state)
#define	SCNE_POP_BUFFER(cp)	((p)->comp_scanner_ext->scne_pop_buffer_state)

#define	SCNE_SCAN_BUFFER(cp)	((p)->comp_scanner_ext->snce_scan_buffer)
#define	SCNE_SCAN_STRING(cp)	((p)->comp_scanner_ext->snce_scan_string)
#define	SCNE_SCAN_BYTES(cp)	((p)->comp_scanner_ext->snce_scan_bytes)

#define	SCNE_ALLOC(cp)		((p)->comp_scanner_ext->scne_alloc)
#define	SCNE_REALLOC(cp)	((p)->comp_scanner_ext->scne_realloc)
#define	SCNE_FREE(cp)		((p)->comp_scanner_ext->scne_free)

#define	SCNE_INIT(cp)		((p)->comp_scanner_ext->scne_init)
#define	SCNE_INIT_EXTRA(cp)	((p)->comp_scanner_ext->scne_init_extra)
#define	SCNE_FINI(cp)		((p)->comp_scanner_ext->scne_fini)
#define	SCNE_GET_EXTRA(cp)	((p)->comp_scanner_ext->scne_get_extra)
#define	SCNE_SET_EXTRA(cp)	((p)->comp_scanner_ext->scne_set_extra)


#define	COMP_SCNE(p)	((p != NULL) ? ((p)->comp_scanner_ext) : NULL)
#define	SCNE_FILE(p)	((COMP_SCNE(p) != NULL) ? ((COMP_SCNE(p))->scne_file) : NULL)
#define	SCNE_FDES(p)	((SCNE_FILE(p) != NULL) ? ((SCNE_FILE(p))->data_.file) : NULL)
#define	FDES_NAME(p)	((SCNE_FDES(p) != NULL) ? ((SCNE_FDES(p))->fd_name) : NULL)
#define	FDES_HDL(p)	((SCNE_FDES(p) != NULL) ? ((SCNE_FDES(p))->fd_hdl) : NULL)
#define	FDES_MODE(p)	((SCNE_FDES(p) != NULL) ? ((SCNE_FDES(p))->fd_mode) : NULL)
#define	FDES_LINENO(p)	((SCNE_FDES(p) != NULL) ? ((SCNE_FDES(p))->fd_lineno) : NULL)
#define	FDES_BUFFER(p)	((SCNE_FDES(p) != NULL) ? ((SCNE_FDES(p))->fd_buffer) : NULL)

#define	SCNE_DEBUG(p)	((COMP_SCNE(p) != NULL) ? ((COMP_SCNE(p))->scne_dbg) : 0)
#define	SCNE_STACK(p)	((COMP_SCNE(p) != NULL) ? ((COMP_SCNE(p))->scne_file_stack) : NULL)
#define	SCNE_SAVE(p)	((COMP_SCNE(p) != NULL) ? ((COMP_SCNE(p))->scne_file_save) : NULL)


int	 scne_init(fncrc_t *fncrc, sess_t *sp, scne_t *p);
int	 scne_new(fncrc_t *fncrc, sess_t *sp, scne_t **pp);
int	 scne_fini(fncrc_t *fncrc, sess_t *sp, scne_t *p);
int	 scne_drop(fncrc_t *fncrc, sess_t *sp, scne_t *p);
int	 scne_prn(fncrc_t *fncrc, sess_t *sp, scne_t *p, FILE *out, leng_t n);

/*
	-------------------------------------------------------------------------
 */


struct parser_extra_s {
	int	 parser_dbg;
	int	 parser_errors;
	int	 parser_warnings;
	data_t	*parser_errortxt;
	data_t	*parser_fatal;

	data_t	*parser_tree;
	data_t	*parser_symtree;
	data_t	*parser_reftree;
	data_t	*parser_exptree;
	data_t	*parser_restree;

	dll_t	 parser_assoc_lst;
};
typedef struct parser_extra_s parser_t;

int	 parser_new(fncrc_t *fncrc, sess_t *sp, parser_t **pp);
int	 parser_drop(fncrc_t *fncrc, sess_t *sp, parser_t *p);
int	 parser_prn(fncrc_t *fncrc, sess_t *sp, parser_t *p, FILE *out, leng_t n);

/*
 *	Compiler descriptor
	------------------------------------------------------------------------
 */
struct comp_s {
	char		*comp_name;
	int		 comp_dbg;
	sess_t		*comp_sp;
	int		(*comp_scanner)(YYSTYPE*, fncrc_t*, sess_t*, comp_t*, yyscan_t);
	int		(*comp_scanner_init)();
	int		(*comp_scanner_fini)();
	int		(*comp_scanner_init_extra)();
	yyscan_t	 comp_scanner_db;
	scne_t		*comp_scanner_ext;
	int		(*comp_parser)(fncrc_t*, sess_t*, comp_t*, yyscan_t);
	parser_t	*comp_parser_ext;
	int		(*comp_eval)(fncrc_t*, sess_t*, comp_t*, leng_t, data_link_t*, data_t **);
	int		(*comp_gen)(fncrc_t*, sess_t*, comp_t*);
	data_t		*comp_tree;

	void		(*comp_free_fnc)(void *);
};

#define COMP_DBG(p)	((p)->comp_dbg)
#define	COMP_NO_DBG	0
#define	COMP_SHORT_DBG	1
#define	COMP_PARAM_DBG	2
#define	COMP_DATA_DBG	3
#define	COMP_STACK_DBG	4
#define	COMP_CIRC_DBG	5
#define	COMP_FULL_DBG	6

#define COMP_SCANNER(p)	((p)->comp_scanner_db)
#define COMP_LEXDBG(p)	((p)->comp_scanner_ext->scne_dbg)
#define COMP_YACCDBG(p)	((p)->comp_parser_ext->parser_dbg)
#define COMP_LEXEXT(p)	((p)->comp_scanner_ext)
#define COMP_YACCEXT(p)	((p)->comp_parser_ext)

#define	COMP_PTREE(p)	((p)->comp_parser_ext->parser_tree)
#define	COMP_SYMTREE(p)	((p)->comp_parser_ext->parser_symtree)
#define	COMP_REFTREE(p)	((p)->comp_parser_ext->parser_reftree)
#define	COMP_EXPTREE(p)	((p)->comp_parser_ext->parser_exptree)
#define	COMP_RESTREE(p)	((p)->comp_parser_ext->parser_restree)
#define	COMP_ERRORS(p)	((p)->comp_parser_ext->parser_errors)
#define	COMP_WARNINGS(p)	((p)->comp_parser_ext->parser_warnings)
#define	COMP_ERRTXT(p)	((p)->comp_parser_ext->parser_errortxt)
#define	COMP_FATAL(p)	((p)->comp_parser_ext->parser_fatal)

#define COMP_MSG_LEN	255


#define	COMP_ID_SET(ack, sp, dp, id)					\
	if(data_id_set(ack, sp, dp, id) != FNC_OKAY) {			\
		FNC_ATXT(ack, "data_id_set");				\
		YYABORT;						\
	}

#define COMP_SEQU_SET(ack, sp, dst, src)				\
	if(data_sequ_inherit(ack, sp, (src), (dst)) != FNC_OKAY) {	\
		FNC_ATXT(ack, "data_sequ_inherit");			\
		YYABORT;						\
	}

#define COMP_SEQU_INHERIT(ack, sp, src, dst)				\
	if(data_sequ_inherit(ack, sp, (src), (dst)) != FNC_OKAY) {	\
		FNC_ATXT(ack, "data_sequ_inherit");			\
		YYABORT;						\
	}

#define	COMP_LST_NEW(ack, sp, id, dp)					\
	if(data_lst_new(ack, sp, id, &(dp)) != FNC_OKAY) {		\
		FNC_ATXT(ack, "data_lst_new");				\
		YYABORT;						\
	}

#define	COMP_LST_INSF(ack, sp, lst, elm)				\
{									\
	data_link_t	*ign = NULL;					\
									\
	if(data_lst_ins_first(ack, sp, lst, elm, &ign) != FNC_OKAY) {	\
		FNC_ATXT(ack, "data_lst_ins_first");			\
		YYABORT;						\
	}								\
}

#define	COMP_LST_INSL(ack, sp, lst, elm)				\
{									\
	data_link_t	*ign = NULL;					\
									\
	if(data_lst_ins_last(ack, sp, lst, elm, &ign) != FNC_OKAY) {	\
		FNC_ATXT(ack, "data_lst_ins_last");			\
		YYABORT;						\
	}								\
}

#define	COMP_DROP(ack, sp, dp)						\
	if(data_drop(ack, sp, dp) != FNC_OKAY) {			\
		FNC_ATXT(ack, "data_drop");				\
		YYABORT;						\
	}

/*
	------------------------------------------------------------------------
 */
int		comp_include(
			fncrc_t *fncrc,
			sess_t *sp,
			comp_t *cp,
			data_t *dp,
			data_t **pp
		);
/*
	comp_preproc.c prototypen
	------------------------------------------------------------------------
 */
extern	data_id_t	 preproc_id;
#define	PREPROC		&preproc_id

int		comp_inc_open(
			fncrc_t *fncrc,
			sess_t *sp,
			comp_t *cp,
			data_t *dp,
			data_t **pp
		);
int		comp_inc_eof(
			fncrc_t *fncrc,
			sess_t *sp,
			comp_t *cp
		);
int		comp_prep_inc(
			fncrc_t		*fncrc,
			sess_t		*sp,
			comp_t		*cp,
			leng_t		 argc,
			data_link_t	*argv,
			data_t		**pp
		);

/*
	------------------------------------------------------------------------
 */
extern	data_id_t	include_id;
extern	data_id_t	filename_id;
extern	data_id_t	aml_id;
extern	data_id_t	symtree_id;
extern	data_id_t	reftree_id;
extern	data_id_t	exprtree_id;
extern	data_id_t	restree_id;
extern	data_id_t	error_id;
extern	data_id_t	errtxt_id;
extern	data_id_t	octet_id;
extern	data_id_t	assign_id;
extern	data_id_t	name_id;
extern	data_id_t	value_id;
extern	data_id_t	ident_id;
extern	data_id_t	normalized_id;
extern	data_id_t	symbol_id;
extern	data_id_t	obj_identifier_id;
extern	data_id_t	reference_id;
extern	data_id_t	array_index_id;
extern	data_id_t	struct_id;
extern	data_id_t	expr_id;
extern	data_id_t	array_id;
extern	data_id_t	string_id;
extern	data_id_t	number_id;
extern	data_id_t	numeric_id;
extern	data_id_t	boolean_id;
extern	data_id_t	any_id;

#define	INCLUDE		&include_id
#define	FILENAME	&filename_id
#define	AML		&aml_id
#define	SYMTREE		&symtree_id
#define	REFTREE		&reftree_id
#define	EXPRTREE	&exprtree_id
#define	RESTREE		&restree_id
#define	ERROR		&error_id
#define	ERRTXT		&errtxt_id
#define	OCTET		&octet_id
#define	ASSIGN		&assign_id
#define	NAME		&name_id
#define	VALUE		&value_id
#define	IDENT		&ident_id
#define	NORMALIZED	&normalized_id
#define	SYMBOL		&symbol_id
#define	OBJ_IDENTIFIER	&obj_identifier_id
#define	REFERENCE	&reference_id
#define	ARRAY_INDEX	&array_index_id
#define	EXPR		&expr_id
#define	STRUCT		&struct_id
#define	ARRAY		&array_id
#define	STRING		&string_id
#define	NUMBER		&number_id
#define	NUMERIC		&numeric_id
#define	BOOLEAN		&boolean_id
#define	ANY		&any_id

/*
	comp_error.c Prototypes
	------------------------------------------------------------------------
 */
int	 comp_nsym_2cstr(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*nsym,
		char	**pp
		);
int	 comp_ref_2cstr(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*ref,
		char	**pp
		);
int	 comp_err_new(
		fncrc_t	*fncrc,
		sess_t	*sp,
		char	*msg,
		data_t	**pp
		);
int	 comp_name_error(
		fncrc_t	*fncrc,
		sess_t	*sp,
		comp_t	*cp,
		data_t	*p1,
		data_t	*p2,
		data_t	**pp
		);
int	 comp_ref_error(
		fncrc_t	*fncrc,
		sess_t	*sp,
		comp_t	*cp,
		data_t	*ref,
		data_t	**pp
		);
int	 comp_arrref_error(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		data_t		*ref,
		data_t		*arr_idx,
		data_t		**pp
		);
int	 comp_env_error(
		fncrc_t	*fncrc,
		sess_t	*sp,
		comp_t	*cp,
		data_t	*vname,
		data_t	**pp
		);
int	 comp_get_error(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*nsym,
		data_t	**pp
		);
/*
	comp_utils.c Prototypes
	------------------------------------------------------------------------
 */
data_code_t	comp_id_cmp(data_id_t *p1, data_id_t *p2);
int	comp_chk_id(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*p,
		data_id_t *id
		);

int	comp_varg_2normalized(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	**pp, ...
		);

int	comp_cstr_2normalized(
		fncrc_t	*fncrc,
		sess_t	*sp,
		char	*cstr,
		data_t	**pp
		);

/*
	comp_get.c Prototypes
	------------------------------------------------------------------------
 */
int	 comp_assign_2name(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*dp,
		data_t	**pp
		);
int	 comp_assign_2value(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*dp,
		data_t	**pp
		);
int	 comp_name_2assign(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*dp,
		data_t	**pp
		);
int	 comp_value_2assign(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*dp,
		data_t	**pp
		);
int	 comp_name_2value(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*dp,
		data_t	**pp
		);
int	 comp_value_2content(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*dp,
		data_t	**pp
		);
int	 comp_value_type(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*dp,
		data_t	**pp
		);
int	 comp_vcont_by_nsym(		// VALUE-Content by NORMALIZED-Symbol
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*nsym,
		data_t	*sym_tree,
		data_t	**pp
		);

/*
	comp_resolve.c Prototypes
	------------------------------------------------------------------------
 */
int	 comp_symbol_from_name(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*name,
		data_t	**pp
		);
int	 comp_lookup_symtree(
		fncrc_t	*fncrc,
		sess_t	*sp,
		comp_t	*cp,
		data_t	*nn,
		data_t	**pp
		);
int	 comp_normalize_name(		// Transform given symbol into NORMALIZED-Symbol
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*name,
		data_t	**pp
		);
int	 comp_ref_2vlnk(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		data_link_t	*reflnk,
		data_link_t	**pp
		);
int	 comp_resref(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		data_link_t	*reflnk,
		data_link_t	**pp
		);
int	comp_resolve(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp
		);

/*
	comp_builtins.c Prototypes
	------------------------------------------------------------------------
 */
extern	data_id_t	 builtin_id;
#define	BUILTIN		&builtin_id
typedef	int	(*comp_builtin)(fncrc_t *, sess_t *, comp_t *, leng_t, data_link_t *, data_t **);


extern	const data_t	 env_builtin;
#define	ENV_BUILTIN_FNC	&env_builtin
int	 env_builtin_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);

extern	const data_t		 date_builtin;
#define	DATE_BUILTIN_FNC	&date_builtin
#define	DATE_STR_LEN		1024
int	 date_builtin_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);

extern	const data_t		 uname_builtin;
#define	UNAME_BUILTIN_FNC	&uname_builtin
int	 uname_builtin_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);

extern	const data_t		 len_builtin;
#define	LEN_BUILTIN_FNC		&len_builtin
int	 len_builtin_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);

extern	const data_t		 pwd_builtin;
#define	PWD_BUILTIN_FNC		&pwd_builtin
int	 pwd_builtin_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);


#ifdef  __cplusplus
}
#endif

#endif /* __BHS_COMPILE_H__ */
