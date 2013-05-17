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
  namestring current_name;

  // Check for opening {
  //smp->getsymbol(sym, id, num);
  
  id = 1;
  current_name = name_listp->getname(id);
  cout << current_name << endl;
  if (current_name.compare("{") == 0)
  {
	// Error for no opening {
    errorhandling(no_opening_brace);
    return 0;
  }
  return 1;
}

void parser::errorhandling (error error_num) 
{
  switch(error_num) {
	case 0:
	  cout << "No opening brace.\n";
	  break; 
	default:
	  cout << "Unknown error.\n"; 
  }
}

parser::parser (network* network_mod, devices* devices_mod,
		monitor* monitor_mod, scanner* scanner_mod, names* scanner_output)
{
  //netp = network_mod;  /* make internal copies of these class pointers */
  //dmp = devices_mod;   /* so we can call functions from these classes  */
  //mmp = monitor_mod;   /* eg. to call makeconnection from the network  */
  //smp = scanner_mod;   /* class you say:                               */
                       /* netz->makeconnection (i1, i2, o1, o2, ok);   */
  name_listp = scanner_output;	
  /* any other initialisation you want to do? */

}







