#include "NetBuilderDynamics_frame.h"


extern struct POPULATION *population;
extern int pop_selected;
extern int fdms_selected;
extern  NetBuilderDynamics_frame* Dynamics_frame;
NetBuilderModel_frame *Model_frame = NULL;
Visualization_frame *Viz_frame=NULL;
extern int model_table;
extern struct DYNAMICS *Neuron_Models;
extern unsigned int total_neuron_models;
extern int total_fdomains;
extern bool visual_show;

#define AxialResist 10.0 //[Mohms] the minimum value for axial resistance (Ra); default value is 10.

NetBuilderDynamics_frame::NetBuilderDynamics_frame( wxDialog* parent )
:
Dynamics_frame( parent )
{

}

void NetBuilderDynamics_frame::OnInitDynamics( wxInitDialogEvent& event )
{
     model_table=1;

     int cols=population[pop_selected].total_fdomains;
     int numbcols=dynamics_grid->GetNumberCols();

     dynamics_grid->ClearGrid();

      if (cols<5)
	  dynamics_grid->DeleteCols(0,numbcols-cols,true);
      else if (cols>5)
	  dynamics_grid->AppendCols(cols-numbcols,true);
	
      wxGridCellAttr *attr;
      attr = new wxGridCellAttr;
      attr->SetBackgroundColour(*wxWHITE);
	
      dynamics_grid->SetColLabelValue(0,wxT("soma"));
      dynamics_grid->SetCellValue(0,0,wxString(population[pop_selected].fdomains[0].label,wxConvUTF8));
      dynamics_grid->SetCellValue(2,0,dbl2wxStr(population[pop_selected].fdomains[0].r));
      dynamics_grid->SetReadOnly(1,0,true);
      dynamics_grid->SetReadOnly(3,0,true);
      dynamics_grid->SetReadOnly(4,0,true);
      dynamics_grid->SetColAttr(0, attr);
      dynamics_grid->SetCellBackgroundColour(1,0,*wxLIGHT_GREY);
      dynamics_grid->SetCellBackgroundColour(3,0,*wxLIGHT_GREY);
      dynamics_grid->SetCellBackgroundColour(4,0,*wxLIGHT_GREY);
      
      wxString choices_0[total_neuron_models];
      int i=0;
      for (unsigned int m = 0; m < total_neuron_models; m++){
	  if (Neuron_Models[m].evt_gen==1){
	    choices_0[i]= wxString(Neuron_Models[m].model_label,wxConvUTF8) ;
	    i=i+1;
	  }
      }
      
      dynamics_grid->SetCellRenderer(5,0, new wxGridCellChoiceRenderer());
      dynamics_grid->SetCellEditor(5, 0, new wxFastComboEditor(i, choices_0,true));
      dynamics_grid->SetCellValue(5, 0, wxString(population[pop_selected].fdomains[0].dynamics.model_label,wxConvUTF8));
      dynamics_grid->SetCellOverflow(5, 0, false);
      
     
      wxString choices_n[total_neuron_models];
      int j=0;
      for (unsigned int m=0; m<total_neuron_models;m++){
	 choices_n[j]=wxString(Neuron_Models[m].model_label,wxConvUTF8) ;
	 j=j+1;
      }

      for (int fdm = 1; fdm < cols; fdm++)
	{
	  dynamics_grid->SetColLabelValue(fdm,wxString(population[pop_selected].fdomains[fdm].label,wxConvUTF8));
	}
      for (int fdm = 1; fdm < cols; fdm++)
	{
	  dynamics_grid->SetCellValue(0,fdm,wxString(population[pop_selected].fdomains[fdm].label,wxConvUTF8));
	  wxString position= wxT("(") + wxString::Format(wxT("%#.1lf"),population[pop_selected].fdomains[fdm].x) + wxT(",") + wxString::Format(wxT("%#.1lf"),population[pop_selected].fdomains[fdm].y) + wxT(",") + wxString::Format(wxT("%#.1lf"),population[pop_selected].fdomains[fdm].z) + wxT(")") ;
	  dynamics_grid->SetCellValue(1,fdm,position);
	  dynamics_grid->SetCellValue(2,fdm,dbl2wxStr(population[pop_selected].fdomains[fdm].r));
	  dynamics_grid->SetCellValue(3,fdm,dbl2wxStr(population[pop_selected].fdomains[fdm].Ra));
	  dynamics_grid->SetCellValue(4,fdm,wxString(population[pop_selected].fdomains[fdm].parent_label,wxConvUTF8));
	  dynamics_grid->SetCellRenderer(5,fdm, new wxGridCellChoiceRenderer());
	  dynamics_grid->SetCellEditor(5, fdm, new wxFastComboEditor(j, choices_n,true));
	  dynamics_grid->SetCellValue(5, fdm, wxString(population[pop_selected].fdomains[fdm].dynamics.model_label,wxConvUTF8));
	  dynamics_grid->SetCellOverflow(5, fdm, false);
 	  dynamics_grid->SetColAttr(fdm, attr);
 	  dynamics_grid->SetColSize(fdm,110);
	}
      dynamics_grid->SetColLabelSize( 30 );

      if (cols<6)
	  GetSizer()->SetSizeHints(this);
}

void NetBuilderDynamics_frame::OnCellClick( wxGridEvent& event )
{
    if (event.GetRow()==5){
	Model_frame = new NetBuilderModel_frame(this);
    	event.Skip();
    }
    event.Skip();
}

void NetBuilderDynamics_frame::Onbtn_view_neuronClick( wxCommandEvent& event )
{
    if (visual_show == false){
	Viz_frame = new NetBuilderVisualization_frame(this);
	Viz_frame -> Show(true);
    }
    else
	event.Skip();
}

void NetBuilderDynamics_frame::Onbtn_Dynamics_OKClick( wxCommandEvent& event )
{

	Properties_frame *myparent= (Properties_frame*) GetParent();
	char position[50];
	int return_value;
	int error=0;
	char ind_label[MAXLABELSIZE];

	 wxStr2str(dynamics_grid->GetCellValue(0,0),population[pop_selected].fdomains[0].label);
	(dynamics_grid->GetCellValue(2,0)).ToDouble(&population[pop_selected].fdomains[0].r);
	
	population[pop_selected].fdomains[0].x = 0.0;
	population[pop_selected].fdomains[0].y = 0.0;
	population[pop_selected].fdomains[0].z = 0.0;
	population[pop_selected].fdomains[0].Ra = 10;
	str_equal("",population[pop_selected].fdomains[0].parent_label);
	population[pop_selected].fdomains[0].parent_ID = -1;

	  if (population[pop_selected].fdomains[0].r <=0.0){
		population[pop_selected].fdomains[0].r=3.0;
		dynamics_grid->SetCellTextColour(2,0,*wxRED);
		dynamics_grid->Refresh();
		error=-1;
	}

	for (int fdm = 1; fdm < population[pop_selected].total_fdomains; fdm++)
	{
	    wxStr2str(dynamics_grid->GetCellValue(0,fdm),population[pop_selected].fdomains[fdm].label);
	    wxStr2str(dynamics_grid->GetCellValue(1,fdm),position);
	    return_value=sscanf(position,"(%lf,%lf,%lf)", &population[pop_selected].fdomains[fdm].x, &population[pop_selected].fdomains[fdm].y, &population[pop_selected].fdomains[fdm].z);
	    if (return_value!=3) 
	      {
		population[pop_selected].fdomains[fdm].x = 0.0;
		population[pop_selected].fdomains[fdm].y = 0.0;
		population[pop_selected].fdomains[fdm].z = 0.0;
		dynamics_grid->SetCellTextColour(1,fdm,*wxRED);
		dynamics_grid->Refresh();
		error=-1;
	      }

	    (dynamics_grid->GetCellValue(2,fdm)).ToDouble(&population[pop_selected].fdomains[fdm].r);
	    if (population[pop_selected].fdomains[fdm].r <=0.0){
		  population[pop_selected].fdomains[fdm].r=3.0;
		  dynamics_grid->SetCellTextColour(2,fdm,*wxRED);
		  dynamics_grid->Refresh();
		  error=-1;
	    }

	    (dynamics_grid->GetCellValue(3,fdm)).ToDouble(&population[pop_selected].fdomains[fdm].Ra);
	    if (population[pop_selected].fdomains[fdm].Ra < AxialResist){
		  population[pop_selected].fdomains[fdm].Ra=10;
		  dynamics_grid->SetCellTextColour(3,fdm,*wxRED);
		  dynamics_grid->Refresh();
		  error=-1;
	    }

	    wxStr2str(dynamics_grid->GetCellValue(4,fdm),population[pop_selected].fdomains[fdm].parent_label);
	    int n=0;
	    for (int i = 0; i < population[pop_selected].total_fdomains; i++){
		      if (strcmp(population[pop_selected].fdomains[fdm].parent_label,population[pop_selected].fdomains[i].label)!=0){
			    n=n+1;
		      }
	    }
	    
	    int pid=0;
	    for (int i = 0; i < population[pop_selected].total_fdomains; i++){
		      if (strcmp(population[pop_selected].fdomains[fdm].parent_label,population[pop_selected].fdomains[i].label)==0){
			    pid=i;
		      }
	    }
	    population[pop_selected].fdomains[fdm].parent_ID=pid;
	    
	    //check if error inserted
	    if (n!=population[pop_selected].total_fdomains-1 || strcmp(population[pop_selected].fdomains[fdm].parent_label,population[pop_selected].fdomains[fdm].label)==0){
		snprintf(ind_label,MAXLABELSIZE,"soma");
		str_equal(ind_label,population[pop_selected].fdomains[fdm].parent_label);
		population[pop_selected].fdomains[fdm].parent_ID=0;
		dynamics_grid->SetCellTextColour(4,fdm,*wxRED);
		dynamics_grid->Refresh();
		error=-1;
	    }

	}

	if (error==0){
	    this->Close();
	    myparent->Enable(true);
	    if (visual_show==true)
		{
		  Viz_frame->Destroy();
		  visual_show=false;
		}
	}
}
