#ifndef devicetable_h
#define devicetable_h

#include <string>
//#include "parser.h"
#include <vector>
#include <utility>
enum device_type {NONE=-1,AND, NAND, OR, NOR, XOR, DTYPE, CLK, SW,UNDEFINED};

using namespace std;

typedef int name;
typedef string namestring;
typedef int numinputs;

struct devicestruct{
	namestring ns;
	device_type dt;
	numinputs ni;};

class devicetable{
	vector <devicestruct> device_table;

public:
	string getswitch(int swnum);

  name lookup (namestring str, device_type type,numinputs numinput);
    /* Returns the internal representation of the name given in character  */
    /* form.  If the name is not already in the name table, it is          */
    /* automatically inserted.                                             */
    
	device_type gettype(namestring str);
	numinputs getinputs(namestring str);

#ifdef GOAWAY 
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

 // devicetable ();
    /* names initialises the name table.  This procedure is called at      */
    /* system initialisation before any of the above procedures/functions  */
    /* are used.                                                           */
#endif
};

#endif /* names_h */



