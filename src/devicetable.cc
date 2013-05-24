#include "devicetable.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;


/* Name storage and retrieval routines */

//devicetable::devicetable()  /* the constructor */
//{
//	nametable.push_back(std::make_pair("blankname",NONE);
  /* over to you */
//}

name devicetable::lookup (namestring str,device_type type)
{
  for(int i = 0; i < device_table.size(); i++) {
    if (device_table[i].first == str) {
      return i;
    }
  }
  device_table.push_back(std::make_pair(str,type));
  return device_table.size() - 1;
}

device_type devicetable::gettype(namestring str)
{
 for(int i = 0; i < device_table.size(); i++) {
    if (device_table[i].first == str) {
      return device_table[i].second;
    }}
 	return UNDEFINED;
}

/*
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
	

int names::namelength (name id)
{
	if (id < nametable.size())
	{
		cout << nametable[id].length();
	}
	else
	{
		cout << "Error, index out of range." << endl;
	}	*/
	//}
