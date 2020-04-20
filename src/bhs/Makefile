include ../../make.inc/make.gbl
MODULE_NAME=libbhs
MODULE_CDEFS= -DMEMORY_DEBUGGER -DFNC_PARAM_CHK -DFNC_CALL_TRACE
# MODULE_LDFLG=	-G
ifeq ($(M_ARCH),armv7l)
	MODULE_LDFLG= -shared -Wl,-init=bhs_post_load -Wl,-fini=bhs_pre_unload
else
	MODULE_LDFLG=	-shared 
endif

INC_DIR=../../include ../../include/bhs 
OBJ_DIR=./obj
LIB_DIR=../../lib
#DYN_LIBS=-ldl

LFILES=aml_scanner.l 
YFILES=aml_parser.y 

SRCS= \
	arr.c \
	ascii.c \
	bool.c \
	byte.c \
	compat.c \
	dbg.c \
	dll.c \
	file.c \
	fncrc.c \
	length.c \
	mem.c \
	number.c \
	prn.c \
	sess.c \
	str.c \
	conv.c \
\
	comp_builtins.c \
	compile.c \
	comp_error.c \
	comp_get.c \
	comp_preproc.c \
	comp_resolve.c \
	comp_utils.c \
\
	data_assoc.c \
	data_bool.c \
	data.c \
	data_circ.c \
	data_codes.c \
	data_conv.c \
	data_designator.c \
	data_file.c \
	data_function.c \
	data_get.c \
	data_ident.c \
	data_link.c \
	data_list.c \
	data_list_utils.c \
	data_number.c \
	data_sequ.c \
	data_stack.c \
	data_string.c \
	data_string_utils.c \
\
	aml.c \
	aml_addr_fnc.c \
	aml_and_fnc.c \
	aml_deref_fnc.c \
	aml_div_fnc.c \
	aml_eq_fnc.c \
	aml_error.c \
	aml_eval.c \
	aml_ge_fnc.c \
	aml_gt_fnc.c \
	aml_le_fnc.c \
	aml_logical_and_fnc.c \
	aml_logical_not_fnc.c \
	aml_logical_or_fnc.c \
	aml_lt_fnc.c \
	aml_minus_fnc.c \
	aml_mod_fnc.c \
	aml_mul_fnc.c \
	aml_ne_fnc.c \
	aml_not_fnc.c \
	aml_or_fnc.c \
	aml_plus_fnc.c \
	aml_ptr_fnc.c \
	aml_shl_fnc.c \
	aml_shr_fnc.c \
	aml_ternary_fnc.c \
	aml_unary_minus_fnc.c \
	aml_unary_plus_fnc.c \
	aml_xor_fnc.c \
\
	bhs.c

INCS = \
	_mem.h \
	aml.h \
	bnf.h

include ../../make.inc/make.defs
LIBRARY=$(LIBDIR)/$(MODULE_NAME).so
ARCHIVE=$(LIBDIR)/$(MODULE_NAME).a

TARGET= ${OBJDIR} ${LIBDIR} ${ARCHIVE} ${LIBRARY}

all : ${TARGET}

include ../../make.inc/make.rules

aml_parser.h : aml_parser.y aml.h ../../include/bhs/fncrc.h ../../include/bhs/compile.h
	${YACC} aml_parser.y

aml_parser.c : aml_parser.y aml.h ../../include/bhs/fncrc.h ../../include/bhs/compile.h
	${YACC} aml_parser.y

aml_scanner.c : aml_scanner.l aml_parser.h aml.h ../../include/bhs/fncrc.h ../../include/bhs/compile.h
	${LEX} aml_scanner.l

-include $(DEPEND_FILE)

include ../../make.inc/make.cmds

