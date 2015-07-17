#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "compression.h"

//To compile the following code you have to link zlib, zlibstat.lib file, with your project.
//You have to define ZLIB_WINAPI in project properties -> c/c++ -> preprocessor definitions

int main(void){
	unsigned char *compressed = { 0 }, *uncompressed = { 0 };
	unsigned int compressedLen = 0, uncompressedLen = 0;

	unsigned char data2compress[] = "...A long character buffer...\0";

	if ((compressedLen = CompressBuffer(&compressed, data2compress, strlen(data2compress))) > 0){

		if ((uncompressedLen = UncompressBuffer(&uncompressed, compressed, compressedLen)) > 0){
			printf("Original data: %s\n\n", uncompressed);
			printf("Before compression: %d, After compression: %d\n", strlen(data2compress), compressedLen);
		}
	}

	return EXIT_SUCCESS;
}
