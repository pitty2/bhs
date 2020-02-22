%{
#include <strings.h>
#include "aml.h"
%}

%define api.pure full
%file-prefix "aml_parser"
%name-prefix "aml_"
%output "aml_parser.c"
%verbose
%debug
%defines
%token-table
%error-verbose

%union {
	data_t	*dp;
}

%param	{ fncrc_t *fncrc }
%param	{ sess_t *sp }
%param	{ comp_t *cp }
%param	{ yyscan_t scanner }

%initial-action {
	int		 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);

	yydebug = COMP_YACCDBG(cp);
}

%{
void	yyerror (fncrc_t *fncrc, sess_t *sp, comp_t *cp, yyscan_t scanner, char const *msg);
int	expr_reduce(fncrc_t *fncrc, sess_t *sp, comp_t *cp, data_t *expr, data_t **pp);
%}


%token	<dp>	LEX_FATAL_ERROR
%destructor	{ data_drop (fncrc, sp, $$); }	INCLUDE_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	FILENAME_TOK
%token	<dp>	INCLUDE_TOK
%token	<dp>	FILENAME_TOK

%destructor	{ data_drop (fncrc, sp, $$); }	ID_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	OBJ_ID_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	STRING_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	CHAR_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	HEX_NUMBER_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	DEC_NUMBER_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	OCT_NUMBER_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	BIN_NUMBER_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	TRUE_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	FALSE_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	'='
%destructor	{ data_drop (fncrc, sp, $$); }	';'
%destructor	{ data_drop (fncrc, sp, $$); }	':'
%destructor	{ data_drop (fncrc, sp, $$); }	','
%destructor	{ data_drop (fncrc, sp, $$); }	'{'
%destructor	{ data_drop (fncrc, sp, $$); }	'}'
%destructor	{ data_drop (fncrc, sp, $$); }	'['
%destructor	{ data_drop (fncrc, sp, $$); }	']'
%destructor	{ data_drop (fncrc, sp, $$); }	'('
%destructor	{ data_drop (fncrc, sp, $$); }	')'
%destructor	{ data_drop (fncrc, sp, $$); }	'.'
%destructor	{ data_drop (fncrc, sp, $$); }	'$'
%destructor	{ data_drop (fncrc, sp, $$); }	'+'
%destructor	{ data_drop (fncrc, sp, $$); }	'-'
%destructor	{ data_drop (fncrc, sp, $$); }	'/'
%destructor	{ data_drop (fncrc, sp, $$); }	'*'
%destructor	{ data_drop (fncrc, sp, $$); }	'%'
%destructor	{ data_drop (fncrc, sp, $$); }	'&'
%destructor	{ data_drop (fncrc, sp, $$); }	'|'
%destructor	{ data_drop (fncrc, sp, $$); }	'!'
%destructor	{ data_drop (fncrc, sp, $$); }	'~'
%destructor	{ data_drop (fncrc, sp, $$); }	'^'
%destructor	{ data_drop (fncrc, sp, $$); }	'>'
%destructor	{ data_drop (fncrc, sp, $$); }	'<'
%destructor	{ data_drop (fncrc, sp, $$); }	'#'
%destructor	{ data_drop (fncrc, sp, $$); }	PTR_OP_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	SHL_OP_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	SHR_OP_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	GE_OP_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	LE_OP_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	EQ_OP_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	NE_OP_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	L_AND_OP_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	L_OR_OP_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	'?'
%destructor	{ data_drop (fncrc, sp, $$); }	DATE_BUILTIN_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	ENV_BUILTIN_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	LEN_BUILTIN_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	PWD_BUILTIN_TOK
%destructor	{ data_drop (fncrc, sp, $$); }	UNAME_BUILTIN_TOK
%token	<dp>	ID_TOK
%token	<dp>	OBJ_ID_TOK
%token	<dp>	STRING_TOK
%token	<dp>	CHAR_TOK
%token	<dp>	HEX_NUMBER_TOK
%token	<dp>	DEC_NUMBER_TOK
%token	<dp>	OCT_NUMBER_TOK
%token	<dp>	BIN_NUMBER_TOK
%token	<dp>	TRUE_TOK
%token	<dp>	FALSE_TOK
%token	<dp>	'='
%token	<dp>	';'
%token	<dp>	','
%token	<dp>	'{'
%token	<dp>	'}'
%token	<dp>	'['
%token	<dp>	']'
%right	<dp>	'('
%left	<dp>	')'
%token	<dp>	'.'
%token	<dp>	'$'
%left	<dp>	'+'
%left	<dp>	'-'
%left	<dp>	'/'
%left	<dp>	'*'
%left	<dp>	'%'
%token	<dp>	'&'
%token	<dp>	'|'
%token	<dp>	'!'
%token	<dp>	'~'
%token	<dp>	'^'
%token	<dp>	'>'
%token	<dp>	'<'
%token	<dp>	'#'
%token	<dp>	PTR_OP_TOK
%token	<dp>	SHL_OP_TOK
%token	<dp>	SHR_OP_TOK
%token	<dp>	GE_OP_TOK
%token	<dp>	LE_OP_TOK
%token	<dp>	EQ_OP_TOK
%token	<dp>	NE_OP_TOK
%token	<dp>	L_AND_OP_TOK
%token	<dp>	L_OR_OP_TOK
%left	<dp>	'?'
%token	<dp>	':'

%token	<dp>	DATE_BUILTIN_TOK
%token	<dp>	ENV_BUILTIN_TOK
%token	<dp>	LEN_BUILTIN_TOK
%token	<dp>	PWD_BUILTIN_TOK
%token	<dp>	UNAME_BUILTIN_TOK

%destructor	{ data_drop (fncrc, sp, $$); } rules
%destructor	{ data_drop (fncrc, sp, $$); } stmt
%destructor	{ data_drop (fncrc, sp, $$); } assign
%destructor	{ data_drop (fncrc, sp, $$); } include
%destructor	{ data_drop (fncrc, sp, $$); } name
%destructor	{ data_drop (fncrc, sp, $$); } value
%destructor	{ data_drop (fncrc, sp, $$); } builtin
%destructor	{ data_drop (fncrc, sp, $$); } expr
%destructor	{ data_drop (fncrc, sp, $$); } _expr
%destructor	{ data_drop (fncrc, sp, $$); } unary_expr
%destructor	{ data_drop (fncrc, sp, $$); } bool_expr
%destructor	{ data_drop (fncrc, sp, $$); } bool_op
%destructor	{ data_drop (fncrc, sp, $$); } boolean
%destructor	{ data_drop (fncrc, sp, $$); } expr_op
%destructor	{ data_drop (fncrc, sp, $$); } unary_op
%destructor	{ data_drop (fncrc, sp, $$); } ternary
%destructor	{ data_drop (fncrc, sp, $$); } array
%destructor	{ data_drop (fncrc, sp, $$); } expr_list
%destructor	{ data_drop (fncrc, sp, $$); } struct 
%destructor	{ data_drop (fncrc, sp, $$); } item
%destructor	{ data_drop (fncrc, sp, $$); } scalar
%destructor	{ data_drop (fncrc, sp, $$); } sub_expr
%destructor	{ data_drop (fncrc, sp, $$); } array_index
%destructor	{ data_drop (fncrc, sp, $$); } reference
%destructor	{ data_drop (fncrc, sp, $$); } ref_name
%destructor	{ data_drop (fncrc, sp, $$); } string
%destructor	{ data_drop (fncrc, sp, $$); } number_list
%destructor	{ data_drop (fncrc, sp, $$); } _number
%destructor	{ data_drop (fncrc, sp, $$); } number

%type	<dp>	aml 
%type	<dp>	rules
%type	<dp>	stmt
%type	<dp>	assign
%type	<dp>	include

%type	<dp>	name
%type	<dp>	value

%type	<dp>	builtin
%type	<dp>	expr
%type	<dp>	bool_expr
%type	<dp>	bool_op
%type	<dp>	boolean
%type	<dp>	_expr
%type	<dp>	unary_expr
%type	<dp>	expr_op
%type	<dp>	unary_op
%type	<dp>	ternary

%type	<dp>	array
%type	<dp>	expr_list
%type	<dp>	struct

%type	<dp>	item
%type	<dp>	scalar
%type	<dp>	sub_expr

%type	<dp>	array_index
%type	<dp>	reference
%type	<dp>	ref_name
%type	<dp>	string
%type	<dp>	number_list
%type	<dp>	number
%type	<dp>	_number

%start	aml

%%
aml		: rules
		{
			if ( $1 != NULL ) {
				$$ = $1;
				COMP_ID_SET(fncrc, sp, $$, AML);
			} else {
				COMP_LST_NEW(fncrc, sp, AML, $$);
			}
			COMP_PTREE(cp) = $$;
			if(COMP_ERRORS(cp) != 0) {
				FNCRC_RESULT(FNC_ERROR);
			}
			if(COMP_DBG(cp) >= COMP_SHORT_DBG) {
				DBGOUT(sp->sess_err,
					"result(parser_tree = 0x%p)",
					COMP_PTREE(cp));
				DBGOUT(sp->sess_err,
					"return(%d)",
					fncrc->fnc_result);
			}
		}
		;

rules		: stmt
		{
		data_code_t	code = DATA_NIL;

			COMP_LST_NEW(fncrc, sp, RULES, $$);
			YY_TRY(data_id_cmp, sp, DID($1), ERROR, &code);
			if(code == DATA_EQ) {
				COMP_LST_INSL(fncrc, sp, $1, COMP_ERRTXT(cp));
				COMP_ERRTXT(cp) = NULL;
			} 
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		| rules stmt
		{
		data_code_t	code = DATA_NIL;

			YY_TRY(data_id_cmp, sp, DID($2), ERROR, &code);
			if(code == DATA_EQ) {
				COMP_LST_INSL(fncrc, sp, $2, COMP_ERRTXT(cp));
				COMP_ERRTXT(cp) = NULL;
			} 
			COMP_LST_INSL(fncrc, sp, $$, $2);
		}
		;

stmt		: assign
		{
			$$ = $1;
		}
		| include
		{
			$$ = $1;
		}
		;

assign		: name '=' value ';'
		{
			COMP_LST_NEW(fncrc, sp, ASSIGN, $$);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $2);
			COMP_LST_INSL(fncrc, sp, $$, $3);
			COMP_DROP(fncrc, sp, $4);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		| INCLUDE_TOK expr ';'
		{
		int		 rc = FNC_OKAY;
		leng_t		 argc = 1ULL;
		data_link_t	*argv = NULL;
		data_t		*xx = NULL;

			TRY(data_lst_new,	_door, sp, PREPROC, &xx);
			TRY(data_lst_ins_last,	_door, sp, xx, $2, &argv);
			TRY(comp_prep_inc,	_door, sp, cp, argc, argv, &($$));
			COMP_SEQU_SET(fncrc, sp, $$, $1);

		_door:
			COMP_DROP(fncrc, sp, $1);
			COMP_DROP(fncrc, sp, $2);
			COMP_DROP(fncrc, sp, $3);
			COMP_DROP(fncrc, sp, xx);
			if(rc != FNC_OKAY) {
				YYABORT;
			}
		}
		| error ';'
		{
			COMP_LST_NEW(fncrc, sp, ERROR, $$);
			COMP_LST_INSF(fncrc, sp, $$, $2);
			COMP_SEQU_SET(fncrc, sp, $$, $2);
			yyerrok;
			yyclearin;
			COMP_ERRORS(cp)++;
		}
		| LEX_FATAL_ERROR
		{
			FNC_ATXT(fncrc, "aml_lex");
			YYABORT;
		}
		;

include		: INCLUDE_TOK FILENAME_TOK
		{
		int	rc = FNC_OKAY;

			TRY(comp_inc_open, __abort__, sp, cp, $2, &($$));
			if (rc != FNC_OKAY) {
__abort__:
				data_drop(fncrc, sp, $1);
				data_drop(fncrc, sp, $2);
				YYABORT;
			}
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		;

name		: ID_TOK
		{
			COMP_LST_NEW(fncrc, sp, NAME, $$);
			COMP_ID_SET(fncrc, sp, $1, SYMBOL);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		| OBJ_ID_TOK
		{
			COMP_LST_NEW(fncrc, sp, NAME, $$);
			COMP_ID_SET(fncrc, sp, $1, SYMBOL);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		;

value		: expr
		{
			COMP_LST_NEW(fncrc, sp, VALUE, $$);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		| array
		{
			COMP_LST_NEW(fncrc, sp, VALUE, $$);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		| struct
		{
			COMP_LST_NEW(fncrc, sp, VALUE, $$);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		;

expr		: _expr
		{
			YY_TRY(expr_reduce, sp, cp, $1, &($$));
		}
		| bool_expr
		{
			$$ = $1;
		}
		| ternary
		{
			$$ = $1;
		}
		;

bool_expr	: item bool_op item
		{
			COMP_LST_NEW(fncrc, sp, BOOL_EXPR, $$);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_LST_INSL(fncrc, sp, $$, $2);
			COMP_LST_INSL(fncrc, sp, $$, $3);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		| bool_expr bool_op item
		{
			COMP_LST_INSL(fncrc, sp, $$, $2);
			COMP_LST_INSL(fncrc, sp, $$, $3);
		}
		;

bool_op		: '<'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&lt_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, BOOL_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| LE_OP_TOK
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&le_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, BOOL_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '>'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&gt_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, BOOL_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| GE_OP_TOK
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&ge_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, BOOL_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| EQ_OP_TOK
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&eq_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, BOOL_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| NE_OP_TOK
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&ne_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, BOOL_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| L_AND_OP_TOK
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&logical_and_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, BOOL_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| L_OR_OP_TOK
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&logical_or_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, BOOL_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		;

ternary		: sub_expr '?' expr ':' expr
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&ternary__fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, TERNARY, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_LST_INSL(fncrc, sp, $$, $3);
			COMP_LST_INSL(fncrc, sp, $$, $5);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $2);
			COMP_DROP(fncrc, sp, $4);
		}
		;

_expr		: unary_expr
		{
			COMP_LST_NEW(fncrc, sp, EXPR, $$);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		| _expr expr_op unary_expr
		{
			COMP_LST_INSL(fncrc, sp, $$, $2);
			COMP_LST_INSL(fncrc, sp, $$, $3);
		}
		;

expr_op		: '*'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&mul_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, EXPR_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '/'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&div_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, EXPR_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '%'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&mod_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, EXPR_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '+'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&plus_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, EXPR_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '-'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&minus_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, EXPR_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| SHL_OP_TOK
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&shl_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, EXPR_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| SHR_OP_TOK
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&shr_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, EXPR_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '&'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&and_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, EXPR_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '^'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&xor_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, EXPR_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '|'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&or_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, EXPR_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		;

unary_expr	: unary_op item
		{
			COMP_LST_NEW(fncrc, sp, UNARY_EXPR, $$);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_LST_INSL(fncrc, sp, $$, $2);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		| item
		{
			$$ = $1;
		}
		;

unary_op	: '!'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&logical_not_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, UNARY_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '~'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&not_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, UNARY_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '+'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&unary_plus_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, UNARY_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '-'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&unary_minus_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, UNARY_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '*'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&deref_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, UNARY_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| '&'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)&addr_op_fnc, &fnc);
			COMP_LST_NEW(fncrc, sp, UNARY_OP, $$);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		;

item		: scalar
		{
			$$ = $1;
		}
		| sub_expr
		{
			$$ = $1;
		}
		| builtin
		{
			$$ = $1;
		}
		;

sub_expr	:  '(' expr ')'
		{
			$$ = $2;
			COMP_ID_SET(fncrc, sp, $$, SUB_EXPR);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
			COMP_DROP(fncrc, sp, $3);
		}
		;

array		: '[' expr_list ']'
		{
			$$ = $2;
			COMP_ID_SET(fncrc, sp, $$, ARRAY);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
			COMP_DROP(fncrc, sp, $3);
		}
		;

struct		: '{' rules '}'
		{
			$$ = $2;
			COMP_ID_SET(fncrc, sp, $$, STRUCT);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
			COMP_DROP(fncrc, sp, $3);
		}
		;

expr_list	: value
		{
			COMP_LST_NEW(fncrc, sp, EXPR_LIST, $$);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		| expr_list ',' value
		{
			COMP_DROP(fncrc, sp, $2);
			COMP_LST_INSL(fncrc, sp, $$, $3);
		}
		;

scalar		: reference
		{
			$$ = $1;
			COMP_LST_INSL(fncrc, sp, COMP_REFTREE(cp), $1);
		}
		| string
		{
			$$ = $1;
		}
		| number
		{
			$$ = $1;
		}
		| boolean
		{
			$$ = $1;
		}
		;

string		: STRING_TOK
		{
			$$ = $1;
		}
		;

boolean		: TRUE_TOK
		{
			YY_TRY(data_bool_new, sp, BOOLEAN, TRUE, &($$));
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| FALSE_TOK
		{
			YY_TRY(data_bool_new, sp, BOOLEAN, FALSE, &($$));
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		;

reference	: ref_name
		{
			COMP_LST_NEW(fncrc, sp, REFERENCE, $$);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		| ref_name array_index
		{
			COMP_LST_NEW(fncrc, sp, REFERENCE, $$);
			COMP_LST_INSL(fncrc, sp, $$, $1);
			COMP_LST_INSL(fncrc, sp, $$, $2);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
		}
		;

ref_name	: name
		{
		data_t		*name = NULL;

			YY_TRY(data_get_first, sp, $1, &name);
			COMP_LST_NEW(fncrc, sp, NORMALIZED, $$);
			COMP_LST_INSL(fncrc, sp, $$, name);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
		}
		| ref_name PTR_OP_TOK name
		{
		data_t		*name = NULL;

			YY_TRY(data_get_first, sp, $3, &name);
			COMP_LST_INSL(fncrc, sp, $$, name);
			COMP_DROP(fncrc, sp, $2);
			COMP_DROP(fncrc, sp, $3);
		}
		;

array_index	: '[' number_list ']'
		{
			$$ = $2;
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_DROP(fncrc, sp, $1);
			COMP_DROP(fncrc, sp, $3);
		}
		;

number_list	: number
		{
			COMP_LST_NEW(fncrc, sp, ARRAY_INDEX, $$);
			COMP_LST_INSL(fncrc, sp, $$, $1);
		}
		| number_list ',' number
		{
			COMP_LST_INSL(fncrc, sp, $$, $3);
			COMP_DROP(fncrc, sp, $2);
		}
		;

number		: _number
		{
			$$ = $1;
		}
		;

_number		: HEX_NUMBER_TOK
		{
		data_t	*dp = NULL;

			YY_TRY(data_str_2data_num, sp, $1, 16, &dp);
			COMP_ID_SET(fncrc, sp, dp, NUMERIC);
			COMP_SEQU_SET(fncrc, sp, $$, dp);
			COMP_DROP(fncrc, sp, $1);
			$$ = dp;
		}
		| DEC_NUMBER_TOK
		{
		data_t	*dp = NULL;

			YY_TRY(data_str_2data_num, sp, $1, 10, &dp);
			COMP_ID_SET(fncrc, sp, dp, NUMERIC);
			COMP_SEQU_SET(fncrc, sp, $$, dp);
			COMP_DROP(fncrc, sp, $1);
			$$ = dp;
		}
		| OCT_NUMBER_TOK
		{
		data_t	*dp = NULL;

			YY_TRY(data_str_2data_num, sp, $1, 8, &dp);
			COMP_ID_SET(fncrc, sp, dp, NUMERIC);
			COMP_SEQU_SET(fncrc, sp, $$, dp);
			COMP_DROP(fncrc, sp, $1);
			$$ = dp;
		}
		| BIN_NUMBER_TOK
		{
		data_t	*dp = NULL;

			YY_TRY(data_str_2data_num, sp, $1, 2, &dp);
			COMP_ID_SET(fncrc, sp, dp, NUMERIC);
			COMP_SEQU_SET(fncrc, sp, $$, dp);
			COMP_DROP(fncrc, sp, $1);
			$$ = dp;
		}
		;

builtin		: ENV_BUILTIN_TOK '(' ID_TOK ')'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)ENV_BUILTIN_FNC, &fnc);
			COMP_LST_NEW(fncrc, sp, BUILTIN, $$);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_ID_SET(fncrc, sp, $3, SYMBOL);
			COMP_LST_INSL(fncrc, sp, $$, $3);
			COMP_DROP(fncrc, sp, $1);
			COMP_DROP(fncrc, sp, $2);
			COMP_DROP(fncrc, sp, $4);
		}
		| LEN_BUILTIN_TOK '(' expr ')'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)LEN_BUILTIN_FNC, &fnc);
			COMP_LST_NEW(fncrc, sp, BUILTIN, $$);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_LST_INSL(fncrc, sp, $$, $3);
			COMP_DROP(fncrc, sp, $1);
			COMP_DROP(fncrc, sp, $2);
			COMP_DROP(fncrc, sp, $4);
		}
		| DATE_BUILTIN_TOK '(' string ')'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)DATE_BUILTIN_FNC, &fnc);
			COMP_LST_NEW(fncrc, sp, BUILTIN, $$);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_ID_SET(fncrc, sp, $3, STRING);
			COMP_LST_INSL(fncrc, sp, $$, $3);
			COMP_DROP(fncrc, sp, $1);
			COMP_DROP(fncrc, sp, $2);
			COMP_DROP(fncrc, sp, $4);
		}
		| UNAME_BUILTIN_TOK '(' ID_TOK ')'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)UNAME_BUILTIN_FNC, &fnc);
			COMP_LST_NEW(fncrc, sp, BUILTIN, $$);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_ID_SET(fncrc, sp, $3, STRING);
			COMP_LST_INSL(fncrc, sp, $$, $3);
			COMP_DROP(fncrc, sp, $1);
			COMP_DROP(fncrc, sp, $2);
			COMP_DROP(fncrc, sp, $4);
		}
		| PWD_BUILTIN_TOK '(' ')'
		{
		data_t *fnc = NULL;

			YY_TRY(data_fnc_cpy, sp, (data_t *)PWD_BUILTIN_FNC, &fnc);
			COMP_LST_NEW(fncrc, sp, BUILTIN, $$);
			COMP_SEQU_SET(fncrc, sp, $$, $1);
			COMP_LST_INSL(fncrc, sp, $$, fnc);
			COMP_DROP(fncrc, sp, $1);
			COMP_DROP(fncrc, sp, $2);
			COMP_DROP(fncrc, sp, $3);
		}
		;
%%

void
yyerror(
	fncrc_t		*fncrc,
	sess_t		*sp,
	comp_t		*cp,
	yyscan_t	 scanner,
	char const	*msg
) {
	int	 rc = FNC_OKAY;
	char	*buf = NULL;
	int	 lno = 0;;
	char	*fn = NULL;
	data_t	*dp = NULL;
	int	 sysrc = 0;

	lno = COMP_SCNE(cp)->scne_get_lineno(COMP_SCANNER(cp));
	fn = FDES_NAME(cp);

	if((buf = CALLOC(1, COMP_MSG_LEN +1)) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (COMP_MSG_LEN +1));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}	
	sysrc = snprintf(buf, COMP_MSG_LEN,
		"%s(+%d): %s", TSTR(fn), lno, TSTR(msg));
	if(sysrc < 0) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(FNC_CAUSE_SYSCALL);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "snprintf() failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	FPRINTF(sp->sess_err, "%s\n", buf);

	TRY(data_str_new, _next, sp, ERRTXT, strlen(buf), buf, &dp);
_next:
	if(rc == FNC_OKAY) {
		COMP_ERRTXT(cp) = dp;
	}
	COMP_ERRORS(cp)++;
	FNCRC_RESULT(FNC_ERROR);
	FNCRC_CAUSE(COMP_CAUSE_SYNTAX);
	FNCRC_SYSERR(0);
	FNC_TXT(fncrc, "%s", buf);
	fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
_door:
	if(buf != NULL) {
		FREE(buf);
	}
	return;
}

int
expr_reduce(
	fncrc_t	*fncrc,
	sess_t	*sp,
	comp_t	*cp,
	data_t	*expr,
	data_t	**pp
) {
	int	 rc = FNC_OKAY;
	data_t	*dp = NULL;
	leng_t	 cnt = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, sp);
	FNC_CHKINP(rc, cp);
	FNC_CHKINP(rc, expr);
	FNC_CHKOUTP(rc, pp);

	TRY(data_lst_cnt, _door, sp, expr, &cnt);
	if(cnt > 1) {
		dp = expr;
	} else {
		data_link_t	*lnk;

		TRY(data_lst_first, _door, sp, expr, &lnk);
		TRY(data_lst_remove, _door, sp, lnk, &dp);
		TRY(data_drop, _door, sp, expr);
	}
_door:
	*pp = dp;
	FNC_RETURN(rc);
}
