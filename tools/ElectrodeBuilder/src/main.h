#ifndef __ELECTRODEBUILDER_H__
#define __ELECTRODEBUILDER_H__

/** @brief Main class used for Initialize the application.

    Detailed description follows here.
    @author Sousa, M.
    @date August 2010
    */


#ifdef WX_GCH  
#include "wx/wx_pch.h"  
#else  
#include "wx/wx.h"  
#endif

#include "Gui.h"
#include "ElectrodeBuilder_MainFrame.h"
#include "ElectrodeBuilder_ParametersFrame.h"

using namespace std;

class ElectrodeBuilder : public wxApp{
/** Default constructor. Does something.*/

    public:
	/** Initializes the application. */
        virtual bool OnInit(); 

};

wxString dbl2wxStr(double mydbl);


struct ELECTRODE{
  char elect[25];
  double *params;
  double time;
  long range_i;
  long range_o;
};


#endif // ELECTRODEBUILDER_H
