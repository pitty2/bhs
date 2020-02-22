#ifndef __BHS_HEADER_H__
#define __BHS_HEADER_H__

#ifdef Linux
#define _GNU_SOURCE
#endif

#include <bhs/arr.h>
#include <bhs/ascii.h>
#include <bhs/bool.h>
#include <bhs/byte.h>
#include <bhs/compat.h>
#include <bhs/compile.h>
#include <bhs/conv.h>
#include <bhs/data_conv.h>
#include <bhs/data.h>
#include <bhs/dbg.h>
#include <bhs/dll.h>
#include <bhs/error.h>
#include <bhs/file.h>
#include <bhs/fncrc.h>
#include <bhs/length.h>
#include <bhs/mem.h>
#include <bhs/number.h>
#include <bhs/prn.h>
#include <bhs/sess.h>
#include <bhs/str.h>

#ifdef  __cplusplus
extern "C" {
#endif

int	bhs_value_by_symbol(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*tree,
		char	*sym,
		data_t	**pp
	);
int	bhs_value_2cstr(
		fncrc_t	*fncrc,
		sess_t	*sp,
		data_t	*val,
		char	**pp
	);

extern	data_id_t	aml_id;
extern	data_id_t	symtree_id;
extern	data_id_t	reftree_id;
extern	data_id_t	error_id;

#define	SYMTREE		&symtree_id
#define	REFTREE		&reftree_id
#define	AML		&aml_id
#define	ERROR		&error_id

#ifdef  __cplusplus
}
#endif

#endif /* __BHS_HEADER_H__ */
