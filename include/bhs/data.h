#ifndef __BHS_DATA_H__
#define __BHS_DATA_H__

#include <strings.h>
#include <bhs/bool.h>
#include <bhs/byte.h>
#include <bhs/number.h>
#include <bhs/length.h>
#include <bhs/dll.h>
#include <bhs/arr.h>
#include <bhs/str.h>
#include <bhs/file.h>
#include <bhs/conv.h>
#include <bhs/sess.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern	char	*data_cause[];
#define	DATA_CAUSE_OKAY		data_cause[0]
#define	DATA_CAUSE_NOMEM	data_cause[1]
#define	DATA_CAUSE_SYSCALL	data_cause[2]
#define	DATA_CAUSE_DESIGNATOR	data_cause[3]
#define	DATA_CAUSE_NOTINLIST	data_cause[4]
#define	DATA_CAUSE_HIRACHIE	data_cause[5]
#define	DATA_CAUSE_CONVERSION	data_cause[6]
#define	DATA_CAUSE_CIRCUIT	data_cause[7]
#define	DATA_CAUSE_METHOD	data_cause[8]
#define	DATA_CAUSE_IDENTIFIER	data_cause[9]

/*
	-----------------------------------------------------------------------
 */
typedef struct data_s		 data_t;
typedef struct data_des_s	 data_des_t;
typedef struct data_s		 data_id_t;
typedef struct data_s		*data_code_t;
typedef	struct data_link_s	 data_link_t;
typedef struct data_sequ_s	 data_sequ_t;

/*
	-----------------------------------------------------------------------
 */
extern	data_des_t		_data_designator;
extern	data_des_t		_data_id_designator;
extern	data_des_t		_data_list_designator;
extern	data_des_t		_data_number_designator;
extern	data_des_t		_data_string_designator;
extern	data_des_t		_data_boolean_designator;
extern	data_des_t		_data_function_designator;
extern	data_des_t		_data_file_designator;
#define DATA_DESIGNATOR		&_data_designator
#define DATA_IDENT_DESIGNATOR	&_data_id_designator
#define DATA_LIST_DESIGNATOR	&_data_list_designator
#define DATA_STRING_DESIGNATOR	&_data_string_designator
#define DATA_NUMBER_DESIGNATOR	&_data_number_designator
#define DATA_BOOL_DESIGNATOR	&_data_boolean_designator
#define DATA_FUNC_DESIGNATOR	&_data_function_designator
#define DATA_FILE_DESIGNATOR	&_data_file_designator

typedef enum data_designator_token_e { /* Data-Designator-Token*/
	DATA_DEFAULT_TOKEN = 1000,
	DATA_IDENT_TOKEN,
	DATA_LIST_TOKEN,
	DATA_STRING_TOKEN,
	DATA_NUMBER_TOKEN,
	DATA_BOOL_TOKEN,
	DATA_FNC_TOKEN
} data_dsg_tok_t;

extern	data_t		 data_eq_code;
extern	data_t		 data_ne_code;
extern	data_t		 data_lt_code;
extern	data_t		 data_le_code;
extern	data_t		 data_gt_code;
extern	data_t		 data_ge_code;
extern	data_t		 data_okay_code;
extern	data_t		 data_nil_code;
#define	DATA_EQ		&data_eq_code
#define	DATA_NE		&data_ne_code
#define	DATA_LT		&data_lt_code
#define	DATA_LE		&data_le_code
#define	DATA_GT		&data_gt_code
#define	DATA_GE		&data_ge_code
#define	DATA_OKAY	&data_okay_code
#define	DATA_NIL	&data_nil_code

/*
	Data Designator routines
	-----------------------------------------------------------------------
 */
extern	int	 data_des_get(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*dp,
			data_des_t	**dsg
			);
extern	int	 data_des_2cstr(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_des_t	*p,
			char		**cstr
			);
extern	int	 data_des_prn(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_des_t	*p,
			FILE		*out,
			leng_t		 n
			);

/*
	Data Identifier routines
	-----------------------------------------------------------------------
 */
extern	int	 data_id_new(
			fncrc_t		*fncrc,
			sess_t		*sp,
			char		*name,
			data_id_t	**pp
			);
extern	int	 data_id_drop(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*id
			);
extern	int	 data_id_cpy(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*src,
			data_id_t	**dst
			);
extern	int	 data_id_cmp(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*p1,
			data_id_t	*p2,
			data_code_t	*code
			);
extern	int	 data_id_prn(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*p,
			FILE		*out,
			leng_t		 n
			);
extern	int	 data_id_2cstr(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			char		**cstr
			);
extern	int	 data_id_set(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_id_t	*id
			);
extern	int	 data_id_get(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_id_t	**pp
			);

/*
	Data Sequence routines
	-----------------------------------------------------------------------
 */
extern	int	data_sequ_new(
			fncrc_t		*fncrc,
			sess_t		*sp,
			leng_t		 num,
			char		*name,
			data_sequ_t	**pp
			);
extern	int	 data_sequ_set(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_sequ_t	*sequ
			);
extern	int	 data_sequ_get(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*dp,
			data_sequ_t	**pp
			);
extern	int	 data_sequ_inherit(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*src,
			data_t		*dst
			);
extern	int	 data_sequ_2cstr(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_sequ_t	*sequ,
			char		**pp
			);
extern	int	 data_sequ_prn(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_sequ_t	*p,
			FILE		*out,
			leng_t		 n
			);
/*
	Data basic primitives
	-----------------------------------------------------------------------
 */
extern	int	 data_size(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			leng_t		*sz
			);
extern	int	 data_init(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*ident,
			data_t		*dp
			);
extern	int	 data_new(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*ident,
			data_t		**dp
			);
extern	int	 data_fini(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);
extern	int	 data_drop(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);
extern	int	 data_cpy(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*src,
			data_t		**dst
			);
extern	int	 data_cmp(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p1,
			data_t		*p2,
			data_code_t	*code
			);
extern	int	 data_prn(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			FILE		*out,
			leng_t		 n
			);
/*
	-----------------------------------------------------------------------
 */
extern	int	 data_lst_init(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*ident,
			data_t		*p
			);
extern	int	 data_lst_new(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*ident,
			data_t		**pp
			);
extern	int	 data_lst_fini(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);
extern	int	 data_lst_drop(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);
extern	int	 data_lst_cpy(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*src,
			data_t		**dst
			);
extern	int	 data_lst_cmp(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p1,
			data_t		*p2,
			data_code_t	*code
			);
extern	int	 data_lst_prn(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			FILE		*out,
			leng_t		 n
			);

extern	int	 data_lst_ins_first(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*list,
			data_t		*elem,
			data_link_t	**pp
			);
extern	int	 data_lst_ins_last(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*list,
			data_t		*elem,
			data_link_t	**pp
			);
extern	int	 data_lst_remove(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_link_t	*link,
			data_t		**pp
			);
extern	int	 data_lst_ins_prev(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_link_t	*link,
			data_t		*elem,
			data_link_t	**pp
			);
extern	int	 data_lst_ins_next(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_link_t	*link,
			data_t		*elem,
			data_link_t	**pp
			);
extern	int	 data_lst_first(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*list,
			data_link_t	**pp
			);
extern	int	 data_lst_last(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*list,
			data_link_t	**pp
			);
extern	int	 data_lst_prev(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_link_t	*link,
			data_link_t	**pp
			);
extern	int	 data_lst_next(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_link_t	*link,
			data_link_t	**pp
			);
extern	int	 data_lst_nth(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			leng_t		pos, /* 1 ... */
			data_link_t	**pp
			);
extern	int	 data_lst_cnt(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			leng_t		*pp
			);

/*
	-----------------------------------------------------------------------
 */
extern	int	 data_str_init(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_id_t	*ident,
			leng_t		 len,
			char		*word
			);
extern	int	 data_str_new(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*ident,
			leng_t		 len,
			char		*word,
			data_t		**pp
			);
extern	int	 data_str_fini(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);
extern	int	 data_str_drop(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);
extern	int	 data_str_cpy(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*src,
			data_t		**dst
			);
extern	int	 data_str_cmp(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p1,
			data_t		*p2,
			data_code_t	*code
			);
extern	int	 data_str_prn(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			FILE		*out,
			leng_t		 n
			);
extern	int	 data_str_len(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t 		*p,
			leng_t		*len
			);
extern	int	 data_str_cat(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p1,
			data_t		*p2,
			data_t		**pp
			);
extern	int	 data_strn_cpy(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*src,
			leng_t		 n,
			data_t		**dst
			);
extern	int	 data_str_2num(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			leng_t		 base,
			data_t		**pp
			);
extern	int	 data_str_2cstr(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*dp,
			char		**pp
			);
extern	int	 data_str_split(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p1,
			data_t		*p2,
			data_t		**pp
			);

/*
	-----------------------------------------------------------------------
 */
extern	int	data_num_init(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_id_t	*ident,
			number_t	 number
			);
extern	int	data_num_new(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*ident,
			number_t	 number,
			data_t		**pp
			);
extern	int	data_num_fini(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);
extern	int	data_num_drop(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);
extern	int	data_num_cpy(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*src,
			data_t		**dst
			);
extern	int	data_num_cmp(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p1,
			data_t		*p2,
			data_code_t	*code
			);
extern	int	data_num_prn(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			FILE		*out,
			leng_t		 n
			);
extern	int	data_num_2string(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*src,
			data_t		**pp
			);

/*
	-----------------------------------------------------------------------
 */
extern	int	data_bool_init(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*dp,
			data_id_t	*ident,
			bool_t		 val
			);
extern	int	data_bool_new(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*ident,
			bool_t		 val,
			data_t		**pp
			);
extern	int	data_bool_fini(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);
extern	int	data_bool_drop(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);
extern	int	data_bool_cpy(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*src,
			data_t		**dst
			);
extern	int	data_bool_cmp(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p1,
			data_t		*p2,
			data_code_t	*code
			);
extern	int	data_bool_prn(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			FILE		*out,
			leng_t		 n
			);

/*
	-----------------------------------------------------------------------
 */
extern	int	datalnk_by_address(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_t		*addr,
			data_link_t	**pp
			);
extern	int	datalnk_by_ident(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_id_t	*id,
			data_link_t	**pp
			);
extern	int	data_by_ident(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_id_t	*id,
			data_t		**pp
			);
extern	int	datalnk_parent_by_address(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_t		*addr,
			data_link_t	**pp
			);
extern	int	datalnk_parent_by_ident(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_id_t	*id,
			data_link_t	**pp
			);
extern	int	data_parent_by_ident(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_id_t	*id,
			data_t		**pp
			);
extern	int	data_lookup(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*list,
			data_t		*p,
			data_t		**pp
			);
extern	int	data_get_first(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*list,
			data_t		**pp
			);
extern	int	data_get_last(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*list,
			data_t		**pp
			);
extern int	data_cnt_by_lnk(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_link_t	*lnk,
			leng_t		*pp
			);

/*
	-----------------------------------------------------------------------
 */
typedef int     (*data_fnc)();

extern int	data_fnc_init(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*dp,
			data_id_t	*ident,
			data_fnc	 fnc
			);

extern	int	data_fnc_new(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*ident,
			data_fnc	 fnc,
			data_t		**p
			);

extern	int	data_fnc_fini(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);

extern	int	data_fnc_drop(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);

extern	int	data_fnc_cpy(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*src,
			data_t		**dst
			);

extern	int	data_fnc_cmp(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p1,
			data_t		*p2,
			data_code_t	*pp
			);

extern	int	data_fnc_prn(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			FILE		*out,
			leng_t		 n
			);

/*
	-----------------------------------------------------------------------
 */
extern	int	data_file_init(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			data_id_t	*ident,
			char		*name
			);

extern	int	data_file_new(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_id_t	*ident,
			char		*name,
			data_t		**pp
			);

extern	int	data_file_fini(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);

extern	int	data_file_drop(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p
			);

extern	int	data_file_cpy(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*src,
			data_t		**dst
			);

extern	int	data_file_cmp(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p1,
			data_t		*p2,
			data_code_t	*pp
			);

extern	int	data_file_prn(
			fncrc_t		*fncrc,
			sess_t		*sp,
			data_t		*p,
			FILE		*out,
			leng_t		 n
			);

extern	int	data_file_open(
			fncrc_t *fncrc,
			sess_t  *sp,
			data_t  *p,
			char    *mode
			);

extern	int	data_file_close(
			fncrc_t *fncrc,
			sess_t  *sp,
			data_t  *p
			);




/*
	-------------------------------------------------------------------------------------------
 */
extern	dll_t	data_assoc_list;

#define	DATA_DBG(p)	((p)->sess_data_dbg)	
#define DATA_NONE_DBG	0
#define DATA_SHORT_DBG	1
#define DATA_SEQU_DBG	2
#define DATA_ADDR_DBG	3
#define DATA_LONG_DBG	4
#define DATA_CIRC_DBG	5
#define DATA_DUMP_DBG	6
/*
	-----------------------------------------------------------------------
 */
typedef struct data_assoc_s	data_assoc_t;
typedef struct data_walk_s	data_walk_t;
typedef union data_value_u	data_value_t;
typedef struct data_circuit_s	data_circ_t;

/*
	-----------------------------------------------------------------------
 */
struct data_des_s {		// data designator
	char		*des_name;
	int		(*des_init)();
	int		(*des_new)();
	int		(*des_fini)();
	int		(*des_drop)();
	int		(*des_put)();
	int		(*des_get)();
	int		(*des_cpy)();
	int		(*des_cmp)();
	int		(*des_prn)();
};
#define	DES_INIT(dp)	((dp)->data_designator->des_init)
#define	DES_NEW(dp)	((dp)->data_designator->des_new)
#define	DES_FINI(dp)	((dp)->data_designator->des_fini)
#define	DES_DROP(dp)	((dp)->data_designator->des_drop)
#define	DES_CPY(dp)	((dp)->data_designator->des_cpy)
#define	DES_CMP(dp)	((dp)->data_designator->des_cmp)
#define	DES_PRN(dp)	((dp)->data_designator->des_prn)

#define	CHK_DES(_rc, _dp, _des)								\
	if((_dp)->data_designator != _des) {						\
		rc = FNC_ERROR;								\
		FNCRC_RESULT(rc);							\
		FNCRC_CAUSE(DATA_CAUSE_DESIGNATOR);					\
		FNCRC_SYSERR(0);							\
		FNC_TXT(fncrc, "%s expected!", TSTR((_des)->des_name));			\
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);	\
		return(_rc);								\
	}

union data_value_u {		// data value container
	dll_t		*list;
	arr_t		*arr;
	str_t		*string;
	number_t	 number;
	bool_t		 boolean;
	fdes_t		*file;
	void		*aux;
	int 		(*func)();
};

#define	DATA_ID_SIZE	256
struct data_s {			// data object struct
	dll_t		 data_refby;
	data_des_t 	*data_designator;
	data_value_t	 data_;
	void		(*data_free)(void *);
	data_id_t	*data_id;
	data_sequ_t	*data_sequ;
};
#define	DREF(p)		(&(p)->data_refby)
#define	DES(p)		((p)->data_designator)
#define	DFREE(p)	((p)->data_free)
#define	DID(p)		((p)->data_id)
#define SEQU(p)		((p)->data_sequ)

#define DLST(p)		((p)->data_.list)
#define DSTR(p)		((p)->data_.string)
#define DNUM(p)		((p)->data_.number)
#define DBOOL(p)	((p)->data_.boolean)
#define DFUNC(p)	((p)->data_.func)
#define DFILE(p)	((p)->data_.file)

#define DCSTR(p)	((char *)(p)->data_.string->base)
#define DCSTR_NXT(p)	((char *)(p)->data_.string->next)

/*
	-----------------------------------------------------------------------
 */
#if	defined(__SUNPRO_C)

#define	DECLARE_DATA_IDENT(name, str)					\
static									\
byte_t	name##cstr[] = str;						\
str_t	name##_str_t = {						\
		sizeof(byte_t),						\
		&name##cstr[0],						\
		&name##cstr[0] + sizeof(name##cstr) -1,			\
		&name##cstr[0] + sizeof(name##cstr) -1			\
	};								\
data_t	name = {							\
	.data_refby = {							\
		.next = (dll_t *)&name,					\
		.prev = (dll_t *)&name,					\
		.head = (dll_t *)&name					\
	},								\
	.data_designator = DATA_IDENT_DESIGNATOR,			\
	.data_= {							\
		.string = &name##_str_t					\
	},								\
	.data_free = NULL						\
}

#define	DECLARE_DATA_STRING(name, str)					\
static									\
byte_t	name##cstr[] = str;						\
str_t	name##_str_t = {						\
		sizeof(byte_t),						\
		&name##cstr[0],						\
		&name##cstr[0] + sizeof(name##cstr) -1,			\
		&name##cstr[0] + sizeof(name##cstr) -1			\
	};								\
data_t	name = {							\
	.data_refby = {							\
		.next = (dll_t *)&name,					\
		.prev = (dll_t *)&name,					\
		.head = (dll_t *)&name					\
	},								\
	.data_designator = DATA_STRING_DESIGNATOR,			\
	.data_= {							\
		.string = &name##_str_t					\
	},								\
	.data_free = NULL						\
}

#define	DECLARE_DATA_NUMBER(name, value)				\
data_t	name = {							\
	.data_refby = {							\
		.next = (dll_t *)&name,					\
		.prev = (dll_t *)&name,					\
		.head = (dll_t *)&name					\
	},								\
	.data_designator = DATA_NUMBER_DESIGNATOR,			\
	.data_= {							\
		.number = value						\
	},								\
	.data_free = NULL						\
}

#define	DECLARE_DATA_BOOLEAN(name, value)				\
data_t	name = {							\
	.data_refby = {							\
		.next = (dll_t *)&name,					\
		.prev = (dll_t *)&name,					\
		.head = (dll_t *)&name					\
	},								\
	.data_designator = DATA_BOOL_DESIGNATOR,			\
	.data_= {							\
		.boolean = value					\
	},								\
	.data_free = NULL						\
}

#elif	defined(__GNUC__)

#define	DECLARE_DATA_IDENT(name, str)					\
static									\
byte_t	name##cstr[] = str;						\
str_t	name##_str_t = {						\
		sizeof(byte_t),						\
		&name##cstr[0],						\
		&name##cstr[0] + sizeof(name##cstr) -1,			\
		&name##cstr[0] + sizeof(name##cstr) -1			\
	};								\
data_t	name = {							\
	data_refby : {							\
		next : (dll_t *)&name,					\
		prev : (dll_t *)&name,					\
		head : (dll_t *)&name					\
	},								\
	data_designator : DATA_IDENT_DESIGNATOR,			\
	data_: {							\
		string : &name##_str_t					\
	},								\
	data_free : NULL						\
}

#define	DECLARE_DATA_STRING(name, str)					\
static									\
byte_t	name##cstr[] = str;						\
str_t	name##_str_t = {						\
		sizeof(byte_t),						\
		&name##cstr[0],						\
		&name##cstr[0] + sizeof(name##cstr) -1,			\
		&name##cstr[0] + sizeof(name##cstr) -1			\
	};								\
data_t	name = {							\
	data_refby : {							\
		next : (dll_t *)&name,					\
		prev : (dll_t *)&name,					\
		head : (dll_t *)&name					\
	},								\
	data_designator : DATA_STRING_DESIGNATOR,			\
	data_: {							\
		string : &name##_str_t					\
	},								\
	data_free : NULL						\
}

#define	DECLARE_DATA_NUMBER(name, value)				\
data_t	name = {							\
	data_refby : {							\
		next : (dll_t *)&name,					\
		prev : (dll_t *)&name,					\
		head : (dll_t *)&name					\
	},								\
	data_designator : DATA_NUMBER_DESIGNATOR,			\
	data_: {							\
		number : value						\
	},								\
	data_free : NULL						\
}

#define	DECLARE_DATA_BOOLEAN(name, value)				\
data_t	name = {							\
	data_refby : {							\
		next : (dll_t *)&name,					\
		prev : (dll_t *)&name,					\
		head : (dll_t *)&name					\
	},								\
	data_designator : DATA_BOOL_DESIGNATOR,				\
	data_: {							\
		boolean : value						\
	},								\
	data_free : NULL						\
}

#else
#error("used compiler is not yet supported!")
#endif /* compiler */
/*
	-----------------------------------------------------------------------
 */


/*
	common data methods
	-----------------------------------------------------------------------
 */
int	 data_clear_refby(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*p
		);
int	 data_hdr_2cstr(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*dp,
		char	**pp
		);
int	 data_prn_refby(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*p,
		FILE	*out,
		leng_t	 n
		);
int	 data_hdr_prn(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*p,
		FILE	*out,
		leng_t	 n
		);

int	 _data_fini(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_t		*p
		);
int	 _data_drop(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_t		*p
		);
int	 _data_cpy(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_t		*src,
		data_t		**dst
		);
int	 _data_cmp(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_t		*p1,
		data_t		*p2,
		data_code_t	*code
		);
int	 _data_prn(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*p,
		FILE	*out,
		leng_t	 n
		);
/*
	-----------------------------------------------------------------------
 */
struct data_link_s {
	dll_t		 link_entry;	/* insertion point */
	data_t		*link_owner;
	data_assoc_t	*link_assoc;
};

int	 data_link_init(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_t		*owner,
		data_assoc_t	*assoc,
		data_link_t	*lnk
		);
int	 data_link_new(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_t		*owner,
		data_assoc_t	*assoc,
		data_link_t	**pp
		);
int	 data_link_fini(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_link_t	*lnk
		);
int	 data_link_drop(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_link_t	*lnk
		);
int	 data_link_points_parent(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_link_t	*lnk,
		bool_t		*pp
		);
int	 data_link_points_child(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_link_t	*lnk,
		bool_t		*pp
		);
int	 data_link_2plink(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_link_t	*lnk,
		data_link_t	**pp
		);
int	 data_link_2parent(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_link_t	*lnk,
		data_t		**pp
		);
int	 data_link_2clink(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_link_t	*lnk,
		data_link_t	**pp
		);
int	 data_link_2child(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_link_t	*lnk,
		data_t		**pp
		);
int 	 data_link_2assoc(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_link_t	*lnk,
		data_assoc_t	**pp
		);
int 	 data_link_2cstr(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_link_t	*lnk,
		char		**cstr
		);
int	 data_link_prn(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_link_t	*lnk,
		FILE		*out,
		leng_t		 n
		);
/*
	-----------------------------------------------------------------------
 */
struct data_assoc_s {
#define	PARENT	0
#define	CHILD	1
	dll_t		 assoc_entry;
	data_link_t	 assoc_link[2];
#if(0)
	data_t		*assoc_loop;
#endif
};

int	 data_assoc_init(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_assoc_t	*p,
		data_t		*child
		);
int	 data_assoc_new(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_t		*child,
		data_assoc_t	**pp
		);
int	 data_assoc_fini(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_assoc_t	*p
		);
int	 data_assoc_drop(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_assoc_t	*p
		);
int	 data_assoc_2parent(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_assoc_t	*asp,
		data_t		**pp
		);
int	 data_assoc_2child(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_assoc_t	*asp,
		data_t		**pp
		);
int	 lookup_child(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_t		*lst,
		data_t		*search,
volatile	leng_t		*dist,
		data_assoc_t	**pp
		);
data_assoc_t	*lookup_parent(
		data_t	*dp,
		data_t	*child,
		leng_t	*dist
		);
int	 data_assoc_lookup(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_t		*p,
		data_assoc_t	**pp
		);
int	 data_assoc_2cstr(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_assoc_t	*asp,
		char		**cstr
		);
int	 data_assoc_prn(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_assoc_t	*p,
		FILE		*out,
		leng_t		 n
		);
int	 data_assoc_list_prn(
		fncrc_t		*fncrc,
		sess_t		*sp,
		FILE		*out,
		leng_t		 n
		);
/*
	-----------------------------------------------------------------------
 */
typedef	dll_t	data_stack_t;

struct stack_item_s {
	dll_t		 entry;
	data_link_t	*elm;
};
typedef struct stack_item_s	stack_elm_t;

#define	SESS_STACK(sp)		(data_stack_t *)((sp)->sess_data_stack[0])
#define	SESS_STACK_PTR(sp)	(data_stack_t **)&((sp)->sess_data_stack[0])
#define	SESS_AUX_STACK(sp)	(data_stack_t *)((sp)->sess_data_stack[1])
#define	SESS_AUX_STACK_PTR(sp)	(data_stack_t **)&((sp)->sess_data_stack[1])

int	 data_stack_new(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_stack_t	**stack
		);
int	 data_stack_drop(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_stack_t	**stack
		);
int	 data_stack_push(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_stack_t	**stack,
		data_link_t	*elm
		);
int	 data_stack_pop(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_stack_t	**stack,
		data_link_t	**elm
		);
int	 data_stack_lookup(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_stack_t	*stack,
		data_t		*search,
		data_circ_t	**pp
		);
int	 data_stack_prn(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_stack_t	*stack,
		FILE		*out,
		leng_t		 n
		);

/*
	-----------------------------------------------------------------------
 */
struct	data_circuit_s {
	data_assoc_t	*ci_origin;	/* Originator des Zyklus */
	data_t		*ci_cref;	/* child, der Originator enthaelt */
	leng_t		 ci_dist;	/* Baumtiefe des Links */
	data_stack_t	*ci_stack;	/* Stack-Trace des Zyklus */
};
int	 data_circ_new(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_assoc_t	*origin,
		data_t		*cref,
		leng_t		 dist,
		data_circ_t	**pp
		);
int	 data_circ_drop(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_circ_t	*ci
		);
int	 data_circ_cmp(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_circ_t	*ci1,
		data_circ_t	*ci2,
		data_code_t	*pp
		);
int	 data_circ_prn(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_circ_t	*ci,
		FILE		*out,
		leng_t		 n
		);

/*
	-----------------------------------------------------------------------
 */
struct data_walk_s {
	dll_t		 walk_entry;	// entry for stack
	dll_t		 walk_stack;	// stack
	data_assoc_t	*walk_superior; // NULL ? stack : stack-element
	data_t		*walk_cref;	// cycle originator referrence
};

int	 data_walk_init(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_walk_t	*p,
		data_assoc_t	*asp
		);
int	 data_walk_new(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_assoc_t	*asp,
		data_walk_t	**pp
		);
int	 data_walk_drop(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_walk_t	*p
		);
int	 data_walk_push(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_walk_t	*stack,
		data_assoc_t	*asp,
		data_walk_t	**pp
		);
int 	 data_walk_pop(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_walk_t	**stack,
		data_assoc_t	**elm
		);
int	 data_walk_lookup(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_walk_t	*stack,
		data_t		*search,
		data_walk_t	**pp
		);
int	 data_walk_distance(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_walk_t	*stack,
		data_walk_t	*search,
		leng_t		*n
		);
int	 data_walk_prn(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_walk_t	*p,
		FILE		*out,
		leng_t		 n
		);

/*
	-----------------------------------------------------------------------
 */
struct data_sequ_s {
	leng_t	 num;
	char	*name;
};

int	 data_sequ_drop(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_sequ_t	*sequ
		);
int	 data_sequ_cpy(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_sequ_t	*src,
		data_sequ_t	**dst
		);
int	 data_sequ_prn(
		fncrc_t		*fncrc,
		sess_t		*sp,
		data_sequ_t	*p,
		FILE		*out,
		leng_t		 n
		);

#define	SEQU_NUM(p)		(((p)->data_sequ != NULL) ? p->data_sequ->num : 0)
#define	SEQU_NAME(p)		(((p)->data_sequ != NULL) ? p->data_sequ->name : "{nil}")

/*
	-----------------------------------------------------------------------
 */

#ifdef __SUNPRO_C
#define	DECLARE_DATA_FUNC(name, id, fnc)		\
const data_t	name = {				\
	.data_refby = {					\
		.next = (dll_t *)&name,			\
		.prev = (dll_t *)&name,			\
		.head = (dll_t *)&name			\
	},						\
	.data_designator = DATA_FUNC_DESIGNATOR,	\
	.data_= {					\
		.func = fnc				\
	},						\
	.data_free = NULL,				\
	.data_id = id					\
}

#else

#define	DECLARE_DATA_FUNC(name, id, fnc)		\
const data_t	name = {				\
	data_refby : {					\
		next : (dll_t *)&name,			\
		prev : (dll_t *)&name,			\
		head : (dll_t *)&name			\
	},						\
	data_designator : DATA_FUNC_DESIGNATOR,		\
	data_: {					\
		func : fnc				\
	},						\
	data_free : NULL,				\
	data_id : id					\
}

#endif /* __SUNPRO_CC */

#ifdef  __cplusplus
}
#endif

#endif /*__BHS_DATA_H__ */
