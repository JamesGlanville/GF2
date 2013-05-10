#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
using namespace std;


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
	typedef string namestring;

	ifstream inf;
	char ch;
	bool eofile;
	int number=0;

	if (argc!=2) { 
		cout << "Usage:  " << argv[0] << " [filename]" << endl;
		exit(1);
	}

	inf.open(argv[1]);
	if (!inf) {
		cout << "Error: cannot open file " << argv[1] << " for reading " << endl;
		exit(1);
	}

//	eofile = (inf.get(ch)==0);

	while (!eofile) {
		getnumber(&inf,ch,eofile,number);
//		cout <<ch;
//		if (isspace(ch)) cout << ".";
//		else cout << ch;
		eofile = (inf.get(ch) == 0);
	}
	inf.close();
}
