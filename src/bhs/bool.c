#include <bhs/prn.h>
#include <bhs/length.h>
#include <bhs/bool.h>

bool_t	 bool_list[] = {
	FALSE,
	TRUE
};

char	*bool_name_str[] = {
	"FALSE",
	"TRUE"
};

leng_t	bool_min = FALSE;
leng_t	bool_max = TRUE;

leng_t	bool_first = FALSE;
leng_t	bool_last = TRUE;

leng_t	bool_count = sizeof(bool_list);
leng_t	bool_size = sizeof(bool_t);
