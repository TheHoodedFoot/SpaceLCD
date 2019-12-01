export PATH := ./:$(PATH)

SOURCES:=splcd.c #$(wildcard *.cpp)
OUTPUT=splcd

CC      = gcc
CFLAGS = -Wall -ggdb3 -O0

all: compile_commands.json $(SOURCES) $(OUTPUT) 

$(OUTPUT): $(SOURCES)
	$(CC) $(shell pkgconf --cflags libusb-1.0) $(shell pkgconf --libs libusb-1.0) $(CFLAGS) -o $@ $^

compile_commands.json: $(SOURCES)
	bear make $(OUTPUT)

clean:
	rm -f $(OUTPUT) compile_commands.json

# bulk.txt: bulk.pcapng
# 	tcpdump -r $^ -tx > $@

bulk.h: bulk.txt
	spparse.py < $^ > $@

lcdon: $(OUTPUT)
	$(OUTPUT) -c -w 0x311 bytes/lcdon.bin

lcdoff: $(OUTPUT)
	$(OUTPUT) -c -w 0x311 bytes/lcdoff.bin

format:
	clang-format -style=file -i $(SOURCES) splcd.c || /bin/true

debug:
	gdb --args ./splcd -v -c -w 0x0311 /tmp/hex.bin

resetperm:
	bin/resetperm
