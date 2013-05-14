#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <vector>
#include "check.h"
using namespace std;

typedef string namestring;
typedef int name_index;
typedef unsigned int length;

vector <namestring> name_list;

length namelength (name_index id)
{
	if (id < name_list.size())
	{
		cout << name_list[id].length();
	}
	else
	{
		cout << "Error, index out of range." << endl;
	}	
}

void writename (name_index id)
{
	if (id < name_list.size())
	{
		cout << name_list[id];
	}
	else
	{
		cout << "Error, index out of range." << endl;
	}
}

name_index lookup (namestring str)
{
	
	if (find(name_list.begin(), name_list.end(), str) != name_list.end())
	{
		return distance(name_list.begin(),find(name_list.begin(), name_list.end(), str));
	}
	else
	{
		name_list.push_back(str);
		return name_list.size()-1;
	}
}

void print_name_table()
{
	for (vector<string>::iterator it = name_list.begin(); it!= name_list.end(); it++)
	{
		cout << "Index: " << distance(name_list.begin(),it) << " ";
		cout << "Name: " << *it << endl;
	}
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
			lookup(str);
		}
		else 
		{
			cout << "Warning: name \'" << str  <<"\' was truncated." << endl;
			cout << "Name: " << str.substr(0,8) << endl; //Prints only first 8 chars of str.
			lookup(str.substr(0,8));
		}
	}
}

void getnumber (ifstream *infp, char &curch, bool &eofile, int &number)
{
	number= 0;

	while ((curch-'0') >= 0 && (curch-'0') <10 && eofile == false)
	{
		number = number *10; //Decimal left shift by 1 place.
		number += (curch - '0'); //Fill in least significant decimal place.
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
	print_name_table();

}
