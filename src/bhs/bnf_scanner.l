%top{
#include <bhs/dbg.h>
#include <bhs/mem.h>
#include <bhs/fncrc.h>

#include "bnf.h"
#include "bnf_parser.h"

void	 bnf_fatal_error(char *msg , yyscan_t yyscanner);
int	 bnf_scn_setup(fncrc_t *fncrc, sess_t *sp, scne_t *p);
int	 bnf_token_by_ident(char *key);
int	 bnf_lval_new(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		data_id_t	*id,
		leng_t		 len,
		char		*text,
		data_t		**pp
	);

}

%option 8bit reentrant bison-bridge
%option outfile="bnf_scanner.c" header-file="bnf_scanner.h"
%option verbose
%option warn
%option debug
%option prefix="bnf_"
%option yylineno
%option pointer
%option noyywrap
%option stack
%option extra-type="comp_t *"

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
	if (yy_flex_debug > 1 ) {						\
		FPRINTF(sp->sess_err, " ----- Enter BNF-scanner -----\n");	\
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
#define YY_LEX_FATAL_ERROR(msg) bnf_fatal_error( msg , yyscanner)
#endif

%}

string	\"[^"]+\"
char	\'.\'
ws	[ \t\r]+
nl	\n

alp	[[:alpha:]]
aln	[[:alnum:]]
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
			FPRINTF(sp->sess_err, " ---- Setup BNF-scanner ----\n");
		}
		rc = bnf_scn_setup(fncrc, sp, COMP_LEXEXT(param));
		FNC_RETURN(rc);
	}
%}

{string} {
	char *tp;

	tp = yytext +1;
	rc = BNF_STRING_TOK;
	if(bnf_lval_new(fncrc, sp, param, BNF_STRING, yyleng -2, tp, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

{char} {
	char *tp;

	tp = yytext +1;
	rc = BNF_CHAR_TOK;
	if(bnf_lval_new(fncrc, sp, param, BNF_CHAR, yyleng -2, tp, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

{ident} {
	int	 keytok = 0;

	rc = BNF_ID_TOK;
	if((keytok = bnf_token_by_ident(yytext)) != 0) {
		rc = keytok;
	}
	if(bnf_lval_new(fncrc, sp, param, BNF_IDENT, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
}

"::=" {
	rc = BNF_ASGN_TOK;
	if(bnf_lval_new(fncrc, sp, param, BNF_ASGN, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
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

\r	{
	rc = FNC_OKAY;
}

{ws}	{
	rc = FNC_OKAY;
}

. {
	rc = yytext[0];
	if(bnf_lval_new(fncrc, sp, param, OCTET, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
		FNC_RETURN(rc);
	}
	rc = LEX_FATAL_ERROR;
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
	rc = BNF_INCLUDE_TOK;
	if(bnf_lval_new(fncrc, sp, param, BNF_INCLUDE, yyleng, yytext, &yylval->dp) == FNC_OKAY) {
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
	rc = BNF_FILENAME_TOK;
	if(data_file_new(fncrc, sp, BNF_INCLUDE, yytext, &yylval->dp) == FNC_OKAY) {
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
scn_key_t	 bnf_keywords[] = {
	{ "date",		BNF_DATE_BUILTIN_TOK } ,
	{ "env",		BNF_ENV_BUILTIN_TOK } ,
	{ "len",		BNF_LEN_BUILTIN_TOK } ,
	{ "pwd",		BNF_PWD_BUILTIN_TOK } ,
	{ "uname",		BNF_UNAME_BUILTIN_TOK } ,
	{ (char *)NULL,		0 } 
};

int
bnf_token_by_ident(
	char	*key
) {
	scn_key_t	*p = NULL;

	for(p = &bnf_keywords[0]; p->name != NULL; p++) {
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
bnf_get_current_buffer(
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
bnf_scn_setup(
	fncrc_t	*fncrc,
	sess_t	*sp,
	scne_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, p);

	p->scne_restart = bnf_restart;
	p->scne_get_current_buffer = bnf_get_current_buffer;
	p->scne_switch_to_buffer = bnf__switch_to_buffer;
	p->scne_create_buffer = bnf__create_buffer;
	p->scne_delete_buffer = bnf__delete_buffer;
	p->scne_flush_buffer = bnf__flush_buffer;
	p->scne_push_buffer_state = bnf_push_buffer_state;
	p->scne_pop_buffer_state = bnf_pop_buffer_state;

	p->snce_scan_buffer = bnf__scan_buffer;
	p->snce_scan_string = bnf__scan_string;
	p->snce_scan_bytes = bnf__scan_bytes;

	p->scne_alloc = bnf_alloc;
	p->scne_realloc = bnf_realloc;
	p->scne_free = bnf_free;

	p->scne_init = bnf_lex_init;
	p->scne_init_extra = bnf_lex_init_extra;
	p->scne_get_extra = bnf_get_extra;
	p->scne_set_extra = bnf_set_extra;
	p->scne_fini = bnf_lex_destroy;

	p->scne_get_debug = bnf_get_debug;
	p->scne_set_debug = bnf_set_debug;

	p->scne_get_in = bnf_get_in;
	p->scne_set_in = bnf_set_in;
	p->scne_get_out = bnf_get_out;
	p->scne_set_out = bnf_set_out;

	p->scne_get_leng = bnf_get_leng;
	p->scne_get_text = bnf_get_text;
	p->scne_get_lineno = bnf_get_lineno;
	p->scne_set_lineno = bnf_set_lineno;

	p->scne_get_lval = bnf_get_lval;
	p->scne_set_lval = bnf_set_lval;

	FNC_RETURN(rc);
}

/*
	------------------------------------------------------------------------
 */
int
bnf_lval_new(
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
	FNC_CHKINL(rc, len);
	FNC_CHKINP(rc, text);
	FNC_CHKINP(rc, pp);

	line = cp->comp_scanner_ext->scne_get_lineno(cp->comp_scanner_db);
	file_name = FDES_NAME(cp);

	TRY(data_str_new,	_door, sp, id, len, text, &dp);
	TRY(data_sequ_new,	_door, sp, line, file_name, &sequ);
	TRY(data_sequ_set,	_door, sp, dp, sequ);

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
	FNC_RETURN(rc);
}

