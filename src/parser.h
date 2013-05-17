#ifndef parser_h
#define parser_h

#include "names.h"
#include "scanner.h"
#include "network.h"
#include "devices.h"
#include "monitor.h"


using namespace std;

class parser {
  network* netp; // instantiations of various classes for parser to use.
  devices* dmp; 
  monitor* mmp;
  scanner* smp;
  names* name_listp;
  enum error {no_opening_brace};
  
  /* put other stuff that the class uses internally here */
  /* also declare internal functions                     */
  void errorhandling(error error_num);

 public:
  bool readin ();
    /* Reads the definition of the logic system and builds the             */
    /* corresponding internal representation via calls to the 'Network'    */
    /* module and the 'Devices' module.                                    */

  parser (/*network* network_mod, devices* devices_mod,
	  monitor* monitor_mod, scanner* scanner_mod,*/ names* scanner_output);
    /* the constructor takes pointers to various other classes as parameters */
};

#endif /* parser_h */

