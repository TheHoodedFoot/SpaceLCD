#!/bin/sh

# Get 512 byte header from working chrome
dd bs=1 status=none count=600 if=bytes/wholechrome.bulk > header.bin

# Decompress and recompress the body
{ printf "\x78\x9c"; dd if=bytes/wholechrome.bulk bs=1 status=none skip=600; } | ./zpipe -d | ./zpipe | dd bs=1 status=none skip=2 > body.bin

# Pad to length if necessary
DIFFERENCE=$(echo "4716 - ($(ls -l frankenframe.bulk | awk '{ print $5 }') - 512)" | bc)
dd if=bytes/wholechrome.bulk bs=1 status=none skip=$(ls -l frankenframe.bulk | awk '{ print $5 }') > padding.bin

# Bolt the body back on
cat header.bin body.bin > frankenframe.bulk

