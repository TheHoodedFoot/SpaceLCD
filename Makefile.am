lib_LTLIBRARIES = libspacelcd.la
libspacelcd_la_SOURCES = src/libspacelcd.c src/svg.cpp src/usb.c src/bgr.c src/deflate.c
libspacelcd_la_LDFLAGS = $(ZLIB_LIBS) $(LIBRSVG_LIBS) $(LIBUSB_LIBS)
libspacelcd_la_CFLAGS = $(ZLIB_CFLAGS) $(LIBRSVG_CFLAGS) $(LIBUSB_CFLAGS)
libspacelcd_la_CXXFLAGS = $(ZLIB_CFLAGS) $(LIBRSVG_CFLAGS) $(LIBUSB_CFLAGS)
ACLOCAL_AMFLAGS = -I m4

# Custom Rules

# SUBDIRS = src doc

# .PHONY: subdirs $(SUBDIRS) clean test

# Rebuild json database and ctags if missing
# ifneq ("$(wildcard compile_commands.json)","")
# MAKE_TYPE := test
# else
# MAKE_TYPE := initialise
# endif

# default:
# 	make $(MAKE_TYPE)

# initialise:
# 	make clean
# 	.hooks/ctags
# 	bear make subdirs

# test:
# 	$(MAKE) -C src test


# subdirs: $(SUBDIRS)

# $(SUBDIRS):
# 	$(MAKE) -C $@

# clean:
# 	rm -f .git/tags{,extra} compile_commands.json
# 	$(MAKE) -C src clean

format:
	$(MAKE) -C src format

reset:
	$(MAKE) -C src reset

startup:
	$(MAKE) -C src startup

shutdown:
	$(MAKE) -C src shutdown
