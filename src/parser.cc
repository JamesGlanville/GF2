#include <iostream>
#include "parser.h"

using namespace std;

/* The parser for the circuit definition files */


bool parser::readin (void)
{
  // Stores scanner output
  symbol sym;
  name id;
  int num;

  // Check for opening {
  smz->getsymbol(sym, id, num);
  if (.compare(str2) != 0)
  
	
}

parser::parser (network* network_mod, devices* devices_mod,
		monitor* monitor_mod, scanner* scanner_mod, names* scanner_output)
{
  netz = network_mod;  /* make internal copies of these class pointers */
  dmz = devices_mod;   /* so we can call functions from these classes  */
  mmz = monitor_mod;   /* eg. to call makeconnection from the network  */
  smz = scanner_mod;   /* class you say:                               */
                       /* netz->makeconnection (i1, i2, o1, o2, ok);   */
  name_listz = scanner_output;	
  /* any other initialisation you want to do? */

}







