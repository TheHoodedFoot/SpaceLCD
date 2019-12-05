# Rebuild json database and ctags if missing
ifneq ("$(wildcard compile_commands.json)","")
MAKE_TYPE := test
else
MAKE_TYPE := initialise
endif

default:
	make $(MAKE_TYPE)

initialise: $(SUBDIRS)
	make clean
	.hooks/ctags
	bear make subdirs


SUBDIRS = src doc

.PHONY: subdirs $(SUBDIRS) clean test

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

doc: src

test:
	$(MAKE) -C src

clean:
	rm -f .git/tags{,extra} compile_commands.json
	$(MAKE) -C src clean
