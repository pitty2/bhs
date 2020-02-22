#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>
#include <bhs/fncrc.h>
#include <bhs/sess.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern	char	*file_cause[];
#define	FILE_CAUSE_OKAY		 file_cause[0]
#define	FILE_CAUSE_MODE		 file_cause[1]
#define	FILE_CAUSE_NOMEM	 file_cause[2]
#define	FILE_CAUSE_SYSCALL	 file_cause[3]

struct file_descriptor_s {
	char	*fd_name;
	FILE	*fd_hdl;
	int	 fd_dsc;	/* Filedescriptor */
	char	*fd_addr;	/* mmap-address */
	char	*fd_mode;
	int	 fd_lineno;
	void	*fd_buffer;
};
typedef struct file_descriptor_s fdes_t;

extern	int	fdes_init(
			fncrc_t	*fncrc,
			char	*name,
			fdes_t	*fp
			);
extern	int	fdes_new(
			fncrc_t	*fncrc,
			char	*name,
			fdes_t	**pp
			);
extern	int	fdes_fini(
			fncrc_t	*fncrc,
			fdes_t	*fp
			);
extern	int	fdes_drop(
			fncrc_t	*fncrc,
			fdes_t	*fp
			);
extern	int	fdes_cpy(
			fncrc_t	*fncrc,
			fdes_t	*fp,
			fdes_t	**pp
			);
extern	int	fdes_cmp(
			fncrc_t	*fncrc,
			fdes_t	*fp1,
			fdes_t	*fp2,
			int	*code
			);
extern	int	fdes_prn(
			fncrc_t	*fncrc,
			fdes_t	*fp,
			FILE	*out,
			leng_t	 n
			);
extern	int	fdes_open(
			fncrc_t	*fncrc,
			char	*mode,
			fdes_t	*fp
			);
extern	int	fdes_close(
			fncrc_t	*fncrc,
			fdes_t	*fp
			);
extern	int	fdes_set_buffer(
			fncrc_t	*fncrc,
			void	*buffer,
			fdes_t	*fp
			);

#ifdef  __cplusplus
}
#endif

#endif /* __FILE_H__ */
