#ifndef __BHS_COMMON_ARR_H__
#define __BHS_COMMON_ARR_H__
/*
	Specification of "arrays"
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bhs/fncrc.h>
#include <bhs/length.h>
#include <bhs/byte.h>
#include <bhs/mem.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern	char	*arr_cause[];
#define	ARR_CAUSE_OKAY		arr_cause[0]
#define	ARR_CAUSE_BOUNDARIES	arr_cause[1]
#define	ARR_CAUSE_ELEMENT_SIZE	arr_cause[2]
#define	ARR_CAUSE_NOMEM		arr_cause[3]
#define	ARR_CAUSE_SYSCALL	arr_cause[4]

typedef struct array_s			arr_t;

struct array_s {
	leng_t	 elsz;	/* size of elements */
	byte_t	*base;	/* begin of dimension memory-segment */
	byte_t	*next;	/* next free place in dimension-segment */
	byte_t	*halt;	/* behind the end of dimension-segment */
};

#define ARR_FIRST(aptr)	((aptr)->base)
#define ARR_LAST(aptr)	((aptr)->next - (aptr)->elsz)
#define	ARR_SIZE(aptr)	((aptr)->halt - (aptr)->base)
#define	ARR_AVAIL(aptr)	((aptr)->halt - (aptr)->next)

#define ARR_WALK(el, typ, aptr) \
	for( el = (typ *)arr_first(aptr); el != NULL; \
		el = (typ *)arr_next(aptr, el) )

#define NUL_ARR	{ 0, NULL, NULL, NULL }

#define	SET_ARR(name, type)			\
	{					\
		sizeof(type),			\
		(char *)&name,			\
		(char *)&name + sizeof(name),	\
		(char *)&name + sizeof(name)	\
	}

#define	 arr_get(p, i)	arr_elm_ref(p, i)

extern int	 arr_init	(fncrc_t *fncrc, arr_t *aptr, leng_t elsz, leng_t len);
extern int	 arr_new	(fncrc_t *fncrc, leng_t elsz, leng_t len, arr_t **pp);
extern int	 arr_fini	(fncrc_t *fncrc, arr_t *aptr);
extern int	 arr_drop	(fncrc_t *fncrc, arr_t *aptr);

extern int	 arr_set	(fncrc_t *fncrc, arr_t *aptr, leng_t elcnt, void *word);
extern int	 arr_resize	(fncrc_t *fncrc, arr_t *aptr, leng_t elcnt);
extern int	 arr_insert	(fncrc_t *fncrc, arr_t *aptr, void *ielm, leng_t idx);
extern int	 arr_append	(fncrc_t *fncrc, arr_t *aptr, void *ielm);
extern int	 arr_remove	(fncrc_t *fncrc, arr_t *aptr, leng_t idx, void **pp);

extern int	 arr_elm_idx	(fncrc_t *fncrc, arr_t *aptr, void *ref, leng_t *idx);
extern int	 arr_elm_ref	(fncrc_t *fncrc, arr_t *aptr, leng_t idx, byte_t **p);

extern void	*arr_first	(arr_t *aptr);
extern void	*arr_last	(arr_t *aptr);
extern void	*arr_next	(arr_t *aptr, void *ref);
extern void	*arr_prev	(arr_t *aptr, void *ref);
extern leng_t	 arr_count	(arr_t *aptr);
extern leng_t	 arr_size	(arr_t *aptr);
extern leng_t	 arr_avail	(arr_t *aptr);
extern void	 arr_prn	(arr_t *aptr, FILE *out, leng_t n);

#ifdef  __cplusplus
}
#endif

#endif /* __BHS_COMMON_ARR_H__ */
