DIRS=bhs

all:
	for d in ${DIRS} ; do \
		${MAKE} -C $${d} -e BITS=64 ; \
	done

dep:
	for d in ${DIRS} ; do \
		${MAKE} -C $${d} -e BITS=64 $@ ; \
	done

tag:
	for d in ${DIRS} ; do \
		${MAKE} -C $${d} $@ ; \
	done

clobber:
	for d in ${DIRS} ; do \
		${MAKE} -C $${d} -e BITS=64 $@ ; \
	done

