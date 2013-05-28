/*
TODO:
connections created
monitors created
all inputs are connected
(look in lab handout for what has been missed)
continuing to read errors after one is found
total error 
*/
#include "parser.h"

using namespace std;

/* The parser for the circuit definition files */

bool parser::readin (void)
{
  // Stores parsing function output
  name id;
  name dev1id;
  name inid;
  name dev2id;
  name outid;
  int num;
  device_type current_device_type;
  bool endOfSection = 0;

  // {
  if(parseToken(opencurly)) return PARSER_FAIL;
  
  // DEVICES
  if(parseSectionHeader(DEV)) return PARSER_FAIL;

  // {
  if(parseToken(opencurly)) return PARSER_FAIL;

  // Parsing defined devices
  // First device name
  if(parseDeviceName(id)) return PARSER_FAIL;
  
  // = 
  if(parseToken(equals)) return PARSER_FAIL;
  
  // Device type 
  if(parseDeviceType(current_device_type)) return PARSER_FAIL;

  // Nametable shouldn't allow multiple of the same name - THIS NEEDS DOING (I think)
  if(createDevice(current_device_type,id)) return PARSER_FAIL;
  
  // ;
  if(parseToken(semicol)) return PARSER_FAIL;
  
  // Subsequent device names + checks for closing curly
  if(parseDeviceName(id,endOfSection)) return PARSER_FAIL;
  
  while(!endOfSection) 
  {
    if(parseToken(equals)) return PARSER_FAIL;
    if(parseDeviceType(current_device_type)) return PARSER_FAIL;
    if(createDevice(current_device_type,id)) return PARSER_FAIL;
    if(parseToken(semicol)) return PARSER_FAIL;
    // Goes at end of loop because it also detects the closing curly brace
    if(parseDeviceName(id,endOfSection)) return PARSER_FAIL;
  }
  
  // CONNECTIONS
  if(parseSectionHeader(CONN)) return PARSER_FAIL;
  
  // {
  if(parseToken(opencurly)) return PARSER_FAIL;
  
  // Reset end of section marker
  endOfSection = 0;

  // Parse connections
  if(parseConnInputName(dev1id,inid,endOfSection)) return PARSER_FAIL;

  while(!endOfSection)
  {
    if(parseToken(consym)) return PARSER_FAIL;
    if(parseConnOutputName(dev2id,outid)) return PARSER_FAIL;
    if(createConn(dev1id,dev2id,inid,outid)) return PARSER_FAIL;
    if(parseToken(semicol)) return PARSER_FAIL;
    if(parseConnInputName(dev1id,inid,endOfSection)) return PARSER_FAIL;
  }
  
  // Should check that all inputs are connected at this point

  // MONITORS 
  if(parseSectionHeader(MON)) return PARSER_FAIL;

  // {
  if(parseToken(opencurly)) return PARSER_FAIL;
  
  // Reset end of section marker
  endOfSection = 0;

  // Parse monitor name - id refers to monitor nametable
  if(parseMonitorName(dev1id)) return PARSER_FAIL;
  if(parseToken(consym)) return PARSER_FAIL;
  if(parseConnOutputName(dev2id,outid)) return PARSER_FAIL;
  if(createMonitor(dev1id,dev2id,outid)) return PARSER_FAIL;
  if(parseToken(semicol)) return PARSER_FAIL;

  while(!endOfSection)
  {
	if(parseToken(consym)) return PARSER_FAIL;
	if(parseConnOutputName(dev2id,outid)) return PARSER_FAIL;
	if(createMonitor(dev1id,dev2id,outid)) return PARSER_FAIL;
	if(parseToken(semicol)) return PARSER_FAIL;
	if(parseMonitorName(dev1id,endOfSection)) return PARSER_FAIL;
  }

  // }
  if(parseToken(closecurly)) return PARSER_FAIL;

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
    case monitor_name_expected:
      smz->printError("Expected monitor name");
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
    case device_not_defined:
      smz->printError("Device not defined");
      break;
    case invalid_input:
      smz->printError("Not a valid device input");
      break;
    case no_inputs:
      smz->printError("Device does not have any inputs");
      break;
    case more_inputs_than_defined:
      smz->printError("More inputs than defined");
      break;
    case invalid_output:
      smz->printError("Not a valid device output");
      break;
	case one_monitor_required:
	  smz->printError("At least one monitor definition is required");
	  break;
	case inputs_two_to_sixteen:
	  smz->printError("Gate requires between 2 and 16 inputs");
      break;
	case clk_param:
	  smz->printError("Clock requires a positive, integer period");
	  break;
	case switch_param:
	  smz->printError("Switch can only be set to 1 or 0");
	  break;
	case device_not_unique:
		smz->printError("Device names must be unique");
		break;
	case monitor_not_unique:
		smz->printError("Monitor names must be unique");
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
      	cout << sym << endl;
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

// Gives back (in id) the name location in the devices nametable
bool parser::parseDeviceName (name &id)
{
  symbol sym;
  int num;
  name symid;
  
  smz->getsymbol(sym,symid,num);
  switch(sym) { 
    case namesym:
      // Here is where the device name should be stored somewhere useful
      id = nm_devicez->lookup(nmz->getname(symid));
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

// Gives back (in id) the name location in the devices nametable - should semantic error check for repeated names
bool parser::parseDeviceName (name &id, bool &endOfDevices)
{
  symbol sym;
  int num;
  name symid;
  endOfDevices = 0;
  
  smz->getsymbol(sym,symid,num);
  switch(sym) { 
    case namesym:
	  // Check that device name is not already used - will return non-zero if already in use
	  if(nm_devicez->cvtname(nmz->getname(symid))) 
	  {
		errorHandling(device_not_unique);
		return PARSER_FAIL;
	  }
	  // Save in device nametable
      id = nm_devicez->lookup(nmz->getname(symid));      
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
// Semantic checking of parameters should occur here
bool parser::createDevice (device_type current_device_type, name id)
{
  int param_value;
  bool ok;
  asignal signal;
  name devicet_id;

  switch(current_device_type)
  {
    case AND:
      if(parseParam(param_value)) return PARSER_FAIL;
	  // Semantic check on parameter values
	  if(param_value < 2 || param_value >16) 
	  {
		errorHandling(inputs_two_to_sixteen);
		return PARSER_FAIL;
	  }
	  // Adds device to device table
      devicet_id = dtz->lookup(nm_devicez->getname(id),current_device_type,param_value);
      cout << "Created AND gate with " << param_value << " inputs, with name \"" << nm_devicez->getname(id) << "\".\n";
      dmz->makedevice (andgate, id, param_value, ok); 
      if (!ok){cout <<"error creating and gate"<<endl;}
      break;
      
    case NAND:
      if(parseParam(param_value)) return PARSER_FAIL;
	  // Semantic check on parameter values
	  if(param_value < 2 || param_value >16) 
	  {
		errorHandling(inputs_two_to_sixteen);
		return PARSER_FAIL;
	  }
	  // Adds device to device table
      devicet_id = dtz->lookup(nm_devicez->getname(id),current_device_type,param_value);
      cout << "Created NAND gate with " << param_value << " inputs, with name \"" << nm_devicez->getname(id) << "\".\n";
      dmz->makedevice (nandgate, id, param_value, ok); 
      if (!ok){cout <<"error creating nand gate"<<endl;}
      break;
      
    case OR:
      if(parseParam(param_value)) return PARSER_FAIL;
	  // Semantic check on parameter values
	  if(param_value < 2 || param_value >16) 
	  {
		errorHandling(inputs_two_to_sixteen);
		return PARSER_FAIL;
	  }
	  // Adds device to device table
      devicet_id = dtz->lookup(nm_devicez->getname(id),current_device_type,param_value);
      cout << "Created OR gate with " << param_value << " inputs, with name \"" << nm_devicez->getname(id) << "\".\n";
      dmz->makedevice (orgate, id, param_value, ok); 
      if (!ok){cout <<"error creating or gate"<<endl;}
      break;
   
    case NOR:
      if(parseParam(param_value)) return PARSER_FAIL;
	  // Semantic check on parameter values
  if(param_value < 2 || param_value >16) 
	  {
		errorHandling(inputs_two_to_sixteen);
		return PARSER_FAIL;
	  }
	  // Adds device to device table
      devicet_id = dtz->lookup(nm_devicez->getname(id),current_device_type,param_value);
      cout << "Created NOR gate with " << param_value << " inputs, with name \"" << nm_devicez->getname(id) << "\".\n";
      dmz->makedevice (norgate, id, param_value, ok); 
      if (!ok){cout <<"error creating nor gate"<<endl;}
      break;
      
    case XOR:
	  // Adds device to device table
      devicet_id = dtz->lookup(nm_devicez->getname(id),current_device_type,param_value);
      cout << "Created XOR gate with " << param_value << " inputs, with name \"" << nm_devicez->getname(id) << "\".\n";
      dmz->makedevice (xorgate, id, 2, ok);
      if (!ok){cout <<"error creating xor gate"<<endl;}
      break;
      
    case DTYPE:
      devicet_id = dtz->lookup(nm_devicez->getname(id),current_device_type,0);
      cout << "Created DTYPE, with name \"" << nm_devicez->getname(id) << "\".\n";
      dmz->makedevice (dtype, id, 0, ok); 
      if (!ok){cout <<"error creating dtype"<<endl;}
      break;
      
    case CLK:
      if(parseParam(param_value)) return PARSER_FAIL;
	  if(param_value<1)
	  {
		errorHandling(clk_param);
		return PARSER_FAIL;
	  }

      devicet_id = dtz->lookup(nm_devicez->getname(id),current_device_type,0);
      cout << "Created CLK with period of " << param_value << " and name \"" << nm_devicez->getname(id) << "\".\n";
      dmz->makedevice (aclock, id, param_value, ok); //THIS IS PROBABLY INCORRECT. SECOND VALUE SHOULD BE FREQUENCY, NOT PERIOD.
      if (!ok){cout <<"error creating clock"<<endl;}
      break;
      
    case SW:
      if(parseParam(param_value)) return PARSER_FAIL;
	  if(!(param_value == 0 || param_value == 1))
	  {
		errorHandling(switch_param);
		return PARSER_FAIL;
	  }

      devicet_id = dtz->lookup(nm_devicez->getname(id),current_device_type,0);
      cout << "Created SW with initial state " << param_value << " and name \"" << nm_devicez->getname(id) << "\".\n";
      signal = low;
      if (param_value){signal=high;}
      dmz->makedevice (aswitch, id, 0, ok);
      if (!ok) {cout << "Switch not created."<<endl;}
      dmz->setswitch (id, signal, ok);
      if (!ok) {cout << "Switch not found"<<endl;}
      break;

    default:
      cout << "Something has gone really wrong.\n";
  }
  return PARSER_PASS;
}

bool parser::parseConnInputName(name &devid, name &inpid, bool &endOfSection)
{
  symbol sym;
  int num;
  device_type dt;
  int no_of_inputs;
  string input;
  stringstream ss;
  
  // Retrieve device name
  smz->getsymbol(sym, devid, num);
  // Check it is valid
  switch(sym) { 
    case namesym:
      dt = dtz->gettype(nmz->getname(devid));    
      break;
    case closecurly:
      // Device section ended
      endOfSection = 1;
      return PARSER_PASS;
    default:
      // Generic device not defined error
      errorHandling(device_not_defined);
      return PARSER_FAIL;
  }
  
  // Needs implementing when devicetable changes
  no_of_inputs = 16;
  // Check against device table
  switch(dt)
  {
    case UNDEFINED:
      errorHandling(device_not_defined);
      return PARSER_FAIL;
    case NONE:
      cout << "Device table error!!\n";
      return PARSER_FAIL;
    case CLK:
    case SW:
      errorHandling(no_inputs);
      return PARSER_FAIL;
    case AND:
    case NAND:
    case OR:
    case NOR:
    case XOR:
    case DTYPE:
      break;
    default:
      cout << "Error in ConnInputName\n";
  }
  
  if(parseToken(fullstop)) return PARSER_FAIL;
  
  smz->getsymbol(sym,inpid,num);
  if (sym == namesym) {
    input = nmz->getname(inpid);
  }
  else {
    errorHandling(invalid_input);
    return PARSER_FAIL;
  }
  // Semantic checking of inputs 
  switch(dt)
  {
    case DTYPE:
      if(input.compare("data")==0) {return PARSER_PASS;} 
      else if(input.compare("clk")==0) {return PARSER_PASS;} 
      else if(input.compare("set")==0) {return PARSER_PASS;} 
      else if(input.compare("clear")==0) {return PARSER_PASS;} 
      else 
      {
        errorHandling(invalid_input);
        return PARSER_FAIL;
      }
    case XOR:
      if(input.compare("i1")==0) {return PARSER_PASS;}
      else if(input.compare("i2")==0) {return PARSER_PASS;}
      else 
      {
        errorHandling(invalid_input);
        return PARSER_FAIL;
      }
    case AND:
    case NAND:
    case OR:
    case NOR:
      for (int i=1;i<=16;i++)
      {
        ss.str("");
        ss << "i"<<i;
        if (input.compare(ss.str())==0){return PARSER_PASS;}  
      } 
      errorHandling(invalid_input);
      return PARSER_FAIL;
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

bool parser::parseConnOutputName(name &devid, name &outid)
{
  symbol sym;
  int num;
  device_type dt;
  string output;
  
  // Retrieve device name
  smz->getsymbol(sym, devid, num);
  // Check it is valid
  switch(sym) { 
    case namesym:
      dt = dtz->gettype(nmz->getname(devid));    
      break;
    default:
      // Generic device not defined error
      errorHandling(device_not_defined);
      return PARSER_FAIL;
  }
  
  // Check against device table
  switch(dt)
  {
    case UNDEFINED:
      errorHandling(device_not_defined);
      return PARSER_FAIL;
    case NONE:
      cout << "Device table error!!\n";
      return PARSER_FAIL;
    case CLK:
    case SW:
    case AND:
    case NAND:
    case OR:
    case NOR:
    case XOR:
    case DTYPE:
      break;
    default:
      cout << "Error in ConnOutputName\n";
  }
  
  // Semantic checking of dtype outputs
  switch(dt)
  {
    case DTYPE:
      if(parseToken(fullstop)) return PARSER_FAIL;
      smz->getsymbol(sym,outid,num);
      if (sym == namesym) {
        output = nmz->getname(outid);
      }
      else {
        errorHandling(invalid_output);
        return PARSER_FAIL;
      }
      if(output.compare("q")==0) {return PARSER_PASS;} 
      else if(output.compare("qbar")==0) {return PARSER_PASS;} 
      else 
      {
        errorHandling(invalid_output);
        return PARSER_FAIL;
      }
    default:
      // For all other devices the output name is blank
      outid = 0;
  }
  return PARSER_PASS;  
}

// Should just be a simple function call hopefully
bool parser::createConn(name dev1id,name dev2id,name inid,name outid)
{
  bool ok;
  netz->makeconnection(dev1id,inid,dev2id,outid,ok);
  if(!ok)
  {
    cout << "ERROR: Couldn't make connection\n";
    return PARSER_FAIL;
  }
  cout << "Connect " << nmz->getname(dev1id) << ", input " << nmz->getname(inid) << ", to " << nmz->getname(dev2id) << ", output " << nmz->getname(outid) << endl;
  return PARSER_PASS;
}

// Gives back (in id) the name location in the monitor nametable
bool parser::parseMonitorName (name &id)
{
  symbol sym;
  int num;
  name symid;
  
  smz->getsymbol(sym,symid,num);
  switch(sym) { 
    case namesym:	  // Check that monitor name is not already used - will return non-zero if already in use
      if(nm_monitorz->cvtname(nmz->getname(symid))) 
      {
      errorHandling(monitor_not_unique);
      return PARSER_FAIL;
      }
      // Here is where the monitor name should be stored somewhere useful
      id = nm_monitorz->lookup(nmz->getname(symid));
      break;
    case closecurly:
      // Error for at least one monitor must be defined
      errorHandling(one_monitor_required);
      return PARSER_FAIL;
    case numsym:
      // All names must begin with a letter
      errorHandling(names_begin_letter);
      return PARSER_FAIL;
    default:
      // Generic expected a name here error
      errorHandling(monitor_name_expected);
      return PARSER_FAIL;
  }
  
  return PARSER_PASS;
}

// Gives back (in id) the name location in the monitors nametable - should semantic error check for repeated names
bool parser::parseMonitorName (name &id, bool &endOfSection)
{
  symbol sym;
  int num;
  name symid;
  endOfSection = 0;
  
  smz->getsymbol(sym,symid,num);
  switch(sym) { 
    case namesym:
      // Here is where the device name should be stored somewhere useful
      // SEMANTIC CHECKING TO OCCUR here
      id = nm_monitorz->lookup(nmz->getname(symid));      
      break;
    case closecurly:
      // Monitors section ended
      endOfSection = 1;
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

bool parser::createMonitor(name monitorName, name dev2id, name outid) 
{
	cout << "Create monitor " << nm_monitorz->getname(monitorName) << " monitoring " << nmz->getname(dev2id) << ", output " << nmz->getname(outid) << endl;
	return PARSER_PASS
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
  nm_devicez = new names();
  dtz = new devicetable();
}







