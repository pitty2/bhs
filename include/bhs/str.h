#ifndef __BHS_COMMON_STR_H__
#define __BHS_COMMON_STR_H__

#include <bhs/length.h>
#include <bhs/number.h>
#include <bhs/arr.h>
#include <bhs/fncrc.h>
#include <bhs/ascii.h>

#ifdef  __cplusplus
extern "C" {
#endif

extern	char	*str_cause[];
#define	STR_CAUSE_OKAY		str_cause[0]
#define	STR_CAUSE_ELEMENT_SIZE	str_cause[1]
#define	STR_CAUSE_NOMEM		str_cause[2]
#define	STR_CAUSE_SYSCALL	str_cause[3]

typedef arr_t		str_t;

#define	STR_WORD(str)	((str)->base)
#define	STR_LEN(str)	((str)->next - (str)->base)

#define	DECLARE_STR(name, string)					\
byte_t	name##_cstring[] = string;					\
str_t	name =	{							\
		sizeof(byte_t),						\
		(byte_t *)&name##_cstring,				\
		(byte_t *)&name##_cstring + sizeof(name##_cstring),	\
		(byte_t *)&name##_cstring + sizeof(name##_cstring)	\
	}

extern	int	 str_init	(fncrc_t *fncrc, str_t *p, leng_t len, byte_t *word);
extern	int	 str_new	(fncrc_t *fncrc, leng_t len, byte_t *word, str_t **pp);
extern	int	 str_fini	(fncrc_t *fncrc, str_t *p);
extern	int	 str_drop	(fncrc_t *fncrc, str_t *p);
extern	int	 str_cpy	(fncrc_t *fncrc, str_t *src, str_t **pp);
extern	int	 str_n_cpy	(fncrc_t *fncrc, str_t *src, leng_t n, str_t **pp);
extern	int	 str_cat	(fncrc_t *fncrc, str_t *s1, str_t *s2, str_t **pp);

extern	leng_t	 str_len(str_t *s);
extern	int	 str_cmp(str_t *s1, str_t *s2);
extern	int	 str_n_cmp(str_t *s1, str_t *s2, leng_t n);

extern	void	 str_prn(str_t *str, FILE *out, leng_t n);
extern	void	 str_dump(str_t *p, char *name, FILE *out, leng_t n);

#ifdef  __cplusplus
}
#endif

#endif /* __BHS_COMMON_STR_H__ */
