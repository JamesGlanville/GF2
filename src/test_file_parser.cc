#include "test_file_parser.h"

using namespace std;

bool test_file_parser::testFileDefinition (void) 
{
  symbol sym;
  name id;
  int num;
  bool error = 0;
  
  cout << "Expected errors: \n";
  
  smz->getsymbol(sym,id,num);

  if(sym != badsym)
  {
    cout << sym << endl;
    error = 1;
  }
  
  smz->getsymbol(sym,id,num);
  while(sym != badsym)
  {
    switch (sym)
    {
      case namesym:
        cout << nmz->getname(id) << " ";
        break;
      case openparen:
        cout << "(";
        break;
      case closeparen:
        cout << ")";
        break;
      case consym:
        cout << "<=";
        break;
      case semicol:
        cout << ";";
        break;
      case equals:
        cout << "=";
        break;
      case opencurly:
        cout << "{";
        break;
      case closecurly:
        cout << "}";
        break;
      case numsym:
        cout << num;
        break;
      case fullstop:
        cout << endl;
        break;
      case DEV:
        cout << "DEVICES ";
        break;
      case CONN:
        cout << "CONNECTIONS ";
        break;
      case MON:
        cout << "MONITORS ";
        break;
    }
    smz->getsymbol(sym,id,num);
  }
  
  if(error) 
  {
    cout << endl << "No file description included or badly formatted description." << endl;
    return PARSER_FAIL; 
  }
  
  cout << endl << endl;
  return PARSER_PASS;  
}

test_file_parser::test_file_parser (scanner* scanner_mod,
                                    names* name_table)
{
  smz = scanner_mod;
  nmz = name_table;
}


