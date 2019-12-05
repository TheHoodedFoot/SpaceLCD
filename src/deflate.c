#include "deflate.h"
#include "zlib.h"

int
crush(uint8_t *source, uint8_t *dest, int len)
{
	z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = len;		  // size of input
	stream.next_in = source;	  // input char array
	stream.avail_out = DEFLATED_MAX;  // size of output
	stream.next_out = dest;		  // output char array

	deflateInit2(&stream, -1, Z_DEFLATED, -15, 9, Z_FIXED);
	int result = deflate(&stream, Z_FINISH);
	int outputsize = DEFLATED_MAX - stream.avail_out;
	deflateEnd(&stream);
	if (result == Z_STREAM_END) return outputsize;
	return -1;
}
