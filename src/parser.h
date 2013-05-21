#ifndef parser_h
#define parser_h

#include "names.h"
#include "network.h"
#include "devices.h"
#include "monitor.h"
#define PARSER_TEST
#include <iostream>
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
  enum error {unknown, no_opening_brace, no_devices, one_device_required, names_begin_letter, device_name_expected, number_param_expected, not_valid_device};
  enum device_type {AND, NAND, OR, NOR, XOR, DTYPE, CLK, SW};
  
  /* put other stuff that the class uses internally here */
  /* also declare internal functions                     */
  void errorHandling(error error_num);
  // Function to parse consym, semicol, equals, openparen, closeparen, opencurly and closecurly
  bool parseToken(symbol token);
  // Function to parse DEV, INIT, CONN and MON
  bool parseSectionHeader(symbol header);
  // Function to parse device names
  bool parseDeviceName(); 
  // Detects device type and return in parameter
  bool parseDeviceType(device_type &current_device_type);
  // Gives the value of the device definition parameter in the parameter
  bool parseParam(int &param_value);

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

