#include "NetBuilderMainFrame.h"


using namespace std;
NetBuilderDistributions_frame* Distributions_frame = NULL;
extern unsigned int total_populations;
extern struct POPULATION *population;
extern NetBuilderSummary_frame* Summary;
extern bool summary_show;
wxString old_path;

#define MAX_POP_NUMBER 32


enum wxbuildinfoformat {short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
wxString wxbuild(wxVERSION_STRING);

if (format == long_f )
{
#if defined(__WXMSW__)
    wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
    wxbuild << _T("-Mac");
#elif defined(__UNIX__)
    wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
    wxbuild << _T("-Unicode build");
#else
    wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
}
return wxbuild;
}

NetBuilderMainFrame::NetBuilderMainFrame( wxWindow* parent )
:
MainFrame( parent, wxID_ANY, wxT("NetBuilder"), wxPoint(300,50), wxSize(-1,680))
{

}

void NetBuilderMainFrame::OnClose( wxCloseEvent& event )
{
	Free_Memory();
	if (summary_show==true)
	    Summary->Destroy();
	this->Destroy();
}

void NetBuilderMainFrame::OnQuit( wxCommandEvent& event )
{
	Free_Memory();
	if (summary_show==true)
	    Summary->Destroy();
	this->Destroy();
}

void NetBuilderMainFrame::OnAbout( wxCommandEvent& event )
{
	wxString msg = wxT("\nNETBUILDER \n\nMafalda Sousa [mafsousa@ibmc.up.pt] \nPaulo Castro Aguiar [pauloaguiar@fc.up.pt]");
        wxMessageBox(msg , _("Welcome to NetBuilder"));
}

void NetBuilderMainFrame::OnSelectRB( wxCommandEvent& event )
{
	edt_total_population->Enable( false );
	m_filePicker->Enable( true );
}

void NetBuilderMainFrame::OnFileChanged( wxFileDirPickerEvent& event )
{	
	wxString path;
	path = m_filePicker->GetPath();
	int error=0;
	char p_file[200];
	strncpy(p_file,path.mb_str(),200);
	p_file[200-1]='\0';

	error =  Load_Netbuilder_File(p_file);     //read network file
	if (error==0){
	  wxLogMessage(_T(">> Your selected network file was loaded"));
	  edt_total_population->Enable( false);
	  rad_new->Enable( false);
	  btn_pop_properties->Enable( true );
	  btn_pop_topology->Enable( true);
	  btn_conn_matrix->Enable( true );
	  btn_conn_fdms->Enable( true );
	  btn_conn_distributions->Enable( true );
	  btn_syn_delays->Enable( true );
	  btn_syn_conductances->Enable( true );
	  btn_syn_plasticity->Enable( true );
	  btn_summary->Enable( true );
	  btn_store->Enable( true );
	  btn_createnet->Enable (true );
	}
      else{
	text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	wxLogMessage(_(">> WARNING: You must select a valid data file"));
	text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
    }
}

void NetBuilderMainFrame::OnCreateRB( wxCommandEvent& event )
{
	m_filePicker->Enable( false );
	edt_total_population->Enable( true );
}

void NetBuilderMainFrame::On_total_population_Update( wxCommandEvent& event )
{

	char pop[MAXLABELSIZE];
	int pop_aux=0;
	wxStr2str(edt_total_population->GetValue(),pop);
	pop_aux=atoi(pop);
	
	if (pop_aux>0 && pop_aux<=MAX_POP_NUMBER){
	    total_populations=pop_aux;
	    Initialize_Populations(total_populations);
	    //Disable choose panel
	    edt_total_population->Enable( false);
	    rad_new->Enable( false);	    
	    m_filePicker->Enable( true);
	    rad_edit->Enable( true);
	    rad_edit->SetValue(1);

	    
	    //Enable buttons
	    btn_pop_properties->Enable( true );
	    btn_pop_topology->Enable( true);
	    btn_summary->Enable( true );
	    btn_summary->SetValue(false);
	    btn_store->Enable( true );
	    btn_conn_matrix->Enable( true );
	    btn_createnet->Enable (true );
	   }
	else
	{
	  text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	  wxLogMessage(wxT(">> WARNING: total populations must be an integer bigger than 0 and less than 33."));
	  text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	 }
}

void NetBuilderMainFrame::OnPropertiesClick( wxCommandEvent& event )
{
	this->Enable(false);
	text_status->SetForegroundColour( wxColour( 128,128 ,128 ) );
	wxLogMessage(wxT(">> Use the size property to arrange each population in a box array. All neurons in a population must contain at least 1 functional domain (soma). Set the dynamics and parameters for each fdomain using the edit button. Define the topology of each functional domain (position and radius [um]), the axial resistence (Ra [Mohm] >= 10.0) and choose which parent is it linked to. The available dynamics are taken from the Neuron_Models.lib file."));
	text_status->SetForegroundColour( wxColour( 0,0 ,0 ) );
	NetBuilderProperties_frame* Properties_frame=new NetBuilderProperties_frame(this);
	Properties_frame->Show();
}

void NetBuilderMainFrame::OnTopopolyClick( wxCommandEvent& event )
{
	this->Enable(false);
	text_status->SetForegroundColour( wxColour( 128,128 ,128 ) );
	wxLogMessage(wxT(">> Use this table to place all population arrays in 3D space: X0, Y0 and Z0 sets the location of the first neuron in the population; the arrays are filled in X->Y->Z order. The distance between neurons follows a normal distribution with parameters d_mean and d_std."));
	text_status->SetForegroundColour( wxColour( 0,0 ,0 ) );
	NetBuilderTopology_frame* Topology_frame=new NetBuilderTopology_frame(this);
	Topology_frame->Show();
}

void NetBuilderMainFrame::OnConnClick( wxCommandEvent& event )
{
	this->Enable(false);
	text_status->SetForegroundColour( wxColour( 128,128 ,128 ) );
	wxLogMessage(wxT(">> Setup the network's connectivity table. For each entry, the row defines the source population and the column defines the target population. The value sets the number of synapses established for each neuron. Different connectivity properties can be choosen from the Options button."));
	text_status->SetForegroundColour( wxColour( 0,0 ,0 ) );
	NetBuilderConnectivity_frame* Connectivity_frame=new NetBuilderConnectivity_frame(this);
	Connectivity_frame->Show();
}

void NetBuilderMainFrame::OnFdmsClick( wxCommandEvent& event )
{
	this->Enable(false);
	text_status->SetForegroundColour( wxColour( 128,128 ,128 ) );
	wxLogMessage(wxT(">> Set the fdomain target for each type of connection. All fdomains in the pos-synaptic population are valid targets."));
	text_status->SetForegroundColour( wxColour( 0,0 ,0 ) );
	NetBuilderFdms_frame* Fdms_frame=new NetBuilderFdms_frame(this);
	Fdms_frame->Show();
}

void NetBuilderMainFrame::OnDistributionsClick( wxCommandEvent& event )
{
	this->Enable(false);
	text_status->SetForegroundColour( wxColour( 128,128 ,128 ) );
	wxLogMessage(wxT(">> Define the connectivity volumes for each type of connection using a 3D Normal distribution. Independent Uniform distributions are used instead in the directions of space where the standard deviation values are large. A linear volume projection from the pre-synaptic population array into the pos-synaptic population array is used to set the distributions mean values."));
	text_status->SetForegroundColour( wxColour( 0,0 ,0 ) );
	Distributions_frame=new NetBuilderDistributions_frame(this);
	Distributions_frame->Show();
}

void NetBuilderMainFrame::OnDelaysClick( wxCommandEvent& event )
{
	this->Enable(false);
	text_status->SetForegroundColour( wxColour( 128,128 ,128 ) );
	wxLogMessage(wxT(">> Set the synaptic delays for each connection."));
	text_status->SetForegroundColour( wxColour( 0,0 ,0 ) );
	NetBuilderDelays_frame* Delays_frame=new NetBuilderDelays_frame(this);
	Delays_frame->Show();
}

void NetBuilderMainFrame::OnConductancesClick( wxCommandEvent& event )
{
	this->Enable(false);
	text_status->SetForegroundColour( wxColour( 128,128 ,128 ) );
	wxLogMessage(wxT(">> Choose the model defining the conductance dynamics for each type of connection and set the required parameters. The available dynamics are taken from the Synapse_Models.lib file."));
	text_status->SetForegroundColour( wxColour( 0,0 ,0 ) );
	NetBuilderConductances_frame* Conductances_frame=new NetBuilderConductances_frame(this);
	Conductances_frame->Show();
}

void NetBuilderMainFrame::OnPlasticityClick( wxCommandEvent& event )
{
	this->Enable(false);
	text_status->SetForegroundColour( wxColour( 128,128 ,128 ) );
	wxLogMessage(wxT(">> Choose the model defining the plasticity rules for each type of connection and set the required parameters. The available rules are taken from the Plasticity_Models.lib file."));
	text_status->SetForegroundColour( wxColour( 0,0 ,0 ) );
	NetBuilderPlasticity_frame* Plasticity_frame=new NetBuilderPlasticity_frame(this);
	Plasticity_frame->Show();
}

void NetBuilderMainFrame::On_btn_summary_Click( wxCommandEvent& event )
{	
	if (btn_summary->GetValue()==true && summary_show==false){
	    Summary = new NetBuilderSummary_frame(this);
	    Summary->Show(true);
	    summary_show=true;
	}
	else if (btn_summary->GetValue()==false && summary_show==true){
	    Summary->Show(false);
	    summary_show=false;
	}
}
void NetBuilderMainFrame::OnStoreClick( wxCommandEvent& event )
{
	//save network file
	static const wxChar *FILETYPES = _T("Data Files|*.dat|"
	"C/C++ source files|*.cpp;*.cc;*.c|"
	"C/C++ header files|*.hpp;*.h|"
	"Make files|Mak*;mak*|"
	"Java files|*java|"
	"Hypertext markup files|*html;*htm;*HTML;*HTM|"
	"All files|*.*"
	);

	wxFileDialog* openFileDialog = new wxFileDialog( this, _("Save file"), _(""), _("network.dat"), FILETYPES, wxSAVE, wxDefaultPosition);
	wxString path;
	if ( openFileDialog->ShowModal() == wxID_OK )
	{
		path.append( openFileDialog->GetDirectory() );
		path.append( wxFileName::GetPathSeparator() );
		path.append( openFileDialog->GetFilename() );
		//theText->LoadFile(path);
		SetStatusText(path, 0);
		SetStatusText(openFileDialog->GetDirectory(),1);
	}

	if (path==_("")){
	    text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	    wxLogMessage(_(">> WARNING: You need to specify a valid data file."));
	    text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	    return;
	}
	else{
	    char network_file[500];
	    strncpy(network_file,path.mb_str(),500);
	    network_file[500-1]='\0';	
	    Save_Netbuilder_File(network_file);
	    wxLogMessage(wxT(">> The network model file is saved"));
	}
}

void NetBuilderMainFrame::OnCreateClick( wxCommandEvent& event )
{
	int erro;
	static const wxChar *FILETYPES = _T("Data Files|*.net|"
	"All files|*.*"
	);

	wxFileDialog* openFileDialog = new wxFileDialog( this, _("Save file"), _(""), _("network.net"), FILETYPES, wxSAVE, wxDefaultPosition);
	wxString path;
	if ( openFileDialog->ShowModal() == wxID_OK )
	{
		path.append( openFileDialog->GetDirectory() );
		path.append( wxFileName::GetPathSeparator() );
		path.append( openFileDialog->GetFilename() );
		//theText->LoadFile(path);
		SetStatusText(path, 0);
		SetStatusText(openFileDialog->GetDirectory(),1);
	}

	if (path==_("")){
	    text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	    wxLogMessage(_(">> WARNING: You need to specify a valid data file."));
	    text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	    return;
	}
	else{

	    char network_file[500];
	    strncpy(network_file,path.mb_str(),500);
	    network_file[500-1]='\0';	
	    wxLogMessage(wxT(">> The network model file is now being created. In large networks this may take a long time."));

	    text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	    erro=Main_Core(network_file);
	    text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	      if (erro==-1){
		  text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
		  wxLogMessage(wxT(">> WARNING: Can't fint proper targets for the connections you stablished .The network model file was not created."));
		  text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	      }
	      else if (erro==1){
		  text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
		  wxLogMessage(wxT(">> The network model was not created!."));
		  text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	      }
	      else
		  wxLogMessage(wxT(">> FINISHED!!.")); 
		  
	 }

}

