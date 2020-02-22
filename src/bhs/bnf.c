#include "bnf.h"

/*
	------------------------------------------------------------------------
 */
#if	defined(__SUNPRO_C)
comp_t	bnf_comp = {
	.comp_name = "Backus-Naur-Form compiler",
	.comp_dbg = 0,
	.comp_scanner = bnf_lex,
	.comp_scanner_init = bnf_lex_init,
	.comp_scanner_fini = bnf_lex_destroy,
	.comp_scanner_init_extra = bnf_lex_init_extra,

	.comp_parser = bnf_parse,
	.comp_eval = bnf_eval,
	.comp_gen = bnf_gen
};

#elif 	defined(__GNUC__)
comp_t	bnf_comp = {
	comp_name : "AML compiler",
	comp_dbg : 0,
	comp_scanner : bnf_lex,
	comp_scanner_init : bnf_lex_init,
	comp_scanner_fini : bnf_lex_destroy,
	comp_scanner_init_extra : bnf_lex_init_extra,

	comp_parser : bnf_parse,
	comp_eval : aml_eval,
	comp_gen : aml_gen
};

#else
#error("used compiler is not yet supported!")
#endif

/*
	------------------------------------------------------------------------
 */
DECLARE_DATA_IDENT(bnf_hex_id,		"BNF_HEX_NUMBER");
DECLARE_DATA_IDENT(bnf_dec_id,		"BNF_DEC_NUMBER");
DECLARE_DATA_IDENT(bnf_oct_id,		"BNF_OCT_NUMBER");
DECLARE_DATA_IDENT(bnf_bin_id,		"BNF_BIN_NUMBER");

DECLARE_DATA_IDENT(bnf_rules_id,	"bnf-rules");
DECLARE_DATA_IDENT(bnf_bool_expr_id,	"bnf-bool-expr");
DECLARE_DATA_IDENT(bnf_unary_expr_id,	"bnf-unary-expr");
DECLARE_DATA_IDENT(bnf_sub_expr_id,	"bnf-sub-expr");
DECLARE_DATA_IDENT(bnf_expr_list_id,	"bnf-expr-list");
DECLARE_DATA_IDENT(bnf_item_id,		"bnf-item");
DECLARE_DATA_IDENT(bnf_scalar_id,	"bnf-scalar");

DECLARE_DATA_IDENT(bnf_expr_op_id,	"bnf-expr-op");
DECLARE_DATA_IDENT(bnf_bool_op_id,	"bnf-bool-op");
DECLARE_DATA_IDENT(bnf_unary_op_id,	"bnf-unary-op");

/*
	------------------------------------------------------------------------
 */
