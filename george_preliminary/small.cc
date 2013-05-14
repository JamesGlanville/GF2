#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>
#include "check.h"
using namespace std;
typedef string namestring;
typedef int name;
typedef unsigned int length;
const int maxlength = 8;
vector<namestring> nametable;

void skipspaces (ifstream *infp, char &curch, bool &eofile);
void getnumber (ifstream *infp, char &church, bool &eofile, int &number);
void getname (ifstream *infp, char &curch, bool &eofile, name &strindex);
name lookup (namestring str);
void printnametable ();
void writename (name id);
length namelength (name id);

int main (int argc, char **argv)
{
  ifstream inf;     // input file
  char ch;          // current character
  bool eofile;       // true when end of file is reached
  int number;     // stores the current number
  namestring str; // stores current name
  name index;
  length  strlength;
  check checker;

  if (argc != 2) {
    cout << "Usage:         " << argv[0] << "   [filename]" << endl;
    exit(1);
  }

  inf.open(argv[1]);
  if (!inf) {
    cout << "Error: cannot open file  " << argv[1] << "  for reading" << endl;
    exit(1);
  }

  while(!eofile) {
    inf.get(ch);
    skipspaces(&inf,ch,eofile);
    getname(&inf, ch, eofile, index);
    if(eofile) break;
    writename(index);
    checker.ckstr(nametable[index]);
    strlength = namelength(index);
    //cout << strlength << " ";
  }

  /* exercise 2
  while (!eofile) {
    inf.get(ch);
    skipspaces (&inf,ch,eofile);
    if (eofile == 0) {
      getnumber(&inf,ch,eofile,number);
      cout << number << endl;
    }
    }*/

  printnametable();
  
  //writename(4);
  //cout << namelength(4);
  inf.close();
}

void skipspaces (ifstream *infp, char &curch, bool &eofile)
{
  while (isspace(curch) && eofile == 0) {
    //cout << "Skipping space" << endl;
    eofile = (infp->get(curch)==0);
  }
  return;
}

void getnumber (ifstream *infp, char &curch, bool &eofile, int &number)
{
  number = 0;
  
  while(eofile == 0 && isdigit(curch)) {
    //cout << curch << " " << atoi(&curch) <<  endl;
    number = number*10 + atoi(&curch);
    eofile = (infp->get(curch)==0);
  }
 
  return;
}

void getname (ifstream *infp, char &curch, bool &eofile, name &strindex) {
  namestring str = "";
  int lengthstr = 0;
  while(eofile == 0 && (isdigit(curch) || isalpha(curch))) {
    if (lengthstr < maxlength) {
      str = str + curch;
    }
    lengthstr++;
    eofile = (infp->get(curch)==0);
  }
  if (!eofile) {
  strindex = lookup(str);
  }
  // if (lengthstr > maxlength) cout << "WARNING: following name longer than " << maxlength << " characters.\n";
  return;
}

name lookup (namestring str) {
  name index;
  // Potentially can use nice vector functions here
  for(index=0; index < nametable.size(); index++) {
    if (str.compare(nametable[index]) == 0) {
      return index;
    }
  }
  nametable.push_back(str);
  return nametable.size()-1;
}

void printnametable() {
  for(name i=0; i < nametable.size(); i++) {
    cout << nametable[i] << endl;
  }
  return;
}

// These functions should check vectors are indexed out of bounds
void writename(name id) {
  cout << nametable[id] << " ";
}

length namelength(name id) {
  return nametable[id].length();
}
