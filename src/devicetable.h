#ifndef devicetable_h
#define devicetable_h

#include <string>
#include "parser.h"
#include <vector>
#include <utility>

using namespace std;

typedef int name;
typedef string namestring;

class devicetable{
	vector<pair<namestring,device_type> > device_table;

public:
  name lookup (namestring str, device_type type);
    /* Returns the internal representation of the name given in character  */
    /* form.  If the name is not already in the name table, it is          */
    /* automatically inserted.                                             */
    
	device_type gettype(namestring str);


  name cvtname (namestring str);
    /* Returns the internal representation of the name given in character  */
    /* form.  If the name is not in the name table then 'blankname' is     */
    /* returned.                                                           */

  void writename (name id);
    /* Prints out the given name on the console                            */
    
  string getname (name id);
    /* Returns the string */

  int namelength (name id);
    /* Returns length ie number of characters in given name                */

  devicetable (void);
    /* names initialises the name table.  This procedure is called at      */
    /* system initialisation before any of the above procedures/functions  */
    /* are used.                                                           */
};

#endif /* names_h */


