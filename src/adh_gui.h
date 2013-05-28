 #infdef gui_h
#define gui_h

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include "names.h"
#include "devices.h"
#include "monitor.h"

enum {

};

class MyGLCanvas;

class MyFrame: public wxFrame
{
 public:
  /* Constructor */
  MyFrame(wxWindow *parent,
	  const wxString& title,
	  const wxPoint& pos,
          const wxSize& size,
	  names *names_mod = NULL,
          decices *devices_mod = NULL,
	  monitor *monitor_mod = NULL,
          long style = wxDEFAULT_FRAME_STYLE);

 private:
  MyGLCanvas *canvas;                  /* GL drawing area widget */
  names *nmz;                          /* pointer to names class */
  devices *dmz;                        /* pointer to devices class */
  monitor *mmz;                        /* pointer to monitor class */

  void runnetwork(int ncycles);        /* function to run the logic testing */
  void OnExit(wxCommandEvent& event);  /* callback for exit menu item */
  void OnAbout(wxCommandEvent& event); /* callback for about menu */
  DECLARE_EVENT_TABLE()
};

class MyGLCanvas: public wxGLCanvas
{
 public:
  /* constructor */
  MyGLCanvas(wxWindow *parent,
	     wxWindowID id = wxID_ANY,
	     monitor* monitor_mod = NULL,
	     names* names_mod = NULL,
	     const wxPoint& pos = wxDefaultPosition,
	     const wxSize& size = wxDefaultSize,
	     long style = 0,
	     const wxString& name = wxT("MyGLCanvas"));

  /* function to draw canvas contents */
  void Render(wxString example_text = wxT(""), int cycles = -1);

 private:
  bool init;			     /* has GL context been initialised? */
  monitor *mmz;			     /* for extracting signal traces */
  names *nmz;			     /* for extracting signal names */
  void InitGL();		     /* function to init GL context */
  void OnSize(wxSizeEvent& event);   /* callback for resizeing */
  void OnPaint(wxPaintEvent& event); /* callback for canvas exposed */
  void OnMouse(wxMouseEvent& event); /* callback for mouse inside canvas */
  
};

#endif	/* gui_h */
