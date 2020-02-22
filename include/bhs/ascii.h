#ifndef __BHS_COMMON_ASCII_H__
#define __BHS_COMMON_ASCII_H__

#include <stdio.h>
#include <bhs/bool.h>
#include <bhs/length.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum ascii_enum {
	NUL	= 0,
	SOH,
	STX,
	ETX,
	EOT,
	ENQ,
	ACK,
	BEL,

	BS,
	HT,
	NL,
	VT,
	NP,
	CR,
	SO,
	SI,

	DLE,
	DC1,
	DC2,
	DC3,
	DC4,
	NAK,
	SYN,
	ETB,

	CAN,
	EM,
	SUB,
	ESC,
	FS,
	GS,
	RS,
	US,

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
} ascii_e;

typedef unsigned char	 ascii_t;

extern	ascii_t		 ascii_list[];
extern	char		*ascii_name_str[];

extern	bool_t		 ascii_is_digit(ascii_t c);
extern	bool_t		 ascii_is_hexdigit(ascii_t c);
extern	bool_t		 ascii_is_upper(ascii_t c);
extern	bool_t		 ascii_is_lower(ascii_t c);
extern	bool_t		 ascii_is_alpha(ascii_t c);
extern	bool_t		 ascii_is_alnum(ascii_t c);
extern	bool_t		 ascii_is_blank(ascii_t c);
extern	bool_t		 ascii_is_space(ascii_t c);
extern	bool_t		 ascii_is_newline(ascii_t c);
extern	bool_t		 ascii_is_ctrl(ascii_t c);
extern	bool_t		 ascii_is_graph(ascii_t c);
extern	bool_t		 ascii_is_punct(ascii_t c);
extern	bool_t		 ascii_is_ascii(int c);
extern	void		 ascii_dump(ascii_t c, FILE *out, leng_t n);

#ifdef  __cplusplus
}
#endif

#endif /*___BHS_COMMON_ASCII_H___*/
