#ifndef parser_h
#define parser_h

#include "names.h"
#include "devicetable.h"
#include "network.h"
#include "devices.h"
#include "monitor.h"
//#define PARSER_TEST
#include <iostream>
#include <sstream>
#include "scanner.h"
using namespace std;

#define PARSER_PASS 0;
#define PARSER_FAIL 1;

class parser {
  #ifndef PARSER_TEST
  network* netz; // instantiations of various classes for parser to use.
  devices* dmz; 
  monitor* mmz;
  #endif
  scanner* smz;
  names* nmz;
  names* nm_devicez;
  names* nm_monitorz;
  devicetable* dtz;
  enum error {unknown, no_opening_brace, no_devices, one_device_required, 
              names_begin_letter, device_name_expected, number_param_expected, 
              not_valid_device, number_expected, device_not_defined, invalid_input,
              no_inputs, more_inputs_than_defined, invalid_output, one_monitor_required,
			  monitor_name_expected, inputs_two_to_sixteen, clk_param, switch_param,
              device_not_unique, monitor_not_unique};
  
  /* put other stuff that the class uses internally here */
  /* also declare internal functions                     */
  void errorHandling(error error_num);
  // Function to parse consym, semicol, equals, openparen, closeparen, opencurly and closecurly
  bool parseToken(symbol token);
  // Function to parse DEV, INIT, CONN and MON
  bool parseSectionHeader(symbol header);
  // Function to parse a device name and add device name to device nametable
  bool parseDeviceName(name &id); 
  // Overloaded function to also check for the closing curly brace that indicates end of section
  bool parseDeviceName(name &id, bool &endOfDevices);
  // Detects device type and return in parameter
  bool parseDeviceType(device_type &current_device_type);
  // Gives the value of the device definition parameter in the parameter
  bool parseParam(int &param_value);
  // Parses parameter if necessary and then creates a device of give type and name
  // For CLK and SW it stores the names
  bool createDevice(device_type current_device_type, name id);
  // Parses a number (keeps everything neat) - POSSIBLY NOT REQUIRED CHECK AT END
  bool parseNumber(int &num);
  // Parses the connection input 
  bool parseConnInputName(name &devid, name &inpid, bool &endOfSection); 
  // Parse the connection output
  bool parseConnOutputName(name &devid, name &outid);
  // Create the connection
  bool createConn(name dev1id,name dev2id,name inid,name outid);
  // Parses monitor name
  bool parseMonitorName(name &id);
  // Parses monitor name and checks for closing curly
  bool parseMonitorName(name &id, bool &endOfSection);
  // Creates monitor
  bool createMonitor(name monitorName, name dev2id, name outid); 


 public:
  bool readin();
    /* Reads the definition of the logic system and builds the             */
    /* corresponding internal representation via calls to the 'Network'    */
    /* module and the 'Devices' module.                                    */

  parser (
          #ifndef PARSER_TEST
          network* network_mod, 
          devices* devices_mod,
          monitor* monitor_mod, 
          #endif
          scanner* scanner_mod, 
          names* name_table);
    /* the constructor takes pointers to various other classes as parameters */
};

#endif /* parser_h */

