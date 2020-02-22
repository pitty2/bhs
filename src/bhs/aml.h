#ifndef __AML_HEAD_H__
#define __AML_HEAD_H__

#include <bhs/compile.h>

#ifdef  __cplusplus
extern "C" {
#endif

/*
	------------------------------------------------------------------------
 */
int	 aml_lex		(YYSTYPE*, fncrc_t*, sess_t*, comp_t*, yyscan_t);
int	 aml_parse		(fncrc_t *, sess_t *, comp_t *, yyscan_t);
int	 aml_gen		(fncrc_t *, sess_t *, comp_t *);
int	 aml_lex_init		(yyscan_t *);
int	 aml_lex_init_extra	(comp_t *, yyscan_t *);
int	 aml_lex_destroy	(yyscan_t);

/*
	------------------------------------------------------------------------
 */
extern	data_id_t	hex_id;
extern	data_id_t	dec_id;
extern	data_id_t	oct_id;
extern	data_id_t	bin_id;
#define	HEX		&hex_id
#define	DEC		&dec_id
#define	OCT		&oct_id
#define	BIN		&bin_id

extern	data_id_t	rules_id;
extern	data_id_t	bool_expr_id;
extern	data_id_t	unary_expr_id;
extern	data_id_t	sub_expr_id;
extern	data_id_t	expr_list_id;
extern	data_id_t	item_id;
extern	data_id_t	scalar_id;

#define	RULES		&rules_id
#define	BOOL_EXPR	&bool_expr_id
#define	UNARY_EXPR	&unary_expr_id
#define	SUB_EXPR	&sub_expr_id
#define	EXPR_LIST	&expr_list_id
#define	ITEM		&item_id
#define	SCALAR		&scalar_id

extern	data_id_t	 expr_op_id;
extern	data_id_t	 bool_op_id;
extern	data_id_t	 unary_op_id;
#define	EXPR_OP		&expr_op_id
#define	BOOL_OP		&bool_op_id
#define	UNARY_OP	&unary_op_id

/*
	------------------------------------------------------------------------
 */
extern	data_id_t	 pointer_op_id;
extern	data_id_t	 unary_plus_op_id;
extern	data_id_t	 unary_minus_op_id;
extern	data_id_t	 logical_not_op_id;
extern	data_id_t	 deref_op_id;
extern	data_id_t	 addr_op_id;
extern	data_id_t	 not_op_id;
extern	data_id_t	 mul_op_id;
extern	data_id_t	 div_op_id;
extern	data_id_t	 mod_op_id;
extern	data_id_t	 plus_op_id;
extern	data_id_t	 minus_op_id;
extern	data_id_t	 shl_op_id;
extern	data_id_t	 shr_op_id;
extern	data_id_t	 lt_op_id;
extern	data_id_t	 le_op_id;
extern	data_id_t	 gt_op_id;
extern	data_id_t	 ge_op_id;
extern	data_id_t	 eq_op_id;
extern	data_id_t	 ne_op_id;
extern	data_id_t	 and_op_id;
extern	data_id_t	 xor_op_id;
extern	data_id_t	 or_op_id;
extern	data_id_t	 logical_and_op_id;
extern	data_id_t	 logical_or_op_id;
extern	data_id_t	 ternary_id;

#define	PTR		&pointer_op_id
#define	U_PLUS		&unary_plus_op_id
#define	U_MINUS		&unary_minus_op_id
#define	L_NOT		&logical_not_op_id
#define DEREF		&deref_op_id
#define	ADDR		&addr_op_id
#define	NOT		&not_op_id
#define	MUL		&mul_op_id
#define	DIV		&div_op_id
#define	MOD		&mod_op_id
#define	PLUS		&plus_op_id
#define	MINUS		&minus_op_id
#define	SHL		&shl_op_id
#define	SHR		&shr_op_id
#define	LT		&lt_op_id
#define	LE		&le_op_id
#define	GT		&gt_op_id
#define	GE		&ge_op_id
#define	EQ		&eq_op_id
#define	NE		&ne_op_id
#define	AND		&and_op_id
#define	XOR		&xor_op_id
#define	OR		&or_op_id
#define	L_AND		&logical_and_op_id
#define	L_OR		&logical_or_op_id
#define	TERNARY		&ternary_id

/*
	------------------------------------------------------------------------
 */
extern	const data_t	 pointer_op_fnc;
extern	const data_t	 unary_plus_op_fnc;
extern	const data_t	 unary_minus_op_fnc;
extern	const data_t	 logical_not_op_fnc;
extern	const data_t	 deref_op_fnc;
extern	const data_t	 addr_op_fnc;
extern	const data_t	 not_op_fnc;
extern	const data_t	 mul_op_fnc;
extern	const data_t	 div_op_fnc;
extern	const data_t	 mod_op_fnc;
extern	const data_t	 plus_op_fnc;
extern	const data_t	 minus_op_fnc;
extern	const data_t	 shl_op_fnc;
extern	const data_t	 shr_op_fnc;
extern	const data_t	 lt_op_fnc;
extern	const data_t	 le_op_fnc;
extern	const data_t	 gt_op_fnc;
extern	const data_t	 ge_op_fnc;
extern	const data_t	 eq_op_fnc;
extern	const data_t	 ne_op_fnc;
extern	const data_t	 and_op_fnc;
extern	const data_t	 xor_op_fnc;
extern	const data_t	 or_op_fnc;
extern	const data_t	 logical_and_op_fnc;
extern	const data_t	 logical_or_op_fnc;
extern	const data_t	 ternary__fnc;

int	 aml_id_reduce_prec(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		data_t		*expr
		);
int	 error_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		char		*errtxt,
		data_t		**pp
		);

typedef	int	(*aml_op_fnc)(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 ptr_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 unary_plus_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 unary_minus_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 logical_not_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 deref_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 addr_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 not_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 mul_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 div_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 mod_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 plus_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 minus_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 shl_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 shr_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 lt_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 le_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 gt_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 ge_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 eq_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 ne_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 and_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 xor_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 or_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 logical_and_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 logical_or_fnc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		comp_t		*cp,
		leng_t		 argc,
		data_link_t	*argv,
		data_t		**pp
		);
int	 ternary_fnc(
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
int	aml_eval(fncrc_t *, sess_t *, comp_t *, leng_t, data_link_t *, data_t **);


#ifdef  __cplusplus
}
#endif

#endif /* __AML_HEAD_H__ */
