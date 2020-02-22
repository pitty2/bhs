#ifndef __BHS_CONF_H__
#define __BHS_CONF_H__
#include <bhs/fncrc.h>
#include <bhs/bool.h>
#include <bhs/byte.h>
#include <bhs/ascii.h>
#include <bhs/number.h>
#include <bhs/str.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern	char	*conv_cause[];
#define	CONV_CAUSE_OKAY		conv_cause[0]
#define	CONV_CAUSE_ERROR	conv_cause[1]
#define	CONV_CAUSE_NOMEM	conv_cause[2]
#define	CONV_CAUSE_SYSCALL	conv_cause[3]

extern	int	 ascii_2cint	(ascii_t c);
extern	int	 cstr_2str	(fncrc_t *fncrc, char *cstr, str_t **pp);
extern	int	 str_2cstr	(fncrc_t *fncrc, str_t *p, char **pp);
extern	int	 str_2num	(fncrc_t *fncrc, str_t *str, leng_t base, number_t *pp);

#ifdef  __cplusplus
}
#endif

#endif  /* __BHS_CONF_H__ */
