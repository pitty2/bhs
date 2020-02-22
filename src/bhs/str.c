#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <bhs/str.h>

char	*str_cause[] = {
	"Str okay",
	"Str element size!",
	"Str malloc() failed!",
	"Str systemcall failed!"
};

/*
	-------------------------------------------------------
 */
int
str_init(
	fncrc_t	*fncrc,
	str_t	*p,
	leng_t	 len,
	byte_t	*word
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, p);
	FNC_CHKINL(rc, len);
	FNC_CHKINP(rc, word);

	TRY(arr_init, _door, p, sizeof(byte_t), (len + 1));
	memmove(p->base, word, (p->elsz * len));
	p->next = p->base + (p->elsz * len);

_door:
	FNC_RETURN(rc);
}

int
str_new(
	fncrc_t	*fncrc,
	leng_t	 len,
	byte_t	*word,
	str_t	**pp
) {
	int	 rc = FNC_OKAY;
	str_t	*p = NULL;

	FNCRC_CHECK;
	FNC_CHKINL(rc, len);
	FNC_CHKINP(rc, word);
	FNC_CHKOUTP(rc, pp);

	if((p = (str_t *)CALLOC(1, sizeof(*p))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(STR_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, %llu) failed!", (leng_t)sizeof(*p));
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	TRY(str_init, _next1, p, len, word);

_next1:
	if(rc != FNC_OKAY) {
		FREE(p);
		p = NULL;
	}

_door:
	*pp = p;
	FNC_RETURN(rc);
}

int
str_fini(
	fncrc_t	*fncrc,
	str_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, p);

	rc = arr_fini(fncrc, p);
	FNC_RETURN(rc);
}

int
str_drop(
	fncrc_t	*fncrc,
	str_t	*p
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, p);

	rc = arr_drop(fncrc, p);
	FNC_RETURN(rc);
}

/*
	-------------------------------------------------------
 */
int
str_cpy(
	fncrc_t	*fncrc,
	str_t	*src,
	str_t	**pp
) {
	int	 rc = FNC_OKAY;
	str_t	*dst = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, src);
	FNC_CHKOUTP(rc, pp);

	rc = str_new(fncrc, STR_LEN(src), STR_WORD(src), &dst);
	*pp = dst;
	FNC_RETURN(rc);
}

int
str_n_cpy(
	fncrc_t	*fncrc,
	str_t	*src,
	leng_t	 n,
	str_t	**pp
) {
	int	 rc = FNC_OKAY;
	str_t	*dst;
	leng_t	 len = n;

	FNCRC_CHECK;
	FNC_CHKINP(rc, src);
	FNC_CHKINL(rc, n);
	FNC_CHKOUTP(rc, pp);

	if(len > STR_LEN(src)) {
		len = STR_LEN(src);
	}
	rc = str_new(fncrc, len, STR_WORD(src), &dst);
	*pp = dst;
	FNC_RETURN(rc);
}

int
str_cat(
	fncrc_t	*fncrc,
	str_t	*s1,
	str_t	*s2,
	str_t	**pp
) {
	int	 rc = FNC_OKAY;

	FNCRC_CHECK;
	FNC_CHKINP(rc, s1);
	FNC_CHKINP(rc, s2);
	FNC_CHKOUTP(rc, pp);

	if(ARR_AVAIL(s1) < (STR_LEN(s2) +1)) {
		TRY(arr_resize, _door, s1, (STR_LEN(s1) + STR_LEN(s2) +1));
	}
	memmove(s1->next, s2->base, STR_LEN(s2));
	s1->next += STR_LEN(s2);
	*(s1->next) = (byte_t)0;

_door:
	*pp = s1;
	FNC_RETURN(rc);
}

/*
	-------------------------------------------------------
 */
leng_t
str_len(str_t *s) {
	leng_t	 len = 0LL;

	len = s->next - s->base;
	return(len);
}

int
str_cmp(str_t *s1, str_t *s2) {
	int	rc = 0;
	byte_t	*p1, *p2;

	if(s1 != s2) {
		for(	p1 = STR_WORD(s1), p2 = STR_WORD(s2);
			(p1 < s1->next) && (p2 < s2->next);
			p1++, p2++)
		{
			if(*p1 != *p2) {
				rc = (*p1 > *p2) ? 1 : (-1);
				goto door;
			}
		}
		if(p1 < s1->next) {
			rc = 1;
		}
		if(p2 < s2->next) {
			rc = (-1);
		}
	}
door:
	return(rc);
}

int
str_n_cmp(str_t *s1, str_t *s2, leng_t n) {
	int	rc = 0;
	byte_t	*p1, *p2;
	leng_t	 cnt = 0L;

	if((s1 != s2) && (n > 0)) {
		leng_t	 l1 = STR_LEN(s1);
		leng_t	 l2 = STR_LEN(s2);

		if((l1 >= n) && (l2 >= n)) {
			for(	p1 = STR_WORD(s1), p2 = STR_WORD(s2), cnt = 0L;
				cnt < n;
				p1++, p2++, cnt++
			) {
				if(*p1 != *p2) {
					rc = (*p1 > *p2) ? 1 : (-1);
					goto door;
				}
			}
		} else {
			rc = (l1 > l2)? 1 : (-1);
		}
	}
door:
	return(rc);
}

/*
	----------------------------------------------------------------------------------
 */
void
str_prn(str_t *str, FILE *out, leng_t n) {
	byte_t	*p = NULL;

	if(str != NULL) {
		PRNN(n);
		for(p = str->base; p < str->next; p++) {
			if(isprint((int)*p)) {
				PRNO("%c", *p);
			} else {
				PRNO(".");
			}
		}
	}
}

void
str_prn_ascii(str_t *str, FILE *out, leng_t n) {
	byte_t	*p = NULL;

	if(str != NULL) {
		PRNN(n);
		for(p = str->base; p < str->next; p++) {
			if((*p <= Space) || (*p >= DEL)) {
				if(*p <= (byte_t)DEL) {
					PRNO("(%s[0x%x])", ascii_name_str[*p], *p);
				} else {
					PRNO("(0x%x)", *p);
				}
			} else {
				PRNO("%c", *p);
			}
		}
		PRNO("\n");
	}
}

void
str_dump(str_t *p, char *name, FILE *out, leng_t n) {

	PRNN(n); PRNO("(%p)(str_t *)[%llu] %s : {\n", p, (leng_t)sizeof(*p), TSTR(name));
	n++;
	if(p != NULL) {
		leng_t	 cnt = 0L;
		byte_t	*ptr = NULL;

		PRNN(n); PRNO("elsz : %lld\n", p->elsz);
		PRNN(n); PRNO("base : %p\n", p->base);
		PRNN(n); PRNO("next : %p\n", p->next);
		PRNN(n); PRNO("halt : %p\n", p->halt);

		PRNN(n); PRNO("len : %llu {\n", (leng_t)(p->next - p->base));
		n++;
		for(ptr = p->base, cnt = 1; ptr < p->next; ptr++, cnt++) {
			if(cnt == 1) {
				PRNN(n);
			}
			if(cnt == 5) {
				PRNO(" -  ");
			}
			if((*ptr < ' ') || (*ptr > 126)) {
				PRNO(". 0x%02x",  *ptr);
			} else {
				PRNO("%c 0x%02x", *ptr, *ptr);
			}
			if(cnt == 8) {
				PRNO("\n");
				cnt = 0;
			} else {
				PRNO(" ");
			}
		}
		if(cnt < 8) {
			PRNO("\n");
		}
		n--;
		PRNN(n); PRNO("}\n");
	} else {
		PRNN(n); PRNO("NULL\n");
	}
	n--;
	PRNN(n); PRNO("}\n");
}
