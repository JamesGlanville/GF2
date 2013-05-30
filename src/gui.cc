#include "gui.h"
#include <GL/glut.h>
#include "wx_icon.xpm"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

wxTextCtrl* textout;

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

void MyGLCanvas::Render(int monren, int cycles)
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
      if (mmz->getsignaltrace(monren, i, s)) {
	if (s==low) y = 10.0;
	if (s==high) y = 30.0;
	glVertex2f(20*i+10.0, y);
	glVertex2f(20*i+30.0, y);
      }
    }
    glEnd();

  }
  /*
  else { // draw an artificial trace

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
  */

  /*
  // Example of how to use GLUT to draw text on the canvas
  glColor3f(0.0, 0.0, 1.0);
  glRasterPos2f(10, 100);
  for (i = 0; i < example_text.Len(); i++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, example_text[i]);
    }
  */

  // We've been drawing to the back buffer, flush the graphics
  // pipeline and swap the back buffer to the front 
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
  //text.Printf(wxT("Canvas redrawn by OnPaint callback, canvas size is %d by %d"), w, h);
  Render();
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
  /*
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
  */
}

// MyFrame /////////////////////////////////////////////////////////////////////


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(wxID_EXIT,       MyFrame::OnExit)
  EVT_MENU(wxID_ABOUT,      MyFrame::OnAbout)
  EVT_BUTTON(FILE_BUTTON, MyFrame::OnFileButton)
  EVT_BUTTON(LOAD_BUTTON, MyFrame::OnLoadButton)
  EVT_BUTTON(RUN_BUTTON_ID, MyFrame::OnRunButton)
  EVT_BUTTON(CONT_BUTTON_ID, MyFrame::OnContButton)
  EVT_COMBOBOX(SWITCH_LIST, MyFrame::OnSwitchSelect)
  EVT_COMBOBOX(SWITCH_OPTION, MyFrame::OnSwitchOption)
  EVT_COMBOBOX(MONITOR_ADD, MyFrame::OnAddMonitor)
  EVT_COMBOBOX(MONITOR_REM, MyFrame::OnRemMonitor)
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

  topsizer = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer *filesizer = new wxBoxSizer(wxHORIZONTAL);
  filesizer->Add(new wxButton(this, FILE_BUTTON, wxT("Select File")),
		 0,
		 wxALL | wxALIGN_CENTER_VERTICAL,
		 10);
  filesizer->Add(new wxButton(this, LOAD_BUTTON, wxT("Load Data")),
		 0,
		 wxALL | wxALIGN_CENTER_VERTICAL,
		 10);
  topsizer->Add(filesizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

  wxBoxSizer *ctrlsizer = new wxBoxSizer(wxHORIZONTAL);
  ctrlsizer->Add(new wxStaticText(this, wxID_ANY, wxT("Cycles:")),
		 0,             /* make vertically unstrechable */
		 wxALL | wxALIGN_CENTER_VERTICAL,         /* border all around */
		 10);           /* border size */
  spin_cycles = new wxSpinCtrl(this, CYCLES_SPIN, wxString(wxT("10")), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 50, 10);
  ctrlsizer->Add(spin_cycles, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
  ctrlsizer->Add(new wxButton(this, RUN_BUTTON_ID, wxT("Run")),
		 0,
		 wxALL | wxEXPAND,
		 10);
  ctrlsizer->Add(new wxButton(this, CONT_BUTTON_ID, wxT("Continue")),
		 0,
		 wxALL | wxEXPAND,
		 10);
  topsizer->Add(ctrlsizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

  wxBoxSizer *switchsizer = new wxBoxSizer(wxHORIZONTAL);
  switchsizer->Add(new wxStaticText(this, wxID_ANY, wxT("Switches:")),
		   0,
		   wxALL | wxALIGN_CENTER_VERTICAL,
		   10);
  switch_list = new wxComboBox(this, SWITCH_LIST, wxEmptyString);
  switchsizer->Add(switch_list, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
  switch_option = new wxComboBox(this, SWITCH_OPTION, wxEmptyString);
  switch_option->Append(wxT("HIGH"));
  switch_option->Append(wxT("LOW"));
  switchsizer->Add(switch_option, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
  topsizer->Add(switchsizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

  wxBoxSizer *monitorsizer = new wxBoxSizer(wxHORIZONTAL);
  monitorsizer->Add(new wxStaticText(this, wxID_ANY, wxT("Add Monitor:")),
		    0,
		    wxALL | wxALIGN_CENTER_VERTICAL,
		    10);
  add_monitor = new wxComboBox(this, MONITOR_ADD, wxEmptyString);
  monitorsizer->Add(add_monitor, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
  monitorsizer->Add(new wxStaticText(this, wxID_ANY, wxT("Remove Monitor:")),
		    0,
		    wxALL | wxALIGN_CENTER_VERTICAL,
		    10);
  rem_monitor = new wxComboBox(this, MONITOR_REM, wxEmptyString);
  //  rem_monitor->Append(wxT("a used monitor"));
  //  rem_monitor->Append(wxT("another used monitor"));
  monitorsizer->Add(rem_monitor, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
  topsizer->Add(monitorsizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);


  wxScrolledWindow* disp_scroll = new wxScrolledWindow(this,
						       -1,
						       wxDefaultPosition,
						       wxDefaultSize,
				   wxSUNKEN_BORDER | wxHSCROLL | wxVSCROLL | wxFULL_REPAINT_ON_RESIZE);

  toptracesizer = new wxBoxSizer(wxVERTICAL);

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
      tracelabels.push_back(new wxStaticText(disp_scroll, wxID_ANY, tracename));
      vtracesizers[i]->Add(tracelabels[i], 0, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 10);
      //vtracesizers[i]->Add(new wxStaticText(disp_scroll, wxID_ANY, tracename),
      //			   0,
      //			   wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
      //			   10);
      vtracesizers[i]->Add(canvases[i], 1, wxALL | wxEXPAND, 10);
      toptracesizer->Add(vtracesizers[i], 0, wxEXPAND | wxALL, 10);
      toptracesizer->Hide(vtracesizers[i], true);
      toptracesizer->Layout();
    }
  
  topsizer->Add(disp_scroll, 1, wxEXPAND | wxALL, 10);

  wxBoxSizer *termwinsizer = new wxBoxSizer(wxVERTICAL);
  termwinsizer->Add(new wxStaticText(this, wxID_ANY, wxT("Messages:")),
		    0,
		    wxLEFT | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
		    10);
  textout = new wxTextCtrl(this,
			wxID_ANY,
			wxT(""),
			wxDefaultPosition,
			wxSize(400,100),
			wxTE_MULTILINE | wxHSCROLL);
  wxStreamToTextRedirector redirect(textout);
  termwinsizer->Add(textout, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
  topsizer->Add(termwinsizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

  /*
  cout << endl << "Lorem ipsum dolor sit amet, consectetur adipiscing elit." << endl;
  cout << "Morbi viverra purus nec leo blandit vel mattis turpis bibendum. Integer dictum eleifend tellus, sit amet commodo nibh scelerisque nec." << endl;
  cout << " Vivamus malesuada sodales justo, ut luctus nulla molestie
  vitae. Nulla rutrum urna quis magna tristique rhoncus. Etiam auctor
  consequat ligula eget placerat. Integer vulputate, velit vitae
  venenatis convallis, nibh nibh euismod leo, in mollis quam massa sit
  amet ipsum." << endl;
  */

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
//   canvas->Render(wxT("Run button pressed"), cypppclescompleted);
// }

void MyFrame::OnFileButton(wxCommandEvent &event)
{

  wxString filetoopenpath;
  wxFileDialog *loadNewFile = new wxFileDialog(this,
					     wxT("Choose file to open"),
					     wxEmptyString,
					     wxEmptyString,
					     _("DEF files (*.def)|*.def|TXT file (*.txt)|*.txt"),
					     wxFD_OPEN,
					     wxDefaultPosition);
  if (loadNewFile->ShowModal() == wxID_OK) // if click OPEN rather
					   // than CANCEL
    {
      filetoopenpath = loadNewFile->GetPath();
    }
  
  loadNewFile->Destroy();

  filetoopen = (string)filetoopenpath.mb_str();
}

void MyFrame::OnLoadButton(wxCommandEvent &event)
{
  wxStreamToTextRedirector redirect(textout);
  // get switches and put in the switches dialog box
  int i = 0;  
  while (dmz->getswitch(i).compare("") != 0)
    {
      //wxStreamToTextRedirector redirect(text);
      //cout << dmz->getswitch(i) << " is a switch." << endl;
      if(switch_list->FindString(wxString::FromAscii(dmz->getswitch(i).c_str())) == -1)
	{
	  switch_list->Append(wxString::FromAscii(dmz->getswitch(i).c_str()));
	}
      i++;
    }


  // get monitors and put them in the add monitors dialog box
  for (int i=0; i < mmz->moncount(); i++)
    {
      if(add_monitor->FindString(wxString::FromAscii(mmz->getmonprettyname(i).c_str())) == -1)
	{
	  // get name as string, convert to char* then to wxstring
	  add_monitor->Append(wxString::FromAscii(mmz->getmonprettyname(i).c_str()));
	}
    }
  cout << "Loaded devices from file." << endl;


}

void MyFrame::OnRunButton(wxCommandEvent &event)
{
  // run network from scratch for selected number of cycles.
  int n, ncycles;
  cyclescompleted = 0;
  mmz->resetmonitor();
  //wxStreamToTextRedirector redirect(textout);
  //cout << "run network for " << spin_cycles->GetValue() << " cycles." << endl;
  runnetwork(spin_cycles->GetValue());
}

void MyFrame::OnContButton(wxCommandEvent &event)
{
  // continue simulation - same as OnRunButton but w/o resetting cyclescompleted
  int n, ncycles;
  //wxStreamToTextRedirector redirect(textout);
  //cout << "network has run " << cyclescompleted << " cycles." << endl;
  //cout << "and will continue with " << spin_cycles->GetValue() << " more cycles." << endl;
  runnetwork(spin_cycles->GetValue());
}

void MyFrame::runnetwork(int ncycles)
{
  // Function to run the network, derived from corresponding function
  // in userint.cc
  wxStreamToTextRedirector redirect(textout);
  bool ok = true;
  int n = ncycles;

  //cout << "in run network function." << endl;

  if (cyclescompleted + n >= 350)
    {
      cout << "Error: too many cycles." << endl;
      cout << "All good things must come to an end." << endl;
      cout << "(Maximum is 350.)" <<endl;
      return;
    }

  while ((n > 0) && ok) {
    dmz->executedevices (ok);
    if (ok)
      {
	n--;
	mmz->recordsignals ();
      }
    else
      cout << "Error: network is oscillating" << endl;
  }

  //cout << "finished while" << endl;
  if (ok)
    {
      cyclescompleted = cyclescompleted + ncycles;
    }
  else
    {
      cyclescompleted = 0;
    }
  cout << "network has run " << cyclescompleted << " cycles." << endl;

  int mon;

  // draw each trace
  for (int i=0; toptracesizer->IsShown(vtracesizers[i]); i++)
    {
      mon = 0;
      while (mmz->getmonprettyname(mon) != (string)tracelabels[i]->GetLabel().mb_str() && mon < 5)
	{
	  mon++;
	}
      canvases[i]->Render(mon, cyclescompleted);
    }
}

void MyFrame::OnSwitchSelect(wxCommandEvent& event)
{
  
}

void MyFrame::OnSwitchOption(wxCommandEvent& event)
{
  wxStreamToTextRedirector redirect(textout);

  bool ok;
  if (switch_option->GetValue() == wxT("HIGH"))
    {
      dmz->setswitch(nmz->lookup((string)switch_list->GetValue().mb_str()), high, ok);
      if (ok)
	cout << "switch \"" << switch_list->GetValue().ToAscii()
	     << "\" set to HIGH" << endl;
      else
	cout << "Error: switch \"" << switch_list->GetValue().ToAscii()
	     << "\" not found." << endl;
    }
  else if (switch_option->GetValue() == wxT("LOW"))
    {
      dmz->setswitch(nmz->lookup((string)switch_list->GetValue().mb_str()), low, ok);
      if (ok)
	cout << "switch \"" << switch_list->GetValue().ToAscii()
	     << "\" set to LOW" << endl;
      else
	cout << "Error: switch \"" << switch_list->GetValue().ToAscii()
	     << "\" not found." << endl;
    }
}

void MyFrame::OnAddMonitor(wxCommandEvent& event)
{
  rem_monitor->Append(add_monitor->GetValue());
  //cout << "Add trace at monitor point: " <<
  //add_monitor->GetValue().ToAscii() << endl;

  int i=0;
  while (toptracesizer->IsShown(vtracesizers[i]))
    {
      i++;
    }
  tracelabels[i]->SetLabel(add_monitor->GetValue());
  vtracesizers[i]->Layout();
  toptracesizer->Show(vtracesizers[i], true, true);
  toptracesizer->Layout();
  topsizer->Layout();

  add_monitor->Delete(add_monitor->FindString(add_monitor->GetValue()));
  add_monitor->SetValue(wxT(""));
}

void MyFrame::OnRemMonitor(wxCommandEvent& event)
{
  add_monitor->Append(rem_monitor->GetValue());

  int i = 0;
  while (tracelabels[i]->GetLabel() != rem_monitor->GetValue())
    {
      i++;
    }

  // bump down all traces above to fill empty slot
  // first, find how many are shown below:
  int j = i+1;
  while(toptracesizer->IsShown(vtracesizers[j]) && j<10)
    {
      j++;
    }
  j = j-i-1;			// there are now j below emptied trace
  for(int k=0; k<j; k++)
    {
      tracelabels[i+k]->SetLabel(tracelabels[i+k+1]->GetLabel());
      vtracesizers[i+k]->Layout();
    }
  tracelabels[i+j]->SetLabel(wxT(""));
  vtracesizers[i+j]->Layout();
  toptracesizer->Hide(vtracesizers[i+j], true);
  toptracesizer->Layout();
  topsizer->Layout();

  /*
  tracelabels[i]->SetLabel(wxT(""));
  vtracesizers[i]->Layout();
  toptracesizer->Hide(vtracesizers[i], true);
  toptracesizer->Layout();
  topsizer->Layout();
  */

  rem_monitor->Delete(rem_monitor->FindString(rem_monitor->GetValue()));
  rem_monitor->SetValue(wxT(""));
  runnetwork(0);
  //cout << "Remove trace at monitor point: " << rem_monitor->GetValue().ToAscii() << endl;
}

