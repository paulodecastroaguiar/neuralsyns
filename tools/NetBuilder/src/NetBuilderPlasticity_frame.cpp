#include "NetBuilderPlasticity_frame.h"

extern unsigned int total_populations;
extern struct POPULATION *population;
extern struct SYNAPSES **synapses;
extern NetBuilderSummary_frame* Summary;
extern NetBuilderModel_frame *Model_frame;
extern struct DYNAMICS *Plasticity_Models;
extern unsigned int total_plas_models;
extern bool summary_show; 
extern int **conn_values;
extern int model_table;
extern wxString dynamic_model;

NetBuilderPlasticity_frame::NetBuilderPlasticity_frame( wxWindow* parent )
:
Plasticity_frame( parent )
{

}

void NetBuilderPlasticity_frame::OnInitPlasticity( wxInitDialogEvent& event )
{
	model_table=3;
	wxGridCellAttr *attr;
	attr = new wxGridCellAttr;
	attr->SetBackgroundColour(*wxLIGHT_GREY);
	wxGridCellAttr *attr1;
	attr1 = new wxGridCellAttr;
	attr1->SetBackgroundColour(*wxWHITE);

        wxString choices[total_plas_models];
     
	for (unsigned int choice=0; choice<total_plas_models;choice++){
 	    choices[choice]=wxString(Plasticity_Models[choice].model_label,wxConvUTF8) ;
	}

	int pop_number=total_populations;
	plasticity_grid->ClearGrid();
	int numbcols=plasticity_grid->GetNumberCols();
	
	if (pop_number<5)
	{
		plasticity_grid->DeleteRows(0,numbcols-pop_number,true);
		plasticity_grid->DeleteCols(0,numbcols-pop_number,true);
	}else if (pop_number>5)
	{
		plasticity_grid->AppendCols(pop_number-numbcols,true);
		plasticity_grid->AppendRows(pop_number-numbcols,true);
	}

	plasticity_grid->SetDefaultColSize(95, true);
	for (int i=0;i<pop_number;++i)
	{
	    plasticity_grid->SetRowLabelValue( i,wxString(population[i].label,wxConvUTF8));
	    plasticity_grid->SetColLabelValue(i, wxString(population[i].label,wxConvUTF8));
	    for (int j=0;j<pop_number;++j)
	      {
		
		if (conn_values[i][j]!=0)
		  {
		    plasticity_grid->SetAttr(i,j,attr1);
		    plasticity_grid->SetCellRenderer(i,j, new wxGridCellChoiceRenderer());
		    plasticity_grid->SetCellEditor(i, j, new wxFastComboEditor(WXSIZEOF(choices), choices,true));
		    plasticity_grid->SetCellValue(i, j, wxString(synapses[i][j].plasticity_dynamics.model_label,wxConvUTF8));
		    plasticity_grid->SetCellOverflow(i, j, false);
		  }
		else 
		  {
		    plasticity_grid->SetAttr(i,j,attr);
		    plasticity_grid->SetReadOnly(i,j,true);
		  }
		}
	}
	//plasticity_grid->AutoSizeColumns();
	//plasticity_grid->AutoSizeRows();
	if (pop_number<6){
	  GetSizer()->SetSizeHints(this);	
	}
}

void NetBuilderPlasticity_frame::OnCellClick( wxGridEvent& event )
{
	Model_frame = new NetBuilderModel_frame(this);
	event.Skip();

}

void NetBuilderPlasticity_frame::Onbtn_Plasticity_OKClick( wxCommandEvent& event )
{
	MainFrame *myparent=(MainFrame *) GetParent();
	myparent->Enable(true);
	this->Close();
	wxLogMessage(wxT(">> Plasticity models (re)inserted."));
        if (summary_show==true)
	  {
	  Summary->Destroy();
	  Summary = new NetBuilderSummary_frame(this);
	  Summary->Show(true);
	  summary_show=true;
	  }
}
