#!/bin/sh

SEARCHFILE=bytes/partialred.bulk
FILESIZE=$(ls -l $SEARCHFILE | awk '{ print $5 }')
echo "FILESIZE: $FILESIZE"

for offset in $(seq 0 $FILESIZE)
do
	DECOMPRESSED_SIZE=$({ printf "\x78\x9c"; dd if=$SEARCHFILE bs=1 status=none skip=$offset; } | ./zpipe -d 2>/dev/null | wc -c)
	if [ "$DECOMPRESSED_SIZE" != "0" ]
	then
		echo "$offset: $DECOMPRESSED_SIZE"
	fi
done
