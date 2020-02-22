include ../../make.inc/make.gbl
MODULE_NAME=libdll
MODULE_CDEFS= -DMEMORY_DEBUGGER -DFNC_PARAM_CHK -DFNC_CALL_TRACE
# MODULE_LDFLG=	-G
ifeq ($(M_ARCH),armv7l)
	MODULE_LDFLG= -shared
else
	MODULE_LDFLG= -shared
endif
# MODULE_LDFLG=	-shared -Xlinker -z -Xlinker nodefs 
# 		-Xlinker -z -Xlinker initarray=bhs_post_load \
# 		-Xlinker -z -Xlinker finiarray=bhs_pre_unload

INC_DIR=../../include
OBJ_DIR=./obj
LIB_DIR=../../lib

LFILES=
YFILES=

SRCS= \
	dll.c 

INCS = 

include $(TOP)/make.inc/make.defs
LIBRARY=$(LIBDIR)/$(MODULE_NAME).so
ARCHIVE=$(LIBDIR)/$(MODULE_NAME).a

TARGET= ${OBJDIR} ${LIBDIR} ${ARCHIVE} ${LIBRARY}

all : ${TARGET}

include $(TOP)/make.inc/make.rules

-include $(DEPEND_FILE)

include $(TOP)/make.inc/make.cmds

