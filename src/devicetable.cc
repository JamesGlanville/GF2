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

name devicetable::lookup (namestring str,device_type type,numinputs numinput)
{
	devicestruct ds;
	ds.ns=str;
	ds.ni=numinput;
	ds.dt= type;
	
  for(int i = 0; i < device_table.size(); i++) {
    if (device_table[i].ns == str) {
      return i;
    }
  }
  device_table.push_back(ds);
  return device_table.size() - 1;
}

string devicetable::getswitch(int swnum)
{
	int currentsw=0;
	for (int i = 0; i < device_table.size();i++)
	{
		if (device_table[i].dt == SW)
		{
			if (swnum == currentsw)
			{
				return device_table[i].ns;
			}
			else
			{
				currentsw++;
			}
		}
	}
	
	return "";
}

device_type devicetable::gettype(namestring str)
{
 for(int i = 0; i < device_table.size(); i++) {
    if (device_table[i].ns == str) {
      return device_table[i].dt;
    }}
 	return UNDEFINED;
}

numinputs devicetable::getinputs(namestring str)
{
 for(int i = 0; i < device_table.size(); i++) {
    if (device_table[i].ns == str) {
      return device_table[i].ni;
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
