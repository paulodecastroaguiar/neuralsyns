///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "gui.h"


///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	//this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );
	this->SetBackgroundColour( COLORBASE );

	mbar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Quit") ),  wxT("Quit the application"), wxITEM_NORMAL );

	fileMenu->Append( menuFileQuit );

	//menuFilePy = new wxMenuItem( fileMenu, idMenuPy, wxString( wxT("&Execute Python") ), wxT("Execute Python at Status"), wxITEM_CHECK);

	//fileMenu->Append( menuFilePy );

	mbar->Append( fileMenu, wxT("&File") );

	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );

	mbar->Append( helpMenu, wxT("&Help") );

	this->SetMenuBar( mbar );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1->SetBackgroundColour( COLORBASE );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	text_title = new wxStaticText( m_panel1, wxID_ANY, wxT("NeuralSyns Network Builder"), wxDefaultPosition, wxDefaultSize, 0 );
	text_title->Wrap( -1 );
	text_title->SetFont( wxFont( 20, 70, 90, 90, false, wxEmptyString ) );

	bSizer11->Add( text_title, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxStaticBoxSizer* pan_choose;
	pan_choose = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Please choose") ), wxVERTICAL );

	m_panel111 = new wxPanel( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel111->SetBackgroundColour( COLORBASE );
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );

	rad_edit = new wxRadioButton( m_panel111, wxID_ANY, wxT("Edit an existing network data file"), wxDefaultPosition, wxDefaultSize, 0 );
	rad_edit->SetValue( true );
	//rad_edit->SetToolTip( wxT("Select to open a .xml file") );

	gSizer1->Add( rad_edit, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_filePicker = new wxFilePickerCtrl( m_panel111, wxID_ANY, _("/home/mafalda"), wxT("Select a network .dat file"), wxT("*.dat"), wxDefaultPosition, wxDefaultSize, wxFLP_CHANGE_DIR|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN );
	gSizer1->Add( m_filePicker, 0, wxALL|wxEXPAND, 5 );

	rad_new = new wxRadioButton( m_panel111, wxID_ANY, wxT("Create a network from scratch"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1->Add( rad_new, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	edt_total_population = new wxTextCtrl( m_panel111, wxID_ANY, wxT("Enter the number of populations"), wxPoint( -1,-1 ), wxDefaultSize, wxTE_PROCESS_ENTER );
	edt_total_population->Enable( false );

	gSizer1->Add( edt_total_population, 0, wxALL|wxEXPAND, 5 );

	m_panel111->SetSizer( gSizer1 );
	m_panel111->Layout();
	gSizer1->Fit( m_panel111 );
	pan_choose->Add( m_panel111, 1, wxEXPAND | wxALL, 5 );

	bSizer11->Add( pan_choose, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* pan_poppulations;
	pan_poppulations = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Populations") ), wxVERTICAL );

	m_panel121 = new wxPanel( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel121->SetBackgroundColour( COLORBASE );
	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxVERTICAL );


	bSizer121->Add( 0, 0, 1, wxEXPAND, 5 );

	btn_pop_properties = new wxButton( m_panel121, wxID_ANY, wxT("properties"), wxPoint( -1,-1 ), wxSize( 120,30 ), 0 );
	btn_pop_properties->Enable( false );
	btn_pop_properties->SetToolTip( wxT("Set general properties for each population") );

	bSizer121->Add( btn_pop_properties, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	bSizer121->Add( 0, 0, 1, wxEXPAND, 5 );

	btn_pop_topology = new wxButton( m_panel121, wxID_ANY, wxT("topology"), wxDefaultPosition, wxSize( 120,30 ), 0 );
	btn_pop_topology->Enable( false );
	btn_pop_topology->SetToolTip( wxT("Place the populations in 3D space") );

	bSizer121->Add( btn_pop_topology, 0, wxALL, 5 );


	bSizer121->Add( 0, 0, 1, wxEXPAND, 5 );

	m_panel121->SetSizer( bSizer121 );
	m_panel121->Layout();
	bSizer121->Fit( m_panel121 );
	pan_poppulations->Add( m_panel121, 1, wxEXPAND | wxALL, 5 );

	fgSizer1->Add( pan_poppulations, 0, wxEXPAND|wxALL, 5 );

	wxStaticBoxSizer* pan_Connectivity;
	pan_Connectivity = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Connectivity") ), wxVERTICAL );

	m_panel122 = new wxPanel( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel122->SetBackgroundColour( COLORBASE );
	wxBoxSizer* bSizer122;
	bSizer122 = new wxBoxSizer( wxVERTICAL );

	btn_conn_matrix = new wxButton( m_panel122, wxID_ANY, wxT("conn matrix"), wxDefaultPosition, wxSize( 120,30 ), 0 );
	btn_conn_matrix->Enable( false );
	btn_conn_matrix->SetToolTip( wxT("Set the network's connectivity") );

	bSizer122->Add( btn_conn_matrix, 0, wxALL, 5 );

	btn_conn_fdms = new wxButton( m_panel122, wxID_ANY, wxT("target fdms"), wxDefaultPosition, wxSize( 120,30 ), 0 );
	btn_conn_fdms->Enable( false );
	btn_conn_fdms->SetToolTip( wxT("Define the target fdomains for each connection") );

	bSizer122->Add( btn_conn_fdms, 0, wxALL, 5 );

	btn_conn_distributions = new wxButton( m_panel122, wxID_ANY, wxT("distributions"), wxDefaultPosition, wxSize( 120,30 ), 0 );
	btn_conn_distributions->Enable( false );
	btn_conn_distributions->SetToolTip( wxT("Set connectivity volumes using 3D Normal distributions") );

	bSizer122->Add( btn_conn_distributions, 0, wxALL, 5 );

	m_panel122->SetSizer( bSizer122 );
	m_panel122->Layout();
	bSizer122->Fit( m_panel122 );
	pan_Connectivity->Add( m_panel122, 1, wxEXPAND | wxALL, 5 );

	fgSizer1->Add( pan_Connectivity, 0, wxEXPAND|wxALL, 5 );

	wxStaticBoxSizer* pan_Synapses;
	pan_Synapses = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Synapses") ), wxVERTICAL );

	m_panel123 = new wxPanel( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel123->SetBackgroundColour( COLORBASE );
	wxBoxSizer* bSizer123;
	bSizer123 = new wxBoxSizer( wxVERTICAL );


	bSizer123->Add( 0, 0, 1, wxEXPAND, 5 );

	btn_syn_delays = new wxButton( m_panel123, wxID_ANY, wxT("delays"), wxDefaultPosition, wxSize( 120,30 ), 0 );
	btn_syn_delays->Enable( false );
	btn_syn_delays->SetToolTip( wxT("Set the synaptic delay for each connection") );

	bSizer123->Add( btn_syn_delays, 0, wxALL, 5 );

	btn_syn_conductances = new wxButton( m_panel123, wxID_ANY, wxT("conductances"), wxDefaultPosition, wxSize( 120,30 ), 0 );
	btn_syn_conductances->Enable( false );
	btn_syn_conductances->SetToolTip( wxT("Select conductance models and define parameters") );

	bSizer123->Add( btn_syn_conductances, 0, wxALL, 5 );


	bSizer123->Add( 0, 0, 1, wxEXPAND, 5 );

	btn_syn_plasticity = new wxButton( m_panel123, wxID_ANY, wxT("plasticity"), wxDefaultPosition, wxSize( 120,30 ), 0 );
	btn_syn_plasticity->Enable( false );
	btn_syn_plasticity->SetToolTip( wxT("Select plasticity models and define parameters") );

	bSizer123->Add( btn_syn_plasticity, 0, wxALL|wxEXPAND, 5 );


	bSizer123->Add( 0, 0, 1, wxEXPAND, 5 );

	m_panel123->SetSizer( bSizer123 );
	m_panel123->Layout();
	bSizer123->Fit( m_panel123 );
	pan_Synapses->Add( m_panel123, 1, wxEXPAND | wxALL, 5 );

	fgSizer1->Add( pan_Synapses, 0, wxEXPAND|wxALL, 5 );

	bSizer12->Add( fgSizer1, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5 );

	bSizer11->Add( bSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxSHAPED, 5 );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );

	btn_store = new wxButton( m_panel1, wxID_ANY, wxT("Store data file"), wxDefaultPosition, wxSize( 120,40 ), 0 );
	btn_store->Enable( false );
	btn_store->SetToolTip(wxT("Save NetBuilder data file"));
	bSizer13->Add( btn_store, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	btn_createnet = new wxButton( m_panel1, wxID_ANY, wxT("Create .net file"), wxDefaultPosition, wxSize( 120,40 ), 0 );
	btn_createnet->Enable( false );
	btn_createnet->SetToolTip(wxT("Create and save a simulation-ready file"));
	bSizer13->Add( btn_createnet, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	btn_summary = new wxToggleButton( m_panel1, wxID_ANY, wxT("Summary"), wxDefaultPosition, wxSize( 120,40 ), 0 );
	btn_summary->Enable( false );
	bSizer13->Add( btn_summary, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	btn_summary->SetToolTip(wxT("Review the network's main properties"));

	/*btn_python = new wxButton( m_panel1, wxID_ANY, wxT("Edit .net file"), wxDefaultPosition, wxSize( 90,40 ), 0 );
	btn_python->Enable( false );
	bSizer13->Add( btn_python, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	btn_python->SetToolTip(wxT("Edit .net file through python interpreter"));*/

	bSizer11->Add( bSizer13, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	wxStaticBoxSizer* pan_status;
	pan_status = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Status") ), wxVERTICAL );

	text_status = new wxTextCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,180),  wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_READONLY);
	text_status->SetFont( wxFont( 8, 70, 90, 90, false, wxEmptyString ));

	text_status->SetToolTip(wxT("This is your status textbox, with warning messages"));
	log_status = wxLog::SetActiveTarget(new wxLogTextCtrl(text_status));
	wxLog::SetTimestamp(NULL);
	pan_status->Add( text_status, 1, wxALL|wxEXPAND, 5 );

	bSizer11->Add( pan_status, 1, wxEXPAND|wxALL, 5 );

	m_panel1->SetSizer( bSizer11 );
	m_panel1->Layout();
	bSizer11->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND, 5 );

	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	this->Connect( menuFileQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnQuit ) );
	//this->Connect( menuFilePy->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnPython ) );
	this->Connect( menuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnAbout ) );
	rad_edit->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::OnSelectRB ), NULL, this );
	m_filePicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( MainFrame::OnFileChanged ), NULL, this );
	rad_new->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::OnCreateRB ), NULL, this );
    edt_total_population->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::On_total_population_Update ), NULL, this );
    btn_pop_properties->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnPropertiesClick ), NULL, this );
	btn_pop_topology->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnTopopolyClick ), NULL, this );
	btn_conn_matrix->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConnClick ), NULL, this );
	btn_conn_fdms->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnFdmsClick ), NULL, this );
	btn_conn_distributions->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnDistributionsClick ), NULL, this );
	btn_syn_delays->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnDelaysClick ), NULL, this );
	btn_syn_conductances->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConductancesClick ), NULL, this );
	btn_syn_plasticity->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnPlasticityClick ), NULL, this );
	btn_store->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnStoreClick ), NULL, this );
	btn_summary->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainFrame::On_btn_summary_Click ), NULL, this );
	btn_createnet->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnCreateClick ), NULL, this );
	//btn_python->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnPythonClick ), NULL, this );
	//text_status->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::On_Status_TextEnter ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnQuit ) );
	//this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnPython ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::OnAbout ) );
	rad_edit->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::OnSelectRB ), NULL, this );
	m_filePicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( MainFrame::OnFileChanged ), NULL, this );
	rad_new->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( MainFrame::OnCreateRB ), NULL, this );
	edt_total_population->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::On_total_population_Update ), NULL, this );
	btn_pop_properties->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnPropertiesClick ), NULL, this );
	btn_pop_topology->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnTopopolyClick ), NULL, this );
	btn_conn_matrix->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConnClick ), NULL, this );
	btn_conn_fdms->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnFdmsClick ), NULL, this );
	btn_conn_distributions->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnDistributionsClick ), NULL, this );
	btn_syn_delays->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnDelaysClick ), NULL, this );
	btn_syn_conductances->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnConductancesClick ), NULL, this );
	btn_syn_plasticity->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnPlasticityClick ), NULL, this );
	btn_store->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnStoreClick ), NULL, this );
	btn_summary->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainFrame::On_btn_summary_Click ), NULL, this );
	btn_createnet->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnCreateClick ), NULL, this );
	//btn_python->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnPythonClick ), NULL, this );

}

/////////////////
Properties_frame::Properties_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
  this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	this->SetBackgroundColour( COLORBASE );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_panel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel2->SetBackgroundColour( COLORBASE );
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer211;
	bSizer211 = new wxBoxSizer( wxVERTICAL );

	Properties_grid = new wxGrid( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize );

	// Grid
	Properties_grid->CreateGrid( 4, 5 );
	Properties_grid->EnableEditing( true );
	Properties_grid->EnableGridLines( true );
	Properties_grid->EnableDragGridSize( false );
	Properties_grid->SetMargins( 0, 0 );

	// Columns
	Properties_grid->EnableDragColMove( false );
	Properties_grid->EnableDragColSize( true );
	Properties_grid->SetColLabelSize( 20 );
	Properties_grid->SetColLabelValue( 0, wxT("Pop00") );
	Properties_grid->SetColLabelValue( 1, wxT("Pop01") );
	Properties_grid->SetColLabelValue( 2, wxT("Pop02") );
	Properties_grid->SetColLabelValue( 3, wxT("Pop03") );
	Properties_grid->SetColLabelValue( 4, wxT("Pop04") );
	Properties_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	Properties_grid->EnableDragRowSize( true );
	Properties_grid->SetRowSize( 3, 48 );
	Properties_grid->SetRowLabelSize( 100 );
	Properties_grid->SetRowLabelValue( 0, wxT("label") );
	Properties_grid->SetRowLabelValue( 1, wxT("size (X*Y*Z)") );
	Properties_grid->SetRowLabelValue( 2, wxT("fdomains") );
	Properties_grid->SetRowLabelValue( 3, wxT("dynamics") );
	Properties_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance
	Properties_grid->SetLabelBackgroundColour( COLORBASE );
	// Cell Defaults
	Properties_grid->SetDefaultCellBackgroundColour( COLORBASE );
	Properties_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	Properties_grid->SetForegroundColour(COLORBASE );
	Properties_grid->SetBackgroundColour( COLORBASE );
	bSizer211->Add( Properties_grid, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	bSizer21->Add( bSizer211, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	btn_pop_Cancel = new wxButton( m_panel2, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	btn_pop_Cancel->Hide();

	fgSizer2->Add( btn_pop_Cancel, 0, wxALL, 5 );

	btn_pop_OK = new wxButton( m_panel2, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( btn_pop_OK, 0, wxALL|wxALIGN_BOTTOM|wxALIGN_RIGHT, 5 );

	bSizer21->Add( fgSizer2, 0, wxALIGN_RIGHT|wxRIGHT|wxLEFT, 5 );

	m_panel2->SetSizer( bSizer21 );
	m_panel2->Layout();
	bSizer21->Fit( m_panel2 );
	bSizer2->Add( m_panel2, 1, wxEXPAND|wxALL, 5 );

	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Properties_frame::OnInitProperties ) );
	Properties_grid->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Properties_frame::OnCellClick ), NULL, this );
	btn_pop_Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Properties_frame::Onbtn_pop_CancelClick ), NULL, this );
	btn_pop_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Properties_frame::Onbtn_pop_OKClick ), NULL, this );
}

Properties_frame::~Properties_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Properties_frame::OnInitProperties ) );
	Properties_grid->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Properties_frame::OnCellClick ), NULL, this );
	btn_pop_Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Properties_frame::Onbtn_pop_CancelClick ), NULL, this );
	btn_pop_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Properties_frame::Onbtn_pop_OKClick ), NULL, this );
}

////////////////////////
Topology_frame::Topology_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_panel3 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel3->SetBackgroundColour( COLORBASE );
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer311;
	bSizer311 = new wxBoxSizer( wxVERTICAL );

	topology_grid = new wxGrid( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	// Grid
	topology_grid->CreateGrid( 5, 5 );
	topology_grid->EnableEditing( true );
	topology_grid->EnableGridLines( true );
	topology_grid->EnableDragGridSize( false );
	topology_grid->SetMargins( 0, 0 );

	// Columns
	topology_grid->SetDefaultColSize(95, true);
	topology_grid->EnableDragColMove( false );
	topology_grid->EnableDragColSize( true );
	topology_grid->SetColLabelSize( 50 );
	topology_grid->SetColLabelValue( 0, wxT("Pop00") );
	topology_grid->SetColLabelValue( 1, wxT("Pop01") );
	topology_grid->SetColLabelValue( 2, wxT("Pop02") );
	topology_grid->SetColLabelValue( 3, wxT("Pop03") );
	topology_grid->SetColLabelValue( 4, wxT("Pop04") );
	topology_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	topology_grid->EnableDragRowSize( true );

	topology_grid->SetRowLabelSize( 110 );
	topology_grid->SetRowLabelValue( 0, wxT("X0 [um]") );
	topology_grid->SetRowLabelValue( 1, wxT("Y0 [um]") );
	topology_grid->SetRowLabelValue( 2, wxT("Z0 [um]") );
	topology_grid->SetRowLabelValue( 3, wxT("d_mean [um]") );
	topology_grid->SetRowLabelValue( 4, wxT("d_std [um]") );
	topology_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance
	topology_grid->SetLabelBackgroundColour( COLORBASE );
	// Cell Defaults
	topology_grid->SetDefaultCellBackgroundColour( COLORBASE );
	topology_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	topology_grid->SetForegroundColour(COLORBASE );
	topology_grid->SetBackgroundColour( COLORBASE );
	bSizer311->Add( topology_grid, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	bSizer31->Add( bSizer311, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	btn_top_Cancel = new wxButton( m_panel3, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	btn_top_Cancel->Hide();

	fgSizer3->Add( btn_top_Cancel, 0, wxALL, 5 );

	btn_top_OK = new wxButton( m_panel3, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( btn_top_OK, 0, wxALL|wxALIGN_BOTTOM|wxALIGN_RIGHT, 5 );

	bSizer31->Add( fgSizer3, 0, wxALIGN_RIGHT|wxRIGHT|wxLEFT, 5 );

	m_panel3->SetSizer( bSizer31 );
	m_panel3->Layout();
	bSizer3->Fit( m_panel3 );
	bSizer3->Add( m_panel3, 1, wxEXPAND | wxALL, 5 );

	this->SetSizer( bSizer3 );
	this->Layout();
	bSizer3->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Topology_frame::OnInitTopology ) );
	btn_top_Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Topology_frame::Onbtn_top_CancelClick ), NULL, this );
	btn_top_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Topology_frame::Onbtn_top_OKClick ), NULL, this );
}

Topology_frame::~Topology_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Topology_frame::OnInitTopology ) );
	btn_top_Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Topology_frame::Onbtn_top_CancelClick ), NULL, this );
	btn_top_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Topology_frame::Onbtn_top_OKClick ), NULL, this );
}

//////////////////////////////////////
Connectivity_frame::Connectivity_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel4->SetBackgroundColour( COLORBASE );
	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer( wxVERTICAL );


	conn_grid = new wxGrid( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER|wxFULL_REPAINT_ON_RESIZE);

	// Grid
	conn_grid->CreateGrid( 5, 5 );
	conn_grid->EnableEditing( true );
	conn_grid->EnableGridLines( true );
	conn_grid->EnableDragGridSize( false );
	conn_grid->SetMargins( 0, 0 );

	// Columns
	conn_grid->SetDefaultColSize(95, true);
	conn_grid->EnableDragColMove( false );
	conn_grid->EnableDragColSize( true );
	conn_grid->SetColLabelSize( 30 );
	conn_grid->SetColLabelValue( 0, wxT("Pop00") );
	conn_grid->SetColLabelValue( 1, wxT("Pop01") );
	conn_grid->SetColLabelValue( 2, wxT("Pop02") );
	conn_grid->SetColLabelValue( 3, wxT("Pop03") );
	conn_grid->SetColLabelValue( 4, wxT("Pop04") );
	conn_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	conn_grid->EnableDragRowSize( true );
	conn_grid->SetRowLabelSize( 95 );
	conn_grid->SetRowLabelValue( 0, wxT("Pop00") );
	conn_grid->SetRowLabelValue( 1, wxT("Pop01") );
	conn_grid->SetRowLabelValue( 2, wxT("Pop02") );
	conn_grid->SetRowLabelValue( 3, wxT("Pop03") );
	conn_grid->SetRowLabelValue( 4, wxT("Pop04") );
	conn_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance
	conn_grid->SetLabelBackgroundColour( COLORBASE );
	// Cell Defaults
	conn_grid->SetDefaultCellBackgroundColour( COLORBASE );
	conn_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	conn_grid->SetForegroundColour(COLORBASE );
	conn_grid->SetBackgroundColour( COLORBASE );

	bSizer41->Add( conn_grid, 1, wxALIGN_CENTER_VERTICAL| wxALL, 5 );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	btn_conn_Options = new wxButton( m_panel4, wxID_ANY, wxT("&Options"), wxDefaultPosition, wxDefaultSize, 0 );

	btn_conn_OK = new wxButton( m_panel4, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );

	fgSizer4->Add( btn_conn_Options, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	fgSizer4->Add( btn_conn_OK, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	bSizer41->Add( fgSizer4, 0, wxALIGN_RIGHT|wxRIGHT|wxLEFT, 5 );

	m_panel4->SetSizer( bSizer41 );
	m_panel4->Layout();
	bSizer41->Fit( m_panel4 );
	bSizer4->Add( m_panel4, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxALL, 5 );

	this->SetSizer( bSizer4 );
	this->Layout();
	bSizer4->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Connectivity_frame::OnInitConnectivity ) );
	btn_conn_Options->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Connectivity_frame::Onbtn_conn_OptionsClick ), NULL, this );
	btn_conn_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Connectivity_frame::Onbtn_conn_OKClick ), NULL, this );
}

Connectivity_frame::~Connectivity_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Connectivity_frame::OnInitConnectivity ) );
	btn_conn_Options->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Connectivity_frame::Onbtn_conn_OptionsClick ), NULL, this );
	btn_conn_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Connectivity_frame::Onbtn_conn_OKClick ), NULL, this );
}

///////////////////////////
Options_frame::Options_frame( wxDialog* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxVERTICAL );

	m_panel18 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel18->SetBackgroundColour( COLORBASE );
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText7 = new wxStaticText( m_panel18, wxID_ANY, wxT("Synapses"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	m_staticText7->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	fgSizer8->Add( m_staticText7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString syn_choicesChoices[] = { wxT("Homogeneous"), wxT("Heterogeneous") };
	int syn_choicesNChoices = sizeof( syn_choicesChoices ) / sizeof( wxString );
	syn_choices = new wxChoice( m_panel18, wxID_ANY, wxDefaultPosition, wxDefaultSize, syn_choicesNChoices, syn_choicesChoices, 0 );
	syn_choices->SetSelection( 0 );
	syn_choices->SetToolTip(_("Homogeneous: synapses within a connection pair share the same parameters;\nHeterogeneous: each synapse have its own copy of the parameters."));
	fgSizer8->Add( syn_choices, 0, wxALL|wxEXPAND, 5 );

	m_staticText8 = new wxStaticText( m_panel18, wxID_ANY, wxT("Connectivity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	m_staticText8->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	fgSizer8->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString conn_choicesChoices[] = { wxT("Divergent"), wxT("Convergent") };
	int conn_choicesNChoices = sizeof( conn_choicesChoices ) / sizeof( wxString );
	conn_choices = new wxChoice( m_panel18, wxID_ANY, wxDefaultPosition, wxDefaultSize, conn_choicesNChoices, conn_choicesChoices, 0 );
	conn_choices->SetSelection( 0 );
	conn_choices->SetToolTip(_("Divergent: each source sends 'n' synapses; Convergent: each target receives 'n' synapses."));
	fgSizer8->Add( conn_choices, 0, wxALL|wxEXPAND, 5 );

	m_staticText9 = new wxStaticText( m_panel18, wxID_ANY, wxT("Auto-connections"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	m_staticText9->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	fgSizer8->Add( m_staticText9, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString auto_choicesChoices[] = { wxT("Disable"), wxT("Enable") };
	int auto_choicesNChoices = sizeof( auto_choicesChoices ) / sizeof( wxString );
	auto_choices = new wxChoice( m_panel18, wxID_ANY, wxDefaultPosition, wxDefaultSize, auto_choicesNChoices, auto_choicesChoices, 0 );
	auto_choices->SetSelection( 0 );
	auto_choices->SetToolTip(_("Enable or disable auto-connections when recurrent connections are being used."));
	fgSizer8->Add( auto_choices, 0, wxALL|wxEXPAND, 5 );

	m_staticText10 = new wxStaticText( m_panel18, wxID_ANY, wxT("Multiplicity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	m_staticText10->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	fgSizer8->Add( m_staticText10, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString mult_choicesChoices[] = { wxT("Disable"), wxT("Enable") };
	int mult_choicesNChoices = sizeof( mult_choicesChoices ) / sizeof( wxString );
	mult_choices = new wxChoice( m_panel18, wxID_ANY, wxDefaultPosition, wxDefaultSize, mult_choicesNChoices, mult_choicesChoices, 0 );
	mult_choices->SetSelection( 0 );
	mult_choices->SetToolTip(_("Enable or disable multiple connections from/to the same neuron (assuming convergence/divergence) when Uniform distributions are being used."));
	fgSizer8->Add( mult_choices, 0, wxALL|wxEXPAND, 5 );

	bSizer37->Add( fgSizer8, 1, wxEXPAND|wxALL, 5 );

	btn_options_OK = new wxButton( m_panel18, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer37->Add( btn_options_OK, 0, wxALL|wxALIGN_RIGHT, 5 );

	m_panel18->SetSizer( bSizer37 );
	m_panel18->Layout();
	bSizer37->Fit( m_panel18 );
	bSizer36->Add( m_panel18, 0, wxALL, 5 );

	this->SetSizer( bSizer36 );
	this->Layout();
	bSizer36->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Options_frame::OnInitOptions ) );
	syn_choices->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Options_frame::On_syn_choices_Selection ), NULL, this );
	conn_choices->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Options_frame::On_conn_choices_Selection ), NULL, this );
	auto_choices->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Options_frame::On_auto_choices_Selection ), NULL, this );
	mult_choices->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Options_frame::On_mult_choices_Selection ), NULL, this );
	btn_options_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Options_frame::On_btn_options_OKClick ), NULL, this );
}

Options_frame::~Options_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Options_frame::OnInitOptions ) );
	syn_choices->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Options_frame::On_syn_choices_Selection ), NULL, this );
	conn_choices->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Options_frame::On_conn_choices_Selection ), NULL, this );
	auto_choices->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Options_frame::On_auto_choices_Selection ), NULL, this );
	mult_choices->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Options_frame::On_mult_choices_Selection ), NULL, this );
	btn_options_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Options_frame::On_btn_options_OKClick ), NULL, this );
}

///////////////////////////////////////
Fdms_frame::Fdms_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_panel5 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel5->SetBackgroundColour( COLORBASE);
	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer511;
	bSizer511 = new wxBoxSizer( wxVERTICAL );

	fdms_grid = new wxGrid( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	// Grid
	fdms_grid->CreateGrid( 5,5 );
	fdms_grid->EnableEditing( true );
	fdms_grid->EnableGridLines( true );
	fdms_grid->EnableDragGridSize( false );
	fdms_grid->SetMargins( 0, 0 );

	// Columns
	fdms_grid->SetDefaultColSize(95, true);
	fdms_grid->EnableDragColMove( false );
	fdms_grid->EnableDragColSize( true );
	fdms_grid->SetColLabelSize( 30 );
	fdms_grid->SetColLabelValue( 0, wxT("Pop00") );
	fdms_grid->SetColLabelValue( 1, wxT("Pop01") );
	fdms_grid->SetColLabelValue( 2, wxT("Pop02") );
	fdms_grid->SetColLabelValue( 3, wxT("Pop03") );
	fdms_grid->SetColLabelValue( 4, wxT("Pop04") );
	fdms_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	fdms_grid->EnableDragRowSize( true );
	fdms_grid->SetRowLabelSize( 95 );
	fdms_grid->SetRowLabelValue( 0, wxT("Pop00") );
	fdms_grid->SetRowLabelValue( 1, wxT("Pop01") );
	fdms_grid->SetRowLabelValue( 2, wxT("Pop02") );
	fdms_grid->SetRowLabelValue( 3, wxT("Pop03") );
	fdms_grid->SetRowLabelValue( 4, wxT("Pop04") );
	fdms_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance
	fdms_grid->SetLabelBackgroundColour( COLORBASE );
	// Cell Defaults
	fdms_grid->SetDefaultCellBackgroundColour( COLORBASE );
	fdms_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	fdms_grid->SetForegroundColour(COLORBASE );
	fdms_grid->SetBackgroundColour( COLORBASE );
	bSizer511->Add( fdms_grid, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	bSizer51->Add( bSizer511, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	btn_fdms_OK = new wxButton( m_panel5, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( btn_fdms_OK, 1, wxALL|wxALIGN_BOTTOM|wxEXPAND|wxALIGN_RIGHT, 5 );

	bSizer51->Add( fgSizer5, 0, wxALIGN_RIGHT, 5 );

	m_panel5->SetSizer( bSizer51 );
	m_panel5->Layout();
	bSizer51->Fit( m_panel5 );
	bSizer5->Add( m_panel5, 1, wxEXPAND | wxALL, 5 );

	this->SetSizer( bSizer5 );
	this->Layout();
	bSizer5->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Fdms_frame::OnInitFdms ) );
	fdms_grid->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Fdms_frame::OnCellClickFdms ), NULL, this );
	btn_fdms_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Fdms_frame::Onbtn_fdms_OKClick ), NULL, this );
}

Fdms_frame::~Fdms_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Fdms_frame::OnInitFdms ) );
	fdms_grid->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Fdms_frame::OnCellClickFdms ), NULL, this );
	btn_fdms_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Fdms_frame::Onbtn_fdms_OKClick ), NULL, this );
}



/////////////////////////////////////////
Distributions_frame::Distributions_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_panel6 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel6->SetBackgroundColour( COLORBASE);
	m_panel6->SetToolTip( wxT("Distribution is defined by three sigmas (x,y,z)\nA Uniform distribution is used when sigma(.)>dim(.)*d_mean") );
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer611;
	bSizer611 = new wxBoxSizer( wxVERTICAL );

	dist_grid = new wxGrid( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize );

	// Grid
	dist_grid->CreateGrid( 5, 5 );
	dist_grid->EnableEditing( true );
	dist_grid->EnableGridLines( true );
	dist_grid->EnableDragGridSize( false );
	dist_grid->SetMargins( 0, 0 );

	// Columns

	dist_grid->SetDefaultColSize(95, true);
	dist_grid->EnableDragColMove( false );
	dist_grid->EnableDragColSize( true );
	//dist_grid->SetColLabelSize( 30 );
	dist_grid->SetColLabelValue( 0, wxT("Pop00") );
	dist_grid->SetColLabelValue( 1, wxT("Pop01") );
	dist_grid->SetColLabelValue( 2, wxT("Pop02") );
	dist_grid->SetColLabelValue( 3, wxT("Pop03") );
	dist_grid->SetColLabelValue( 4, wxT("Pop04") );
	dist_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	dist_grid->EnableDragRowSize( true );
	dist_grid->SetRowLabelSize( 95 );
	for (int i=0; i<5; i++)
	    dist_grid->SetRowSize(i,60);
	dist_grid->SetRowLabelValue( 0, wxT("Pop00") );
	dist_grid->SetRowLabelValue( 1, wxT("Pop01") );
	dist_grid->SetRowLabelValue( 2, wxT("Pop02") );
	dist_grid->SetRowLabelValue( 3, wxT("Pop03") );
	dist_grid->SetRowLabelValue( 4, wxT("Pop04") );
	dist_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance
	dist_grid->SetLabelBackgroundColour( COLORBASE );
	// Cell Defaults
	dist_grid->SetDefaultCellBackgroundColour( COLORBASE );
	dist_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	dist_grid->SetForegroundColour(COLORBASE );
	dist_grid->SetBackgroundColour( COLORBASE );
	bSizer611->Add( dist_grid, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	bSizer61->Add( bSizer611, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	btn_dist_OK = new wxButton( m_panel6, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( btn_dist_OK, 0, wxALL|wxALIGN_BOTTOM|wxALIGN_RIGHT, 5 );

	bSizer61->Add( fgSizer6, 0, wxALIGN_RIGHT, 5 );

	m_panel6->SetSizer( bSizer61 );
	m_panel6->Layout();
	bSizer61->Fit( m_panel6 );
	bSizer6->Add( m_panel6, 1, wxEXPAND | wxALL, 5 );

	this->SetSizer( bSizer6 );
	this->Layout();
	bSizer6->Fit( this );
	this->Center(wxCENTER_ON_SCREEN );
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Distributions_frame::OnInitdist ) );
	dist_grid->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Distributions_frame::OnCellClick ), NULL, this );
	btn_dist_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Distributions_frame::Onbtn_dist_OKClick ), NULL, this );
}

Distributions_frame::~Distributions_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Distributions_frame::OnInitdist ) );
	dist_grid->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Distributions_frame::OnCellClick ), NULL, this );
	btn_dist_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Distributions_frame::Onbtn_dist_OKClick ), NULL, this );
}

//////////////////////////////////////////////////////////////////

Sigma::Sigma( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( COLORBASE );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("sigma x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	fgSizer1->Add( m_textCtrl1, 0, wxALL, 5 );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("sigma y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,wxTE_RIGHT);
	fgSizer1->Add( m_textCtrl2, 0, wxALL, 5 );

	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("sigma z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl3 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	fgSizer1->Add( m_textCtrl3, 0, wxALL, 5 );


	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );

	ok_button = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer1->Add( ok_button, 0, wxALL|wxALIGN_RIGHT, 5 );

	this->SetSizer( fgSizer1 );
	this->Layout();
	fgSizer1->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Sigma::OnInitSigma ) );
	ok_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Sigma::Ok_Button_Click ), NULL, this );
}

Sigma::~Sigma()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Sigma::OnInitSigma ) );
	ok_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Sigma::Ok_Button_Click ), NULL, this );
}


///////////////////////////////////////
Delays_frame::Delays_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_panel5 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel5->SetBackgroundColour( COLORBASE);
	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer511;
	bSizer511 = new wxBoxSizer( wxVERTICAL );

	delays_grid = new wxGrid( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	// Grid
	delays_grid->CreateGrid( 5,5 );
	delays_grid->EnableEditing( true );
	delays_grid->EnableGridLines( true );
	delays_grid->EnableDragGridSize( false );
	delays_grid->SetMargins( 0, 0 );

	// Columns

	delays_grid->SetDefaultColSize(95, true);
	delays_grid->EnableDragColMove( false );
	delays_grid->EnableDragColSize( true );
	delays_grid->SetColLabelSize( 30 );
	delays_grid->SetColLabelValue( 0, wxT("Pop00") );
	delays_grid->SetColLabelValue( 1, wxT("Pop01") );
	delays_grid->SetColLabelValue( 2, wxT("Pop02") );
	delays_grid->SetColLabelValue( 3, wxT("Pop03") );
	delays_grid->SetColLabelValue( 4, wxT("Pop04") );
	delays_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	delays_grid->EnableDragRowSize( true );
	delays_grid->SetRowLabelSize( 95 );
	delays_grid->SetRowLabelValue( 0, wxT("Pop00") );
	delays_grid->SetRowLabelValue( 1, wxT("Pop01") );
	delays_grid->SetRowLabelValue( 2, wxT("Pop02") );
	delays_grid->SetRowLabelValue( 3, wxT("Pop03") );
	delays_grid->SetRowLabelValue( 4, wxT("Pop04") );
	delays_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance
	delays_grid->SetLabelBackgroundColour( COLORBASE );
	// Cell Defaults
	delays_grid->SetDefaultCellBackgroundColour( COLORBASE );
	delays_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	delays_grid->SetForegroundColour(COLORBASE );
	delays_grid->SetBackgroundColour( COLORBASE );
	bSizer511->Add( delays_grid, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	bSizer51->Add( bSizer511, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	btn_delays_OK = new wxButton( m_panel5, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( btn_delays_OK, 1, wxALL|wxALIGN_BOTTOM|wxEXPAND|wxALIGN_RIGHT, 5 );

	bSizer51->Add( fgSizer5, 0, wxALIGN_RIGHT, 5 );

	m_panel5->SetSizer( bSizer51 );
	m_panel5->Layout();
	bSizer51->Fit( m_panel5 );
	bSizer5->Add( m_panel5, 1, wxEXPAND | wxALL, 5 );

	this->SetSizer( bSizer5 );
	this->Layout();
	bSizer5->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Delays_frame::OnInitDelays ) );
	delays_grid->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Delays_frame::OnCellClickDelays ), NULL, this );
	btn_delays_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Delays_frame::Onbtn_delays_OKClick ), NULL, this );
}

Delays_frame::~Delays_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Delays_frame::OnInitDelays ) );
	delays_grid->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Delays_frame::OnCellClickDelays ), NULL, this );
	btn_delays_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Delays_frame::Onbtn_delays_OKClick ), NULL, this );
}


///////////////////////////////////////////////////////////////////////////

Conductances_frame::Conductances_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	m_panel7 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel7->SetBackgroundColour( COLORBASE);

	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer711;
	bSizer711 = new wxBoxSizer( wxVERTICAL );

	conductances_grid = new wxGrid( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE);

	// Grid
	conductances_grid->CreateGrid( 5, 5 );
	conductances_grid->EnableEditing( true );
	conductances_grid->EnableGridLines( true );
	conductances_grid->EnableDragGridSize( false );
	conductances_grid->SetMargins( 0, 0 );

	// Columns
	conductances_grid->SetDefaultColSize(95, true);
	conductances_grid->EnableDragColMove( false );
	conductances_grid->EnableDragColSize( true );
	conductances_grid->SetColLabelSize( 30 );
	conductances_grid->SetColLabelValue( 0, wxT("Pop00") );
	conductances_grid->SetColLabelValue( 1, wxT("Pop01") );
	conductances_grid->SetColLabelValue( 2, wxT("Pop02") );
	conductances_grid->SetColLabelValue( 3, wxT("Pop03") );
	conductances_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	conductances_grid->EnableDragRowSize( true );
	conductances_grid->SetRowLabelSize( 95 );
	conductances_grid->SetRowLabelValue( 0, wxT("Pop00") );
	conductances_grid->SetRowLabelValue( 1, wxT("Pop01") );
	conductances_grid->SetRowLabelValue( 2, wxT("Pop02") );
	conductances_grid->SetRowLabelValue( 3, wxT("Pop03") );
	conductances_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance
	conductances_grid->SetLabelBackgroundColour( COLORBASE );
	// Cell Defaults
	conductances_grid->SetDefaultCellBackgroundColour( COLORBASE );
	conductances_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	conductances_grid->SetForegroundColour(COLORBASE );
	conductances_grid->SetBackgroundColour( COLORBASE );
	bSizer711->Add( conductances_grid, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	bSizer71->Add( bSizer711, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	btn_Conductances_Cancel = new wxButton( m_panel7, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	btn_Conductances_Cancel->Hide();

	fgSizer7->Add( btn_Conductances_Cancel, 0, wxALL, 5 );

	btn_Conductances_OK = new wxButton( m_panel7, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( btn_Conductances_OK, 0, wxALL|wxALIGN_BOTTOM|wxALIGN_RIGHT, 5 );

	bSizer71->Add( fgSizer7, 0, wxALIGN_RIGHT, 5 );

	m_panel7->SetSizer( bSizer71 );
	m_panel7->Layout();
	bSizer71->Fit( m_panel7 );
	bSizer7->Add( m_panel7, 1, wxEXPAND | wxALL, 5 );

	this->SetSizer( bSizer7 );
	this->Layout();
	bSizer7->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Conductances_frame::OnInitConductances ) );
	conductances_grid->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Conductances_frame::OnCellClick ), NULL, this );
	btn_Conductances_Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Conductances_frame::Onbtn_Conductances_CancelClick ), NULL, this );
	btn_Conductances_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Conductances_frame::Onbtn_Conductances_OKClick ), NULL, this );
}

Conductances_frame::~Conductances_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Conductances_frame::OnInitConductances ) );
	conductances_grid->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Conductances_frame::OnCellClick ), NULL, this );
	btn_Conductances_Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Conductances_frame::Onbtn_Conductances_CancelClick ), NULL, this );
	btn_Conductances_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Conductances_frame::Onbtn_Conductances_OKClick ), NULL, this );
}

///////////////////////////////////////////////////////////////////////////

Plasticity_frame::Plasticity_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	m_panel8 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel8->SetBackgroundColour( COLORBASE);
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxVERTICAL );

	m_panel81 = new wxPanel( m_panel8, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel81->SetBackgroundColour( COLORBASE);
	wxBoxSizer* bSizer811;
	bSizer811 = new wxBoxSizer( wxVERTICAL );

	plasticity_grid = new wxGrid( m_panel81, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE);

	// Grid
	plasticity_grid->CreateGrid( 5, 5 );
	plasticity_grid->EnableEditing( true );
	plasticity_grid->EnableGridLines( true );
	plasticity_grid->EnableDragGridSize( false );
	plasticity_grid->SetMargins( 0, 0 );

	// Columns
	plasticity_grid->SetDefaultColSize(95, true);
	plasticity_grid->EnableDragColMove( false );
	plasticity_grid->EnableDragColSize( true );
	plasticity_grid->SetColLabelSize( 30 );
	plasticity_grid->SetColLabelValue( 0, wxT("Pop00") );
	plasticity_grid->SetColLabelValue( 1, wxT("Pop01") );
	plasticity_grid->SetColLabelValue( 2, wxT("Pop02") );
	plasticity_grid->SetColLabelValue( 3, wxT("Pop03") );
	plasticity_grid->SetColLabelValue( 4, wxT("Pop04") );
	plasticity_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	plasticity_grid->EnableDragRowSize( true );
	plasticity_grid->SetRowLabelSize( 95 );
	plasticity_grid->SetRowLabelValue( 0, wxT("Pop00") );
	plasticity_grid->SetRowLabelValue( 1, wxT("Pop01") );
	plasticity_grid->SetRowLabelValue( 2, wxT("Pop02") );
	plasticity_grid->SetRowLabelValue( 3, wxT("Pop03") );
	plasticity_grid->SetRowLabelValue( 4, wxT("Pop04") );
	plasticity_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance
	plasticity_grid->SetLabelBackgroundColour( COLORBASE );
	// Cell Defaults
	plasticity_grid->SetDefaultCellBackgroundColour( COLORBASE );
	plasticity_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	plasticity_grid->SetForegroundColour(COLORBASE );
	plasticity_grid->SetBackgroundColour( COLORBASE );
	bSizer811->Add( plasticity_grid, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_panel81->SetSizer( bSizer811 );
	m_panel81->Layout();
	bSizer811->Fit( m_panel81 );
	bSizer81->Add( m_panel81, 0, wxALL, 5 );

	m_panel82 = new wxPanel( m_panel8, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel82 ->SetBackgroundColour( COLORBASE );
	wxBoxSizer* bSizer812;
	bSizer812 = new wxBoxSizer( wxVERTICAL );

	btn_Plasticity_OK = new wxButton( m_panel82, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer812->Add( btn_Plasticity_OK, 0, wxALL|wxALIGN_BOTTOM|wxALIGN_RIGHT, 5 );

	m_panel82->SetSizer( bSizer812 );
	m_panel82->Layout();
	bSizer812->Fit( m_panel82 );
	bSizer81->Add( m_panel82, 0, wxALIGN_RIGHT|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_panel8->SetSizer( bSizer81 );
	m_panel8->Layout();
	bSizer81->Fit( m_panel8 );
	bSizer8->Add( m_panel8, 0, wxALL, 5 );

	this->SetSizer( bSizer8 );
	this->Layout();
	bSizer8->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Plasticity_frame::OnInitPlasticity ) );
	plasticity_grid->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Plasticity_frame::OnCellClick ), NULL, this );
	btn_Plasticity_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Plasticity_frame::Onbtn_Plasticity_OKClick ), NULL, this );
}

Plasticity_frame::~Plasticity_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Plasticity_frame::OnInitPlasticity ) );
	plasticity_grid->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Plasticity_frame::OnCellClick ), NULL, this );
	btn_Plasticity_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Plasticity_frame::Onbtn_Plasticity_OKClick ), NULL, this );
}


///////////////////////////////////////////////////////////////////////////

Model_frame::Model_frame( wxDialog* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	m_panel9 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel9->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer91;
	bSizer91 = new wxBoxSizer( wxVERTICAL );

	model_grid = new wxGrid( m_panel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE );

	// Grid
	model_grid->CreateGrid( 4, 1 );
	model_grid->EnableEditing( true );
	model_grid->EnableGridLines( true );
	model_grid->EnableDragGridSize( false );
	model_grid->SetMargins( 0, 0 );

	// Columns
	model_grid->EnableDragColMove( false );
	model_grid->EnableDragColSize( true );
	model_grid->SetColLabelSize( 30 );
	model_grid->SetColLabelValue( 0, wxT("model") );
	model_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	model_grid->EnableDragRowSize( true );
	model_grid->SetRowLabelSize( 150 );
	model_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance
	model_grid->SetLabelBackgroundColour( COLORBASE );
	// Cell Defaults
	model_grid->SetDefaultCellBackgroundColour( COLORBASE );
	model_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	model_grid->SetForegroundColour(COLORBASE );
	model_grid->SetBackgroundColour( COLORBASE );
	bSizer91->Add( model_grid, 1, wxALL|wxEXPAND, 5 );

	m_panel9->SetSizer( bSizer91 );
	m_panel9->Layout();
	bSizer91->Fit( m_panel9 );
	bSizer9->Add( m_panel9, 1, wxEXPAND | wxALL, 5 );

	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	btn_Model_OK = new wxButton( this, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( btn_Model_OK, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	bSizer9->Add( fgSizer9, 0, wxALIGN_RIGHT, 5 );

	this->SetSizer( bSizer9 );
	this->Layout();
	bSizer9->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Model_frame::OnInitModel ) );
	btn_Model_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Model_frame::Onbtn_Model_OKClick ), NULL, this );
}

Model_frame::~Model_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Model_frame::OnInitModel ) );
	btn_Model_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Model_frame::Onbtn_Model_OKClick ), NULL, this );
}
///////////////////////////////////////////////////////////////////////////

Dynamics_frame::Dynamics_frame( wxDialog* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	m_panel10 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel10->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxVERTICAL );

	dynamics_grid = new wxGrid( m_panel10, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	// Grid
	dynamics_grid->CreateGrid( 6, 5 );
	dynamics_grid->EnableEditing( true );
	dynamics_grid->EnableGridLines( true );
	dynamics_grid->EnableDragGridSize( false );
	dynamics_grid->SetMargins( 0, 0 );

	// Columns
	dynamics_grid->SetDefaultColSize(90, true);
	dynamics_grid->EnableDragColMove( false );
	dynamics_grid->EnableDragColSize( true );
	dynamics_grid->SetColLabelSize( 50 );
	dynamics_grid->SetColLabelValue( 0, wxT("soma") );
	dynamics_grid->SetColLabelValue( 1, wxT("fdm01") );
	dynamics_grid->SetColLabelValue( 2, wxT("fdm02") );
	dynamics_grid->SetColLabelValue( 3, wxT("fdm03") );
	dynamics_grid->SetColLabelValue( 4, wxT("fdm04") );
	dynamics_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	dynamics_grid->EnableDragRowSize( true );
	dynamics_grid->SetRowSize(1,40);
	dynamics_grid->SetRowLabelSize( 95 );
	dynamics_grid->SetRowLabelValue( 0, wxT("label") );
	dynamics_grid->SetRowLabelValue( 1, wxT("position \n(x,y,z) [um]") );
	dynamics_grid->SetRowLabelValue( 2, wxT("radius [um]") );
	dynamics_grid->SetRowLabelValue( 3, wxT("Ra [Mohm]") );
	dynamics_grid->SetRowLabelValue( 4, wxT("parent") );
	dynamics_grid->SetRowLabelValue( 5, wxT("model") );
	dynamics_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance
	dynamics_grid->SetLabelBackgroundColour( COLORBASE );
	// Cell Defaults
	dynamics_grid->SetDefaultCellBackgroundColour( COLORBASE );
	dynamics_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	dynamics_grid->SetForegroundColour(COLORBASE );
	dynamics_grid->SetBackgroundColour( COLORBASE );

	//bSizer101->Add( dynamics_grid, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );

	bSizer111->Add( dynamics_grid, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	bSizer101->Add( bSizer111, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer111;
	fgSizer111 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer111->SetFlexibleDirection( wxBOTH );
	fgSizer111->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	btn_Dynamics_OK = new wxButton( m_panel10, wxID_ANY, wxT("&Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer111->Add( btn_Dynamics_OK, 0, wxALL|wxALIGN_BOTTOM|wxALIGN_RIGHT, 5 );
	btn_view_neuron = new wxButton( m_panel10, wxID_ANY, wxT("&View"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer111->Add( btn_view_neuron, 0, wxALL|wxALIGN_BOTTOM|wxALIGN_RIGHT, 5 );

	bSizer101->Add( fgSizer111, 0, wxALIGN_RIGHT|wxRIGHT|wxLEFT, 5 );

	m_panel10->SetSizer( bSizer101 );
	m_panel10->Layout();
	bSizer101->Fit( m_panel10 );
	bSizer10->Add( m_panel10, 1, wxEXPAND | wxALL, 5 );

	this->SetSizer( bSizer10 );
	this->Layout();
	bSizer10->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Dynamics_frame::OnInitDynamics ) );
	dynamics_grid->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Dynamics_frame::OnCellClick ), NULL, this );
	btn_Dynamics_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dynamics_frame::Onbtn_Dynamics_OKClick ), NULL, this );
	btn_view_neuron->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dynamics_frame::Onbtn_view_neuronClick ), NULL, this );

}

Dynamics_frame::~Dynamics_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Dynamics_frame::OnInitDynamics ) );
	dynamics_grid->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( Dynamics_frame::OnCellClick ), NULL, this );
	btn_Dynamics_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dynamics_frame::Onbtn_Dynamics_OKClick ), NULL, this );
	btn_view_neuron->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Dynamics_frame::Onbtn_view_neuronClick ), NULL, this );
}

///////////////////////////////////////////////////////////
Summary_frame::Summary_frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetBackgroundColour( COLORBASE );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	m_panel11 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );
	m_panel11->SetBackgroundColour( COLORBASE );
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 1, 1, 0, 0 );
	fgSizer12->AddGrowableRow( 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	summary_grid = new wxGrid( m_panel11, wxID_ANY, wxDefaultPosition, wxDefaultSize,0);

	// Grid
	summary_grid->CreateGrid( 15, 3 );
	summary_grid->EnableEditing( false );
	summary_grid->EnableGridLines( false );
	summary_grid->EnableDragGridSize( false );
	summary_grid->SetMargins( 0, 0 );

	// Columns
	summary_grid->SetDefaultColSize(95, true);
	summary_grid->EnableDragColMove( false );
	summary_grid->EnableDragColSize( true );
	summary_grid->SetColLabelSize( 30 );
	summary_grid->SetColSize(0,100);
	summary_grid->SetColSize(1,100);
	summary_grid->SetColSize(2,100);
	summary_grid->SetColLabelValue( 0, wxT("Pop00") );
	summary_grid->SetColLabelValue( 1, wxT("Pop01") );
	summary_grid->SetColLabelValue( 2, wxT("Pop02") );
	summary_grid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	summary_grid->EnableDragRowSize( true );
	summary_grid->SetRowLabelSize( 95 );
	summary_grid->SetRowLabelValue( 0, wxT("Populations") );
	summary_grid->SetRowLabelValue( 1, wxT("label") );
	summary_grid->SetRowLabelValue( 2, wxT("size") );
	summary_grid->SetRowLabelValue( 3, wxT("origin") );
	summary_grid->SetRowLabelValue( 4, wxT("d_mean") );
	summary_grid->SetRowLabelValue( 5, wxT("fdomain") );
	summary_grid->SetRowLabelValue( 6, wxT("model") );
	summary_grid->SetRowLabelValue( 7, wxT("") );
	summary_grid->SetRowLabelValue(8,_T("conn matrix"));
	summary_grid->SetRowLabelValue( 9, wxT("flags") );
	summary_grid->SetRowLabelValue( 10, wxT("target fdms") );
	summary_grid->SetRowLabelValue( 11, wxT("distribution") );
	summary_grid->SetRowLabelValue( 12, wxT("") );
	summary_grid->SetRowLabelValue( 13, wxT("conductances") );
	summary_grid->SetRowLabelValue( 14, wxT("plasticity") );
	summary_grid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Label Appearance
	summary_grid->SetRowSize( 6, 60 );
	//summary_grid->SetRowSize( 9, 60 );
	summary_grid->SetRowSize( 10, 60 );
	summary_grid->SetRowSize( 11, 60 );
	summary_grid->SetRowSize( 13, 60 );
	summary_grid->SetRowSize( 14, 60 );
	summary_grid->SetLabelFont( wxFont( 8, 70, 90, 92, false, wxEmptyString ) );
	// Cell Defaults
	summary_grid->SetDefaultCellFont( wxFont( 8, 70, 90, 90, false, wxEmptyString ) );
	summary_grid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );

	summary_grid->SetLabelBackgroundColour( COLORBASE );
	// Cell Defaults
	summary_grid->SetDefaultCellBackgroundColour( COLORBASE );
	summary_grid->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	summary_grid->SetForegroundColour(COLORBASE );
	summary_grid->SetBackgroundColour( COLORBASE );

	fgSizer12->Add( summary_grid, 1, wxEXPAND|wxALL, 5 );

	m_panel11->SetSizer( fgSizer12 );
	m_panel11->Layout();
	fgSizer12->Fit( m_panel11 );
	bSizer11->Add( m_panel11, 1, wxEXPAND | wxALL, 5 );

	this->SetSizer( bSizer11 );
	this->Layout();

	//this->Centre( wxHORIZONTAL );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Summary_frame::OnCloseSummary ) );
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Summary_frame::OnInitSummary ) );
}

Summary_frame::~Summary_frame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Summary_frame::OnCloseSummary ) );
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Summary_frame::OnInitSummary ) );
}

///////////////////////////////////////////////////////////////////////////

Visualization_frame::Visualization_frame( wxDialog* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
 {
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
 	this->SetBackgroundColour( wxColour( 255, 255, 255 ) );

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	drawPane = new BasicDrawPane( (wxFrame*) this );
	drawPane -> SetBackgroundColour( wxColour( 255, 255, 255 ) );
        sizer->Add(drawPane, 1, wxEXPAND);

	this->SetSizer(sizer);
	this->SetAutoLayout(true);

 	// Connect Events
 	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Visualization_frame::OnClose));
 	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Visualization_frame::OnInitVisualization ) );
 }

Visualization_frame::~Visualization_frame()
 {
 	// Disconnect Events
 	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Visualization_frame::OnClose) );
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Visualization_frame::OnInitVisualization ) );
}





