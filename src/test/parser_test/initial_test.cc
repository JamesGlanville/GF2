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
  
  ifstream * inf = new ifstream;
  inf->open(argv[1]);
  
	if (!inf) {
		cout << "Error: cannot open file " << argv[1] << " for reading " << endl;
		exit(1);
	}
  
  names* comment_nmz = new names();
  scanner* comment_smz = new scanner(comment_nmz, inf);
  test_file_parser* comment_pmz = new test_file_parser(comment_smz, comment_nmz);
  
  if(comment_pmz->testFileDefinition())
  {
    return 1;  
  }
    
	
  names* nmz = new names();
  network* netz = new network(nmz);
  monitor* mmz = new monitor(nmz, netz);
  devices* dmz = new devices(nmz, netz);
  scanner* smz = new scanner(nmz, inf);
  parser* pmz = new parser(netz, dmz, mmz,smz, nmz);
  
  cout << pmz->readin() << endl;
  
  return 0;
}
