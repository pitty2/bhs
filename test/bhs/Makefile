DIRS=common db aml
DATE_DIR=$(shell date "+%Y%m%d_%H%M%S")
SAVE_DIR=.ATTIC/$(DATE_DIR)

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

save:
	$(MAKE) clobber
	mkdir -p $(SAVE_DIR)
	cp -r src $(SAVE_DIR)
	cp -r test $(SAVE_DIR)
	cp -r test_data $(SAVE_DIR)
	cp -r test_comp $(SAVE_DIR)
	cp -r include $(SAVE_DIR)
	cp Makefile $(SAVE_DIR)
	cd .ATTIC; tar cf $(DATE_DIR).tar $(DATE_DIR); gzip $(DATE_DIR).tar; rm -r $(DATE_DIR)

