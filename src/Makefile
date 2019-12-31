ROOT_DIR=$(shell git rev-parse --show-toplevel)

LIBRARY_SOURCES:=libspacelcd.c svg.cpp usb.c bgr.c deflate.c #$(wildcard *.cpp)
LIBRARY_OBJS = $(patsubst %.c,%.o,$(LIBRARY_SOURCES))
LIBRARY_OBJS += $(patsubst %.cpp,%.o,$(LIBRARY_SOURCES))
BINARY_SOURCES = splcdcmd.c
BINARY_OBJS = $(patsubst %.c,%.o,$(BINARY_SOURCES))

BINARY=splcdcmd
LIBRARY=libspacelcd.so
LIBVER = 1
SONAME=$(LIBRARY).$(LIBVER)

PYTHON_MODULE = _spacelcd.so

CC      = ccache gcc
CXX	= ccache g++
CFLAGS = -Wall -ggdb3 -O0

# Unit testing
TESTS = bgr deflate svg
TEST_SOURCES = $(addsuffix .c,$(addprefix t_,$(TESTS)))
TEST_OBJS = $(patsubst %.c,%.o,$(TEST_SOURCES))
TEST_BINARIES = $(patsubst %.c,%_x,$(TEST_SOURCES))
TEST_PYTHON = t_splcd.py

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


#######################################################################
#                            Main Recipes                             #
#######################################################################

test:	$(TEST_BINARIES) $(PYTHON_MODULE)
	$(ROOT_DIR)/bin/testrunner.sh $(TEST_BINARIES) $(TEST_PYTHON)

all: $(LIBRARY_SOURCES) $(BINARY_SOURCES)
	make $(LIBRARY)
	make $(BINARY)
	make $(PYTHON_MODULE)


#######################################################################
#                       Binaries and Libraries                        #
#######################################################################

%.o:	%.cpp
	$(CXX) -fPIC -o $@ -c $^ $(shell pkgconf --cflags glib-2.0) \
		$(shell pkgconf --cflags librsvg-2.0) \
		$(shell pkgconf --cflags libusb-1.0)

%.o:	%.c
	$(CXX) -fPIC -o $@ -c $^ $(shell pkgconf --cflags glib-2.0) \
		$(shell pkgconf --cflags librsvg-2.0) \
		$(shell pkgconf --cflags libusb-1.0)

$(LIBRARY):  svg.o bgr.o deflate.o usb.o libspacelcd.o
	$(CXX) -fPIC -shared -I.  $^ -Wl,-soname=$(SONAME) -o $(SONAME) $(shell pkgconf --libs zlib) $(shell pkgconf --libs librsvg-2.0) $(shell pkgconf --libs libusb-1.0)
	ln -sf $(SONAME) $@

$(BINARY): $(LIBRARY)
	$(CXX)  $(shell pkgconf --libs libusb-1.0) $(shell pkgconf --libs zlib) $(CFLAGS) -o $@ $(BINARY_SOURCES)


#######################################################################
#                             Unit Tests                              #
#######################################################################

# svgtest: svg.o t_svg.c $(UNITY_OBJS)
# 	$(CXX) $(INCFLAGS) $(UNITY_OBJS) svg.o t_svg.c -o t_svg_x $(shell pkgconf --libs librsvg-2.0)

# t_splcd_x: t_splcd.c $(LIBRARY)
# 	$(CXX) -I. $(shell pkgconf --cflags libusb-1.0) -Wl,-rpath=. $(LIBRARY) t_splcd.c -o $@

t_%_x:	$(LIBRARY) $(UNITY_OBJS)
	$(CXX) $(INCFLAGS) -Wl,-rpath=. $(LIBRARY) $(UNITY_OBJS) $(subst _x,,$@).c -o $@


#######################################################################
#                         Unit Test Framework                         #
#######################################################################

unity_memory.o:	$(UNITY_MEMORY_DIR)/unity_memory.c
	$(CXX) $(CFLAGS) $(INCFLAGS) -c $^ -o $@

unity_fixture.o:	$(UNITY_FIXTURE_DIR)/unity_fixture.c
	$(CXX) $(CFLAGS) $(INCFLAGS) -c $^ -o $@

unity.o:	$(UNITY_DIR)/unity.c
	$(CXX) $(CFLAGS) $(INCFLAGS) -c $^ -o $@


#######################################################################
#                           Python Bindings                           #
#######################################################################

$(PYTHON_MODULE):	$(LIBRARY) spacelcd.i
	swig -c++ -python spacelcd.i
	$(CXX) -c -fPIC -Wl,-rpath=. spacelcd_wrap.cxx $(shell python-config --includes)
	$(CXX) -shared -Wl,-rpath=. spacelcd_wrap.o $(LIBRARY) -o $@


#######################################################################
#                                Tools                                #
#######################################################################

clean:
	rm -rf $(BINARY) compile_commands.json *.o t_*_x lib*.so* _* *wrap.c* spacelcd.py

tags:
	rm -f .git/tags .git/tagsextra
	.hooks/ctags

debug:
	gdb --args $(BINARY) -v -c -w 0x0311 /tmp/hex.bin


#######################################################################
#                             Formatting                              #
#######################################################################

format: formatpython
	clang-format -style=file -i *.{c,h,cpp,hpp} || /bin/true

formatpython:
	autopep8 --global-config=$(ROOT_DIR)/res/cfg/pycodestyle --aggressive --aggressive --in-place --jobs -1 $(FORMAT_FILES_PYTHON)
	flake8 --ignore=E402 --max-complexity=10 --show-source $(FORMAT_FILES_PYTHON) || /bin/true

unexpand:
	for file in $(FORMAT_FILES_PYTHON)
	do
		unexpand -t4 $${file} > spacestotabs.tmp && mv spacestotabs.tmp $${file}
	done


#######################################################################
#                           Device Control                            #
#######################################################################

lcdon: $(BINARY)
	$(ROOT_DIR)/src/$(BINARY) -c -w 0x311 $(ROOT_DIR)/res/dat/lcdon.control

lcdoff: $(BINARY)
	$(ROOT_DIR)/src/$(BINARY) -c -w 0x311 $(ROOT_DIR)/res/dat/lcdoff.control

reset: $(BINARY)
	$(ROOT_DIR)/src/$(BINARY) -c -w 0x311 $(ROOT_DIR)/res/dat/stop0*.control

startup: lcdon
	$(ROOT_DIR)/src/$(BINARY) -c -w 0x311 $(ROOT_DIR)/res/dat/start0*.control

shutdown: reset lcdoff

resetperm:
	$(ROOT_DIR)/bin/resetperm

