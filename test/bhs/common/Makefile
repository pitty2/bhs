include ../../../make.inc/make.gbl

INC_DIR=../../../include ../../../include/bhs
LIB_DIR=../../../lib
OBJ_DIR=./obj
BIN_DIR=../../../bin
LIBREFS= -lbhs -lpthread -ldl

SRCS= main.c

include $(TOP)/make.inc/make.defs

ARREFS= ${LIBDIR}/libbhs.a

MODULE_NAME=common_test
MODULE_CDEFS= -DMEMORY_DEBUGGER
ifeq ($(OS_ARCH),SunOS)
	MODULE_LDFLG=-L $(LIBDIR) -R $(LIBDIR)
endif
ifeq ($(OS_ARCH),Linux)
	MODULE_LDFLG=-L $(LIBDIR) -Xlinker -rpath $(LIBDIR)
endif
BIN=$(BINDIR)/$(MODULE_NAME)
# SBIN=$(BINDIR)/s$(MODULE_NAME)

TARGET= ${BIN} ${SBIN}
all : ${OBJDIR} ${BINDIR} ${TARGET}

-include $(DEPEND_FILE)

include $(TOP)/make.inc/make.rules
include $(TOP)/make.inc/make.cmds

