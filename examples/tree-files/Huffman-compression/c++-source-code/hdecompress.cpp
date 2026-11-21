// File: hdecompress.cpp
// The program demonstrates Huffman decompression.
// It takes the name of the compressed file "file.huf"
// and of the decompressed file "originalfile" as arguments
// and performs decompression.

#include <string>

#include "d_hdcomp.h"

using namespace std;

int main(int argc, char **argv)
{
	string compFile(argv[1]), ucompFile(argv[2]);

	hDecompress dcomp(compFile, ucompFile);

	// uncompress the file
	dcomp.decompress();

	return 0;
}
