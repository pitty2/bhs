#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <bhs/conv.h>

char	*conv_cause[] = {
	"Conv okay",
	"Conv conversion error",
	"Conv malloc() failed!",
	"Conv systemcall failed!"
};

/*
	----------------------------------------------------------------------------------
 */
int
cstr_2str(
	fncrc_t	*fncrc,
	char	*cstr,
	str_t	**pp
) {
	int	 rc = FNC_OKAY;
	str_t	*str = NULL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, cstr);
	FNC_CHKOUTP(rc, pp);

	TRY(str_new, _door, strlen(cstr), (byte_t *)cstr, &str);
	*pp = str;

_door:
	FNC_RETURN(rc);
}

/*
	----------------------------------------------------------------------------------
 */
int
ascii_2cint(
	ascii_t	c
) {
	int	 rc = (-1);

	switch(c) {
	case Digit_zero :
		rc = 0;
		break;
	case Digit_one :
		rc = 1;
		break;
	case Digit_two :
		rc = 2;
		break;
	case Digit_three :
		rc = 3;
		break;
	case Digit_four :
		rc = 4;
		break;
	case Digit_five :
		rc = 5;
		break;
	case Digit_six :
		rc = 6;
		break;
	case Digit_seven :
		rc = 7;
		break;
	case Digit_eight :
		rc = 8;
		break;
	case Digit_nine :
		rc = 9;
		break;
	case Latin_capital_letter_A :
	case Latin_small_letter_a :
		rc = 10;
		break;
	case Latin_capital_letter_B :
	case Latin_small_letter_b :
		rc = 11;
		break;
	case Latin_capital_letter_C :
	case Latin_small_letter_c :
		rc = 12;
		break;
	case Latin_capital_letter_D :
	case Latin_small_letter_d :
		rc = 13;
		break;
	case Latin_capital_letter_E :
	case Latin_small_letter_e :
		rc = 14;
		break;
	case Latin_capital_letter_F :
	case Latin_small_letter_f :
		rc = 15;
		break;
	default:
		rc = (-1);
	}
	return(rc);
}


/*
	----------------------------------------------------------------------------------
 */
int
str_2cstr(
	fncrc_t	*fncrc,
	str_t	*p,
	char	**pp
) {
	int	 rc = FNC_OKAY;
	char	*s = NULL;
	leng_t	 len = 0LL;

	FNCRC_CHECK;
	FNC_CHKINP(rc, p);
	FNC_CHKOUTP(rc, pp);

	len = STR_LEN(p);
	if((s = CALLOC(1, (len + 1))) == NULL) {
		rc = FNC_FATAL;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(CONV_CAUSE_NOMEM);
		FNCRC_SYSERR(errno);
		FNC_TXT(fncrc, "CALLOC(1, STR_LEN(p) + 1) failed!");
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}
	memmove(s, p->base, len);
_door:
	*pp = s;
	FNC_RETURN(rc);
}

/*
	----------------------------------------------------------------------------------
 */
int
str_2num(
	fncrc_t		*fncrc,
	str_t		*str,
	leng_t		 base,
	number_t	*pp
) {
	int		 rc = FNC_OKAY;
	number_t	 value = 0LL;
	byte_t		*top = NULL;
	byte_t		*end = NULL;
	byte_t		*np = NULL;
	ascii_t		 c = NUL;
	number_t	 part, m;

	FNCRC_CHECK;
	FNC_CHKINP(rc, str);
	FNC_CHKOUTP(rc, pp);

	if(	(base != 2)
	&&	(base != 8)
	&&	(base != 10)
	&&	(base != 16)
	) {
		rc = FNC_ERROR;
		FNCRC_RESULT(rc);
		FNCRC_CAUSE(CONV_CAUSE_ERROR);
		FNC_TXT(fncrc, "Wrong base (%lld) given!", base);
		fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
		goto _door;
	}

	top = STR_WORD(str);
	end = ARR_LAST(str);
	m = 1;
	for(np = end; np >= top; np--) {
		c = (ascii_t)*np;
		if((part = ascii_2cint(c)) == (-1)) {
			rc = FNC_ERROR;
			FNCRC_RESULT(rc);
			FNCRC_CAUSE(CONV_CAUSE_ERROR);
			FNC_TXT(fncrc, "ascii_2cint(%s) failed!",
				TSTR(ascii_name_str[c]));
			fncrc->fnc_stack_cnt = bhs_backtrace(fncrc->fnc_stack, FNC_STACK_SIZE);
			goto _door;
		}
		value += part * m;
		m = m * (number_t)base;
	}
_door:
	*pp = value;
	FNC_RETURN(rc);
}

