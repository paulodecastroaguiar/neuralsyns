#include "NetBuilderFdms_frame.h"
#include "NetBuilderConnectivity_frame.h"

extern unsigned int total_populations;
extern struct POPULATION *population;
extern NetBuilderSummary_frame* Summary;
extern bool summary_show; 
extern int **conn_values;
extern int **fdms_values;
extern int fdms_check;

NetBuilderFdms_frame::NetBuilderFdms_frame( wxWindow* parent )
:
Fdms_frame( parent )
{

}

void NetBuilderFdms_frame::OnInitFdms( wxInitDialogEvent& event )
{

	wxGridCellAttr *attr;
	attr = new wxGridCellAttr;
	attr->SetBackgroundColour(*wxLIGHT_GREY);

	wxGridCellAttr *attr1;
	attr1 = new wxGridCellAttr;
	attr1->SetBackgroundColour(*wxWHITE);

	int pop_number=total_populations;
	fdms_grid->ClearGrid();
	int numbcols=fdms_grid->GetNumberCols();
	
	if (pop_number<5){
		fdms_grid->DeleteRows(0,numbcols-pop_number,true);
		fdms_grid->DeleteCols(0,numbcols-pop_number,true);
	}
	else if (pop_number>5){
		fdms_grid->AppendCols(pop_number-numbcols,true);
		fdms_grid->AppendRows(pop_number-numbcols,true);
	}

	fdms_grid->SetDefaultColSize(95, true);
	for (int i=0;i<pop_number;++i){
	    fdms_grid->SetRowLabelValue(i,wxString(population[i].label,wxConvUTF8));
	    fdms_grid->SetColLabelValue(i,wxString(population[i].label,wxConvUTF8));
	    fdms_grid->SetColAttr(i,attr1);
	    for (int j=0;j<pop_number;++j){
		wxString choices[population[j].total_fdomains];
		for (int choice=0; choice<population[j].total_fdomains;choice++){
		    choices[choice]=wxString(population[j].fdomains[choice].label,wxConvUTF8) ;
		}
		fdms_grid->SetCellRenderer(i,j, new wxGridCellChoiceRenderer());
		fdms_grid->SetCellEditor(i, j, new wxFastComboEditor(WXSIZEOF(choices), choices,true));
		fdms_grid->SetCellValue(i, j, wxString(population[j].fdomains[fdms_values[i][j]].label,wxConvUTF8));
		fdms_grid->SetCellOverflow(i, j, false);

		if (conn_values[i][j]==0){
		    fdms_grid->SetAttr(i,j,attr);
		    fdms_grid->SetReadOnly(i,j,true);
		    fdms_grid->SetCellValue(i,j,_T(""));
		  }
		
	      }
	}
	
	fdms_grid->SetColLabelSize( 30 );
	//fdms_grid->AutoSizeColumns();
	//fdms_grid->AutoSizeRows();
	if (pop_number<6)
	  GetSizer()->SetSizeHints(this);
	fdms_check=1;//to control fastcomboeditor
}

void NetBuilderFdms_frame::OnCellClickFdms( wxGridEvent& event )
{
       fdms_check=1;
       event.Skip();
}

void NetBuilderFdms_frame::Onbtn_fdms_OKClick( wxCommandEvent& event )
{
  fdms_check=0;
  MainFrame *myparent=(MainFrame *) GetParent();
  int error = 0;
  char new_fdms[MAXLABELSIZE];
  for (unsigned int i=0; i<total_populations; i++){   
     for (unsigned int j=0; j<total_populations; j++){ 
	if (conn_values[i][j]!=0){
		wxStr2str(fdms_grid->GetCellValue(i,j),new_fdms);
		for (int fdm=0; fdm<population[j].total_fdomains;fdm++){
		    if(strncmp(new_fdms,population[j].fdomains[fdm].label,MAXLABELSIZE)==0){
			fdms_values[i][j]=fdm;
			break;
		    }
		}
	}
     }
  }
  
  if (error == 0){ 
      myparent->Enable(true);
      this->Close();
      wxLogMessage(wxT(">> FDMS values (re)inserted."));
       if (summary_show==true){
	  Summary->Destroy();
	  Summary = new NetBuilderSummary_frame(this);
	  Summary->Show(true);
	  summary_show=true;
	  }
    }
}
