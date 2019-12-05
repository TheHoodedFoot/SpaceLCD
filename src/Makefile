export PATH := ./:bin:$(PATH)

ROOT_DIR=$(shell git rev-parse --show-toplevel)

SOURCES:=splcdcmd.c bgr.c deflate.c #$(wildcard *.cpp)
OBJS = $(patsubst %.c,%.o,$(SOURCES))

BINARY=splcdcmd
LIBRARY=libspacelcd.so
LIBVER = 1
SONAME=$(LIBRARY).$(LIBVER)

CC      = ccache gcc
CXX	= ccache g++
CFLAGS = -Wall -ggdb3 -O0

# Unit testing
TESTS = bgr deflate
TEST_SOURCES = $(addsuffix .c,$(addprefix t_,$(TESTS)))
TEST_OBJS = $(patsubst %.c,%.o,$(TEST_SOURCES))
TEST_EXES = $(patsubst %.c,%_x,$(TEST_SOURCES)) t_svg_x

# Unit test framework files
UNITY_FIXTURE_DIR = $(ROOT_DIR)/sub/Unity/extras/fixture/src
UNITY_MEMORY_DIR = $(ROOT_DIR)/sub/Unity/extras/memory/src
UNITY_DIR = $(ROOT_DIR)/sub/Unity/src
UNITY_OBJS = unity.o unity_fixture.o unity_memory.o
UNITY_DEFS = -DUNITY_OUTPUT_COLOR

# Includes
INCFLAGS = -I. -I$(UNITY_DIR) -I$(UNITY_FIXTURE_DIR) -I$(UNITY_MEMORY_DIR)

# Debugging
DEBUG_COMMAND ?= $(shell head -n1 .debugcmd)
DEBUG_TARGET ?= $(shell tail -n1 .debugcmd)

# gdb
# TEST_GDBINIT ?= src/testchamber/.gdbinit
# DEBUG_COMMAND = gdb -tui -x $(TEST_GDBINIT) $(TEST_CMD)

# lldb
# DEBUG_COMMAND = lldb $(shell head -n1 .lldbinit | sed s/#//)
# CXXFLAGS += -fstandalone-debug # for lldb

# Formatting
FORMAT_FILES = *.{h,c,cpp}
FORMAT_FILES_PYTHON = t_splcd.py


# Rebuild json database and ctags if missing
ifneq ("$(wildcard compile_commands.json)","")
MAKE_TYPE := test
else
MAKE_TYPE := initialise
endif


# Main Recipes

# Not used if build as a subdirectory, since bear make
# runs in the parent directory to create compile_commands.json

# all:
# 	make $(MAKE_TYPE)

initialise: $(SOURCES)
	make clean
	make $(LIBRARY)
	make python
# 	bear make $(SOURCES) $(LIBRARY) $(BINARY)

test:	$(TEST_EXES)
	t_bgr_x
	t_deflate_x
	t_svg_x


# Sources
$(BINARY): $(SOURCES)
	$(CXX) $(shell pkgconf --cflags libusb-1.0) $(shell pkgconf --libs libusb-1.0) $(shell pkgconf --libs zlib) $(CFLAGS) -o $@ $^

svg.o:	svg.cpp
	$(CXX) -fPIC -o $@ -c $^ $(shell pkgconf --cflags glib-2.0) $(shell pkgconf --cflags librsvg-2.0) 

usb.o:	usb.c
	$(CXX) -fPIC -o $@ -c $^ $(shell pkgconf --cflags libusb-1.0)

bgr.o:	bgr.c
	$(CXX) -fPIC -c -o $@ $^

deflate.o:	deflate.c
	$(CXX) -fPIC -c -o $@ $^

libspacelcd.o:	libspacelcd.c
	$(CXX) -fPIC -o $@ -c $^ $(shell pkgconf --cflags libusb-1.0)

$(LIBRARY):  svg.o bgr.o deflate.o usb.o libspacelcd.o
	$(CXX) -fPIC -shared -I. $(shell pkgconf --cflags libusb-1.0) $^ -Wl,-soname=$(SONAME) -o $(SONAME) $(shell pkgconf --libs zlib) $(shell pkgconf --libs librsvg-2.0) $(shell pkgconf --libs libusb-1.0)
	ln -sf $(SONAME) $(LIBRARY)

t_%_x:	$(LIBRARY) $(UNITY_OBJS)
	$(CXX) $(INCFLAGS) -Wl,-rpath=. $(LIBRARY) $(UNITY_OBJS) $(subst _x,,$@).c -o $@

# Python bindings
python:	$(LIBRARY) spacelcd.i
	swig -c++ -python spacelcd.i
	$(CXX) -c -fPIC -Wl,-rpath=. spacelcd_wrap.cxx $(shell python-config --includes)
	$(CXX) -shared -Wl,-rpath=. libspacelcd.o bgr.o deflate.o svg.o usb.o spacelcd_wrap.o libspacelcd.so.1 -o _spacelcd.so

# Unit tests
svgtest: svg.o t_svg.c $(UNITY_OBJS)
	$(CXX) $(INCFLAGS) $(UNITY_OBJS) svg.o t_svg.c -o t_svg_x $(shell pkgconf --libs librsvg-2.0)

t_splcd_x: t_splcd.c $(LIBRARY)
	$(CXX) -I. $(shell pkgconf --cflags libusb-1.0) -Wl,-rpath=. $(LIBRARY) t_splcd.c -o $@

# Unit test framework
unity_memory.o:	$(UNITY_MEMORY_DIR)/unity_memory.c
	$(CXX) $(CFLAGS) $(INCFLAGS) -c $^ -o $@

unity_fixture.o:	$(UNITY_FIXTURE_DIR)/unity_fixture.c
	$(CXX) $(CFLAGS) $(INCFLAGS) -c $^ -o $@

unity.o:	$(UNITY_DIR)/unity.c
	$(CXX) $(CFLAGS) $(INCFLAGS) -c $^ -o $@

#######################################################################
#                           Device Control                            #
#######################################################################

lcdon: $(BINARY)
	$(BINARY) -c -w 0x311 $(ROOT_DIR)/res/dat/lcdon.control

lcdoff: $(BINARY)
	$(BINARY) -c -w 0x311 $(ROOT_DIR)/res/dat/lcdoff.control

reset: $(BINARY)
	$(BINARY) -c -w 0x311 $(ROOT_DIR)/res/dat/stop0*.control

startup: lcdon
	$(BINARY) -c -w 0x311 $(ROOT_DIR)/res/dat/start0*.control

shutdown: reset lcdoff

resetperm:
	bin/resetperm

#######################################################################
#                                Tools                                #
#######################################################################

clean:
	rm -rf $(BINARY) compile_commands.json *.o t_*_x lib*.so* _* *wrap.c* spacelcd.py

tags:
	rm -f .git/tags .git/tagsextra
	.hooks/ctags

debug:
	gdb --args ./splcd -v -c -w 0x0311 /tmp/hex.bin

#######################################################################
#                             Formatting                              #
#######################################################################

format: formatpython
	clang-format -style=file -i *.c *.h *.cpp || /bin/true

formatpython:
	autopep8 --ignore=E402 --aggressive --aggressive -i $(FORMAT_FILES_PYTHON)
	flake8 --ignore=E402 --max-complexity=10 --show-source $(FORMAT_FILES_PYTHON) || /bin/true
