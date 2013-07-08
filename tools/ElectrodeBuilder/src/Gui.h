///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __Gui__
#define __Gui__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/sizer.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include <wx/dialog.h>
#include <wx/log.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_panel1;
		wxStaticText* stimuli_text;
		wxStaticText* stimuli_text1;
		wxStaticText* electrode_text;
		wxStaticText* time_text;
		wxStaticText* range_text;
		
		wxTextCtrl* time_textCtrl;
		wxTextCtrl* range_init_textCtrl;
		wxTextCtrl* range_end_texCtrl;
		wxButton* append_btn;
		wxButton* remove_btn;	
		wxButton* finish_btn;
		wxTextCtrl* status_textCtrl;
		wxLog* log_status;
		
		// Virtual event handlers, overide them in your derived class
		//virtual void OnChooseFile( wxFileDirPickerEvent& event ){ event.Skip(); }
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ){ event.Skip(); }
		virtual void On_Electrode_Change( wxCommandEvent& event ){ event.Skip(); }
		virtual void On_Append_Click( wxCommandEvent& event ){ event.Skip(); }
		virtual void On_Remove_Click( wxCommandEvent& event ){ event.Skip(); }
		virtual void On_Finish_Click( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxComboBox* electrode_comboBox;
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ElectrodeBuilder"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxTAB_TRAVERSAL );
		~MainFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ParametersFrame
///////////////////////////////////////////////////////////////////////////////
class ParametersFrame : public wxDialog 
{
	private:
	
	protected:
		wxGrid* table;
		wxButton* Ok_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void On_Init(wxInitDialogEvent& event){ event.Skip(); }
		virtual void On_Ok_Click( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		ParametersFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Parameters"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION );
		~ParametersFrame();
	
};

#endif //__Gui__
