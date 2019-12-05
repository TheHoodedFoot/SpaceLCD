#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as f:
    for num in [word for line in f for word in line.split()]:
        if len(num) == 4:
            sys.stdout.buffer.write(int(num[:2], 16).to_bytes(1, 'big'))
            sys.stdout.buffer.write(int(num[2:4], 16).to_bytes(1, 'big'))
        if len(num) == 2:
            sys.stdout.buffer.write(int(num[:2], 16).to_bytes(1, 'big'))
