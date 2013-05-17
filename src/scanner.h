#ifndef scanner_h
#define scanner_h

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include "names.h"
#include <wx/wx.h>


using namespace std;

typedef enum {namesys, namesym, numsys, devsym, consym, monsym, endsym, numsym, comma, semicol, equals, badsym, eofsym,openparen,closeparen,opencurly, closecurly} symbol;
//typedef string name;
/*
scanner (names* names_mod, const char* defname);

~scanner();
*/





class scanner {
	char curch; //Current input character
	string currentline;
	ifstream inf;
	void nextChar();
	bool eofile;
	int linenum;
	int commentnest; //This stores the number of opening /*'s - closing */'s.
	void rewind();
	void doComments();
	names* nametable;



 public:
	void getsymbol (symbol& s, name& id, int& num);
	scanner(names* nametable,const wxCharBuffer blah);
	void printError (string errordesc);

};



#endif /* scanner_h */

