#ifndef COMPRESSION_H_
#define COMPRESSION_H_

#include <windows.h>
#include <zlib.h>

#define GZIP_ENCODING	16
#define CHUNK			0x4096

int CompressBuffer(unsigned char **dest, const unsigned char *src, unsigned int slen);
int UncompressBuffer(unsigned char **dest, const unsigned char *src, unsigned int slen);

#endif
