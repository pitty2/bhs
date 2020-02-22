#ifndef __BHS_LOCAL_MEM_H__
#define __BHS_LOCAL_MEM_H__
#include <bhs/mem.h>
#include <bhs/dll.h>
#include <bhs/compat.h>
#include <bhs/sess.h>

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

#endif /*__BHS_LOCAL_MEM_H__*/
