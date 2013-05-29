#include "names.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;


/* Name storage and retrieval routines */

names::names(void)  /* the constructor */
{
	nametable.push_back("");
  /* over to you */
}

name names::lookup (namestring str)
{
  for(int i = 0; i < nametable.size(); i++) {
    if (nametable[i] == str) {
      return i;
    }
  }
  nametable.push_back(str);
  return nametable.size() - 1;
}

name names::cvtname (namestring str)
{
 for(int i = 0; i < nametable.size(); i++) {
    if (nametable[i] == str) {
      return i;
    }
  }
	return 0;
}

void names::writename (name id)
{
	if (id < nametable.size())
	{
		cout << nametable[id];
	}
	else
	{
		cout << "Error, index out of range." << endl;
	}}

string names::getname (name id)
{
	if (id < nametable.size())
	{
		return nametable[id];
	}
	else
	{
		cout << "Error, index out of range." << endl;
		return "";
	}
}

int names::tablelength()
{
	return nametable.size();
}


int names::namelength (name id)
{
	if (id < nametable.size())
	{
		cout << nametable[id].length();
	}
	else
	{
		cout << "Error, index out of range." << endl;
	}	}
