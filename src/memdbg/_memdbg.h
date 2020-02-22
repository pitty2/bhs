#ifndef __LOCAL_MEMDBG_H__
#define __LOCAL_MEMDBG_H__
#include <memdbg.h>
#define _GNU_SOURCE
#include <pthread.h>


#ifdef  __cplusplus
extern "C" {
#endif

#define TSTR(p)	((p == NULL) ? "{nil}" : p)
#define TVAL(p)	((p == NULL) ? "{0}" : p)


int	 prn_ferror(FILE *fd);

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

#define PRNW(out, ...)					\
if (prn_ferror(out) == 0 ) {				\
	fprintf (out, "WARNING ");			\
	fprintf (out, "%-15s (+%-4d) ) %15s : ", __FILE__, __LINE__,__func__);	\
	fprintf (out, __VA_ARGS__);			\
	fprintf (out, "\n");				\
	fflush(out);					\
} 

#define PRNE(out, ...)					\
if (prn_ferror(out) == 0 ) {				\
	fprintf (out, "+++ERROR+++ ");			\
	fprintf (out, "%-15s (+%-4d) %15s : ", __FILE__, __LINE__,__func__);	\
	fprintf (out, __VA_ARGS__);			\
	fprintf (out, "\n");				\
	fflush(out);					\
} 

#define PABORT(...)					\
if (prn_ferror(stderr) == 0 ) {				\
	fprintf (stderr, "+++ERROR+++ in ");		\
	fprintf (stderr, "%s +%d ", __FILE__, __LINE__);\
	fprintf (stderr, "%s : ", __func__);		\
	fprintf (stderr, __VA_ARGS__);			\
	fprintf (stderr, "\n");				\
	fflush(stderr);					\
}							\
abort()

/*
	Specification of "doubly linked Lists"
 */
typedef enum bool_enum {
	FALSE = 0,
	TRUE
} bool_e;

typedef unsigned long long int	 leng_t;

typedef bool_e bool_t;

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
extern leng_t	 dll_count	(dll_t *head);

extern void	 dll_prn	(dll_t *dptr, FILE *out, leng_t n);
extern void	 dll_dump	(dll_t *dptr, char *name, FILE *out, leng_t n);

#define	BHS_MUTEX_INIT	PTHREAD_MUTEX_INITIALIZER
typedef	pthread_mutex_t		bhs_mutex_t;
#define bhs_mutex_lock(mx)	pthread_mutex_lock(mx)
#define bhs_mutex_unlock(mx)	pthread_mutex_unlock(mx)
#define	bhs_mutex_destroy(mx)	pthread_mutex_destroy(mx)

extern	void	 bhs_mutex_init(bhs_mutex_t *mx, void *arg);
extern	void	 bhs_fprintf(FILE *out, char *format, ...);

#ifdef _LP64
#define MEM_MAGIC 0x0123456789abcdef
#else
#define MEM_MAGIC 0x89abcdef
#endif

typedef struct memory_control_block_s {
	dll_t	 _lst;

	char		*mcb_name;
	int		 mcb_dbglvl;
	leng_t		 mcb_curr;
	leng_t		 mcb_max;
	dll_t		 mcb_head;
	bhs_mutex_t	 mcb_mutex;
} mcb_t;

typedef struct memory_info_s {
	char	*mi_fnam;
	leng_t	 mi_line;
	char	*mi_func;
	char	*mi_allocfun;
} minfo_t;

typedef struct memory_s {
	dll_t	 _lst;

	mcb_t		*mem_mcb;
	leng_t		 mem_magic;
	leng_t		 mem_size;
	bhs_mutex_t	 mcb_mutex;
	minfo_t		*mem_info;
} mem_t;

extern mcb_t	_mem_default;

void	mem_setup(void);
void	mem_cleanup(void);

#ifdef  __cplusplus
}
#endif

#endif /*__LOCAL_MEMDBG_H__*/
