#include "NetBuilderConductances_frame.h"

extern unsigned int total_populations;
extern struct POPULATION *population;
extern struct SYNAPSES **synapses;
extern NetBuilderSummary_frame* Summary;
extern NetBuilderModel_frame *Model_frame;
extern struct DYNAMICS *Conductances_Models;
extern unsigned int total_syn_models;
extern bool summary_show; 
extern int **conn_values;
extern int pop_selected;	
extern int model_table;



NetBuilderConductances_frame::NetBuilderConductances_frame( wxWindow* parent )
:
Conductances_frame( parent )
{

}

void NetBuilderConductances_frame::OnInitConductances( wxInitDialogEvent& event )
{	
	model_table=2;
	wxGridCellAttr *attr;
	attr = new wxGridCellAttr;
	attr->SetBackgroundColour(*wxLIGHT_GREY);

	wxGridCellAttr *attr1;
	attr1 = new wxGridCellAttr;
	attr1->SetBackgroundColour(*wxWHITE);

	wxString choices[total_syn_models];
	
	for (unsigned int choice=0; choice<total_syn_models;choice++){
		choices[choice]=wxString(Conductances_Models[choice].model_label,wxConvUTF8) ;
	}
	
	int pop_number=total_populations;
	conductances_grid->ClearGrid();
	int numbcols=conductances_grid->GetNumberCols();
	
	if (pop_number<5){
		conductances_grid->DeleteRows(0,numbcols-pop_number,true);
		conductances_grid->DeleteCols(0,numbcols-pop_number,true);
	}else if (pop_number>5){
		conductances_grid->AppendCols(pop_number-numbcols,true);
		conductances_grid->AppendRows(pop_number-numbcols,true);
	}

	conductances_grid->SetDefaultColSize(95, true);
	for (int popi=0;popi<pop_number;popi++)
	{
	    conductances_grid->SetRowLabelValue( popi,wxString(population[popi].label,wxConvUTF8));
	    conductances_grid->SetColLabelValue(popi, wxString(population[popi].label,wxConvUTF8));
	    conductances_grid->SetRowAttr(popi,attr1);
	    for (int popj=0;popj<pop_number;popj++)
	      {
		if (conn_values[popi][popj]!=0)
		  {
		    conductances_grid->SetCellRenderer(popi,popj, new wxGridCellChoiceRenderer());
		    conductances_grid->SetCellEditor(popi, popj, new wxFastComboEditor(WXSIZEOF(choices), choices,true));
		    conductances_grid->SetCellValue(popi, popj, wxString(synapses[popi][popj].conductances_dynamics.model_label,wxConvUTF8) );
		    conductances_grid->SetCellOverflow(popi, popj, false);
		   
		  }
		else 
		  {
		    conductances_grid->SetAttr(popi,popj,attr);
		    conductances_grid->SetReadOnly(popi,popj,true);
		  }
		}
	}
	//conductances_grid->AutoSizeColumns();
	if (pop_number<6)
	GetSizer()->SetSizeHints(this);

//this->Layout();
    // TODO: Implement OnInitFdms
}

void NetBuilderConductances_frame::OnCellClick( wxGridEvent& event )
{
//     char str[MAXLABELSIZE];
//     wxStr2str(conductances_grid->GetCellValue(event.GetRow(),event.GetCol()),str);
//     if (strcmp(Conductances_Models[0].model_label,str) == 0){
//       event.Skip();
//     }
//     else{
      Model_frame = new NetBuilderModel_frame(this);
      event.Skip();
    //}
}

void NetBuilderConductances_frame::Onbtn_Conductances_CancelClick( wxCommandEvent& event )
{
	// TODO: Implement Onbtn_Conductances_CancelClick
}

void NetBuilderConductances_frame::Onbtn_Conductances_OKClick( wxCommandEvent& event )
{
    MainFrame *myparent=(MainFrame *) GetParent();
    myparent->Enable(true);
    this->Close();
    wxLogMessage(wxT(">> Synaptic conductance models (re)inserted."));
    if (summary_show==true)
      {
	Summary->Destroy();
	Summary = new NetBuilderSummary_frame(this);
	Summary->Show(true);
	summary_show=true;
      }
}
