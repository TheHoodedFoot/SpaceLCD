SUBDIRS = src doc

.PHONY: subdirs $(SUBDIRS) clean test

# Rebuild json database and ctags if missing
ifneq ("$(wildcard compile_commands.json)","")
MAKE_TYPE := test
else
MAKE_TYPE := initialise
endif

default:
	make $(MAKE_TYPE)

initialise:
	make clean
	.hooks/ctags
	bear make subdirs

test:
	$(MAKE) -C src test


subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	rm -f .git/tags{,extra} compile_commands.json
	$(MAKE) -C src clean

format:
	$(MAKE) -C src format

reset:
	$(MAKE) -C src reset

startup:
	$(MAKE) -C src startup

shutdown:
	$(MAKE) -C src shutdown
