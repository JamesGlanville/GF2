#include "logsim.h"
#include "userint.h"
#include "gui.h"
#include <GL/glut.h>

#define USE_GUI

extern wxTextCtrl* textout;

IMPLEMENT_APP(MyApp)
  
bool MyApp::OnInit()
  // This function is automatically called when the application starts
{
	ifstream * inf=new ifstream;
	
	if (argc != 2) { // check we have one command line argument
		wcout << "Usage:      " << argv[0] << " [filename]" << endl;
	exit(1);
	}
  
	inf->open(wxString(argv[1]).fn_str());
	if (!inf) {
		cout << "Error: cannot open file " << wxString(argv[1]).fn_str() << " for reading " << endl;
		exit(1);
	}
	
  // Construct the six classes required by the innards of the logic simulator
  nmz = new names();
  dtz = new devicetable();
  netz = new network(nmz);
  dmz = new devices(nmz, netz, dtz);
  mmz = new monitor(nmz, netz);
  smz = new scanner(nmz, inf);
  MyFrame *frame = new MyFrame(NULL, wxT("Logic simulator"), wxDefaultPosition,  wxSize(800, 600), nmz, dmz, mmz);
#ifdef PARSER_TEST  
  pmz = new parser(smz, nmz);
#endif
#ifndef PARSER_TEST
  pmz = new parser(netz,dmz,mmz,smz,nmz,dtz);
#endif
   
    wxStreamToTextRedirector redirect(textout);
     cout << pmz->readin () << endl ; // check the logic file parsed correctly
#ifdef USE_GUI
    // glutInit cannot cope with Unicode command line arguments, so we pass
    // it some fake ASCII ones instead
    char **tmp1; int tmp2 = 0; glutInit(&tmp2, tmp1);
    // Construct the GUI

    frame->Show(true);
    return(true); // enter the GUI event loop
#else
    // Construct the text-based interface
    userint umz(nmz, dmz, mmz);
    umz.userinterface();
#endif /* USE_GUI */
  
  return(false); // exit the application
}
