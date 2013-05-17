#include <iostream>
#include "../../parser.h"

int main (int argc, char **argv) 
{
  ifstream inf; 
  
  names* nt = new names();
  
  namestring str1 = "{";
  namestring str2 = "}";
  
  nt->lookup(str1);
  
  parser* p = new parser(nt);
  bool success = p->readin();
  cout << success << endl;
}
