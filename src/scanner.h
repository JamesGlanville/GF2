#ifndef scanner_h
#define scanner_h

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>

using namespace std;

typedef enum {namesys, numsys, devsym, consym, monsym, endsym, comma, semicol, equals, badsym, eofsym} symbol;
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



 public:
	void getsymbol (symbol& s, name& id, int& num);
	scanner(string filename);
	void printError (string errordesc);

};



#endif /* scanner_h */

