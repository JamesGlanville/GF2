#define SCANNERMETHODSPUBLIC

#include "../../scanner.h" 
#include "../../names.h"

int main(int argc, char **argv)
{
	ifstream * inf=new ifstream;
	
	if (argc != 2) { // check we have one command line argument
		wcout << "Usage:      " << argv[0] << " [filename]" << endl;
		exit(1);
	}
	inf->open(argv[1]);
	if (!inf) {
		cout << "Error: cannot open file " << argv[1] << " for reading " << endl;
		exit(1);
	}
	names* test_nametable;
	cout <<"here";
	scanner* test_scanner = new scanner(test_nametable,inf);
	
	for (int i=0;i<80;i++){
	cout <<test_scanner->curch;
	test_scanner->nextChar();}
	//cout <<test_scanner->curch;}
	

	string lookuptable[]={"namesym","numsym","DEV","INIT","CONN","MON","<=",";","=","badsym","EOF","(",")","{","}"};
	//^^^^^ make sure this is updated along with the enum in scanner.h
	symbol s;
	name id;
	int num;
	while(!test_scanner->eofile){
		cout <<"about to go"<<endl;
		s=DEV;id=0;num=0;
		test_scanner->getsymbol( s,id,num);
		cout << "Symbol: " << lookuptable[s] ;
		if (s==namesym){cout << " Id: " << test_scanner->nametable->getname(id); }
		if (s==numsym){cout << " Num: " << num;}
		cout << "finished." <<endl;
		cout << endl;
	}
  
	return 0;
}
