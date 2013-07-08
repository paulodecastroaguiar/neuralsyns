///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Gui.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxColour(208,208,208)  );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1->SetBackgroundColour( wxColour(208,208,208)  );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );


	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );

	bSizer22->Add( 60, 0, 0, wxEXPAND, 5 );

	electrode_text = new wxStaticText( m_panel1, wxID_ANY, wxT("  Electrode"), wxDefaultPosition, wxDefaultSize, 0 );
	electrode_text->Wrap( -1 );

	bSizer22->Add( electrode_text, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	bSizer22->Add( 35, 0, 0, wxEXPAND, 5 );

	time_text = new wxStaticText( m_panel1, wxID_ANY, wxT("    Time [ms]"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	time_text->Wrap( -1 );
	bSizer22->Add( time_text, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	bSizer22->Add( 5, 0, 0, 0, 5 );

	range_text = new wxStaticText( m_panel1, wxID_ANY, wxT("     Range"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	range_text->Wrap( -1 );
	bSizer22->Add( range_text, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	bSizer2->Add( bSizer22, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );

	electrode_comboBox = new wxComboBox( m_panel1, wxID_ANY, wxT("Fire neurons in range"), wxDefaultPosition, wxSize( 200,30 ), 0, NULL, wxCB_READONLY|wxTE_PROCESS_ENTER );
	electrode_comboBox->Append( wxT("Fire neurons in range") );
	electrode_comboBox->Append( wxT("Fire periodically") );
	electrode_comboBox->Append( wxT("Fire random neurons") );
	electrode_comboBox->Append( wxT("Fire Poisson independently") );
	electrode_comboBox->Append( wxT("Fire Poisson synchronously") );
	electrode_comboBox->Append( wxT("Inject constant current") );
	electrode_comboBox->Append( wxT("Inject random current (Gaussian)") );
	electrode_comboBox->Append( wxT("Inject sinusoidal current"));
	electrode_comboBox->Append( wxT("Record membrane potential from fdm00"));
	electrode_comboBox->Append( wxT("Record membrane potential from all fdomais"));
	electrode_comboBox->Append( wxT("Record state variables from fdm00"));
	electrode_comboBox->Append( wxT("Record state variables from all fdomains"));
	electrode_comboBox->Append( wxT("Record state variables from all synapses"));
	bSizer23->Add( electrode_comboBox, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	electrode_comboBox->SetToolTip(wxT("Choose the electrode type."));

	time_textCtrl = new wxTextCtrl( m_panel1, wxID_ANY, wxT("100.0"), wxDefaultPosition, wxSize( 80,31 ), wxTE_RIGHT );
	bSizer23->Add( time_textCtrl, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	time_textCtrl->SetToolTip(wxT("Start time [ms]."));

	wxBoxSizer* bSizer231;
	bSizer231 = new wxBoxSizer( wxHORIZONTAL );
	range_init_textCtrl = new wxTextCtrl( m_panel1, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 50,31 ), wxTE_RIGHT );
	range_init_textCtrl->SetToolTip(wxT("First neuron ID."));
	bSizer231->Add( range_init_textCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );


	range_end_texCtrl = new wxTextCtrl( m_panel1, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 50,31 ), wxTE_RIGHT );
	range_end_texCtrl->SetToolTip(wxT("Last neuron ID."));
	bSizer231->Add( range_end_texCtrl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	bSizer23->Add( bSizer231, 0, 0, 5 );

	bSizer2->Add( bSizer23, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	append_btn = new wxButton( m_panel1, wxID_ANY, wxT("Append"), wxDefaultPosition, wxDefaultSize, 0 );
	append_btn->SetToolTip(wxT("Append one input electrode with the selected protocol."));
	fgSizer24->Add( append_btn, 0, wxALL, 5 );

	remove_btn = new wxButton( m_panel1, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	remove_btn->SetToolTip(wxT("Remove the last inserted electrode."));
	fgSizer24->Add( remove_btn, 0, wxALL, 5 );


	bSizer2->Add( fgSizer24, 1, wxALIGN_RIGHT, 5 );

	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	stimuli_text1 = new wxStaticText( this, wxID_ANY, wxT("Electrode protocol:"), wxDefaultPosition, wxDefaultSize, 0 );//\nstart time\t[first,last]:electrode\tparameters
	stimuli_text1->Wrap( -1 );
	//stimuli_text1->SetForegroundColour(wxColour(100,100,100));
	bSizer3->Add( stimuli_text1, 0, wxLEFT|wxEXPAND, 5 );

	stimuli_text = new wxStaticText( this, wxID_ANY, wxT(" time\t[range]: elect\tparameters"), wxDefaultPosition, wxDefaultSize, 0 );//\nstart time\t[first,last]:electrode\tparameters
	stimuli_text->Wrap( -1 );
	stimuli_text->SetForegroundColour(wxColour(100,100,100));
	bSizer3->Add( stimuli_text, 0, wxLEFT|wxEXPAND, 5 );


	status_textCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,170 ), wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP|wxSIMPLE_BORDER );
	status_textCtrl->SetToolTip(wxT("List of all the inserted electrodes."));
	log_status = wxLog::SetActiveTarget(new wxLogTextCtrl(status_textCtrl));
	wxLog::SetTimestamp(NULL);
	bSizer3->Add( status_textCtrl, 1, wxALL|wxEXPAND, 5 );

	bSizer1->Add( bSizer3, 1, wxEXPAND|wxALL, 5 );

	wxFlexGridSizer* fgSizer25;
	fgSizer25 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer25->SetFlexibleDirection( wxBOTH );
	fgSizer25->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	finish_btn = new wxButton( this, wxID_ANY, wxT("Finish"), wxDefaultPosition, wxDefaultSize, 0 );
	finish_btn->SetToolTip(wxT("Save in a .elect file the electrode protocol defined in the status box. The program will be close."));
	fgSizer25->Add( finish_btn, 0, wxALL|wxALIGN_RIGHT, 5 );


	bSizer1->Add( fgSizer25, 0, wxALL|wxALIGN_RIGHT, 5 );

	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );

	// Connect Events

	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	electrode_comboBox->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MainFrame::On_Electrode_Change ), NULL, this );
	append_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::On_Append_Click ), NULL, this );
	remove_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::On_Remove_Click ), NULL, this );
	finish_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::On_Finish_Click ), NULL, this );
}


MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnClose ) );
	electrode_comboBox->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MainFrame::On_Electrode_Change ), NULL, this );
	append_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::On_Append_Click ), NULL, this );
	remove_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::On_Remove_Click ), NULL, this );
	finish_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::On_Finish_Click ), NULL, this );
}

ParametersFrame::ParametersFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxColour(208,208,208)  );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	table = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	table->CreateGrid( 2, 1 );
	table->EnableEditing( true );
	table->EnableGridLines( true );
	table->EnableDragGridSize( false );
	table->SetMargins( 0, 0 );

	// Columns
	table->SetDefaultColSize(120, true);
	table->EnableDragColMove( false );
	table->EnableDragColSize( true );
	table->SetColLabelSize( 30 );
	//table->SetColLabelValue( 0, wxT("Periodically") );
	table->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	table->EnableDragRowSize( true );
	table->SetRowLabelSize( 150 );

	table->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

		  // Label Appearance
	table->SetLabelBackgroundColour( wxColour(208,208,208) );
	// Cell Defaults
	table->SetDefaultCellBackgroundColour( wxColour(208,208,208) );
	table->SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	table->SetForegroundColour(wxColour(208,208,208) );
	table->SetBackgroundColour( wxColour(208,208,208) );
	bSizer1->Add( table, 0, wxALL, 5 );

	Ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( Ok_btn, 0, wxALIGN_RIGHT|wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( ParametersFrame::On_Init ) );
	Ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParametersFrame::On_Ok_Click ), NULL, this );
}

ParametersFrame::~ParametersFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( ParametersFrame::On_Init ) );
	Ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParametersFrame::On_Ok_Click ), NULL, this );

}
