#include "adh_gui.h"
#include <GL/glut.h>
#include "wx_icon.xpm"
#include <iostream>

using namespace std;

/******** MyGLCanvas ********/

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
  EVT_SIZE(MyGLCanvas::OnSize)
  EVT_PAINT(MyGLCanvas::OnPaint)
  EVT_MOUSE_EVENTS(MyGLCanvas::OnMouse)
END_EVENT_TABLE()

int wxglcanvas_attib_list[5] = {WX_GL_RGBA,
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
  // constructor
{
  mmz = monitor_mod;
  nmz = names_mod;
  init = false;
  cylcesdisplayed = -1;
}

void MyGLCanvas::Render(wxString example_text);
{
  float y;
  unsigned int i;
  asignal s;

  SetCurrent();
  if (!init) {
    InitGL();
    init = true;
  }
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.0, 0.0, 1.0);
  glRasterPos2f(10, 100);
  for (i = 0; i < example_text.Len(); i++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, example_text[i]);
    }

  glFlush();
  SwapBuffers();
}

void MyGLCanvas::InitGL()
  // function to initialise the GL context
{
  int w, h;

  GetClientSize(&w, &h);
  SetCurrent();
  glDrawBuffer(GL_BACK);
  glClearColor(1.0, 1.0, 0.0);
  glviewport(0, 0, (GLint) w, (GLint) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();x
}

void MyGLCanvas::OnPaint(wxPaintEvent& event)
// callback function when canvas exposed
{
  int w, h;
  wxString text;

  wxPaintDC dc(this);		// required for MS windows
  GetClientSize(&w, &h);
  text.Printf(wxT("Canvas redrawn by OnPaint callback, canvas size is %d by %d"),
	      w, h);
  Render(text);
}

void MyGLCanvas::OnSize(wxSizeEvent& event)
// callback functino for when canvas is resized
{
  wxGLCanvas::OnSize(event);	// required on some platforms
  init = false;
  Refresh();			// required by some buggy nvidia
				// graphics drivers
  Update();			// harmless on other platforms
}

void MyGLCanvas::OnMouse(wxMouseEvent& event)
// callback for mouse events inside GL canvas
{
  wxString text;
  int w, h;

  GetClientSize(&w, &h);
  if (event.ButtonDown()) {
    text.Printf(wxT("Mouse button %d pressed at %d %d"),
		event.GetButton(),
		event.m_x,
		h-event.m_y);
  }
  if (event.ButtonUp()) {
    text.Printf(wxT("Mouse button %d released at %d %d"),
		event.GetButton(),
		event.m_x,
		h-event.m_y);
  }
  if (event.Dragging()) {
    text.Printf(wxT("Mouse dragged to %d %d"),
		event.m_x,
		h-event.m_y);
  }
  if (event.Leaving()) {
    text.Printf(wxT("Mouse left window at %d %d"),
		event.m_x,
		h-event.m_y);
  }
  if (event.ButtonDown() || event. ButtonUp() || event.Dragging() || event.Leavint()) {
    Render(text);
  }
}

/******** MyFrame ********/
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(wxID_EXIT, MyFrame::OnExit)
  EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
END_EVENT_TABLE()

MyFrame::MyFrame(wxWindow *parent,
		 const wxString& title,
		 const wxPoint& pos
		 const wxSize& size
		 names *names_mod,
		 devices *devices_mod,
		 monitor *monitor_mod,
		 long style):
  wxFrame(parent, wxID_Any, title, pos, size, style)
  // constructor
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
  menuBar->Append(fileMenu, wxT("&Help"));
  SetMenuBar(menuBar);

}

void MyFrame::OnExit(wxCommandEvent &event)
{
  Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event)
{
  wxMessageDialog about(this, wxT("LogicSim\n\
    By James Glanville, George Ayris and Andy Holt"),
			wxICON_INFORMATION | wxOK);
  about.ShowModal();
}

