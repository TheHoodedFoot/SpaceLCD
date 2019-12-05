#!/usr/bin/env python3

import sys
import re

# Output header
out = "static const unsigned char bitmap[] = {"

char_count = 0
line_count = 0

things = ""
things_size = 0

for line in sys.stdin:

    x = re.search(".*0x00(..):  (.*)$", line)
    if x is not None:
        buffer = x.group(2) + " "
        for word in re.findall(".... ", buffer):
            things += "0x" + word[:2] + ", 0x" + word[2:4] + ", "
            things_size += 2
            # print(things)
            char_count += 2
        if int(x.group(1)) == 30:
            # print(buffer)
            out += things[:-2] + ","
            things = ""
            things_size = 0
            line_count += 1
            # buffer = ""

# Footer
out += "};"

print("#define BITMAP_PACKET_SIZE 64")
print("#define BITMAP_PACKETS " + str(line_count))
print(out)
if things != "":
    print("#define BITMAP_THINGS_SIZE " + str(things_size))
    print("static const unsigned char bitmap_footer[] = {")
    print(things)
    print("};")
else:
    print("#define BITMAP_THINGS_SIZE 0")
    print("static const unsigned char bitmap_footer[] = { 0x00 };")
