#ifndef SPINET_H
#define SPINET_H

/** @brief Main class used for Initialize the application.

    Detailed description follows here.
    @author Sousa, M.
    @date October 2009
    */


#ifdef WX_GCH  
#include "wx/wx_pch.h"  
#else  
#include "wx/wx.h"  
#endif

#include "gui.h"
#include "NetBuilderMainFrame.h"
#include "NetBuilderSummary_frame.h"

#include <wx/app.h>
#include <wx/statline.h>
#include <wx/generic/aboutdlgg.h>
#include <wx/filepicker.h>
#include <sys/types.h>
#include <wx/window.h>
#include "wx/glcanvas.h"
#include "drawpane.h"

//#include <Python.h>

class NetBuilder : public wxApp{
/** Default constructor. Does something.*/

    public:
	/** Initializes the application. */
        virtual bool OnInit(); 

	unsigned int total_populations; /**<the number of populations*/
	
};

#endif // SPINET_H
