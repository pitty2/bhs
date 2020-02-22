#include "_memdbg.h"

mcb_t	_mem_default = {
	{ NULL, NULL, NULL },
	"memory-default",
	0L, 0L, 0L,
	{
		&_mem_default.mcb_head,
		&_mem_default.mcb_head,
		&_mem_default.mcb_head
	},
	BHS_MUTEX_INIT
};

	void		*mem_default = &_mem_default;
static	bhs_mutex_t	 mem_gmutex = BHS_MUTEX_INIT;
static	dll_t		 mem_list = { &mem_list, &mem_list, &mem_list };


static mem_t *
mem_ctrl(
	void	*ptr
) {
	char	*p = ptr;
	mem_t	*mp = NULL;

	if(ptr == NULL) {
		PRNE(stderr, "ptr == NULL");
		goto _door;
	}

	mp = (mem_t*)(p - sizeof(mem_t));
	if(mp->mem_magic != MEM_MAGIC) {
		PRNW(stderr, "ptr %p: mp->mem_magic != MEM_MAGIC", ptr);
		mp = NULL;
	}
_door:
	return(mp);
}

static minfo_t *
mem_info(
	char	*file,
	size_t	 line,
	char	*func,
	char	*by
) {
	minfo_t	*ip;

	if((ip = calloc(1, sizeof(minfo_t))) == NULL) {
		PRNE(stderr, "ip = calloc(1, sizeof(info_t))) == NULL");
		goto _door;
	}
	if((ip->mi_fnam = strdup(file)) == NULL) {
		PRNE(stderr, "ip->mi_fnam = strdup(file)) == NULL");
	}
	ip->mi_line = line;
	if((ip->mi_func = strdup(func)) == NULL) {
		PRNE(stderr, "ip->mi_func = strdup(func)) == NULL");
	}
	if((ip->mi_allocfun = strdup(by)) == NULL) {
		PRNE(stderr, "ip->mi_allocfun = strdup(by)) == NULL");
	}
_door:
	return(ip);
}
	

static char *
mem_get(
	mcb_t	*mcb,
	char	*file, size_t	 line, char	*func,
	char	*by,
	size_t	 size
) {
	char	*p = NULL;
	size_t	 _size = size;
	mem_t	*mp;

	if(size == 0) {
		PRNW(stderr, "size == 0");
		goto _door;
	}
	if(mcb == NULL) {
		p = malloc(size);
		goto _door;
	}

	_size += sizeof(mem_t);
	if((p = malloc(_size)) == NULL) {
		goto _door;
	}
	mp = (mem_t*)p;
	memset(mp, 0, sizeof(mem_t));
	mp->mem_mcb = mcb;
	mp->mem_magic = MEM_MAGIC;
	mp->mem_size = size;
	if(mcb->mcb_dbglvl > 0) {
		mp->mem_info = mem_info(file, line, func, by);
	}

	bhs_mutex_lock(&mcb->mcb_mutex);

	mcb->mcb_curr += size;
	if(mcb->mcb_curr > mcb->mcb_max) {
		mcb->mcb_max = mcb->mcb_curr;
	}
	dll_insert_prev(&mcb->mcb_head, (dll_t*)mp);

	bhs_mutex_unlock(&mcb->mcb_mutex);

	p += sizeof(mem_t);

_door:
	return(p);
}

void *
mem_malloc(
	void	*m,
	char	*file, size_t	 line, char	*func,
	size_t	 size
) {
	void	*p;
	mcb_t	*mcb = m;

	if(m == NULL) {
		p = malloc(size);
		goto _door;
	}
	p = mem_get(mcb, file, line, func, "malloc", size);
_door:
	return(p);
}

void *
mem_calloc(
	void	*m,
	char	*file, size_t	 line, char	*func,
	size_t	 cnt,
	size_t	 size
) {
	void	*p;
	mcb_t	*mcb = m;

	if(m == NULL) {
		p = calloc(cnt, size);
		goto _door;
	}
	p = mem_get(mcb, file, line, func, "calloc", cnt * size);
	if(p != NULL) {
		memset(p, 0, cnt * size);
	}
_door:
	return(p);
}

void
mem_free(
	void	*ptr
) {
	mem_t	*mp;
	mcb_t	*mcb;
	size_t	 size;

	if((mp = mem_ctrl(ptr)) == NULL) {
#if (1)
		free(ptr);
#endif
		goto _door;
	}
	mcb = mp->mem_mcb;
	mp->mem_magic = 0L;

	bhs_mutex_lock(&mcb->mcb_mutex);

	mcb->mcb_curr -= mp->mem_size;
	dll_remove((dll_t*)mp);

	bhs_mutex_unlock(&mcb->mcb_mutex);

	size = mp->mem_size + sizeof(mem_t);

	if(mp->mem_info != NULL) {
		free(mp->mem_info->mi_fnam);
		free(mp->mem_info->mi_func);
		free(mp->mem_info->mi_allocfun);
		free(mp->mem_info);
	}
	memset(mp, 0, size);
	free(mp);
_door:
	return;
}

void *
mem_realloc(
	void	*m,
	char	*file, size_t	 line, char	*func,
	void	*ptr,
	size_t	 nsize
) {
	char	*oldp = ptr;
	char	*newp = NULL;
	size_t	 osize;
	mem_t	*mp;
	mcb_t	*mcb = m;

	if(nsize == 0) {
		mem_free(ptr);
	}
	if(m == NULL) {
		newp = realloc(ptr, nsize);
		goto _door;
	}
	if(ptr == NULL) {
		newp = mem_get(mcb, file, line, func, "realloc", nsize);
		goto _door;
	}
	if((mp = mem_ctrl(ptr)) == NULL) {
		newp = realloc(ptr, nsize);
		goto _door;
	}
	osize = mp->mem_size;
	newp = mem_get(mcb, file, line, func, "realloc", nsize);
	if(newp == NULL) {
		goto _door;
	}
	if(nsize < osize) {
		memcpy(newp, oldp, nsize);
	} else {
		memcpy(newp, oldp, osize);
	}
	mem_free(ptr);
_door:
	return(newp);
}

char *
mem_strdup(
	void	*m,
	char	*file, size_t	 line, char	*func,
	char	*s
) {
	char	*newp;
	size_t	 len;
	mcb_t	*mcb = m;

	if(s == NULL) {
		goto _door;
	}
	if(m == NULL) {
		newp = strdup(s);
		goto _door;
	}
	len = strlen(s) + 1;
	newp = mem_get(mcb, file, line, func, "strdup", len);
	if(newp != NULL) {
		memmove(newp, s, len);
	}
_door:
	return(newp);
}

void
mem_dlvl(
	void	*hdl,
	int	 dlvl
) {
	mcb_t	*mcb = hdl;

	mcb->mcb_dbglvl = dlvl;
}

void
mem_prn(
	void	*ptr,
	FILE	*out,
	size_t	 n
) {
	mem_t	*mp;


	PRNN(n); PRNO("ptr: 0x%p", ptr);
	if((mp = mem_ctrl(ptr)) != NULL) {
		PRNO(" size: %llu with %s[%llu].%s.%s\n",
			mp->mem_size,
			TSTR(mp->mem_info->mi_fnam),
			mp->mem_info->mi_line,
			TSTR(mp->mem_info->mi_func),
			TSTR(mp->mem_info->mi_allocfun)
		);
	}
}

void
mem_status(
	void	*hdl,
	FILE	*out
) {
	mcb_t	*mcb = hdl;

	mem_t	*mp;

	bhs_mutex_lock(&mcb->mcb_mutex);

	if(mcb->mcb_dbglvl > 0) {
		PRNO("\n----------------------------------------\n");
		PRNO("Statusreport for %s :\n", TSTR(mcb->mcb_name));
		PRNO("current size = %llu, maximum size = %llu\n",
			mcb->mcb_curr,
			mcb->mcb_max
		);
		PRNO("----------------------------------------\n");
		if(mcb->mcb_dbglvl > 1) {
			char	*p;
	
			DLL_WALK(mp, mem_t, &mcb->mcb_head) {
				p = (char *)mp;
				p += sizeof(mem_t);
				mem_prn(p, out, 0); PRNO("\n");
			}
			
			PRNO("----------------------------------------\n");
		}
	}
	bhs_mutex_unlock(&mcb->mcb_mutex);
}

void *
mem_init(
	char	*name,
	int	 dlvl,
	FILE	*log
) {
	mcb_t	*mcb;

	if((mcb = calloc(1, sizeof(mcb_t))) == NULL) {
		goto _door;
	}
	if((mcb->mcb_name = strdup(name)) == NULL) {
		free(mcb);
		mcb = NULL;
		goto _door;
	}
	mcb->mcb_dbglvl = dlvl;
	mcb->mcb_curr = 0L;
	mcb->mcb_max = 0L;
	dll_init(&mcb->mcb_head);

	bhs_mutex_init(&mcb->mcb_mutex, NULL);
	bhs_mutex_lock(&mem_gmutex);
	dll_insert_prev(&mem_list, (dll_t*)mcb);
	bhs_mutex_unlock(&mem_gmutex);
_door:
	return(mcb);
}

void
mem_clean(
	void	*hdl
) {
	mcb_t	*mcb = hdl;
	mem_t	*mp;

	bhs_mutex_lock(&mcb->mcb_mutex);

	while((mp = (mem_t*)DLL_FIRST(&mcb->mcb_head)) != NULL) {
		dll_remove((dll_t*)mp);
		if(mp->mem_info != NULL) {
			free(mp->mem_info->mi_fnam);
			free(mp->mem_info->mi_func);
			free(mp->mem_info->mi_allocfun);
			free(mp->mem_info);
		}
		mcb->mcb_curr -= mp->mem_size;
		free(mp);
	}
	bhs_mutex_unlock(&mcb->mcb_mutex);
}

