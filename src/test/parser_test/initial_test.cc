//#define PARSER_TEST
#include <iostream>
#include "../../parser.h"
#include "../../scanner.h"
#include "../../test_file_parser.h"

int main (int argc, char **argv) 
{
	if (argc != 2) { // check we have one command line argument
    wcout << "Usage:      " << argv[0] << " [filename]" << endl;
    exit(1);
  }
  
  names* comment_nmz = new names();
  scanner* comment_smz = new scanner(comment_nmz, argv[1]);
  test_file_parser* comment_pmz = new test_file_parser(comment_smz, comment_nmz);
  
  if(comment_pmz->testFileDefinition())
  {
    return 1;  
  }
    
	names* nmz = new names();
  scanner* smz = new scanner(nmz, argv[1]);
  parser* pmz = new parser(smz, nmz);
  
  cout << pmz->readin() << endl;
  
  return 0;
}
