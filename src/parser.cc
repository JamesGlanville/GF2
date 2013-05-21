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
  device_type current_device_type;
  int param_value=0;

  // {
  if(parseToken(opencurly)) return PARSER_FAIL;
  
  // devices
  if(parseSectionHeader(DEV)) return PARSER_FAIL;

  // {
  if(parseToken(opencurly)) return PARSER_FAIL;

  // Parsing defined devices
  // Device name
  // This should probably return an name/id for the nametable
  // Need to work out how this interfaces to the circuit creation functions and whether names are involved in that or we need a separate data structure to store them
  if(parseDeviceName()) return PARSER_FAIL;
  
  // = 
  if(parseToken(equals)) return PARSER_FAIL;
  
  // Device type 
  if(parseDeviceType(current_device_type)) return PARSER_FAIL;
  // This should go in a separate function
  switch(current_device_type)
  {
    case AND:
      if(parseParam(param_value)) return PARSER_FAIL;
      cout << "Created AND gate with " << param_value << " inputs\n";
      break;
    case NAND:
      if(parseParam(param_value)) return PARSER_FAIL;
      cout << "Created NAND gate with " << param_value << " inputs\n";
      break;
    case OR:
      if(parseParam(param_value)) return PARSER_FAIL;
      cout << "Created OR gate with " << param_value << " inputs\n";
      break;
    case NOR:
      if(parseParam(param_value)) return PARSER_FAIL;
      cout << "Created NOR gate with " << param_value << " inputs\n";
      break;
    case XOR:
      if(parseParam(param_value)) return PARSER_FAIL;
      cout << "Created XOR gate with " << param_value << " inputs\n";
      break;
    case DTYPE:
      cout << "Created DTYPE\n";
      break;
    case CLK:
      cout << "Created CLK\n";
      break;
    case SW:
      cout << "Created SW\n";
      break;
    default:
      cout << "Something has gone really wrong\n";
  }
  
  // ;
  if(parseToken(semicol)) return PARSER_FAIL;
  
  return PARSER_PASS;
}

void parser::errorHandling (error error_num) 
{
  switch(error_num) {
    case unknown:
      smz->printError("Unknown error"); 
      break;
    case no_opening_brace:
      smz->printError("Expected opening brace");
      break; 
    case no_devices:
      smz->printError("Expected \"devices\" section");
      break;
    case one_device_required:
      smz->printError("At least one device definition is required");
      break;
    case names_begin_letter:
      smz->printError("Names must begin with a letter");
      break;
    case device_name_expected:
      smz->printError("Expected device name");
      break;
    case number_param_expected:
      smz->printError("An integer parameter is required");
      break;
    case not_valid_device:
      smz->printError("Not a valid device type");
    default:
      cout << "You should never see this message\n";
  }
}

bool parser::parseToken (symbol token)
{
  symbol sym;
  name id;
  int num;
  string error_token;
  
  smz->getsymbol(sym,id,num);
  if (sym != token)
  {
    switch(token) 
    {
      case consym:
        error_token = "'<='";
        break;
      case semicol:
        error_token = "';'";
        break;
      case equals:
        error_token = "'='";
        break;
      case openparen:
        error_token = "'('";
        break;
      case closeparen:
        error_token = "')'";
        break;
      case opencurly:
        error_token = "'{'";
        break;
      case closecurly:
        error_token = "'}'";
        break;
      default:
        error_token = "ERRRORR";
    }
    smz->printError("Expected " + error_token);
    return PARSER_FAIL;
  } 
  
  return PARSER_PASS;
}

bool parser::parseSectionHeader (symbol header)
{
  symbol sym;
  name id;
  int num;
  string section_name;
  
  smz->getsymbol(sym,id,num);
  if (sym != header)
  {
    switch(header) 
    {
      case DEV:
        section_name = "\"DEVICES\"";
        break;
      case INIT:
        section_name = "\"INIT\"";
        break;
      case CONN:
        section_name = "\"CONNECTIONS\"";
        break;
      case MON:
        section_name = "\"MONITORS\"";
        break;      
      default:
        section_name = "ERRRORR";
    }
    smz->printError("Expected " + section_name + " section header");
    return PARSER_FAIL;
  } 
  
  return PARSER_PASS;
}

bool parser::parseDeviceName (void)
{
  symbol sym;
  name id;
  int num;
  
  smz->getsymbol(sym,id,num);
  switch(sym) { 
    case namesym:
      // Here is where the device name should be stored somewhere useful
      cout << "Device name \"" << nmz->getname(id) << "\" recognised.\n";
      break;
    case closecurly:
      // Error for at least one device must be defined
      errorHandling(one_device_required);
      return PARSER_FAIL;
    case numsym:
      // All names must begin with a letter
      errorHandling(names_begin_letter);
      return PARSER_FAIL;
    default:
      // Generic expected a name here error
      errorHandling(device_name_expected);
      return PARSER_FAIL;
  }
  
  return PARSER_PASS;
}

// Converts strings of device type into an enumerated type
bool parser::parseDeviceType(device_type &current_device_type)
{
  symbol sym;
  name id;
  int num;
  
  smz->getsymbol(sym,id,num);
  if(nmz->getname(id).compare("and") == 0)
  {
    current_device_type = AND;
  } 
  else if(nmz->getname(id).compare("nand") == 0)
  {
    current_device_type = NAND;    
  }
  else if(nmz->getname(id).compare("or") == 0)
  {
    current_device_type = OR;    
  }
  else if(nmz->getname(id).compare("nor") == 0)
  {
    current_device_type = NOR;  
  }
  else if(nmz->getname(id).compare("xor") == 0)
  {
    current_device_type = XOR;  
  }
  else if(nmz->getname(id).compare("dtype") == 0)
  {
    current_device_type = DTYPE;    
  }
  else if(nmz->getname(id).compare("clk") == 0)
  {
    current_device_type = CLK;    
  }
  else if(nmz->getname(id).compare("sw") == 0)
  {
    current_device_type = SW; 
  }
  else
  {
    errorHandling(not_valid_device);
    return PARSER_FAIL;
  }
  return PARSER_PASS;
}

bool parser::parseParam(int &param_value) 
{
  symbol sym;
  name id;
  int num;
  
  if(parseToken(openparen)) return PARSER_FAIL;
  
  smz->getsymbol(sym,id,num);
  if(sym == numsym)
  {
    param_value = num;
  }
  else  
  {
    errorHandling(number_param_expected);
    return PARSER_FAIL;
  }
  
  if(parseToken(closeparen)) return PARSER_FAIL;
  
  return PARSER_PASS;
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







