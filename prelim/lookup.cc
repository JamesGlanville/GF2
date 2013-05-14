#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <vector>
using namespace std;

const int maxlength = 8;	// max chars in namestring
typedef string namestring;	// define namestring type
vector<namestring> nametable;	// vector storing namestrings
typedef unsigned int name;	// represents name (index to nametable)

void skipspaces (ifstream *infp, char &curch, bool &eofile);
void getnumber (ifstream *infp, char &curch, bool &eofile, int &number);
void getname (ifstream *infp, char &curch, bool &eofile, namestring &str);
name lookup (namestring str);
void printnames ();
void writename (name id);

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

  // main loop: get each namestring and print it.
  eofile = false;
  while(!eofile) {
    inf.get(ch);
    skipspaces(&inf, ch, eofile); // get to first letter of name
    if (eofile) break;
    if (isalpha(ch)) {
      getname(&inf, ch, eofile, str);
    }
    else if (isdigit(ch)) {
      getnumber(&inf, ch, eofile, number);
    }
  }
  
  printnames();

  inf.close();
}

void writename (name id) {
  cout << id <<":" << nametable[id] << endl;
}

void printnames () {
  for (int i = 0; i < nametable.size(); i++) {
    //    cout << nametable[i] << endl;
    //       cout << "printing " << i << endl;
    writename(i);
  }
}

name lookup (namestring str) {
  for(int i = 0; i < nametable.size(); i++) {
    if (nametable[i] == str) {
      return i;
    }
  }
  nametable.push_back(str);
  return nametable.size() - 1;
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
  lookup(str);
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
