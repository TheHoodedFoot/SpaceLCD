# Library
lib_LTLIBRARIES = libspacelcd.la
libspacelcd_la_SOURCES = \
	libspacelcd.c libspacelcd.h \
	render.c render.h \
	usb.c usb.h \
	deflate.c deflate.h

libspacelcd_la_CFLAGS = $(ZLIB_CFLAGS) $(LIBRSVG_CFLAGS) $(LIBUSB_CFLAGS) -I$(top_srcdir)/sub/nanosvg
libspacelcd_la_LDFLAGS = $(ZLIB_LIBS) $(LIBRSVG_LIBS) $(LIBUSB_LIBS) -version-info 1:0:0

# Command-line binary
bin_PROGRAMS = splcdcmd
splcdcmd_SOURCES = splcdcmd.c
splcdcmd_CFLAGS = $(LIBUSB_CFLAGS) $(ZLIB_CFLAGS)
splcdcmd_LDADD = $(LIBUSB_LIBS) $(ZLIB_LIBS)

# Convenience test binary (not distributed, not installed)
noinst_PROGRAMS = send_logo
send_logo_SOURCES = send_logo.c
send_logo_CFLAGS = $(LIBUSB_CFLAGS) $(ZLIB_CFLAGS) $(LIBRSVG_CFLAGS) -DLOGO_PATH="\"$(top_srcdir)/res/img/logo.svg\""
send_logo_LDADD = libspacelcd.la $(LIBUSB_LIBS) $(ZLIB_LIBS) $(LIBRSVG_LIBS)

# Python bindings (conditional)
EXTRA_DIST = spacelcd.i t_splcd.py $(top_srcdir)/res/bin/testrunner.sh

if HAVE_SWIG
noinst_LTLIBRARIES = _spacelcd.la
_spacelcd_la_SOURCES = spacelcd_wrap.cxx
_spacelcd_la_CXXFLAGS = $(PYTHON3_CPPFLAGS) -fPIC
_spacelcd_la_LDFLAGS = -module -avoid-version -shrext .so
_spacelcd_la_LIBADD = libspacelcd.la $(PYTHON3_LDFLAGS)

BUILT_SOURCES = spacelcd_wrap.cxx spacelcd.py

spacelcd_wrap.cxx spacelcd.py: spacelcd.i
	$(SWIG) -c++ -python spacelcd.i

clean-local:
	-test -f spacelcd_wrap.cxx && rm -f spacelcd_wrap.cxx
	-test -f spacelcd.py && rm -f spacelcd.py
	-test -f _spacelcd.so && rm -f _spacelcd.so

# Build the Python extension .so directly (libtool CXX tag produces only .a)
_spacelcd.so: _spacelcd_la-spacelcd_wrap.lo libspacelcd.la
	$(CXX) -shared -fPIC \
		.libs/_spacelcd_la-spacelcd_wrap.o \
		-o .libs/_spacelcd.so \
		.libs/libspacelcd.so.1.0.0 \
		$(PYTHON3_LDFLAGS) \
		$(ZLIB_LIBS) $(LIBRSVG_LIBS) $(LIBUSB_LIBS) \
		-lstdc++ -lpthread
	cp .libs/_spacelcd.so $(srcdir)/_spacelcd.so
	@echo "Built _spacelcd.so — use: PYTHONPATH=$(srcdir) python3"
endif

# Unit tests
check_PROGRAMS = t_deflate_x t_render_x

t_deflate_x_SOURCES = t_deflate.c \
	$(top_srcdir)/sub/Unity/src/unity.c \
	$(top_srcdir)/sub/Unity/extras/fixture/src/unity_fixture.c \
	$(top_srcdir)/sub/Unity/extras/memory/src/unity_memory.c
t_deflate_x_CPPFLAGS = -I. -I$(top_srcdir)/sub/Unity/src \
	-I$(top_srcdir)/sub/Unity/extras/fixture/src \
	-I$(top_srcdir)/sub/Unity/extras/memory/src -DUNITY_OUTPUT_COLOR
t_deflate_x_LDADD = libspacelcd.la $(ZLIB_LIBS) $(LIBRSVG_LIBS) $(LIBUSB_LIBS)

t_render_x_SOURCES = t_render.c \
	$(top_srcdir)/sub/Unity/src/unity.c \
	$(top_srcdir)/sub/Unity/extras/fixture/src/unity_fixture.c \
	$(top_srcdir)/sub/Unity/extras/memory/src/unity_memory.c
t_render_x_CPPFLAGS = -I. -I$(top_srcdir)/sub/Unity/src \
	-I$(top_srcdir)/sub/Unity/extras/fixture/src \
	-I$(top_srcdir)/sub/Unity/extras/memory/src -DUNITY_OUTPUT_COLOR
t_render_x_LDADD = libspacelcd.la $(ZLIB_LIBS) $(LIBRSVG_LIBS) $(LIBUSB_LIBS)

TESTS = $(check_PROGRAMS)

# Convenience targets
.PHONY: lcdon lcdoff reset startup shutdown format formatpython logo

lcdon: splcdcmd
	$(builddir)/splcdcmd -c -w 0x311 $(top_srcdir)/res/dat/lcdon.control

lcdoff: splcdcmd
	$(builddir)/splcdcmd -c -w 0x311 $(top_srcdir)/res/dat/lcdoff.control

reset: splcdcmd
	$(builddir)/splcdcmd -c -w 0x311 $(top_srcdir)/res/dat/stop0*.control

startup: lcdon
	$(builddir)/splcdcmd -c -w 0x311 $(top_srcdir)/res/dat/start0*.control

shutdown: reset lcdon

logo: send_logo
	$(builddir)/send_logo

format:
	clang-format -style=file -i $(srcdir)/*.{c,h} || true

formatpython:
	autopep8 --global-config=$(top_srcdir)/res/cfg/pycodestyle --aggressive --aggressive --in-place $(srcdir)/t_splcd.py || true
	flake8 --ignore=E501 $(srcdir)/t_splcd.py || true
