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


  smz->getsymbol(sym, id, num);
  //id = 1;
  // {
  if (sym != opencurly){
	// Error for no opening {
    errorhandling(no_opening_brace);
    return 0;
  }
  
  smz->getsymbol(sym,id,num);
  // devices
  if (sym != DEV)
  {
	// Error for no devices
    errorhandling(no_devices);
    return 0;
  }
    
  
  // {
  id = 1;
  current_name = name_listz->getname(id);

  if (current_name.compare("{") != 0)
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
	case no_opening_brace:
	  cout << "No opening brace.\n";
	  break; 
	case no_devices:
	  cout << "Expected \"devices\"\n";
	  break;
	default:
	  cout << "Unknown error.\n"; 
  }
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







