#!/bin/sh

SEARCHFILE=dat/fullchrome.bulk
FILESIZE=$(ls -l $SEARCHFILE | awk '{ print $5 }')
MAXCOMPRESSED=$(echo "$FILESIZE-512" | bc)
echo "FILESIZE: $FILESIZE"
echo "MAXCOMPRESSED: $MAXCOMPRESSED"

for length in $(seq 1 $MAXCOMPRESSED)
do
	DECOMPRESSED_SIZE=$({ printf "\x78\x9c"; dd if=$SEARCHFILE bs=1 status=none skip=512 count=$length; } | ./zpipe -d 2>/dev/null | wc -c)
	if [ "$DECOMPRESSED_SIZE" != "0" ]
	then
		echo "$length: $DECOMPRESSED_SIZE"
	fi
done
