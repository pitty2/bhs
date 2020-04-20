include ../../../make.inc/make.gbl

INC_DIR=../../../include ../../../include/bhs
LIB_DIR=../../../lib
OBJ_DIR=./obj
BIN_DIR=../../../bin
LIBREFS= -lbhs -lpthread -ldl

SRCS= \
	main.c 

include ../../../make.inc/make.defs
ARREFS= ${LIBDIR}/libbhs.a
ARREFS+= /usr/lib/x86_64-linux-gnu/libdl.a

MODULE_NAME=aml_test
MODULE_CDEFS= -DMEMORY_DEBUGGER -DFNC_PARAM_CHK
ifeq ($(OS_ARCH),SunOS)
	MODULE_LDFLG=-L $(LIBDIR) -R $(LIBDIR)
	LIBREFS= -lbhs -lpthread
endif
ifeq ($(OS_ARCH),Linux)
	MODULE_LDFLG=-L $(LIBDIR) -Xlinker -rpath $(LIBDIR)
	LIBREFS= -lbhs -lpthread -ldl
endif
BIN=$(BINDIR)/$(MODULE_NAME)
# SBIN=$(BINDIR)/s$(MODULE_NAME)

TARGET= ${BIN} ${SBIN}
all : ${OBJDIR} ${BINDIR} ${TARGET}

-include $(DEPEND_FILE)

include ../../../make.inc/make.rules
include ../../../make.inc/make.cmds

