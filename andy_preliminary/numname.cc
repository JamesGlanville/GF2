#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <cstring>
using namespace std;

const int maxlength = 8;	// max chars in namestring
typedef string namestring;	// define namestring type

void skipspaces (ifstream *infp, char &curch, bool &eofile);
void getnumber (ifstream *infp, char &curch, bool &eofile, int &number);
void getname (ifstream *infp, char &curch, bool &eofile, namestring &str);

int main (int argc, char **argv)
{
  ifstream inf;                // input file
  char ch;                     // current character
  bool eofile;                 // true when end of file is reached
  int number;		       // current number.
  namestring str;	       // current name

  if (argc != 2) {
    cout << "Useage:         " << argv[0] << " [filename]" << endl;
    exit(1);
  }

  inf.open(argv[1]);
  if (!inf) {
    cout << "Error: cannot open file " << argv[1] << " for reading" << endl;
    exit(1);
  }

  /*
  // main loop: get to first number, get number then print it.
  while (!eofile) {
    inf.get(ch);
    skipspaces(&inf, ch, eofile);
    if (eofile) break;
    getnumber(&inf, ch, eofile, number);
    cout << number << endl;
  }
  */

  // main loop: get each namestring and print it.
  eofile = false;
  while(!eofile) {
    inf.get(ch);
    skipspaces(&inf, ch, eofile); // get to first letter of name
    if (eofile) break;
    if (isalpha(ch)) {
      getname(&inf, ch, eofile, str);
      cout << str << endl;
    }
    else if (isdigit(ch)) {
      getnumber(&inf, ch, eofile, number);
      cout << number << endl;
    }
  }

  inf.close();
}

void getname (ifstream *infp, char &curch, bool &eofile, namestring &str) {
  int strlength = 0;
  str = "";
  while (isalnum(curch) && eofile == 0) {
    if (strlength < maxlength)
      str = str + curch;
    strlength++;
    eofile = (infp->get(curch) == 0);
  }
  if (strlength > maxlength)
    cout << "Error: Namestring " << str << " is too long." << endl;
  return;
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
