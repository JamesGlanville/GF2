#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
using namespace std;

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

  eofile = (inf.get(ch) == 0);  // get character
  
  while (!eofile) {
    if (isspace(ch)) cout << '.';
    else cout << ch;
    eofile = (inf.get(ch) == 0); // get next character
  }

  inf.close();
}
