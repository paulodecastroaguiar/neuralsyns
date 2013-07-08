#ifndef _drawpane_
#define _drawpane_

//#include <GL/glut.h>
#include <wx/wx.h>
//#include <wx/glcanvas.h>
 

class BasicDrawPane : public wxPanel
{
    
public:
    BasicDrawPane(wxFrame* parent);
    
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    
    void render(wxDC& dc, struct COORDINATES *coordinates, int total_fdomains );
    
    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
    
    DECLARE_EVENT_TABLE()
};

struct COORDINATES{
  double radius;
  double x;
  double y;
  double z;
  int parent_id;
};
#endif