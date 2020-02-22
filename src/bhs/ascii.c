#include <bhs/prn.h>
#include <bhs/ascii.h>

ascii_t	 ascii_list[] = {
	NUL, SOH, STX, ETX, EOT, ENQ, ACK, BEL,
	BS,  HT,  NL,  VT,  NP,  CR,  SO,  SI,
	DLE, DC1, DC2, DC3, DC4, NAK, SYN, ETB,
	CAN, EM,  SUB, ESC, FS,  GS,  RS,  US,

	Space,
	Exclamation_mark,
	Quotation_mark,
	Number_sign,
	Dollar_sign,
	Percent_sign,
	Apmersand,
	Apostrophe,

	Left_parenthesis,
	Right_parenthesis,
	Asterisk,
	Plus_sign,
	Comma,
	Hyphen_Minus,
	Full_stop,
	Solidus,

	Digit_zero,
	Digit_one,
	Digit_two,
	Digit_three,
	Digit_four,
	Digit_five,
	Digit_six,
	Digit_seven,

	Digit_eight,
	Digit_nine,
	Colon,
	Semicolon,
	Less_than_sign,
	Equals_sign,
	Greater_than_sign,
	Question_mark,

	Commercial_at,
	Latin_capital_letter_A,
	Latin_capital_letter_B,
	Latin_capital_letter_C,
	Latin_capital_letter_D,
	Latin_capital_letter_E,
	Latin_capital_letter_F,
	Latin_capital_letter_G,

	Latin_capital_letter_H,
	Latin_capital_letter_I,
	Latin_capital_letter_J,
	Latin_capital_letter_K,
	Latin_capital_letter_L,
	Latin_capital_letter_M,
	Latin_capital_letter_N,
	Latin_capital_letter_O,

	Latin_capital_letter_P,
	Latin_capital_letter_Q,
	Latin_capital_letter_R,
	Latin_capital_letter_S,
	Latin_capital_letter_T,
	Latin_capital_letter_U,
	Latin_capital_letter_V,
	Latin_capital_letter_W,

	Latin_capital_letter_X,
	Latin_capital_letter_Y,
	Latin_capital_letter_Z,
	Left_square_bracket,
	Reverse_solidus,
	Right_square_bracket,
	Circumflex_accent,
	Low_line,

	Grave_accent,
	Latin_small_letter_a,
	Latin_small_letter_b,
	Latin_small_letter_c,
	Latin_small_letter_d,
	Latin_small_letter_e,
	Latin_small_letter_f,
	Latin_small_letter_g,

	Latin_small_letter_h,
	Latin_small_letter_i,
	Latin_small_letter_j,
	Latin_small_letter_k,
	Latin_small_letter_l,
	Latin_small_letter_m,
	Latin_small_letter_n,
	Latin_small_letter_o,

	Latin_small_letter_p,
	Latin_small_letter_q,
	Latin_small_letter_r,
	Latin_small_letter_s,
	Latin_small_letter_t,
	Latin_small_letter_u,
	Latin_small_letter_v,
	Latin_small_letter_w,

	Latin_small_letter_x,
	Latin_small_letter_y,
	Latin_small_letter_z,
	Left_curly_bracket,
	Vertical_line,
	Right_curly_bracket,
	Tilde,
	DEL
};

char	*ascii_name_str[] = {
	 "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
	 "BS",  "HT",  "NL",  "VT",  "NP",  "CR",  "SO",  "SI",
	 "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
	 "CAN", "EM",  "SUB", "ESC", "FS",  "GS",  "RS",  "US",

	 "Space",
	 "Exclamation mark",
	 "Quotation mark",
	 "Number sign",
	 "Dollar sign",
	 "Percent sign",
	 "Apmersand",
	 "Apostrophe",

	 "Left parenthesis",
	 "Right parenthesis",
	 "Asterisk",
	 "Plus sign",
	 "Comma",
	 "Hyphen-Minus",
	 "Full stop",
	 "Solidus",

	 "Digit zero",
	 "Digit one",
	 "Digit two",
	 "Digit three",
	 "Digit four",
	 "Digit five",
	 "Digit six",
	 "Digit seven",

	 "Digit eight",
	 "Digit nine",
	 "Colon",
	 "Semicolon",
	 "Less-than sign",
	 "Equals sign",
	 "Greater-than sign",
	 "Question mark",

	 "Commercial at",
	 "Latin capital letter A",
	 "Latin capital letter B",
	 "Latin capital letter C",
	 "Latin capital letter D",
	 "Latin capital letter E",
	 "Latin capital letter F",
	 "Latin capital letter G",

	 "Latin capital letter H",
	 "Latin capital letter I",
	 "Latin capital letter J",
	 "Latin capital letter K",
	 "Latin capital letter L",
	 "Latin capital letter M",
	 "Latin capital letter N",
	 "Latin capital letter O",

	 "Latin capital letter P",
	 "Latin capital letter Q",
	 "Latin capital letter R",
	 "Latin capital letter S",
	 "Latin capital letter T",
	 "Latin capital letter U",
	 "Latin capital letter V",
	 "Latin capital letter W",

	 "Latin capital letter X",
	 "Latin capital letter Y",
	 "Latin capital letter Z",
	 "Left square bracket",
	 "Reverse solidus",
	 "Right square bracket",
	 "Circumflex accent",
	 "Low line",

	 "Grave accent",
	 "Latin small letter a",
	 "Latin small letter b",
	 "Latin small letter c",
	 "Latin small letter d",
	 "Latin small letter e",
	 "Latin small letter f",
	 "Latin small letter g",

	 "Latin small letter h",
	 "Latin small letter i",
	 "Latin small letter j",
	 "Latin small letter k",
	 "Latin small letter l",
	 "Latin small letter m",
	 "Latin small letter n",
	 "Latin small letter o",

	 "Latin small letter p",
	 "Latin small letter q",
	 "Latin small letter r",
	 "Latin small letter s",
	 "Latin small letter t",
	 "Latin small letter u",
	 "Latin small letter v",
	 "Latin small letter w",

	 "Latin small letter x",
	 "Latin small letter y",
	 "Latin small letter z",
	 "Left curly bracket",
	 "Vertical line",
	 "Right curly bracket",
	 "Tilde",
	 "DEL"
};

leng_t ascii_min = NUL;
leng_t ascii_max = DEL;

leng_t ascii_first = NUL;
leng_t ascii_last = DEL;

leng_t ascii_count = sizeof(ascii_list);
leng_t ascii_size = 7;

bool_t
ascii_is_digit(ascii_t c) {
	bool_t	 rc = FALSE;

	if((c >= Digit_zero) && (c <= Digit_nine)) {
		rc = TRUE;
	}
	return(rc);
}

bool_t
ascii_is_hexdigit(ascii_t c) {
	bool_t	 rc = FALSE;

	if((rc = ascii_is_digit(c)) != TRUE) {
		if((c >= Latin_capital_letter_A) && (c <= Latin_capital_letter_F)) {
			rc = TRUE;
		} else if((c >= Latin_small_letter_a) && (c <= Latin_small_letter_f)) {
			rc = TRUE;
		}
	}
	return(rc);
}

bool_t
ascii_is_upper(ascii_t c) {
	bool_t	 rc = FALSE;

	if((c >= Latin_capital_letter_A) && (c <= Latin_capital_letter_Y)) {
		rc = TRUE;
	}
	return(rc);
}

bool_t
ascii_is_lower(ascii_t c) {
	bool_t	 rc = FALSE;

	if((c >= Latin_small_letter_a) && (c <= Latin_small_letter_y)) {
		rc = TRUE;
	}
	return(rc);
}

bool_t
ascii_is_alpha(ascii_t c) {
	bool_t	 rc = FALSE;

	if((rc = ascii_is_upper(c)) != TRUE) {
		rc = ascii_is_lower(c);
	}
	return(rc);
}

bool_t
ascii_is_alnum(ascii_t c) {
	bool_t	 rc = FALSE;

	if((rc = ascii_is_alpha(c)) != TRUE) {
		rc = ascii_is_digit(c);
	}
	return(rc);
}

bool_t
ascii_is_blank(ascii_t c) {
	bool_t	 rc = FALSE;

	if((c == Space) || (c == HT)) {
		rc = TRUE;
	}
	return(rc);
}

bool_t
ascii_is_space(ascii_t c) {
	bool_t	 rc = FALSE;

	if((rc = ascii_is_blank(c)) != TRUE) {
		if((c == CR) || (c == NL) || (c == BS) || (c == VT)) {
			rc = TRUE;
		}
	}
	return(rc);
}

bool_t
ascii_is_newline(ascii_t c) {
	bool_t	 rc = FALSE;

	if(c == NL) {
		rc = TRUE;
	}
	return(rc);
}

bool_t
ascii_is_ctrl(ascii_t c) {
	bool_t	 rc = FALSE;

	if(c < Space) {
		rc = TRUE;
	}
	return(rc);
}

bool_t
ascii_is_graph(ascii_t c) {
	bool_t	 rc = FALSE;

	if((c > Space) && (c < DEL)) {
		rc = TRUE;
	}
	return(rc);
}

bool_t
ascii_is_punct(ascii_t c) {
	bool_t	 rc = FALSE;

	if((c >= Exclamation_mark) && (c <= Solidus)) {
		rc = TRUE;
	} else if((c >= Colon) && (c <= Commercial_at)) {
		rc = TRUE;
	} else if((c >= Left_square_bracket) && (c <= Grave_accent)) {
		rc = TRUE;
	} else if((c >= Left_curly_bracket) && (c <= Tilde)) {
		rc = TRUE;
	}
	return(rc);
}

bool_t
ascii_is_ascii(int c) {
	bool_t	 rc = FALSE;

	if((c >= NUL) && (c <= DEL)) {
		rc = TRUE;
	}
	return(rc);
}

void
ascii_dump(
	ascii_t	 c,
	FILE	*out,
	leng_t	 n
) {
	PRNN(n); PRNO("'%c', [0x%x], <%s>\n", c, c, ascii_name_str[c]);
}

