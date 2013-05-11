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

  //  eofile = (inf.get(ch) == 0);  // get character

  /*  while (!eofile) {
    if (isspace(ch)) cout << '.';
    else cout << ch;
    eofile = (inf.get(ch) == 0); // get next character
  }
  */

  while (!eofile) {
    //    cout << "in while loop" << endl;
    //    cout << "ch = " << ch << endl;
    inf.get(ch);
    skipspaces(&inf, ch, eofile);
    cout << ch;
    //    cout << "eofile = " << eofile << endl;
  }

  inf.close();
}

void skipspaces (ifstream *infp, char &curch, bool &eofile)
{
  //  cout << "in skipspaces" << endl;
  while (isspace(curch) && eofile == 0) {
    //cout << "in skipspaces while" << endl;
    //    infp->get(curch);
    eofile = (infp->get(curch) == 0);
    if (curch == 0) {
      eofile = 1;
      return;
    }
  }
  //  cout << "returning from skipspaces" << endl;
  return;
}
    
