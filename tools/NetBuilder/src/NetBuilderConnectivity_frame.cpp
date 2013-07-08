#include "NetBuilderConnectivity_frame.h"

extern unsigned int total_populations;
extern struct POPULATION *population;
extern NetBuilderSummary_frame* Summary;
extern bool summary_show; 
extern int **conn_values;
extern bool DivergentConnectivity;
extern bool Homogeneity;
extern bool Auto_connections;

NetBuilderConnectivity_frame::NetBuilderConnectivity_frame( wxWindow* parent )
:
Connectivity_frame( parent )
{

}

void NetBuilderConnectivity_frame::OnInitConnectivity( wxInitDialogEvent& event )
{
	int pop_number=total_populations;
	conn_grid->ClearGrid();
	int numbcols=conn_grid->GetNumberCols();

	wxGridCellAttr *attr1;
	attr1 = new wxGridCellAttr;
	attr1->SetBackgroundColour(*wxWHITE);
	
	if (pop_number<5)
	{
		conn_grid->DeleteRows(0,numbcols-pop_number,true);
		conn_grid->DeleteCols(0,numbcols-pop_number,true);
	}else if (pop_number>5)
	{
		conn_grid->AppendCols(pop_number-numbcols,true);
		conn_grid->AppendRows(pop_number-numbcols,true);
	}
	conn_grid->SetDefaultColSize(95, true);
	for (int i=0;i<pop_number;++i)
	{
	    conn_grid->SetRowLabelValue( i,wxString(population[i].label,wxConvUTF8));
	    conn_grid->SetColLabelValue(i, wxString(population[i].label,wxConvUTF8));
	    conn_grid->SetRowAttr(i,attr1);
	    for (int j=0;j<pop_number;++j)
	      {conn_grid->SetCellValue(i,j,long2wxStr(conn_values[i][j]));}
	}
	
	conn_grid->SetColLabelSize( 30 );
	//conn_grid->AutoSizeColumns();
	if (pop_number<6){
	   GetSizer()->SetSizeHints(this);
	}
}

void NetBuilderConnectivity_frame::Onbtn_conn_OptionsClick( wxCommandEvent& event )
{
	MainFrame *myparent=(MainFrame *) GetParent();
	this->Enable(false);
	myparent->text_status->SetForegroundColour( wxColour( 128,128 ,128 ) );
	wxLogMessage(wxT(">> Choose homogeneous synapses if you want to use the same parameters for all synapses in the same type of connection. Choose the connectivity interpretation, either Convergent or Divergent. Enable auto-connections (neuron connecting to itself). Enable multiplicity (multiple connections to the same neuron). Multiplicity is exclusive to Uniform distributions"));
	myparent->text_status->SetForegroundColour( wxColour( 0,0,0 ) );
	NetBuilderOptions_frame* Options_frame=new NetBuilderOptions_frame(this);
	Options_frame->Show();
}

void NetBuilderConnectivity_frame::Onbtn_conn_OKClick( wxCommandEvent& event )
{	
  MainFrame *myparent=(MainFrame *) GetParent();
  int error = 0;
  long val=0;
  for (unsigned int i=0; i<total_populations; i++)
  {   
     for (unsigned int j=0; j<total_populations; j++)
      { 
	if((conn_grid->GetCellValue(i,j)).ToLong(&val,10)==false)
	    {
	      myparent->text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	      wxLogMessage(wxT(">> WARNING: connectivity value of population") +  wxString::Format(wxT("%i"),j) + wxT(" is invalid"));
	      myparent->text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	      conn_values[i][j] = 0;
	      error=-1;
	    }
	else
	  {
	  conn_values[i][j]=wxAtol(conn_grid->GetCellValue(i,j));
	  }
      }
  }
  
  if (error == 0){ 
      myparent->Enable(true);
      this->Close();
      myparent->btn_conn_fdms->Enable( true );
      myparent->btn_conn_distributions->Enable( true );
      myparent->btn_syn_delays->Enable( true );
      myparent->btn_syn_conductances->Enable( true );
      myparent->btn_syn_plasticity->Enable( true );
      myparent->btn_createnet->Enable( true );
      wxLogMessage(wxT(">> Connectivity values (re)inserted."));
      if (summary_show==true)
	  {
	  Summary->Destroy();
	  Summary = new NetBuilderSummary_frame(this);
	  Summary->Show(true);
	  summary_show=true;
	  }
 }
}