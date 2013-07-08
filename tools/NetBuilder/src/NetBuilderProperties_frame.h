#ifndef __NetBuilderProperties_frame__
#define __NetBuilderProperties_frame__

/**
@file
Subclass of Properties_frame, which is generated by wxFormBuilder.
*/

#include "gui.h"
#include "data_structures.h"
#include "NetBuilderDynamics_frame.h"
#include "NetBuilderSummary_frame.h"


/** Implementing Properties_frame */
class NetBuilderProperties_frame : public Properties_frame
{
protected:

	// Handlers for Properties_frame events.
	void OnInitProperties( wxInitDialogEvent& event );
	void OnCellClick( wxGridEvent& event );
	//void Onbtn_pop_CancelClick( wxCommandEvent& event );
	void Onbtn_pop_OKClick( wxCommandEvent& event );
	
public:
	/** Constructor */
	//void Update_Size();
	NetBuilderProperties_frame( wxWindow* parent );
};

#endif // __NetBuilderProperties_frame__