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


  smz->getsymbol(sym,id,num);
  // {
  if (sym != opencurly){
    // Error for no opening {
    errorHandling(no_opening_brace);
    cout << "Symbol recieved is " << sym << endl;
    return PARSER_FAIL;
  }
  
  smz->getsymbol(sym,id,num);
  // devices
  if (sym != DEV)
  {
	  // Error for no devices
    errorHandling(no_devices);
    return PARSER_FAIL;
  }
  
  smz->getsymbol(sym,id,num);    
  // {
  if (sym != opencurly)
  {
    // Error for no opening {
    errorHandling(no_opening_brace);
    return PARSER_FAIL;
  }
  
  // Parsing defined devices
  smz->getsymbol(sym,id,num);
  switch(sym) { 
    case namesym:
      cout << "Device name \"" << nmz->getname(id) << "\" recognised.\n";
      break;
    case closecurly:
      // Error for at least one device must be defined
      errorHandling(one_device_required);
      return PARSER_FAIL;
      break;
    case numsym:
      // All names must begin with a letter
      errorHandling(names_begin_letter);
      return PARSER_FAIL;
      break;
    default:
      // Currently unknown error - consider a generic message later?
      errorHandling(unknown);
  }
  
  
  return PARSER_PASS;
}

void parser::errorHandling (error error_num) 
{
  switch(error_num) {
    case unknown:
      smz->printError("Unknown error."); 
      break;
    case no_opening_brace:
      smz->printError("No opening brace.");
      break; 
    case no_devices:
      smz->printError("Expected \"devices\" section.");
      break;
    case one_device_required:
      smz->printError("At least one device definition is required.");
      break;
    case names_begin_letter:
      smz->printError("Names must begin with a letter.");
      break;
    default:
      cout << "You should never see this message\n";
  }
}

parser::parser (
                #ifndef PARSER_TEST
                network* network_mod, 
                devices* devices_mod,
                monitor* monitor_mod, 
                #endif
                scanner* scanner_mod, 
                names* name_table)
{
  #ifndef PARSER_TEST
  netz = network_mod;  /* make internal copies of these class pointers */
  dmz = devices_mod;   /* so we can call functions from these classes  */
  mmz = monitor_mod;   /* eg. to call makeconnection from the network  */
  #endif
  smz = scanner_mod;   /* class you say:                               */
                       /* netz->makeconnection (i1, i2, o1, o2, ok);   */
  nmz = name_table;	
  /* any other initialisation you want to do? */

}







