#include "test_file_parser.h"

using namespace std;

bool test_file_parser::testFileDefinition (void) 
{
  symbol sym;
  name id;
  int num;
  bool error = 0;
  
  smz->getsymbol(sym,id,num);
  if(sym != equals)
  {
    cout << sym << endl;
    error = 1;
  }
  
  smz->getsymbol(sym,id,num);
  if(sym != opencurly)
  {
    cout << sym << endl;
    error = 1;
  }
  
  smz->getsymbol(sym,id,num);
  while(sym == namesym)
  {
    cout << " " << nmz->getname(id) << " ";
    smz->getsymbol(sym,id,num);
  }
  
  if(sym != closecurly)
  {
    cout << sym << endl;
    error = 1;
  }
  
  smz->getsymbol(sym,id,num);
  if(sym != equals) 
  {
    cout << sym << endl;
    error = 1;
  }
  
  if(error) 
  {
    cout << endl << "No file description included or badly formatted description." << endl;
    return PARSER_FAIL; 
  }
  
  cout << endl;
  return PARSER_PASS;  
}

test_file_parser::test_file_parser (scanner* scanner_mod,
                                    names* name_table)
{
  smz = scanner_mod;
  nmz = name_table;
}


