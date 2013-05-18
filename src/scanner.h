#ifndef scanner_h
#define scanner_h

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include "names.h"

using namespace std;

typedef enum {namesym, numsym, DEV, INIT, CONN, MON, consym, semicol, equals, badsym, eofsym, openparen, closeparen, opencurly, closecurly} symbol;
//^^^ This must be changed simultaneously with the lookuptable in scanner.cc

class scanner {
	char curch; //Current input character
	string currentline;
	ifstream inf;
	bool eofile;
	int linenum;
	int commentnest; //This stores the number of opening /*'s minus closing */'s.
	names* nametable;
	
	void nextChar();
	void rewind();
	void doComments();	

 public:
	scanner(names* nametable, const char * filename);
	~scanner();
	void getsymbol (symbol& s, name& id, int& num);
	void printError (string errordesc);
};

#endif /* scanner_h */
