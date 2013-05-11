#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
using namespace std;

typedef string namestring;

void getname (ifstream *infp, char &curch, bool &eofile, namestring &str)
{
//	int length = 0;
	str.clear();
	
	while ((((curch-'0') >= 0 && (curch-'0') <10) || ((curch-'A')>=0 && (curch-'z') <= 0)) && eofile == false)
	{
//		if (length<8) 
//		{
			str.push_back(curch);
//		}
//		else
//		{
//			cout << "Warning, character \"" << (char)curch << "\" not saved." << endl;
//		}
//		length++;
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
			cout << "Name: " << str.substr(0,8) << endl;
		}
	}
}

void getnumber (ifstream *infp, char &curch, bool &eofile, int &number)
{
	//int number = 0;//curch - '0';
	number= 0;
	int digit = 0;

	while ((curch-'0') >= 0 && (curch-'0') <10 && eofile == false)
	{
//		cout << "n"<<number<<"d"<<digit<<"c"<<(curch-'0')<<endl;
		number = number *10;
		number += (curch - '0');
//		number += (curch - '0')*(10^digit);
		digit++;
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
