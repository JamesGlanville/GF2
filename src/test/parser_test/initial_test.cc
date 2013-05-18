#include <iostream>
#include "../../parser.h"
#include <fstream>

int main (int argc, char **argv) 
{
  ifstream inf; 
  
  names* nt = new names();
  
  namestring str1 = "{";
  namestring str2 = "devices";
  namestring str3 = "{";
  
  nt->lookup(str1);
  nt->lookup(str2);
  nt->lookup(str3);
    
  parser* p = new parser(nt);
  bool success = p->readin();
  cout << success << endl;
}
