#define NAMES_CLASS_TEST

#include "../../names.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <assert.h>


using namespace std;

int main()
{
	names test_table;
	
	cout << "TESTING if nametable initialised ok:" << endl;
	assert (test_table.nametable.size()==1);
	cout << "Size of test_table (1) ok." << endl;
	
	cout << test_table.getname(0) <<" is only member." << endl<<endl;
	
	cout << "TESTING lookup method:" << endl;
	cout << "test_table contents: " << endl;
	test_table.lookup("larger string than we'll have to handle, with $£*& strange characters");
	test_table.lookup("8893231938123");
	test_table.lookup("8893231938123");
	test_table.lookup("8893231938123");
	test_table.lookup("8893231938123");
	test_table.lookup("8893231938123");
	test_table.lookup("8893231938123");
	test_table.lookup("8893231938123");
	test_table.lookup("8893231938123");
	test_table.lookup("");
	test_table.lookup("");
	
	assert (test_table.nametable.size()==4);
	cout << "New size of test_table ok (4)" << endl;
	for (int i=0;i<test_table.nametable.size();i++)
	{
		test_table.writename(i);
		cout <<endl;
	}
	assert(test_table.getname(0)=="blankname");
	assert(test_table.getname(1)=="larger string than we'll have to handle, with $£*& strange characters");
	assert(test_table.getname(2)=="8893231938123");
	assert(test_table.getname(3)=="");
	
	cout << "Table contents ok." << endl<<endl;
	
	cout << "Testing writename,getname,namelength gracefully with out of range parameters:" <<endl;
	
	test_table.writename(100);
	test_table.getname(100);
	test_table.namelength(100);
	
	cout << "Expected 3 lines \"Error, index out of...\"" << endl<<endl;
	
	cout << "Testing cvtname works:" <<endl;
	assert(test_table.cvtname("blankname")==0);
	assert(test_table.cvtname("larger string than we'll have to handle, with $£*& strange characters")==1);
	assert(test_table.cvtname("8893231938123")==2);
	assert(test_table.cvtname("")==3);
	assert(test_table.cvtname("NOT A VALIDSTRING")==0);
	cout << "Tests passed." <<endl<<endl;
	
	cout << "Testing namelength: " <<endl;
	test_table.namelength(0); cout << "\t";
	test_table.namelength(1); cout << "\t";
	test_table.namelength(2); cout << "\t";
	test_table.namelength(3); cout << endl;
	cout << "(Expected 9,69,13,0)"<<endl;
	cout << "Testing complete." << endl;
	
	return 0;
}

























