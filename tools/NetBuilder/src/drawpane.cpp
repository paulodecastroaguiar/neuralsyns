#include <wx/wx.h>
#include <wx/sizer.h>
//#include <wx/glcanvas.h>
#include "main.h"
 
// include OpenGL
// #ifdef __WXMAC__
// #include <penGL/glu.h>
// #include <OpenGL/gl.h>
// #else
// #include <GL/glu.h>
// #include <GL/gl.h>
// #endif

struct COORDINATES *coordinates = NULL;
extern int tt_fdomains;
 
BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
// some useful events
/*
 EVT_MOTION(BasicDrawPane::mouseMoved)
 EVT_LEFT_DOWN(BasicDrawPane::mouseDown)
 EVT_LEFT_UP(BasicDrawPane::mouseReleased)
 EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
 EVT_LEAVE_WINDOW(BasicDrawPane::mouseLeftWindow)
 EVT_KEY_DOWN(BasicDrawPane::keyPressed)
 EVT_KEY_UP(BasicDrawPane::keyReleased)
 EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
 */
 
// catch paint events
EVT_PAINT(BasicDrawPane::paintEvent)
 
END_EVENT_TABLE()


BasicDrawPane::BasicDrawPane(wxFrame* parent) :
wxPanel(parent)
{
}
 
/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */
 
void BasicDrawPane::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc, coordinates, tt_fdomains);
}
 
/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void BasicDrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc, coordinates, tt_fdomains);
}
 
/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void BasicDrawPane::render(wxDC&  dc, struct COORDINATES *coordinates, int total_fdomains)
{
  
   double max_x, max_y, min_x, min_y;
   max_x=min_x=coordinates[0].x;
   max_y=min_y=coordinates[0].y;
    
    for(int fdm=1; fdm<total_fdomains;fdm++){
	double new_x=coordinates[fdm].x;
	double new_y=coordinates[fdm].y;
	if (new_x>max_x)
	  max_x=new_x;
	if (new_x<min_x)
	  min_x=new_x;
	if (new_y>max_y)
	  max_y=new_y;
	if (new_y<min_y)
	  min_y=new_y;
    }
  
  double scale= max_x; //pixel/um
  if(scale < -min_x)
    scale = -min_x;
  if(scale < max_y)
    scale = max_y;
  if(scale < -min_y)
    scale = -min_y;
  if(scale < coordinates[0].radius)
    scale = coordinates[0].radius;
     
  scale = 0.9 * 100.0/scale;
   
  
  ///draw functional domains
  for(int fdm=1; fdm<total_fdomains;fdm++){
      int pid=coordinates[fdm].parent_id;
      float x0 = coordinates[pid].x * scale + 100;
      float y0 = -coordinates[pid].y * scale + 100; //mudança de coordenadas pois no glpan y vai de cima para baixo
      float x1 = coordinates[fdm].x * scale + 100 ;
      float y1 = -coordinates[fdm].y * scale + 100 ;
   
      dc.SetBrush(*wxBLUE_BRUSH); // blue filling
      dc.SetPen( wxPen( wxColor(0,0,0), coordinates[fdm].radius * scale ) ); // 10-pixels-thick pink outline
      dc.DrawLine(x0, y0, x1, y1); // draw line across the rectangle
  }
      
   ///draw soma
  
  int R = coordinates[0].radius * scale;
  dc.SetBrush(*wxBLACK_BRUSH); 
  dc.SetPen( wxPen( wxColor(255,0,0), 1) ); // pixels-thick outline
  dc.DrawRectangle(-R+100, -R+100, 2*R, 2*R); 
  
}
