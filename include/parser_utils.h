#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <strings.h>
#include <dll.h>

#ifdef  __cplusplus
extern "C" {
#endif

enum p_token_e {
	PTOK_TERM = 1,
	PTOK_NONTERM
};
typedef enum p_token_e	ptok_e;

struct p_token_s {
	dll_t	 _dll;
	char	*name;
	ptok_e	 kind;
	union {
		char	*str;
		dll_t	 lst;
	} data_;
};
typedef struct p_token_s ptok_t;

#ifdef  __cplusplus
}
#endif

#endif /* __PARSER_H__ */
