#include "scanner.h"

using namespace std;

scanner::scanner(names* name, const wxCharBuffer blah)
{
	symbol s;
	int id;
	int num;
	
	nametable = name;
	string filename = "/home/james/GF2/specification/sample_definition";
	inf.open(filename.c_str());
	if (!inf) {
		cout << "Error: cannot open file " << filename << " for reading " << endl;
		exit(1);
	}
	
	while(!eofile){
	getsymbol( s,id,num);

	cout << "Symbol: " << s ;
	if (s==namesym){cout << " Id: " << nametable->getname(id); }
	if (s==numsym){cout << " Num: " << num;}
	cout << endl;
}
	
}

void scanner::rewind() //Does the opposite of nextChar (reverses its effect)
{
	inf.seekg(inf.tellg());
//	currentline.pop_back();
	if (currentline.size() > 0)
	{
		currentline = currentline.substr(0,currentline.size()-1);
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
	string str="";
	num = 0;
	
	while(1)
	{
		nextChar();
		
		if (eofile) {s=eofsym;return;}
	
		if (isblank(curch))
		{}
		else if (curch == '\n')
		{}
		else if (isalpha(curch))
		{
			//str.push_back(curch);
			while (isalnum(curch) && eofile == false)
			{
				str.push_back(curch);
				nextChar();
			}
			rewind();
			s = namesym;
			id = nametable->lookup(str);
			return;
			
		}
		else if (isdigit(curch))
		{
			while (isdigit(curch) && eofile == false)
			{
				num = (num * 10) + atoi(&curch);
				nextChar();
			}
			rewind();
			s = numsym;
			return;			
		}
		else
		{
			switch (curch) {
				case '<': nextChar(); if (curch == '=') {s=consym; return;} s=badsym; /*cout << "expected = after <..." << endl;*/ return;
				case '=': s = equals; return;
				case ';': s = semicol; return;
				case '{': s = opencurly; return;
				case '}': s = closecurly; return;
				case '(': s = openparen; return;
				case ')': s = closeparen; return;
				case '/': nextChar(); if (curch =='*') {doComments();return;} s=badsym; return;
				default : s = badsym; return;}
		}
	}
} 

void scanner::doComments()
{
	//This method just skips through the code waiting for comments to end. It is called when the / * has already been read.
	commentnest=1;
	char prevchar;
	nextChar();
	while (commentnest > 0)
	{
		prevchar = curch;
		nextChar();
		
		if (eofile) {cout << "Unexpected end of file, comments are not terminated...." << endl;}
		
		if (curch == '*' && prevchar == '/')
		{
			commentnest++;
			nextChar();
		}
		else if (curch == '/' && prevchar == '*')
		{
			commentnest--;
			nextChar();
		}
	}
}

void scanner::printError(string errordesc)
{
	//For now:
	cout << "Error. " << errordesc << " at line " << linenum << "at location (startat0) " << currentline.size()-1 << endl;
	
}
/*
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
nametable.lookup(str);
}
else
{
cout << "Warning: name \'" << str <<"\' was truncated." << endl;
cout << "Name: " << str.substr(0,8) << endl; //Prints only first 8 chars of str.
nametable.lookup(str.substr(0,8));
}
}
}
*/
