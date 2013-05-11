#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
using namespace std;

void skipspaces (ifstream *infp, char &curch, bool &eofile);

int main (int argc, char **argv)
{
  ifstream inf;                // input file
  char ch;                     // current character
  bool eofile;                 // true when end of file is reached

  if (argc != 2) {
    cout << "Useage:         " << argv[0] << " [filename]" << endl;
    exit(1);
  }

  inf.open(argv[1]);
  if (!inf) {
    cout << "Error: cannot open file " << argv[1] << " for reading" << endl;
    exit(1);
  }

  // main loop: until end of file is reached, print next character
  while (!eofile) {
    inf.get(ch);
    skipspaces(&inf, ch, eofile);
    cout << ch;
  }

  inf.close();
}

void skipspaces (ifstream *infp, char &curch, bool &eofile)
{
  while (isspace(curch) && eofile == 0) {
    eofile = (infp->get(curch) == 0);
    if (curch == 0) {
      eofile = 1;
      return;
    }
  }
  return;
}
    
