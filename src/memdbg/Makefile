include ../../make.inc/make.gbl
MODULE_NAME=libmemdbg
MODULE_CDEFS= -DMEMORY_DEBUGGER
# MODULE_LDFLG=	-G
ifeq ($(M_ARCH),armv7l)
	MODULE_LDFLG= -shared 
else
	MODULE_LDFLG= -shared
endif

INC_DIR=./ ../../include 
OBJ_DIR=./obj
LIB_DIR=../../lib

LFILES=
YFILES=

SRCS= \
	prn.c \
	compat.c \
	dll.c \
	memdbg.c 

INCS = \
	_memdbg.h 

include ../../make.inc/make.defs
LIBRARY=$(LIBDIR)/$(MODULE_NAME).so
ARCHIVE=$(LIBDIR)/$(MODULE_NAME).a

TARGET= ${OBJDIR} ${LIBDIR} ${ARCHIVE} ${LIBRARY}

all : ${TARGET}

include ../../make.inc/make.rules
-include $(DEPEND_FILE)

include ../../make.inc/make.cmds

