#include "aml.h"

/*
	------------------------------------------------------------------------
 */
#if	defined(__SUNPRO_C)
comp_t	aml_comp = {
	.comp_name = "AML compiler",
	.comp_dbg = 0,
	.comp_scanner = aml_lex,
	.comp_scanner_init = aml_lex_init,
	.comp_scanner_fini = aml_lex_destroy,
	.comp_scanner_init_extra = aml_lex_init_extra,

	.comp_parser = aml_parse,
	.comp_eval = aml_eval,
	.comp_gen = aml_gen
};

#elif 	defined(__GNUC__)
comp_t	aml_comp = {
	comp_name : "AML compiler",
	comp_dbg : 0,
	comp_scanner : aml_lex,
	comp_scanner_init : aml_lex_init,
	comp_scanner_fini : aml_lex_destroy,
	comp_scanner_init_extra : aml_lex_init_extra,

	comp_parser : aml_parse,
	comp_eval : aml_eval,
	comp_gen : aml_gen
};

#else
#error("used compiler is not yet supported!")
#endif

/*
	------------------------------------------------------------------------
 */
DECLARE_DATA_IDENT(hex_id,		"HEX_NUMBER");
DECLARE_DATA_IDENT(dec_id,		"DEC_NUMBER");
DECLARE_DATA_IDENT(oct_id,		"OCT_NUMBER");
DECLARE_DATA_IDENT(bin_id,		"BIN_NUMBER");

DECLARE_DATA_IDENT(rules_id,		"rules");
DECLARE_DATA_IDENT(bool_expr_id,	"bool-expr");
DECLARE_DATA_IDENT(unary_expr_id,	"unary-expr");
DECLARE_DATA_IDENT(sub_expr_id,		"sub-expr");
DECLARE_DATA_IDENT(expr_list_id,	"expr-list");
DECLARE_DATA_IDENT(item_id,		"item");
DECLARE_DATA_IDENT(scalar_id,		"scalar");

DECLARE_DATA_IDENT(expr_op_id,		"expr-op");
DECLARE_DATA_IDENT(bool_op_id,		"bool-op");
DECLARE_DATA_IDENT(unary_op_id,		"unary-op");

/*
	------------------------------------------------------------------------
 */
