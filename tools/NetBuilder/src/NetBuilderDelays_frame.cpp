#include "NetBuilderDelays_frame.h"
#include "NetBuilderConnectivity_frame.h"

extern unsigned int total_populations;
extern struct POPULATION *population;
extern NetBuilderSummary_frame* Summary;
extern bool summary_show; 
extern int **conn_values;
extern double **delays_values;


NetBuilderDelays_frame::NetBuilderDelays_frame( wxWindow* parent )
:
Delays_frame( parent )
{

}

void NetBuilderDelays_frame::OnInitDelays( wxInitDialogEvent& event )
{

	wxGridCellAttr *attr;
	attr = new wxGridCellAttr;
	attr->SetBackgroundColour(*wxLIGHT_GREY);

	wxGridCellAttr *attr1;
	attr1 = new wxGridCellAttr;
	attr1->SetBackgroundColour(*wxWHITE);

	int pop_number=total_populations;
	delays_grid->ClearGrid();
	int numbcols=delays_grid->GetNumberCols();
	
	if (pop_number<5)
	{
		delays_grid->DeleteRows(0,numbcols-pop_number,true);
		delays_grid->DeleteCols(0,numbcols-pop_number,true);
	}else if (pop_number>5)
	{
		delays_grid->AppendCols(pop_number-numbcols,true);
		delays_grid->AppendRows(pop_number-numbcols,true);
	}

	delays_grid->SetDefaultColSize(95, true);
	for (int i=0;i<pop_number;++i)
	{
	   
	    delays_grid->SetRowLabelValue( i,wxString(population[i].label,wxConvUTF8));
	    delays_grid->SetColLabelValue(i, wxString(population[i].label,wxConvUTF8));
	    delays_grid->SetColAttr(i,attr1);
	    for (int j=0;j<pop_number;++j)
	      {
		delays_grid->SetCellValue(i, j, wxString::Format(wxT("%.2lf"),delays_values[i][j]));
		if (conn_values[i][j]==0)
		  {
		    delays_grid->SetAttr(i,j,attr);
		    delays_grid->SetReadOnly(i,j,true);
		    delays_grid->SetCellValue(i,j,_T(""));
		  }
		
	      }
	}
	
	delays_grid->SetColLabelSize( 30 );
	//delays_grid->AutoSizeColumns();
	//delays_grid->AutoSizeRows();
	if (pop_number<6)
	  GetSizer()->SetSizeHints(this);
	
}


void NetBuilderDelays_frame::Onbtn_delays_OKClick( wxCommandEvent& event )
{
  MainFrame *myparent=(MainFrame *) GetParent();
  int error = 0;
  double val=0.0;
  for (unsigned int i=0; i<total_populations; i++){   
     for (unsigned int j=0; j<total_populations; j++){ 
	if (conn_values[i][j]!=0){
	    (delays_grid->GetCellValue(i,j)).ToDouble(&val);
	    if((delays_grid->GetCellValue(i,j)).ToDouble(&val)==false || val<0.0){
		myparent->text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
		wxLogMessage(wxT(">> WARNING: Delays' value of population ") +  wxString::Format(wxT("%i"),j) + wxT(" is invalid"));
		myparent->text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
		delays_values[i][j] = 5.0;
		error=-1;
	    }
	    else{
		(delays_grid->GetCellValue(i,j)).ToDouble(&delays_values[i][j]);
	    }
	}
    }
  }

        
  if (error == 0){ 
      myparent->Enable(true);
      this->Close();
      wxLogMessage(wxT(">> Delays values (re)inserted."));
       if (summary_show==true){
	  Summary->Destroy();
	  Summary = new NetBuilderSummary_frame(this);
	  Summary->Show(true);
	  summary_show=true;
	  }
    }
}
