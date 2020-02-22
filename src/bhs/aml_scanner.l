%option 8bit
%option reentrant
%option bison-bridge
%option outfile="aml_scanner.c" header-file="aml_scanner.h"
%option verbose
%option warn
%option nodefault
%option debug
%option prefix="aml_"
%option yylineno
%option pointer
%option noyywrap
%option stack
%option extra-type="comp_t *"

%top{
#include <bhs/dbg.h>
#include <bhs/mem.h>
#include <bhs/fncrc.h>

#include "aml.h"
#include "aml_parser.h"

void	 aml_fatal_error(char *msg , yyscan_t yyscanner);
int	 aml_scn_setup(fncrc_t *fncrc, sess_t *sp, scne_t *p);
int	 aml_token_by_ident(char *key);
int	 aml_lval_new(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		data_id_t	*id,
		leng_t		 len,
		char		*text,
		data_t		**pp
	);

}

%{

#define	YY_DECL int yylex(			\
		YYSTYPE	*yylval_param,		\
		fncrc_t	*fncrc,			\
		sess_t	*sp,			\
		comp_t	*param,			\
		yyscan_t yyscanner		\
)

#define YY_USER_INIT 								\
	if (param != NULL) {							\
		yy_flex_debug = SCNE_DEBUG(param);				\
		yyin = FDES_HDL(param);						\
		yyout = sp->sess_out;						\
	}									\
										\
	if (yy_flex_debug > 1 ) {						\
		FPRINTF(sp->sess_err, " ----- Enter aml-scanner -----\n");	\
	}
	

#define YY_USER_ACTION								\
	if (yy_flex_debug > 2) {						\
		FPRINTF(sp->sess_err, "lineno = %d\n", yylineno);		\
	}

#define YY_BREAK 								\
	param->comp_scanner_ext->scne_file->data_.file->fd_lineno = yylineno;	\
	if(rc == LEX_FATAL_ERROR) {						\
		FNC_RETURN(rc);							\
	}									\
	break;



#if (0)
#define YY_LEX_FATAL_ERROR(msg) aml_fatal_error( msg , yyscanner)
#endif

%}

string	\"[^"]*\"
ws	[ \t\r]+
nl	\n

alp	[[:alpha:]]
aln	[[:alnum:]]
dgt	[[:digit:]]
xdgt	[[:xdigit:]]

obj_id	{dgt}+(\.{dgt}+)+
ident	{alp}([\_\.]?{aln}+)*

%x incl
%%


%{
	int	 rc = LEX_FATAL_ERROR;
	char	*fn = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, param);

	fn = FDES_NAME(param);
	
	if(yylval_param == NULL) {
		if(SCNE_DEBUG(param) > 1) {
			FPRINTF(sp->sess_err, " ---- Setup aml-scanner ----\n");
		}
		rc = aml_scn_setup(fncrc, sp, COMP_LEXEXT(param));
		FNC_RETURN(rc);
	}
%}

"TRUE" {
	rc = TRUE_TOK;
	if(aml_lval_new(fncrc, sp, param, STRING, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

"FALSE" {
	rc = FALSE_TOK;
	if(aml_lval_new(fncrc, sp, param, STRING, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

{string} {
	char *tp;

	tp = yytext +1;
	rc = STRING_TOK;
	if(aml_lval_new(fncrc, sp, param, STRING, yyleng -2, tp, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

{ident} {
	int	 keytok = 0;

	rc = ID_TOK;
	if((keytok = aml_token_by_ident(yytext)) != 0) {
		rc = keytok;
	}
	if(aml_lval_new(fncrc, sp, param, IDENT, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

{obj_id} {
	rc = OBJ_ID_TOK;
	if(aml_lval_new(fncrc, sp, param, IDENT, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

'[0-1]+'B {
	rc = BIN_NUMBER_TOK;
	if(aml_lval_new(fncrc, sp, param, BIN, yyleng -3, yytext +1, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

'[0-7]+'O {
	rc = OCT_NUMBER_TOK;
	if(aml_lval_new(fncrc, sp, param, OCT, yyleng -3, yytext +1, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

'{dgt}+'D {
	rc = DEC_NUMBER_TOK;
	if(aml_lval_new(fncrc, sp, param, DEC, yyleng -3, yytext +1, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

'{xdgt}+'H {
	rc = HEX_NUMBER_TOK;
	if(aml_lval_new(fncrc, sp, param, HEX, yyleng -3, yytext +1, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

0[xX]{xdgt}+ {
	rc = HEX_NUMBER_TOK;
	if(aml_lval_new(fncrc, sp, param, HEX, yyleng -2, yytext +2, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

{dgt}+ {
	rc = DEC_NUMBER_TOK;
	if(aml_lval_new(fncrc, sp, param, DEC, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

"->" {
	rc = PTR_OP_TOK;
	if(aml_lval_new(fncrc, sp, param, PTR, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

"<<" {
	rc = SHL_OP_TOK;
	if(aml_lval_new(fncrc, sp, param, SHL, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

">>" {
	rc = SHR_OP_TOK;
	if(aml_lval_new(fncrc, sp, param, SHR, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

">=" {
	rc = GE_OP_TOK;
	if(aml_lval_new(fncrc, sp, param, GE, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

"<=" {
	rc = LE_OP_TOK;
	if(aml_lval_new(fncrc, sp, param, LE, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

"==" {
	rc = EQ_OP_TOK;
	if(aml_lval_new(fncrc, sp, param, EQ, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

"!=" {
	rc = NE_OP_TOK;
	if(aml_lval_new(fncrc, sp, param, NE, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

"&&" {
	rc = L_AND_OP_TOK;
	if(aml_lval_new(fncrc, sp, param, L_AND, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

"||" {
	rc = L_OR_OP_TOK;
	if(aml_lval_new(fncrc, sp, param, L_OR, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

"--".*$ {
	if (yy_flex_debug > 1 ) {
		FPRINTF(sp->sess_err,"ASN.1/SQL comment\n");
		FPRINTF(sp->sess_err,"%s\n", yytext);
	}
	rc = FNC_OKAY;
}

"//".*$ {
	if (yy_flex_debug > 1 ) {
		FPRINTF(sp->sess_err, "C++ comment\n");
		FPRINTF(sp->sess_err,"%s\n", yytext);
	}
	rc = FNC_OKAY;
}

\n	{
	rc = FNC_OKAY;
}

{ws}	{
	rc = FNC_OKAY;
}

'.' {
	rc = CHAR_TOK;
	if(aml_lval_new(fncrc, sp, param, OCTET, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

. {
	rc = yytext[0];
	if(aml_lval_new(fncrc, sp, param, OCTET, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

"/*" {
	int c;

	if (yy_flex_debug > 1 ) {
		FPRINTF(sp->sess_err, "C comment\n");
		FPRINTF(sp->sess_err,"%s\n", yytext);
	}
	while ( 1 ) {
		/* eat up text of comment */
		while ( (c = input(yyscanner)) != '*' && c != EOF ) ;
		if ( c == '*' ) {
			while ( (c = input(yyscanner)) == '*' ) ;
			if ( c == '/' ) {
				break;    /* found the end */
			}
		}
		if ( c == EOF ) {
			FPRINTF(sp->sess_err, "EOF in comment\n" );
			FNC_RETURN(0);
		}
	}
	rc = FNC_OKAY;
}

"#".*$ {
	if(strstr(yytext, "#include") == yytext) {
		REJECT;
	}
	if (yy_flex_debug > 1 ) {
		FPRINTF(sp->sess_err, "shell comment\n");
		FPRINTF(sp->sess_err,"%s\n", yytext);
	}
	rc = FNC_OKAY;
}

"#include" {
	rc = INCLUDE_TOK;
	if(aml_lval_new(fncrc, sp, param, INCLUDE, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
//		BEGIN(incl);
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

<incl>{ws} {
	rc = FNC_OKAY;
}

<incl>[^ \t\n]+ {
	int		_rc = FNC_OKAY;
	data_sequ_t	*sequ = NULL;

	if (yy_flex_debug > 1) {
		COMP_PRN(sp, "(incl)");
	}
	rc = FILENAME_TOK;
	if(data_file_new(fncrc, sp, INCLUDE, yytext, &yylval->dp) == FNC_OKAY) {
		if((_rc = data_sequ_new(fncrc, sp, yylineno, fn, &sequ)) == FNC_OKAY) {
			if((_rc = data_sequ_set(fncrc, sp, yylval->dp, sequ)) == FNC_OKAY) {
				if(yy_flex_debug > 2) {
					data_prn(fncrc, sp, yylval->dp, sp->sess_err, 0);
				}
			}
			BEGIN(INITIAL);
			FNC_RETURN(rc);
		} else {
			FNC_ATXT(fncrc, "data_sequ_set");
		}
	} else {
		FNC_ATXT(fncrc, "data_sequ_new");
		data_drop(fncrc, sp, yylval->dp);
		yylval->dp = NULL;
	}
	rc = LEX_FATAL_ERROR;
}

<<EOF>> {
	rc = 0;

	if((rc = comp_inc_eof(fncrc, sp, param)) == FNC_OKAY) {
		fn = FDES_NAME(param);
		FNC_RETURN(rc);
	}
	if(rc == 1) {
		rc= 0;
	} else {
		rc = LEX_FATAL_ERROR;
	}
}

%%

#if (0)
int
yywrap(yyscan_t yyscanner) {
	return(1);
}
#endif

/*
	------------------------------------------------------------------------
 */
scn_key_t	 aml_keywords[] = {
	{ "date",		DATE_BUILTIN_TOK } ,
	{ "env",		ENV_BUILTIN_TOK } ,
	{ "len",		LEN_BUILTIN_TOK } ,
	{ "pwd",		PWD_BUILTIN_TOK } ,
	{ "uname",		UNAME_BUILTIN_TOK } ,
	{ (char *)NULL,		0 } 
};

int
aml_token_by_ident(
	char	*key
) {
	scn_key_t	*p = NULL;

	for(p = &aml_keywords[0]; p->name != NULL; p++) {
		if(strcmp(key, p->name) == 0) {
			break;
		}
	}
	return(p->token);
}

/*
	------------------------------------------------------------------------
 */
void *
aml_get_current_buffer(
	yyscan_t yyscanner
) {
	void		*rc = NULL;
	struct yyguts_t *yyg = NULL;

	yyg = (struct yyguts_t*)yyscanner;
	rc = YY_CURRENT_BUFFER;
	return(rc);
}

/*
	------------------------------------------------------------------------
 */
int
aml_scn_setup(
	fncrc_t	*fncrc,
	sess_t	*sp,
	scne_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	p->scne_restart = aml_restart;
	p->scne_get_current_buffer = aml_get_current_buffer;
	p->scne_switch_to_buffer = aml__switch_to_buffer;
	p->scne_create_buffer = aml__create_buffer;
	p->scne_delete_buffer = aml__delete_buffer;
	p->scne_flush_buffer = aml__flush_buffer;
	p->scne_push_buffer_state = aml_push_buffer_state;
	p->scne_pop_buffer_state = aml_pop_buffer_state;

	p->snce_scan_buffer = aml__scan_buffer;
	p->snce_scan_string = aml__scan_string;
	p->snce_scan_bytes = aml__scan_bytes;

	p->scne_alloc = aml_alloc;
	p->scne_realloc = aml_realloc;
	p->scne_free = aml_free;

	p->scne_init = aml_lex_init;
	p->scne_init_extra = aml_lex_init_extra;
	p->scne_get_extra = aml_get_extra;
	p->scne_set_extra = aml_set_extra;
	p->scne_fini = aml_lex_destroy;

	p->scne_get_debug = aml_get_debug;
	p->scne_set_debug = aml_set_debug;

	p->scne_get_in = aml_get_in;
	p->scne_set_in = aml_set_in;
	p->scne_get_out = aml_get_out;
	p->scne_set_out = aml_set_out;

	p->scne_get_leng = aml_get_leng;
	p->scne_get_text = aml_get_text;
	p->scne_get_lineno = aml_get_lineno;
	p->scne_set_lineno = aml_set_lineno;

	p->scne_get_lval = aml_get_lval;
	p->scne_set_lval = aml_set_lval;

	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
aml_lval_new(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	data_id_t	*id,
	leng_t		 len,
	char		*text,
	data_t		**pp
) {
	int		 rc = FNC_OKAY;
	data_t		*dp = NULL;

	int		 line = 0;
	char		*file_name = NULL;
	data_sequ_t	*sequ = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, id);
	FNC_CHKINP(rc, text);
	FNC_CHKINP(rc, pp);

	if(COMP_DBG(cp) >= COMP_SHORT_DBG) {
		DBGOUT(sp->sess_err,
			"enter(id=0x%p, len=%llu, text=0x%p, pp=0x%p(0x%p))",
			id, len, text, pp, *pp
		);
	}
	if(COMP_DBG(cp) >= COMP_PARAM_DBG) {
		DBGOUT(sp->sess_err, "in-param(id=%s)", TSTR(DCSTR(id)));
		DBGOUT(sp->sess_err, "in-param(len=%llu)", len);
		DBGOUT(sp->sess_err, "in-param(text=%s)", TSTR(text));
	}
	line = cp->comp_scanner_ext->scne_get_lineno(cp->comp_scanner_db);
	file_name = FDES_NAME(cp);

	TRY(data_str_new, _door, sp, id, len, text, &dp);
	TRY(data_sequ_new, _door, sp, line, file_name, &sequ);
	TRY(data_sequ_set, _door, sp, dp, sequ);

	if(SCNE_DEBUG(cp) > 2) {
		data_prn(sess_next_fncrc(sp), sp, dp, sp->sess_err, 0);
	}
	
_door:
	if(rc != FNC_OKAY) {
		if(dp != NULL) {
			data_drop(sess_next_fncrc(sp), sp, dp);
			dp = NULL;
		}
		if(sequ != NULL) {
			data_sequ_drop(sess_next_fncrc(sp), sp, sequ);
			sequ = NULL;
		}
	}
	*pp = dp;
	if(COMP_DBG(cp) >= COMP_SHORT_DBG) {
		DBGOUT(sp->sess_err, "out-param(pp=0x%p(0x%p))", pp, *pp);
		DBGOUT(sp->sess_err, "return(%d)", rc);
	}
	FNC_RETURN(rc);
}

