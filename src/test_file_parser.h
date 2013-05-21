#ifndef test_file_parser_h
#define test_file_parser_h

#include "names.h"
#include <iostream>
#include "scanner.h"
using namespace std;

#define PARSER_PASS 0;
#define PARSER_FAIL 1;

/* Pre-parses test files that contain an ={ DESCRIPTION }= at the start */

class test_file_parser {
  scanner* smz;
  names* nmz;

 public:
  bool testFileDefinition();

  test_file_parser (scanner* scanner_mod, 
          names* name_table);
    /* the constructor takes pointers to various other classes as parameters */
};

#endif /* test_file_parser_h */
