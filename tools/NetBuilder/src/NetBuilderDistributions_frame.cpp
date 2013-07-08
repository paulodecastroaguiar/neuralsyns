#include "NetBuilderDistributions_frame.h"

extern unsigned int total_populations;
extern struct POPULATION *population;
extern NetBuilderSummary_frame* Summary;
extern NetBuilderMainFrame* frame;
extern bool summary_show; 
extern int **conn_values;
extern double ***dist_values;
extern int ***RANDOM_DIST;
int pop_row = 0;
int pop_col = 0;

NetBuilderDistributions_frame::NetBuilderDistributions_frame( wxWindow* parent )
:
Distributions_frame( parent , wxID_ANY, wxT("Distribution"), wxPoint(200,400), wxSize( -1,-1 )) 
{

}

void NetBuilderDistributions_frame::OnInitdist( wxInitDialogEvent& event )
{
	wxGridCellAttr *attr;
	attr = new wxGridCellAttr;
	attr->SetBackgroundColour(*wxLIGHT_GREY);

	wxGridCellAttr *attr1;
	attr1 = new wxGridCellAttr;
	attr1->SetBackgroundColour(*wxWHITE);

	int pop_number=total_populations;
	dist_grid->ClearGrid();
	int numbcols=dist_grid->GetNumberCols();
	char sigma[100];
	const char *aux={""};
	
	if (pop_number<5)
	{
		dist_grid->DeleteRows(0,numbcols-pop_number,true);
		dist_grid->DeleteCols(0,numbcols-pop_number,true);
	}else if (pop_number>5)
	{
		dist_grid->AppendCols(pop_number-numbcols,true);
		dist_grid->AppendRows(pop_number-numbcols,true);
	}
	
	dist_grid->SetDefaultColSize(95, true);
	for (int i=0;i<pop_number;++i)
	{
	    dist_grid->SetRowLabelValue( i,wxString(population[i].label,wxConvUTF8));
	    dist_grid->SetColLabelValue(i, wxString(population[i].label,wxConvUTF8));
	   
	    dist_grid->SetRowAttr(i,attr1);
	    for (int j=0;j<pop_number;++j) {
		str_equal(aux,sigma);
		sprintf(sigma,"%#.lg\n%#.lg\n%#.lg",dist_values[i][j][0],dist_values[i][j][1], dist_values[i][j][2]);
		dist_grid->SetCellValue(i,j,wxString(sigma,wxConvUTF8));
		dist_grid->SetRowSize(i,60);
		dist_grid->SetCellRenderer(i , j, new wxGridCellAutoWrapStringRenderer());
		dist_grid->SetCellEditor( i,  j , new wxGridCellAutoWrapStringEditor());
		
		if (conn_values[i][j]==0){
		      dist_grid->SetAttr(i,j,attr);
		      dist_grid->SetReadOnly(i,j,true);
		      dist_grid->SetCellOverflow(i, j, true);
		      dist_grid->SetCellValue(i,j,_T(""));
		}
	    }
	}
	if (pop_number<6){
	  GetSizer()->SetSizeHints(this);
	}
	this->SetPosition(wxPoint(200,200));
	this->Center(wxCENTER_ON_SCREEN );
}

void NetBuilderDistributions_frame::OnCellClick( wxGridEvent& event )
{
  pop_row=event.GetRow();
  pop_col=event.GetCol();
  if (conn_values[pop_row][pop_col]!=0){
      this->Enable(false);
      NetBuilderSigma_frame *Sigma = new NetBuilderSigma_frame(this);
      Sigma->Show();
  }
  event.Skip();
	
}

void NetBuilderDistributions_frame::Onbtn_dist_OKClick( wxCommandEvent& event )
{
      frame->Enable(true);
      for (unsigned int i=0; i<total_populations; i++){   
	for (unsigned int j=0; j<total_populations; j++){ 
	    if (conn_values[i][j]!=0){ 
		if (dist_values[i][j][0]>population[j].topology.dim_x*population[j].topology.d_mean) 
		      RANDOM_DIST[i][j][0]=1; //uniform
		else
		      RANDOM_DIST[i][j][1]=0; //gaussian
		if (dist_values[i][j][1]>population[j].topology.dim_y*population[j].topology.d_mean) 
		      RANDOM_DIST[i][j][1]=1; //uniform
		else
		      RANDOM_DIST[i][j][0]=0; //gaussian
		if (dist_values[i][j][2]>population[j].topology.dim_z*population[j].topology.d_mean) 
		      RANDOM_DIST[i][j][2]=1; //uniform
		else
		      RANDOM_DIST[i][j][2]=0; //gaussian 
		      
		if ((RANDOM_DIST[i][j][0]*RANDOM_DIST[i][j][1]*RANDOM_DIST[i][j][2])==1){	
		    wxLogMessage(wxT(">> A Uniform Distribution is being used for the connection pair ") + wxString(population[i].label,wxConvUTF8) + _("-") + wxString(population[j].label,wxConvUTF8));
		}
	    }
	}
      }
      this->Close();
      wxLogMessage(wxT(">> Distribution values (re)inserted."));
      if (summary_show==true)
	  {
	  Summary->Destroy();
	  Summary = new NetBuilderSummary_frame(this);
	  Summary->Show(true);
	  summary_show=true;
	  }
    // TODO: Implement Onbtn_dist_OKClick
}
