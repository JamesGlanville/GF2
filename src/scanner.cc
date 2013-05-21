#include "scanner.h"

//#define SCANNERTEST

using namespace std;

scanner::scanner(names* name, ifstream * file)
{
	nametable = name;
	inf=file;
	linenum=0;
	currentline="";
	
#ifdef SCANNERTEST
	string lookuptable[]={"namesym","numsym","DEV","INIT","CONN","MON","<=",";","=","badsym","EOF","(",")","{","}"};
	//^^^^^ make sure this is updated along with the enum in scanner.h
	symbol s;
	int id;
	int num;
	while(!eofile){
		getsymbol( s,id,num);
		cout << "Symbol: " << lookuptable[s] ;
		if (s==namesym){cout << " Id: " << nametable->getname(id); }
		if (s==numsym){cout << " Num: " << num;}
		cout << endl;
	}
#endif
}

scanner::~scanner()
{
	inf->close();
}

void scanner::rewind()	//Does the opposite of nextChar (reverses its effect)
{						//I suspect this is one of the more bug-prone methods.
	inf->seekg((int)inf->tellg()-1); //Move file pointer back a place.

	if (currentline.size() > 0) //This is probably always the case, problems if not.
	{
		currentline = currentline.substr(0,currentline.size()-1);
	}
}

void scanner::nextChar()
{
	eofile = (inf->get(curch)==0);
	if (curch == '\n') //Seems to happen twice in a row, CR+LF?
	{
//		cout << currentline << endl;
		currentline.clear();
		linenum++;
//		cout << linenum;
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
	
	while(1) //While loop exists so that blanks/newlines can be ignored.
	{
		nextChar();
		
		if (eofile) {s=eofsym;return;}
	
		if (isblank(curch))
		{} //Will loop round to nextChar();
		else if (curch == '\n')
		{}
		else if (isalpha(curch))
		{
			while ((isalnum(curch) || curch=='.')&& eofile == false)
			{
				str.push_back(curch);
				nextChar();
			}
			rewind(); //gone too far, want to leave this char to be read again.
			if (str=="devices") 	{s=DEV;return;} //Should this use nametable?
			if (str=="init")    	{s=INIT;return;}
			if (str=="connections")	{s=CONN;return;}
			if (str=="monitors")	{s=MON;return;}
			id = nametable->lookup(str);

			s = namesym;
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
				case '/': nextChar(); if (curch =='*') {doComments();break;} else{s=badsym; return;}
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
	cout << "Error: " << errordesc << " on line " << linenum << " at location (startat0) " << max((int)currentline.size()-1,0) << endl;
}
