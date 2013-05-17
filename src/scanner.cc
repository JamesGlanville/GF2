#include "scanner.h"

using namespace std;

scanner::scanner(string filename)
{
	inf.open(filename.c_str());
	if (!inf) {
		cout << "Error: cannot open file " << filename << " for reading " << endl;
		exit(1);
	}
}

void scanner::nextChar()
{
	eofile = (inf.get(curch)==0);
	
	if (curch == '\n')
	{
		currentline.clear();
		linenum++;
	}
	else
	{
		currentline.push_back(curch);
	}
	
	curch = tolower(curch); //From here on, everything is lower case.

}

void scanner::getsymbol( symbol& s, name & id, int & num)
{
	string str;
	
	while(1)
	{
		nextChar();
		
		if (eofile) {s=eofsym;return;}
	
		if (isblank(curch))
		{}
		else if (isalpha(curch))
		{
			str.push_back(curch);
			while (isalnum(curch) && eofile == false)
			{
				str.push_back(curch);
				nextChar();
			}
			lookup(str);	
			
		}
		else if (isdigit(curch))
		{
			
		}
		else
		{
			switch (curch) {
				case '<': nextChar(); if (curch == '=') {s=consym; return;} s=badsym; /*cout << "expected = after <..." << endl;*/ return;
				case '=': s = equals; return;
				case ';': s = semicol; return;
				default : s = badsym; return;
		}
	}
}

void scanner::printError(string errordesc)
{
	//For now:
	cout << "Error. " << errordesc << " at line " << linenum << "at location (startat0) " << currentline.size-1 << endl;
	
}

void getname (ifstream *infp, char &curch, bool &eofile, namestring &str)
{
str.clear(); //Sets str to zero-length string.

while (isalnum(curch) && eofile == false)
{
str.push_back(curch); //Appends curch to str.
eofile = (infp->get(curch)==0);
}

if (str.length() > 0)
{
if (str.length() <=8)
{
cout << "Name: " << str << endl ;
lookup(str);
}
else
{
cout << "Warning: name \'" << str <<"\' was truncated." << endl;
cout << "Name: " << str.substr(0,8) << endl; //Prints only first 8 chars of str.
lookup(str.substr(0,8));
}
}
}
