// File: hcompress.cpp
// the program demonstrates Huffman compression. the user
// enters the name of a file to compress as the argument and
// then whether the verbose option is desired. after compressing
// the file, the program outputs the compression ratio.
// if the Huffman tree has no more than 11 nodes, the tree
// is displayed

#include <iostream>
#include <string>
using namespace std;

#include "d_hcomp.h"		// hCompress class
#include "d_util.h"		// for setreal()

int main(int argc, char **argv)
{
	string filename = string(argv[1]);
	bool verbose;
	char response;

	// prompt for whether to use the verbose option
	// for Huffman compression

	cout << "Verbose (y or n): ";
	cin >> response;
	if (response == 'y')
		verbose = true;
	else
		verbose = false;

	cout << endl;

	// declare an hCompress object
	hCompress hc(filename, verbose);

	// compress the file
	hc.compress();

	// output the compression ratio
	cout << "The compression ratio = "
	     << setreal(1,2) << hc.compressionRatio()
	     << endl << endl;

	return 0;
}

