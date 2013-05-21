//#define PARSER_TEST
#include <iostream>
#include "../../parser.h"
#include "../../scanner.h"

int main (int argc, char **argv) 
{
	if (argc != 2) { // check we have one command line argument
    wcout << "Usage:      " << argv[0] << " [filename]" << endl;
    exit(1);
  }
  
	names* nmz = new names();
  scanner* smz = new scanner(nmz, argv[1]);
  parser* pmz = new parser(smz, nmz);
  
  cout << pmz->readin() << endl;
  
  return 0;
}
