#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
using namespace std;

void skipspaces (ifstream *infp, char &curch, bool &eofile);
void getnumber (ifstream *infp, char &curch, bool &eofile, int &number);

int main (int argc, char **argv)
{
  ifstream inf;                // input file
  char ch;                     // current character
  bool eofile;                 // true when end of file is reached
  int number;		       // current number.

  if (argc != 2) {
    cout << "Useage:         " << argv[0] << " [filename]" << endl;
    exit(1);
  }

  inf.open(argv[1]);
  if (!inf) {
    cout << "Error: cannot open file " << argv[1] << " for reading" << endl;
    exit(1);
  }

  // main loop: get to first number, get number then print it.
  while (!eofile) {
    inf.get(ch);
    skipspaces(&inf, ch, eofile);
    getnumber(&inf, ch, eofile, number);
    cout << number << endl;
  }

  inf.close();
}


void getnumber (ifstream *infp, char &curch, bool &eofile, int &number)
{
  number = 0;
  while (isdigit(curch) && eofile == 0) {
    number = (number * 10) + atoi(&curch);
    eofile = (infp->get(curch) == 0);
  }
  return;
}


void skipspaces (ifstream *infp, char &curch, bool &eofile)
{
  while (isspace(curch) && eofile == 0) {
    eofile = (infp->get(curch) == 0);
  }
  return;
}

