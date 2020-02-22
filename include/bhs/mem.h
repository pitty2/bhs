#ifndef __KSE_COMMON_MEM_H__
#define __KSE_COMMON_MEM_H__

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bhs/error.h>
#include <bhs/length.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef MEMORY_DEBUGGER
#define MALLOC(sz)				\
	mem_malloc(				\
		mem_default,			\
		__FILE__,			\
		__LINE__,			\
		(char*)__func__,		\
		sz)

#define CALLOC(esz, sz)				\
	mem_calloc(				\
		mem_default,			\
		__FILE__,			\
		__LINE__,			\
		(char*)__func__,		\
		esz,				\
		sz)

#define REALLOC(ptr, sz)			\
	mem_realloc(				\
		mem_default,			\
		__FILE__,			\
		__LINE__,			\
		(char*)__func__,		\
		ptr,				\
		sz)

#define STRDUP(str)				\
	mem_strdup(				\
		mem_default,			\
		__FILE__,			\
		__LINE__,			\
		(char*)__func__,		\
		str)

#define FREE(p)		mem_free(p)
#define FREE_FNC	mem_free

#else
#define MALLOC(sz)		malloc(sz)
#define CALLOC(esz, sz)		calloc(esz, sz)
#define REALLOC(ptr, sz)	realloc(ptr, sz)
#define STRDUP(str)		strdup(str)
#define FREE(p)			free(p)
#define FREE_FNC		free

#endif /* MEMORY_DEBUGGER */

#define MEM_STAT(out)	mem_status(mem_default, out)
#define MEM_CLEAN()	mem_clean(mem_default)
#define MEM_DLVL(n)	mem_dlvl(mem_default, n)

extern	void	*mem_default;

extern void	*mem_malloc(
			void	*hdl,
			char	*file, leng_t	 line, char	*func,
			leng_t	 size
		);
extern void	*mem_calloc(
			void	*hdl,
			char	*file, leng_t	 line, char	*func,
			leng_t	 cnt,
			leng_t	 size
		);
extern void	*mem_realloc(
			void	*hdl,
			char	*file, leng_t	 line, char	*func,
			void	*ptr,
			leng_t	 size
		);
extern char	*mem_strdup(
			void	*hdl,
			char	*file, leng_t	 line, char	*func,
			char	*str
		);
extern void	 mem_free(
			void	*ptr
		);


extern void	 mem_dlvl(
			void	*hdl,
			int	 dlvl
		);
extern void	 mem_prn(
			void	*hdl,
			FILE	*out,
			leng_t	 n
		);
extern void	 mem_status(
			void	*hdl,
			FILE	*out
		);
extern void	*mem_init(
			char	*name,
			int	 dlvl,
			FILE	*log
		);
extern void	 mem_clean(
			void	*hdl
		);

#ifdef  __cplusplus
}
#endif

#endif /*__KSE_COMMON_MEM_H__*/

