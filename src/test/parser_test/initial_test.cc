#include <iostream>
#include "../../parser.h"
#include "../../monitor.h"
#include "../../devices.h"
#include "../../scanner.h"

int main (int argc, char **argv) 
{
	if (argc != 2) { // check we have one command line argument
    wcout << "Usage:      " << argv[0] << " [filename]" << endl;
    exit(1);
  }
  
	names* nmz = new names();
  network* netz = new network(nmz);
  devices* dmz = new devices(nmz, netz);
  monitor* mmz = new monitor(nmz, netz);
  scanner* smz = new scanner(nmz, argv[1]);
  parser* pmz = new parser(netz, dmz, mmz, smz, nmz);
  
  cout << pmz->readin() << endl;
  
  return 0;
}
