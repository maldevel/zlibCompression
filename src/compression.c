#include "compression.h"

int CompressBuffer(unsigned char **dest, const unsigned char *src, unsigned int slen){

	z_stream stream;
	int bts = 0, ret = 0, dlen = 0;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;

	if (Z_OK != deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY))
		return 0;

	dlen = slen + 1;

	*dest = (unsigned char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dlen);
	if (*dest == NULL) return 0;

	stream.next_in = (z_const Bytef *)src;
	stream.avail_in = slen;

	stream.next_out = (Bytef *)*dest;
	stream.avail_out = dlen;

	if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
		deflateEnd(&stream);
		HeapFree(GetProcessHeap(), 0, *dest);
		*dest = NULL;
		return 0;
	}

	bts = dlen - stream.avail_out;
	ret = stream.total_out;

	if (memcpy_s(*dest, dlen, *dest, bts) != 0) {
		HeapFree(GetProcessHeap(), 0, *dest);
		*dest = NULL;
		ret = 0;
	}

	deflateEnd(&stream);

	return ret;
}


int UncompressBuffer(unsigned char **dest, const unsigned char *src, unsigned int slen){

	z_stream stream;
	Bytef temp[CHUNK];
	int ret = 0;

	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;

	if (Z_OK != inflateInit2(&stream, MAX_WBITS | GZIP_ENCODING))
		return 0;

	stream.next_in = (z_const Bytef *)src;
	stream.avail_in = slen;

	stream.next_out = temp;
	stream.avail_out = CHUNK;

	if (inflate(&stream, Z_FINISH) != Z_STREAM_END) {
		inflateEnd(&stream);
		return 0;
	}

	ret = stream.total_out;

	*dest = (unsigned char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ret + 1);
	if (*dest == NULL) {
		inflateEnd(&stream);
		return 0;
	}

	if (memcpy_s(*dest, ret + 1, temp, ret) != 0) {
		HeapFree(GetProcessHeap(), 0, *dest);
		*dest = NULL;
		ret = 0;
	}

	inflateEnd(&stream);

	return ret;
}
