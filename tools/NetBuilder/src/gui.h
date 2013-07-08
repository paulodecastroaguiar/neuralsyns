//////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////
/** @brief All frames and dialogs construction (build in wxFormBuilder v3.0).

    Detailed description follows here.
    @author Sousa, M.
    @date October 2009
    */

#ifndef __gui__
#define __gui__

#include <wx/colour.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/stattext.h>
#include <wx/radiobut.h>
#include <wx/filepicker.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include <wx/dialog.h>
#include <wx/choice.h>
#include <wx/log.h>
#include <wx/tglbtn.h>
#include <wx/progdlg.h>
#include "drawpane.h"
#include <math.h>


#include "GridCellChoiceRenderer.h"
#include "FastComboEditor.h"

///////////////////////////////////////////////////////////////////////////

#define idMenuQuit 1001
#define idMenuPy 1000
#define idMenuAbout 1002
#define COLORBASE wxColour(208,208,208)

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
	
		wxMenu* helpMenu;
		wxStatusBar* statusBar;
		wxPanel* m_panel1;
		wxPanel* m_panel111;
		wxPanel* m_panel121;
		wxPanel* m_panel122;
		wxPanel* m_panel123;
		wxStaticText* text_title;
		wxRadioButton* rad_edit;
		wxRadioButton* rad_new;
		wxButton* btn_pop_properties;
		wxButton* btn_pop_topology;
		wxLog* log_status;

		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnPython( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnSelectRB( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnFileChanged( wxFileDirPickerEvent& event ){ event.Skip(); }
		virtual void OnCreateRB( wxCommandEvent& event ){ event.Skip(); }
		virtual void On_total_population_Update( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnPropertiesClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTopopolyClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnConnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnFdmsClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDistributionsClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDelaysClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnConductancesClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnPlasticityClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void On_btn_summary_Click( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnStoreClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCreateClick( wxCommandEvent& event ){ event.Skip(); }
		//virtual void OnPythonClick( wxCommandEvent& event ){ event.Skip(); }
		//virtual void On_Status_TextEnter( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxMenuBar* mbar;
		wxMenu* fileMenu;
		//wxMenuItem* menuFilePy;
		wxMenuItem* menuFileQuit;
		wxFilePickerCtrl* m_filePicker;
		wxTextCtrl* edt_total_population;
		wxButton* btn_conn_matrix;
		wxButton* btn_conn_fdms;
		wxButton* btn_conn_distributions;
		wxButton* btn_syn_delays;
		wxButton* btn_syn_conductances;
		
		wxButton* btn_syn_plasticity;
		
		wxButton* btn_store;
		wxButton* btn_createnet;
		wxToggleButton* btn_summary;
		//wxButton* btn_python;
		wxTextCtrl* text_status;

		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("NeuroSyns Network Builder"), const wxPoint& pos = wxPoint( 300,100 ), const wxSize& size = wxSize( -1,670 ), long style = wxCAPTION|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSTAY_ON_TOP|wxTAB_TRAVERSAL );
		~MainFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Properties_frame
///////////////////////////////////////////////////////////////////////////////
class Properties_frame : public wxDialog 
{
	private:
	
	protected:
		
		wxGrid* Properties_grid;
		wxButton* btn_pop_Cancel;
		wxButton* btn_pop_OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitProperties( wxInitDialogEvent& event ){ event.Skip(); }
		virtual void OnCellClick( wxGridEvent& event ){ event.Skip(); }
		virtual void Onbtn_pop_CancelClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void Onbtn_pop_OKClick( wxCommandEvent& event ){ event.Skip(); }
	
	public:
		wxPanel* m_panel2;
		Properties_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Population Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP );
		~Properties_frame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Topology_frame
///////////////////////////////////////////////////////////////////////////////
class Topology_frame : public wxDialog 
{
	private:
	
	protected:
		
		wxGrid* topology_grid;
		wxButton* btn_top_Cancel;
		wxButton* btn_top_OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitTopology( wxInitDialogEvent& event ){ event.Skip(); }
		virtual void Onbtn_top_CancelClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void Onbtn_top_OKClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxPanel* m_panel3;	
		Topology_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Topology"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP );
		~Topology_frame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Connectivity_frame
///////////////////////////////////////////////////////////////////////////////
class Connectivity_frame : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel4;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitConnectivity( wxInitDialogEvent& event ){ event.Skip(); }
		virtual void Onbtn_conn_OptionsClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void Onbtn_conn_OKClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxGrid* conn_grid;
		wxButton* btn_conn_Options;
		wxButton* btn_conn_OK;
		Connectivity_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Connectivity Matrix"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP );
		~Connectivity_frame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Options_frame
//////////////////////////////////////////////////////////////////////////////
class Options_frame : public wxDialog
{
	private:
	
	protected:
		wxPanel* m_panel18;
		wxStaticText* m_staticText7;
		wxChoice* syn_choices;
		wxStaticText* m_staticText8;
		wxChoice* conn_choices;
		wxStaticText* m_staticText9;
		wxChoice* auto_choices;
		wxStaticText* m_staticText10;
		wxChoice* mult_choices;
		wxButton* btn_options_OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitOptions( wxInitDialogEvent& event ){ event.Skip(); }
		virtual void On_syn_choices_Selection( wxCommandEvent& event ){ event.Skip(); }
		virtual void On_conn_choices_Selection( wxCommandEvent& event ){ event.Skip(); }
		virtual void On_auto_choices_Selection( wxCommandEvent& event ){ event.Skip(); }
		virtual void On_mult_choices_Selection( wxCommandEvent& event ){ event.Skip(); }
		virtual void On_btn_options_OKClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		Options_frame( wxDialog* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Options"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxSTAY_ON_TOP );
		~Options_frame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Fdms_frame
///////////////////////////////////////////////////////////////////////////////
class Fdms_frame : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel5;
		wxGrid* fdms_grid;
		wxStaticText* text1;
		//wxButton* btn_fdms_Cancel;
		wxButton* btn_fdms_OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitFdms( wxInitDialogEvent& event ){ event.Skip(); }
		//virtual void Onbtn_fdms_CancelClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void Onbtn_fdms_OKClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCellClickFdms( wxGridEvent& event ){ event.Skip(); }
		
	
	public:
		Fdms_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Target fdms"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP );
		~Fdms_frame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Distributions_frame
///////////////////////////////////////////////////////////////////////////////
class Distributions_frame : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel6;
		wxGrid* dist_grid;
		wxStaticText* m_staticText7;
		wxButton* btn_dist_OK;
		
		// Virtual event handlers, overide them in your derived classpos = wxPoint( 1005,325 )
		virtual void OnInitdist( wxInitDialogEvent& event ){ event.Skip(); }
		virtual void OnCellClick(  wxGridEvent& event ){ event.Skip(); }
		virtual void Onbtn_dist_OKClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		Distributions_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Distribution"), const wxPoint& pos = wxPoint( 200,400 ), const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP );
		~Distributions_frame();
	
};



///////////////////////////////////////////////////////////////////////////////
/// Class Sigma
///////////////////////////////////////////////////////////////////////////////
class Sigma : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrl1;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrl2;
		wxStaticText* m_staticText3;
		wxTextCtrl* m_textCtrl3;
		
		wxButton* ok_button;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitSigma( wxInitDialogEvent& event ){ event.Skip(); }
		virtual void Ok_Button_Click( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		Sigma( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title=wxT(""), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxCLOSE_BOX );
		~Sigma();
	
};


///////////////////////////////////////////////////////////////////////////////
/// Class Delays_frame
///////////////////////////////////////////////////////////////////////////////
class Delays_frame : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel5;
		wxGrid* delays_grid;
		wxStaticText* text1;
		wxButton* btn_delays_OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitDelays( wxInitDialogEvent& event ){ event.Skip(); }
		//virtual void Onbtn_fdms_CancelClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void Onbtn_delays_OKClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCellClickDelays( wxGridEvent& event ){ event.Skip(); }
		
	
	public:
		Delays_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Delays"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP );
		~Delays_frame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Conductances_frame
///////////////////////////////////////////////////////////////////////////////
class Conductances_frame : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel7;
		wxGrid* conductances_grid;
		wxButton* btn_Conductances_Cancel;
		wxButton* btn_Conductances_OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitConductances( wxInitDialogEvent& event ){ event.Skip(); }
		virtual void OnCellClick( wxGridEvent& event ){ event.Skip(); }
		virtual void Onbtn_Conductances_CancelClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void Onbtn_Conductances_OKClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		Conductances_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Synaptic conductances"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP );
		~Conductances_frame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Plasticity_frame
///////////////////////////////////////////////////////////////////////////////
class Plasticity_frame : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel8;
		
		wxGrid* plasticity_grid;
		wxPanel* m_panel82;
		wxButton* btn_Plasticity_OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitPlasticity( wxInitDialogEvent& event ){ event.Skip(); }
		virtual void OnCellClick( wxGridEvent& event ){ event.Skip(); }
		virtual void Onbtn_Plasticity_OKClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		Plasticity_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Synaptic plasticity"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP );
		~Plasticity_frame();
wxPanel* m_panel81;
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Model_frame
///////////////////////////////////////////////////////////////////////////////
class Model_frame : public wxDialog 
{
	private:
	
	protected:
		
		wxGrid* model_grid;
		wxButton* btn_Model_OK;
		
		// Virtual event handlers, overide them in your derived class
	        virtual void OnInitModel (wxInitDialogEvent& event ){ event.Skip(); }
		virtual void Onbtn_conn_OptionsClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void Onbtn_Model_OKClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxPanel* m_panel9;
		Model_frame( wxDialog* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Parameters"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP);
		~Model_frame();
	
};


///////////////////////////////////////////////////////////////////////////////
/// Class Dynamics_frame
///////////////////////////////////////////////////////////////////////////////
class Dynamics_frame : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel10;
		
		wxButton* btn_Dynamics_OK;
		wxButton* btn_view_neuron;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitDynamics( wxInitDialogEvent& event ){ event.Skip(); }
		virtual void OnCellClick( wxGridEvent& event ){ event.Skip(); }
		virtual void Onbtn_Dynamics_OKClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void Onbtn_view_neuronClick( wxCommandEvent& event ){ event.Skip(); }
	
	public:
		wxGrid* dynamics_grid;
		Dynamics_frame( wxDialog* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Dynamics model"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxSTAY_ON_TOP );
		~Dynamics_frame();
	
};


///////////////////////////////////////////////////////////////////////////////
/// Class Summary_frame
///////////////////////////////////////////////////////////////////////////////
class Summary_frame : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_panel11;
		wxGrid* summary_grid;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseSummary( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnInitSummary( wxInitDialogEvent& event ){ event.Skip(); }
		
	
	public:
		Summary_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Summary"), const wxPoint& pos = wxPoint( 830,50), const wxSize& size = wxSize(-1,650), long style = wxCAPTION|wxCLOSE_BOX|wxTAB_TRAVERSAL|wxHSCROLL);
		~Summary_frame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Visualization_frame
///////////////////////////////////////////////////////////////////////////////
class Visualization_frame : public wxDialog 
{
	private:
	
	protected:
		BasicDrawPane * drawPane;
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnInitVisualization( wxInitDialogEvent& event ){ event.Skip(); }
	
	public:
		Visualization_frame( wxDialog* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Visualization"), const wxPoint& pos = wxPoint( -1,300 ), const wxSize& size = wxSize(200,200 ), long style = wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX);
		~Visualization_frame();
	
};



///////////////////////////////////////////////////////////////////////////////
/// Class Python_frame
///////////////////////////////////////////////////////////////////////////////
// class Python_frame : public wxFrame 
// {
// 	private:
// 	
// 	protected:
// 		wxTextCtrl* text_python;
// 		
// 		// Virtual event handlers, overide them in your derived class
// 		virtual void OnTextEnter( wxCommandEvent& event ){ event.Skip(); }
// 		virtual void OnClosePython( wxCloseEvent& event ){ event.Skip(); }
// 		virtual void OnInitPython( wxInitDialogEvent& event ){ event.Skip(); }
// 		
// 	
// 	public:
// 		Python_frame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Python Interpreter"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxSTAY_ON_TOP|wxTAB_TRAVERSAL|wxVSCROLL );
// 		~Python_frame();
// 	
// };



#endif //__gui__
