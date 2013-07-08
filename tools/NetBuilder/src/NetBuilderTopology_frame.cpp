#include "NetBuilderTopology_frame.h"

extern unsigned int total_populations;
extern struct POPULATION *population;
extern NetBuilderSummary_frame* Summary;
extern bool summary_show; 

NetBuilderTopology_frame::NetBuilderTopology_frame( wxWindow* parent )
:
Topology_frame( parent,  wxID_ANY, wxT("Topology"), wxPoint(-1,-1), wxSize(-1,300) )
{

}

void NetBuilderTopology_frame::OnInitTopology( wxInitDialogEvent& event )
{
	int pop_number=total_populations;

	wxGridCellAttr *attr;
	attr = new wxGridCellAttr;
	attr->SetBackgroundColour(*wxWHITE);

	topology_grid->ClearGrid();
	int numbcols=topology_grid->GetNumberCols();
	
	if (pop_number<5)
		topology_grid->DeleteCols(0,numbcols-pop_number,true);
	else if (pop_number>5)
		topology_grid->AppendCols(pop_number-numbcols,true);
	
	topology_grid->SetDefaultColSize(95, true);
	for (int j=0;j<pop_number;++j){
	    wxString ind;
	    if (j<10)
	         ind= wxString::Format(wxT("0%i"), j);
	    else
	         ind= wxString::Format(wxT("%i"), j);
	    	    
	    topology_grid->SetColLabelValue(j,wxString(population[j].label,wxConvUTF8)); 
	    topology_grid->SetCellValue(0,j,dbl2wxStr(population[j].topology.X0));
	    topology_grid->SetCellValue(1,j,dbl2wxStr(population[j].topology.Y0));
	    topology_grid->SetCellValue(2,j,dbl2wxStr(population[j].topology.Z0));
	    topology_grid->SetCellValue(3,j,dbl2wxStr(population[j].topology.d_mean));
	    topology_grid->SetCellValue(4,j,dbl2wxStr(population[j].topology.d_std));
	    topology_grid->SetColAttr(j,attr);
	}
	topology_grid->SetColLabelSize( 30 );
	if (pop_number<6){
	  GetSizer()->SetSizeHints(this);
	}
}

void NetBuilderTopology_frame::Onbtn_top_CancelClick( wxCommandEvent& event )
{
	// TODO: Implement Onbtn_top_CancelClick
}

void NetBuilderTopology_frame::Onbtn_top_OKClick( wxCommandEvent& event )
{
   MainFrame *myparent=(MainFrame *) GetParent();

   int error = 0;
  for (unsigned int i=0; i<total_populations; i++)
  {   
    bool er1 = (topology_grid->GetCellValue(0,i)).ToDouble(&population[i].topology.X0);
    bool er2 = (topology_grid->GetCellValue(1,i)).ToDouble(&population[i].topology.Y0);
    bool er3 = (topology_grid->GetCellValue(2,i)).ToDouble(&population[i].topology.Z0);
    bool er4 = (topology_grid->GetCellValue(3,i)).ToDouble(&population[i].topology.d_mean);
    bool er5 = (topology_grid->GetCellValue(4,i)).ToDouble(&population[i].topology.d_std);
        
    if (er1==false || er2==false || er3==false) 
      { 
	  myparent->text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	  wxLogMessage(wxT(">> WARNING: initial coordinate of population ") +  wxString::Format(wxT("%i"),i) + wxT(" is invalid"));
	  myparent->text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	  population[i].topology.X0 = 0;
	  population[i].topology.Y0 = 0;
	  population[i].topology.Z0 = 0;
	  error=-1;
      }

     if (er4==false || population[i].topology.d_mean < 0) 
      { 
	  myparent->text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	  wxLogMessage(wxT(">> WARNING: mean distance of population ") +  wxString::Format(wxT("%i"),i) + wxT(" is invalid"));
	  myparent->text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	  population[i].topology.d_mean = 0;
	  error=-1;
      }

     if (er5==false || population[i].topology.d_std < 0) 
      { 
	  myparent->text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	  wxLogMessage(wxT(">> WARNING: standard deviation of population ") +  wxString::Format(wxT("%i"),i) + wxT(" is invalid"));
	  myparent->text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	  population[i].topology.d_std = 100;
	  error=-1;
      }

  }
  if (error == 0)
    { this->Close();
      wxLogMessage(wxT(">> Population properties (re)defined."));
      myparent->Enable(true);
      myparent->btn_conn_matrix->Enable( true );
      if (summary_show==true)
	  {
	  Summary->Destroy();
	  Summary = new NetBuilderSummary_frame(this);
	  Summary->Show(true);
	  summary_show=true;
	  }

    }
}