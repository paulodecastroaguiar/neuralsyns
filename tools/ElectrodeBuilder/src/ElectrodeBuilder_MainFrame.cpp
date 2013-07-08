#include "ElectrodeBuilder_MainFrame.h"

char p_file[200];
extern struct ELECTRODE *electrode;
extern int number_elect;
extern char elect[25];
extern double *params;
extern int params_number;

 
ElectrodeBuilder_MainFrame::ElectrodeBuilder_MainFrame( wxWindow* parent )
:
MainFrame( parent )
{

}

void ElectrodeBuilder_MainFrame::OnClose( wxCloseEvent& event )
{
      int answer = wxMessageBox(_("Do you really want to quit the program?"), _("Please, confirm"), wxYES_NO , this);
      if (answer == wxYES){
	    free(electrode);
	    Destroy();
      }
}

void ElectrodeBuilder_MainFrame::On_Electrode_Change( wxCommandEvent& event )
{
	int pos = electrode_comboBox->GetSelection();
	if (pos!=0){
	  ElectrodeBuilder_ParametersFrame *Param_frame = new ElectrodeBuilder_ParametersFrame(this);
	  this->Enable(false);
	  Param_frame -> Show();
	}
	else{
	  strcpy(elect,"injFire");
	  params_number=0;
	}
	event.Skip();
}


void ElectrodeBuilder_MainFrame::On_Append_Click( wxCommandEvent& event )
{
      int erro=0;
      number_elect=number_elect+1;
      if (number_elect!=0){
	  electrode=(ELECTRODE*) realloc(electrode, (number_elect+1)*sizeof(struct ELECTRODE));
      }
      //time
      (time_textCtrl->GetValue()).ToDouble(&electrode[number_elect].time);
      if (electrode[number_elect].time<0){
	  time_textCtrl->SetForegroundColour( wxColour( 247, 22, 10 ) );
	  erro=-1;	 
      }
      //Range
      (range_init_textCtrl->GetValue()).ToLong( &electrode[number_elect].range_i,10);
      (range_end_texCtrl->GetValue()).ToLong( &electrode[number_elect].range_o,10);
      if (electrode[number_elect].range_i<0 ||(range_init_textCtrl->GetValue()).ToLong(&electrode[number_elect].range_i,10)==false){
	  erro=-1;
	  range_init_textCtrl->SetForegroundColour( wxColour( 247, 22, 10 ));
      }
      if (electrode[number_elect].range_o<0 ||(range_end_texCtrl->GetValue()).ToLong(&electrode[number_elect].range_o,10)==false){
	  erro=-1;
	  range_end_texCtrl->SetForegroundColour( wxColour( 247, 22, 10 ));
      }
      if (electrode[number_elect].range_i>electrode[number_elect].range_o){
	  erro=-1;
	  range_init_textCtrl->SetForegroundColour( wxColour( 247, 22, 10 ));
	  range_end_texCtrl->SetForegroundColour( wxColour( 247, 22, 10 ));
      }
  
      strcpy(electrode[number_elect].elect,elect);
      //Parameters
      char parameters[20]="";
      electrode[number_elect].params = (double*) malloc(params_number * sizeof(double));
      for (int i=0; i<params_number; i++){
	electrode[number_elect].params[i]=params[i];
	sprintf(parameters,"%s\t%.lf",parameters,params[i]);
      }
      
      char value[150];
      sprintf(value,"%.1lf\t[%ld,%ld]\t%s %s", electrode[number_elect].time,electrode[number_elect].range_i,electrode[number_elect].range_o,electrode[number_elect].elect, parameters);
      if (erro==0){
	time_textCtrl->SetForegroundColour( wxColour( 0, 0, 0 ) );
        range_init_textCtrl->SetForegroundColour( wxColour( 0, 0, 0 ));
	range_end_texCtrl->SetForegroundColour( wxColour( 0, 0, 0 ));
	wxLogMessage( wxString(value, wxConvUTF8));
      }
      else{
	number_elect=number_elect-1;
      }
}

void ElectrodeBuilder_MainFrame::On_Remove_Click( wxCommandEvent& event )
{
  long line=status_textCtrl->GetNumberOfLines()-2;
  long final=status_textCtrl->GetLastPosition();
  long initial=final-status_textCtrl->GetLineLength(line)-1;
  status_textCtrl->Remove(initial,final);
  
}
void ElectrodeBuilder_MainFrame::On_Finish_Click( wxCommandEvent& event )
{
	static const wxChar *FILETYPES = _T("Data Files|*.elec|"
	"All files|*.*"
	);

	wxFileDialog* openFileDialog = new wxFileDialog( this, _("Save file"), _(""), _("Template_elect.elec"), FILETYPES, wxSAVE, wxDefaultPosition);
	wxString path;
	if ( openFileDialog->ShowModal() == wxID_OK )
	{
		path.append( openFileDialog->GetDirectory() );
		path.append( wxFileName::GetPathSeparator() );
		path.append( openFileDialog->GetFilename() );
		SetStatusText(path, 0);
		SetStatusText(openFileDialog->GetDirectory(),1);
	}

	char elect_file[500];
	strncpy(elect_file,path.mb_str(),500);
	elect_file[500-1]='\0';	
	
	FILE *out_file;
	
	out_file=fopen(elect_file,"w+"); 
	if (out_file == NULL) {
	      printf("%s","You need to have a valid data file.");
	}
	else{
	
	    /// Put a Header in the file with generation info
	    fprintf(out_file, "# ElectrodeBuilder v0.1");
	    fprintf(out_file, "\n# This is a template for an electrodes file.");
	    fprintf(out_file, "\n# The data structure is:");
	    fprintf(out_file, "\n#");
	    fprintf(out_file, "\n# START");
	    fprintf(out_file, "\n#\n# <elect0 start time [ms]>	[<first neuron id>, <last neuron id>] <electrode id> <paramenters par0 par1 par2 ...>");
	    fprintf(out_file, "\n# <elect1 start time [ms]>	[<first neuron id>, <last neuron id>] <electrode id> <paramenters par0 par1 par2 ...>");
	    fprintf(out_file, "\n# ...");
	    fprintf(out_file, "\n# ");
	    fprintf(out_file, "\n# Possible electrodes and respective parameters, are:");
	    fprintf(out_file, "\n# (stimulation electrodes have the 'inj' prefix while recording electrodes have the 'rec' prefix)"); 
	    fprintf(out_file, "\n# fire neurons in range                       : injFire ");
	    fprintf(out_file, "\n# fire neurons periodically                   : injFirePeriodically <period [ms]> <duration [ms]> ");
	    fprintf(out_file, "\n# fires a selected number of random neurons   : injFireRandom <sample size [1]> ");
	    fprintf(out_file, "\n# fire neurons independently, Poisson process : injFirePoissonAsync <mean value [ms]> <duration [ms]> ");
	    fprintf(out_file, "\n# fire neurons synchronously, Poisson process : injFirePoissonSync <mean value [ms]> <duration [ms]> ");
	    fprintf(out_file, "\n# inject constant current                     : injCurrentConstant <current [nA]> <duration [ms]> ");
	    fprintf(out_file, "\n# inject random current (Gaussian)            : injCurrentGaussianNoise <current mean [nA]> <current stdev [nA]> <duration [ms]> ");
	    fprintf(out_file, "\n# inject sinusoidal current                   : injCurrentSine <current amplitude [nA]> <frequency [Hz]> <duration [ms]> ");
	    fprintf(out_file, "\n# record membrane potential from fdm00        : recV0 <record periodicity [1] (integer>0)> <duration [ms]> ");
	    fprintf(out_file, "\n# record membrane potential from all fdomais  : recVAll <record periodicity [1] (integer>0)> <duration [ms]> ");
	    fprintf(out_file, "\n# record state variables from fdm00           : recStates0 <record periodicity [1] (integer>0)> <duration [ms]> ");
	    fprintf(out_file, "\n# record state variables from all fdomains    : recStatesAll <record periodicity [1] (integer>0)> <duration [ms]> ");
	    fprintf(out_file, "\n# record state variables from all synapses    : recSynStates <record periodicity [1] (integer>0)> <duration [ms]> ");
	    fprintf(out_file, "\n# ");
	    fprintf(out_file, "\n# Example:");
	    fprintf(out_file, "\n#\n# START");
	    fprintf(out_file,"\n# 100.0	[100,200]	injCurrentGaussianNoise	1.5  0.2  500.0");
	    fprintf(out_file,"\n# 150.0	[0,100]	injFire");
	    fprintf(out_file,"\n# 250.0	[1,1]	injFirePeriodically		10.0  50");
	    fprintf(out_file, "\n ");
	    fprintf(out_file, "\nSTART");
	    
	    char text_status[500];
	    strcpy(text_status, (status_textCtrl->GetValue()).mb_str());
	    fprintf(out_file,"\n%s",text_status);
	    fprintf(out_file,"\n");
	    
	    fclose(out_file);
	    free(electrode);
	    this->Destroy();
	}

}
