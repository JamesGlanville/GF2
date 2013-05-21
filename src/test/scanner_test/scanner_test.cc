#define SCANNERMETHODSPUBLIC

#include "../../scanner.h" 
#include "../../names.h"

int main(int argc, char **argv)
{
	if (argc != 2) { // check we have one command line argument
		wcout << "Usage:      " << argv[0] << " [filename]" << endl;
		exit(1);
	}
	
	names* test_nametable;
	
	scanner* test_scanner = new scanner(test_nametable,argv[1]);
	test_scanner->rewind();
  
	return 0;
}
