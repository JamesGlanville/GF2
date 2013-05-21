#include "parser.h"

using namespace std;

/* The parser for the circuit definition files */

bool parser::readin (void)
{
  // Stores parsing function output
  name id;
  int num;
  device_type current_device_type;
  bool endOfDevices = 0;

  // {
  if(parseToken(opencurly)) return PARSER_FAIL;
  
  // DEVICES
  if(parseSectionHeader(DEV)) return PARSER_FAIL;

  // {
  if(parseToken(opencurly)) return PARSER_FAIL;

  // Parsing defined devices
  // First device name
  if(parseFirstDeviceName(id)) return PARSER_FAIL;
  
  // = 
  if(parseToken(equals)) return PARSER_FAIL;
  
  // Device type 
  if(parseDeviceType(current_device_type)) return PARSER_FAIL;

  // Creates above device (apart from clock and switches, which get saved until INIT)
  // CLK/SW names should be saved in separate nametables for re-use later
  // Nametable shouldn't allow multiple of the same name
  if(createDevice(current_device_type,id)) return PARSER_FAIL;
  
  // ;
  if(parseToken(semicol)) return PARSER_FAIL;
  
  // Subsequent device names
  if(parseDeviceName(id,endOfDevices)) return PARSER_FAIL;
  
  while(!endOfDevices) 
  {
    if(parseToken(equals)) return PARSER_FAIL;
    if(parseDeviceType(current_device_type)) return PARSER_FAIL;
    if(createDevice(current_device_type,id)) return PARSER_FAIL;
    if(parseToken(semicol)) return PARSER_FAIL;
    // Goes at end of loop because it also detects the closing curly brace
    if(parseDeviceName(id,endOfDevices)) return PARSER_FAIL;
  }
  
  // INIT
  if(parseSectionHeader(INIT)) return PARSER_FAIL;
  
  // {
  if(parseToken(opencurly)) return PARSER_FAIL;
  
  // Reset end of section marker
  endOfDevices = 0;
  
  // Parse initialisations
  if(parseDeviceName(id,endOfDevices)) return PARSER_FAIL;
  
  while(!endOfDevices)
  {
    if(parseToken(equals)) return PARSER_FAIL;
    if(parseNumber(num)) return PARSER_FAIL;
    // Function which does something with num and id goes here
    // Use id to retrieve name, check in clock and switch tables for match, initialise as appropriate
    if(parseToken(semicol)) return PARSER_FAIL;
    if(parseDeviceName(id,endOfDevices)) return PARSER_FAIL;
  }
  
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
      smz->printError("Expected numeric parameter");
      break;
    case number_expected:
      smz->printError("Expected number");
      break;
    case not_valid_device:
      smz->printError("Not a valid device type");
      break;
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

bool parser::parseFirstDeviceName (name &id)
{
  symbol sym;
  int num;
  
  smz->getsymbol(sym,id,num);
  switch(sym) { 
    case namesym:
      // Here is where the device name should be stored somewhere useful
      //cout << "Device name \"" << nmz->getname(id) << "\" recognised.\n";
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

bool parser::parseDeviceName (name &id, bool &endOfDevices)
{
  symbol sym;
  int num;
  endOfDevices = 0;
  
  smz->getsymbol(sym,id,num);
  switch(sym) { 
    case namesym:
      // Here is where the device name should be stored somewhere useful
      //cout << "Device name \"" << nmz->getname(id) << "\" recognised.\n";
      break;
    case closecurly:
      // Device section ended
      endOfDevices = 1;
      break;
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
bool parser::parseDeviceType (device_type &current_device_type)
{
  symbol sym;
  int num;
  name id;
  
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

// Parses parameter if necessary and then creates a device of give type and name
bool parser::createDevice (device_type current_device_type, name id)
{
  int param_value;
  switch(current_device_type)
  {
    case AND:
      if(parseParam(param_value)) return PARSER_FAIL;
      cout << "Created AND gate with " << param_value << " inputs, with name \"" << nmz->getname(id) << "\".\n";
      break;
    case NAND:
      if(parseParam(param_value)) return PARSER_FAIL;
      cout << "Created NAND gate with " << param_value << " inputs, with name \"" << nmz->getname(id) << "\".\n";
      break;
    case OR:
      if(parseParam(param_value)) return PARSER_FAIL;
      cout << "Created OR gate with " << param_value << " inputs, with name \"" << nmz->getname(id) << "\".\n";
      break;
    case NOR:
      if(parseParam(param_value)) return PARSER_FAIL;
      cout << "Created NOR gate with " << param_value << " inputs, with name \"" << nmz->getname(id) << "\".\n";
      break;
    case XOR:
      if(parseParam(param_value)) return PARSER_FAIL;
      cout << "Created XOR gate with " << param_value << " inputs, with name \"" << nmz->getname(id) << "\".\n";
      break;
    case DTYPE:
      cout << "Created DTYPE, with name \"" << nmz->getname(id) << "\".\n";
      break;
    case CLK:
      cout << "Created CLK, with name \"" << nmz->getname(id) << "\".\n";
      break;
    case SW:
      cout << "Created SW, with name \"" << nmz->getname(id) << "\".\n";
      break;
    default:
      cout << "Something has gone really wrong.\n";
  }
  return PARSER_PASS;
}

bool parser::parseNumber(int &num)
{
  symbol sym;
  name id;
  
  smz->getsymbol(sym,id,num);
  if(sym != numsym)
  {
    errorHandling(number_expected);
    return PARSER_FAIL;
  }
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







