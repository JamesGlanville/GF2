#include "gui.h"
#include <GL/glut.h>
#include "wx_icon.xpm"
#include <iostream>
#include <string>

using namespace std;

// MyGLCanvas //////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
  EVT_SIZE(MyGLCanvas::OnSize)
  EVT_PAINT(MyGLCanvas::OnPaint)
  EVT_MOUSE_EVENTS(MyGLCanvas::OnMouse)
END_EVENT_TABLE()
  
int wxglcanvas_attrib_list[5] = {WX_GL_RGBA,
                                 WX_GL_DOUBLEBUFFER,
                                 WX_GL_DEPTH_SIZE,
                                 16, 0};

MyGLCanvas::MyGLCanvas(wxWindow *parent,
                       wxWindowID id,
                       monitor* monitor_mod,
                       names* names_mod,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxString& name):
    wxGLCanvas(parent, id, pos, size, style, name, wxglcanvas_attrib_list)
  // Constructor - initialises private variables
{
  mmz = monitor_mod;
  nmz = names_mod;
  init = false;
  cyclesdisplayed = -1;
}

void MyGLCanvas::Render(wxString example_text, int cycles)
  // Draws canvas contents - the following example writes the string
  // "example text" onto the canvas and draws a signal trace.
  // The trace is artificial if the simulator has not yet been
  // run. When the simulator is run, the number of cycles is passed as
  // a parameter and the first monitor trace is displayed.
{
  float y;
  unsigned int i;
  asignal s;

  if (cycles >= 0) cyclesdisplayed = cycles;

  SetCurrent();
  if (!init) {
    InitGL();
    init = true;
  }
  glClear(GL_COLOR_BUFFER_BIT);

  if ((cyclesdisplayed >= 0) && (mmz->moncount() > 0)) {
    // draw the first monitor signal, get trace from monitor class
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (i=0; i<cyclesdisplayed; i++) {
      if (mmz->getsignaltrace(0, i, s)) {
        if (s==low) y = 10.0;
        if (s==high) y = 30.0;
        glVertex2f(20*i+10.0, y); 
        glVertex2f(20*i+30.0, y);
      }
    }
    glEnd();

  } else { // draw an artificial trace

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (i=0; i<5; i++) {
      if (i%2) y = 10.0;
      else y = 30.0;
      glVertex2f(20*i+10.0, y); 
      glVertex2f(20*i+30.0, y);
    }
    glEnd();
    
  }

  /*
  // Example of how to use GLUT to draw text on the canvas
  glColor3f(0.0, 0.0, 1.0);
  glRasterPos2f(10, 100);
  for (i = 0; i < example_text.Len(); i++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, example_text[i]);
    }
  */

  // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
  glFlush();
  SwapBuffers();
}

void MyGLCanvas::InitGL()
  // Function to initialise the GL context
{
  int w, h;

  GetClientSize(&w, &h);
  SetCurrent();
  glDrawBuffer(GL_BACK);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glViewport(0, 0, (GLint) w, (GLint) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1); 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void MyGLCanvas::OnPaint(wxPaintEvent& event)
  // Callback function for when the canvas is exposed
{
  int w, h;
  wxString text;

  wxPaintDC dc(this); // required for correct refreshing under MS windows
  GetClientSize(&w, &h);
  text.Printf(wxT("Canvas redrawn by OnPaint callback, canvas size is %d by %d"), w, h);
  Render(text);
}

void MyGLCanvas::OnSize(wxSizeEvent& event)
  // Callback function for when the canvas is resized
{
  wxGLCanvas::OnSize(event); // required on some platforms
  init = false;
  Refresh(); // required by some buggy nvidia graphics drivers,
  Update();  // harmless on other platforms!
}

void MyGLCanvas::OnMouse(wxMouseEvent& event)
  // Callback function for mouse events inside the GL canvas
{
  wxString text;
  int w, h;
  GetClientSize(&w, &h);
  if (event.ButtonDown())
    text.Printf(wxT("Mouse button %d pressed at %d %d"),
                event.GetButton(), event.m_x, h-event.m_y);
  if (event.ButtonUp())
    text.Printf(wxT("Mouse button %d released at %d %d"),
                event.GetButton(), event.m_x, h-event.m_y);
  if (event.Dragging())
    text.Printf(wxT("Mouse dragged to %d %d"),
                event.m_x, h-event.m_y);
  if (event.Leaving())
    text.Printf(wxT("Mouse left window at %d %d"),
                event.m_x, h-event.m_y);

  if (event.ButtonDown() || event.ButtonUp() || event.Dragging() || event.Leaving())
    Render(text);
}

// MyFrame /////////////////////////////////////////////////////////////////////


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(wxID_EXIT,       MyFrame::OnExit)
  EVT_MENU(wxID_ABOUT,      MyFrame::OnAbout)
  EVT_BUTTON(RUN_BUTTON_ID, MyFrame::OnRunButton)
  EVT_SPINCTRL(CYCLES_SPIN, MyFrame::OnSpin)
  EVT_SCROLLWIN(MyFrame::OnScroll)
END_EVENT_TABLE()
  
MyFrame::MyFrame(wxWindow *parent,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 names *names_mod,
                 devices *devices_mod,
                 monitor *monitor_mod,
                 long style):
  wxFrame(parent, wxID_ANY, title, pos, size, style)
  // Constructor - initialises pointers to names, devices and monitor
  // classes, lays out widgets using sizers
{
  SetIcon(wxIcon(wx_icon));

  nmz = names_mod;
  dmz = devices_mod;
  mmz = monitor_mod;
  if (nmz == NULL || dmz == NULL || mmz == NULL) {
    cout << "Cannot operate GUI without names, devices and monitor classes" << endl;
    exit(1);
  }

  wxMenu *fileMenu = new wxMenu;
  fileMenu->Append(wxID_EXIT, wxT("&Quit\tCtrl-Q"));
  wxMenu *helpMenu = new wxMenu;
  helpMenu->Append(wxID_ABOUT, wxT("&About"));
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, wxT("&File"));
  menuBar->Append(helpMenu, wxT("&Help"));
  SetMenuBar(menuBar);

  wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
  
  wxBoxSizer *ctrlsizer = new wxBoxSizer(wxHORIZONTAL);
  ctrlsizer->Add(new wxStaticText(this, wxID_ANY, wxT("Cycles:")),
                 0,             /* make vertically unstrechable */
                 wxALL | wxALIGN_CENTER_VERTICAL,         /* border all around */
                 10);           /* border size */ 


  spin_cycles = new wxSpinCtrl(this, CYCLES_SPIN, wxString(wxT("10")));
  ctrlsizer->Add(spin_cycles, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
  ctrlsizer->Add(new wxButton(this, RUN_BUTTON_ID, wxT("Run")),
                 0,
                 wxALL | wxEXPAND,
                 10);
  topsizer->Add(ctrlsizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

  wxScrolledWindow* disp_scroll = new wxScrolledWindow(this,
						       -1,
						       wxDefaultPosition,
						       wxDefaultSize,
				   wxSUNKEN_BORDER | wxHSCROLL | wxVSCROLL);

  wxBoxSizer *toptracesizer = new wxBoxSizer(wxVERTICAL);

  disp_scroll->SetSizer(toptracesizer);
  disp_scroll->SetScrollRate(10, 10);
  disp_scroll->SetAutoLayout(true);

  for(int i = 0; i<10; i++)
    {
      vtracesizers.push_back(new wxBoxSizer(wxHORIZONTAL));
      canvases.push_back(new MyGLCanvas(disp_scroll,
					wxID_ANY,
					monitor_mod,
					names_mod,
					wxPoint(-1,-1),
					wxSize(-1,40)));
      tracesizer = wxT("m");
      tracename = tracesizer << i;
      vtracesizers[i]->Add(new wxStaticText(disp_scroll, wxID_ANY, tracename),
      			   0,
      			   wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
      			   10);
      vtracesizers[i]->Add(canvases[i], 1, wxALL | wxEXPAND, 10);
      toptracesizer->Add(vtracesizers[i], 0, wxEXPAND | wxALL, 10);
    }

  topsizer->Add(disp_scroll, 1, wxEXPAND | wxALL, 10);

  SetSizeHints(400, 400);
  SetSizer(topsizer);

}

void MyFrame::OnExit(wxCommandEvent &event)
{
  Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event)
{
  wxMessageDialog about(this, wxT("LogicSim\n\
    By James Glanville, George Ayris and Andy Holt"),
                        wxT("About LogicSim"),
                        wxICON_INFORMATION | wxOK);
  about.ShowModal();
}

// void MyFrame::OnButton(wxCommandEvent &event)
// // Callback for the push button
// {
//   int n, ncycles;
  
//   cyclescompleted = 0;
//   mmz->resetmonitor ();
//   runnetwork(spin->GetValue());
//   canvas->Render(wxT("Run button pressed"), cyclescompleted);
// }

void MyFrame::OnRunButton(wxCommandEvent &event)
{
  wxLogMessage(wxT("Run Button Pressed"));
}

void MyFrame::OnSpin(wxSpinEvent &event)
  // Callback for the spin control
{
  wxString text;

  text.Printf(wxT("New spinctrl value %d"), event.GetPosition());
  //  canvas->Render(text);
}

// void MyFrame::OnText(wxCommandEvent &event)
//   // Callback for the text entry field
// {
//   wxString text;

//   text.Printf(wxT("New text entered %s"), event.GetString().c_str());
//   canvas->Render(text);
// }

// void MyFrame::runnetwork(int ncycles)
//   // Function to run the network, derived from corresponding function in userint.cc
// {
//   bool ok = true;
//   int n = ncycles;

//   while ((n > 0) && ok) {
//     dmz->executedevices (ok);
//     if (ok) {
//       n--;
//       mmz->recordsignals ();
//     } else
//       cout << "Error: network is oscillating" << endl;
//   }
//   if (ok) cyclescompleted = cyclescompleted + ncycles;
//   else cyclescompleted = 0;
// }

void MyFrame::OnScroll(wxScrollWinEvent& event)
{
  cout << "position = " << event.GetPosition() << endl;
}
