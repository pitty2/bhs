#ifndef __DLL_H__
#define __DLL_H__
/*
	Specification of "doubly linked Lists"
 */
#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define TSTR(p)	((p == NULL) ? "{nil}" : p)
#define TVAL(p)	((p == NULL) ? "{0}" : p)

#if	defined(__SUNPRO_C)
#define FPRINTF(out, ...)			\
if (prn_ferror(out) == 0) {			\
	fprintf (out, __VA_ARGS__);		\
}

#define PRNN(__n__) 				\
if (prn_ferror(out) == 0) {			\
	unsigned long	 __cnt__ = __n__;	\
						\
	while(__cnt__ > 0) {			\
		fprintf(out, "\t");		\
		__cnt__--;			\
	}					\
	fflush(out);				\
}

#define PRNO(...)				\
if (prn_ferror(out) == 0) {			\
	fprintf (out, __VA_ARGS__);		\
	fflush(out);				\
}

#elif	defined(__GNUC__)
#define FPRINTF(out, format, args...)		\
if (prn_ferror(out) == 0) {			\
	fprintf (out, format , ## args);	\
}

#define PRNN(__n__) 				\
if (prn_ferror(out) == 0) {			\
	unsigned long	 __cnt__ = __n__;	\
						\
	while(__cnt__ > 0) {			\
		fprintf(out, "\t");		\
		__cnt__--;			\
	}					\
	fflush(out);				\
}

#define PRNO(format, args...)			\
if (prn_ferror(out) == 0) {			\
	fprintf (out, format , ## args);	\
	fflush(out);				\
}

#else
#error("used compiler is not yet supported!")
#endif /* compiler */

int	 prn_ferror(FILE *fd);

typedef unsigned long long int	dll_sz;

typedef struct dll_s {
	struct dll_s	*next;
	struct dll_s	*prev;
	struct dll_s	*head;
} dll_t;

#define DLL_NIL		{ NULL, NULL, NULL }

#define DLL_NEXT(e)	(((dll_t*)(e))->next)
#define DLL_PREV(e)	(((dll_t*)(e))->prev)
#define DLL_HEAD(e)	(((dll_t*)(e))->head)

#define DLL_ISHEAD(e)	(((dll_t*)(e)) == DLL_HEAD (e))
#define DLL_FIRST(h)	(( DLL_NEXT(h) == (dll_t*)(h)) ? NULL : DLL_NEXT(h) )
#define DLL_LAST(h)	(( DLL_PREV(h) == (dll_t*)(h)) ? NULL : DLL_PREV(h) )
#define DLL_ISLAST(e)	(((dll_t*)(e))->next == ((dll_t*)(e))->head)

#define DLL_ISEMPTY(h)	(((h)->head == (h)->next)? TRUE : FALSE)
#define DLL_INLIST(e)	(((e)->head != NULL)? TRUE : FALSE)

#define DLL_WALK(el, type, head) \
	for(el = (type *)DLL_NEXT((dll_t *)head); !DLL_ISHEAD((dll_t *)el); \
		el = (type *)DLL_NEXT((dll_t *)el))

#define DLL_WALK_DOUBLE(el1, el2, type, head1, head2)				\
	for(	el1 = (type *)DLL_NEXT((dll_t *)head1),				\
		el2 = (type *)DLL_NEXT((dll_t *)head2);				\
		(!DLL_ISHEAD((dll_t *)el1)) && (!DLL_ISHEAD((dll_t *)el2));	\
		el1 = (type *)DLL_NEXT((dll_t *)el1),				\
		el2 = (type *)DLL_NEXT((dll_t *)el2)				\
	)

#define DLL_BACK(el, type, head) \
	for(el = (type *)DLL_PREV((dll_t *)head); !DLL_ISHEAD((dll_t *)el); \
		el = (type *)DLL_PREV((dll_t *)el))

extern void	 dll_init	(dll_t *head);
extern void	 dll_insert_next(dll_t *head, dll_t *elem);
extern void	 dll_insert_prev(dll_t *head, dll_t *elem);
extern void	*dll_remove_next(dll_t *head);
extern void	*dll_remove_prev(dll_t *head);
extern void 	*dll_remove	(dll_t *elem);
extern dll_sz	 dll_count	(dll_t *head);

extern void	 dll_prn	(dll_t *dptr, FILE *out, dll_sz n);
extern void	 dll_dump	(dll_t *dptr, char *name, FILE *out, dll_sz n);

#ifdef  __cplusplus
}
#endif

#endif /* __DLL_H__ */
