#ifndef __BHS_DATA_CONF_H__
#define __BHS_DATA_CONF_H__
#include <bhs/conv.h>
#include <bhs/data.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern	char	*data_conv_cause[];
#define	DATA_CONV_CAUSE_OKAY	data_conv_cause[0]
#define	DATA_CONV_CAUSE_ERROR	data_conv_cause[1]
#define	DATA_CONV_CAUSE_NOMEM	data_conv_cause[2]
#define	DATA_CONV_CAUSE_SYSCALL	data_conv_cause[3]

int	data_num_2data_str(
		fncrc_t		*rncrc,
		sess_t		*sp,
		data_t		*src,
		data_t		**pp
	);
int	data_str_2data_num(
		fncrc_t		*rncrc,
		sess_t		*sp,
		data_t		*src,
		leng_t		 base,
		data_t		**pp
	);
int	data_str_2cstr(
		fncrc_t		*rncrc,
		sess_t		*sp,
		data_t		*src,
		char		**pp
	);


#ifdef  __cplusplus
}
#endif

#endif  /* __BHS_DATA_CONF_H__ */
