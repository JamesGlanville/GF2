#ifndef gui_h
#define gui_h

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include "names.h"
#include "devices.h"
#include "monitor.h"

enum { 
  CYCLES_SPIN = wxID_HIGHEST + 1,
  FILE_BUTTON,
  LOAD_BUTTON,
  RUN_BUTTON_ID,
  CONT_BUTTON_ID,
  DISP_SCROLL,
  SWITCH_LIST,
  SWITCH_OPTION,
  MONITOR_ADD,
  MONITOR_REM,
}; // widget identifiers

class MyGLCanvas;

class MyFrame: public wxFrame
{
 public:
  MyFrame(wxWindow *parent,
	  const wxString& title,
	  const wxPoint& pos,
	  const wxSize& size, 
	  names *names_mod = NULL,
	  devices *devices_mod = NULL,
	  monitor *monitor_mod = NULL, 
	  long style = wxDEFAULT_FRAME_STYLE); // constructor
 private:
  vector<wxBoxSizer*> vtracesizers;       /* Vector to hold sizers for traces */
  vector<MyGLCanvas*> canvases;	          /* vector to hold canvases */
  vector<wxStaticText*> tracelabels;

  wxString tracesizer;
  wxString tracename;
  string devname;
  
  wxBoxSizer *toptracesizer;

  wxSpinCtrl *spin_cycles;
  wxComboBox *switch_list;
  wxComboBox *switch_option;
  wxComboBox *add_monitor;
  wxComboBox *rem_monitor;
  wxWindow *disp_scroll;
  wxTextCtrl *text;
  names *nmz;                             // pointer to names class
  devices *dmz;                           // pointer to devices class
  monitor *mmz;                           // pointer to monitor class
  int cyclescompleted;                    // how many simulation cycles have been completed
  void runnetwork(int ncycles);           // function to run the logic network
  void OnExit(wxCommandEvent& event);     // callback for exit menu item
  void OnAbout(wxCommandEvent& event);    // callback for about menu item
  void OnFileButton(wxCommandEvent &event);
  void OnLoadButton(wxCommandEvent &event);
  void OnRunButton(wxCommandEvent &event);
  void OnContButton(wxCommandEvent &event);
  void OnSwitchSelect(wxCommandEvent& event);
  void OnSwitchOption(wxCommandEvent& event);
  void OnAddMonitor(wxCommandEvent& event);
  void OnRemMonitor(wxCommandEvent& event);
  void DrawTraces(names *names_mod, devices *devices_mod, monitor *monitor_mod);
  DECLARE_EVENT_TABLE()
};
    
class MyGLCanvas: public wxGLCanvas
{
 public:
  MyGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, monitor* monitor_mod = NULL, names* names_mod = NULL,
	     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0,
	     const wxString& name = wxT("MyGLCanvas")); // constructor
  void Render(wxString example_text = wxT(""), int cycles = -1); // function to draw canvas contents
 private:
  bool init;                         // has the GL context been initialised?
  int cyclesdisplayed;               // how many simulation cycles have been displayed
  monitor *mmz;                      // pointer to monitor class, used to extract signal traces
  names *nmz;                        // pointer to names class, used to extract signal names
  void InitGL();                     // function to initialise GL context
  void OnSize(wxSizeEvent& event);   // callback for when canvas is resized
  void OnPaint(wxPaintEvent& event); // callback for when canvas is exposed
  void OnMouse(wxMouseEvent& event); // callback for mouse events inside canvas
  DECLARE_EVENT_TABLE()
};
    
#endif /* gui_h */
