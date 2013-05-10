#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
using namespace std;

void skipspaces(ifstream *infp, char &curch, bool &eofile)
{
	while (curch == ' ' && eofile == false)
	{
		eofile  = (infp->get(curch)==0);
	}


}

int main (int argc, char **argv)
{

	ifstream inf;
	char ch;
	bool eofile;

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
		skipspaces(&inf,ch,eofile);
		cout <<ch;
//		if (isspace(ch)) cout << ".";
//		else cout << ch;
		eofile = (inf.get(ch) == 0);
	}
	
	inf.close();
}
