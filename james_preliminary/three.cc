#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
//#include <map>
//#include <utility>
//#include <tuple>
#include <vector>
#include "check.h"
using namespace std;

typedef string namestring;
/*
typedef vector< tuple <int, string>> nl;
typedef int name_index;

nl name_list;
nl.push_back(tuple<int,string>(21,"JIM"));
*/

//map <int, string> name_table;

vector <string> name_list;

/*name_index lookup (namestring str)
{
	
}*/

void print_name_table()
{
	for (int i=0; i<name_list.size(); i++)
	{
		cout << "Index: " << i << " ";
		cout << "Name: " << name_list[i] << endl;
	}
//	for (nl::const_iterator i = name_list.begin(); i != name_list.end(); ++i)
//	{
//		cout << "Index: " << i->get<0>();
//		cout << " Name: " << i->get<1>() << endl;
//	}
}

void getname (ifstream *infp, char &curch, bool &eofile, namestring &str)
{
	str.clear(); //Sets str to zero-length string.
	
	while ((((curch-'0') >= 0 && (curch-'0') <10) || ((curch-'A')>=0 && (curch-'z') <= 0)) && eofile == false)
	{
		str.push_back(curch); //Appends curch to str.
		eofile = (infp->get(curch)==0);
	}
	
	if (str.length() > 0)
	{
		if (str.length() <=8)
		{
			cout << "Name: " << str << endl ;
		}
		else 
		{
			cout << "Warning: name \'" << str  <<"\' was truncated." << endl;
			cout << "Name: " << str.substr(0,8) << endl; //Prints only first 8 chars of str.
		}
	}
}

void getnumber (ifstream *infp, char &curch, bool &eofile, int &number)
{
	number= 0;
//	int digit = 0;

	while ((curch-'0') >= 0 && (curch-'0') <10 && eofile == false)
	{
		number = number *10; //Decimal left shift by 1 place.
		number += (curch - '0'); //Fill in least significant decimal place.
//		digit++;
		eofile = (infp->get(curch)==0);
	}	

	if (number !=0){cout << "Number was: " << number << endl;}

}

void skipspaces(ifstream *infp, char &curch, bool &eofile)
{
	while (curch == ' ' && eofile == false)
	{
		eofile  = (infp->get(curch)==0);
	}
}

int main (int argc, char **argv)
{
	const int maxlength=8;

	ifstream inf;
	char ch;
	bool eofile;
	int number=0;
	namestring str;

	if (argc!=2) { 
		cout << "Usage:  " << argv[0] << " [filename]" << endl;
		exit(1);
	}

	inf.open(argv[1]);
	if (!inf) {
		cout << "Error: cannot open file " << argv[1] << " for reading " << endl;
		exit(1);
	}

	eofile = (inf.get(ch)==0);

	while (!eofile) {
		getname(&inf,ch,eofile,str);
//		getnumber(&inf,ch,eofile,number);
//		cout <<ch;
//		if (isspace(ch)) cout << ".";
//		else cout << ch;
		eofile = (inf.get(ch) == 0);
	}
	inf.close();
}
