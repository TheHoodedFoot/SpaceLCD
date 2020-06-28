#!/bin/sh

# Get the header from one frame and the body from another
dd status=none bs=1 if=bytes/fullwhite.bulk count=512 of=header.bulk
dd status=none bs=1 if=bytes/wholechrome.bulk skip=512 of=body.bulk

# Create a combined frame
cat header.bulk body.bulk > frankenframe1.bulk

# Decompress and recompress the body
# { printf "\x1f\x8b\x08\x00\x00\x00\x00\x00"; dd bs=1 status=none skip=0 if=body.bulk; } | zlib-flate -uncompress > decompressedzlib-flate.bmp

{ printf "\x1f\x8b\x08\x00\x00\x00\x00\x00\x00\x00"; dd bs=1 status=none skip=0 if=body.bulk; } | gzip -cd > decompressed_gzip.bmp

# cat decompressed_gzip.bmp | zlib-flate -compress | dd bs=1 status=none skip=9 count=500 of=recompressedbody.bulk
cat decompressed_gzip.bmp | gzip -n -c -9 > recompressedbody.bulk

cat header.bulk recompressedbody.bulk > test.bulk

./splcd -b test.bulk



