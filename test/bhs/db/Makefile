include ../../../make.inc/make.gbl

INC_DIR=../../../include ../../../include/bhs 
LIB_DIR=../../../lib
OBJ_DIR=./obj
BIN_DIR=../../../bin
STD_LIBREFS= -lpthread -ldl
LIBREFS= -lbhs ${STD_LIBREFS}

SRCS= \
	main.c 

include ../../../make.inc/make.defs
ARREFS= ${LIBDIR}/libbhs.a

MODULE_NAME=db_test
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

include ../../../make.inc/make.rules
include ../../../make.inc/make.cmds

