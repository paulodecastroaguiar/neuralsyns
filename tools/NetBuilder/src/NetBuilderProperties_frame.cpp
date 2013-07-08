#include "NetBuilderProperties_frame.h"

extern unsigned int total_populations;
extern struct POPULATION *population;
extern struct DYNAMICS *Neuron_Models;
extern unsigned int total_neuron_models;
extern struct DYNAMICS *Synaptic_Models;
extern unsigned int total_syn_models;
extern struct DYNAMICS *Plasticity_Models;
extern unsigned int total_plas_models;
extern int **fdms_values;
extern NetBuilderSummary_frame* Summary;
extern bool summary_show; 
int pop_selected=0;
int total_fdomains=0;
extern int ***RANDOM_DIST;
extern double ***dist_values;
extern double **conn_values;

NetBuilderProperties_frame::NetBuilderProperties_frame( wxWindow* parent )
:
Properties_frame( parent )
{

}

void NetBuilderProperties_frame::OnInitProperties( wxInitDialogEvent& event )
{      
  int pop_number=total_populations;
  char ind_label[MAXLABELSIZE];
  int numbcols=Properties_grid->GetNumberCols();
  
  Properties_grid->ClearGrid();
  if (pop_number<5)
	Properties_grid->DeleteCols(0,numbcols-pop_number,true);
  else if (pop_number>5)
	Properties_grid->AppendCols(pop_number-numbcols,true);
  
  wxGridCellAttr *attr1;
  attr1 = new wxGridCellAttr;
  attr1->SetBackgroundColour(*wxWHITE);
  
  for (int j=0;j<pop_number;++j)
	{
	  if (j<10)
		snprintf(ind_label,MAXLABELSIZE,"Pop0%d",j);
	  else
		snprintf(ind_label,MAXLABELSIZE,"Pop%d",j);
	  Properties_grid->SetColLabelValue(j,wxString(ind_label,wxConvUTF8));
	  Properties_grid->SetCellValue(0,j,wxString(population[j].label,wxConvUTF8));
	  wxString size_pop =  int2wxStr(population[j].topology.dim_x) +  wxT("*") + int2wxStr(population[j].topology.dim_y) +  wxT("*") + int2wxStr(population[j].topology.dim_z);//size= (dim_x,dim_y, dim_z)TODO
	  Properties_grid->SetCellValue(1,j,size_pop);
	  Properties_grid->SetCellValue(2,j,int2wxStr(population[j].total_fdomains));
	  Properties_grid->SetCellValue(3,j,wxT("edit"));
	  Properties_grid->SetReadOnly(3,j,true);  
	}   
	
  wxGridCellAttr *attr;
  attr = new wxGridCellAttr;
  attr->SetBackgroundColour(wxColour(100,120,180));
  Properties_grid->SetRowAttr(0, attr1);
  Properties_grid->SetRowAttr(1, attr1);
  Properties_grid->SetRowAttr(2, attr1);
  Properties_grid->SetRowAttr(3, attr);
  Properties_grid->SetRowSize( 3, 28 );
  Properties_grid->SetColLabelSize( 30 );
  
  if (pop_number<6){
	GetSizer()->SetSizeHints(this);
  }
}

void NetBuilderProperties_frame::OnCellClick( wxGridEvent& event )
{
  MainFrame *myparent=(MainFrame *) GetParent();
  int error=0;
  wxChar *endptr;
  char rest[100];
  if (event.GetRow()==3){
	pop_selected=event.GetCol();
	int total_fdomains_value= (int) wxStrtol(Properties_grid->GetCellValue(2,pop_selected),&endptr,10);
	strcpy(rest,wxString(endptr).mb_str());
	rest[100-1]='\0';
	if (total_fdomains_value > 0 && strcmp(rest,"")==0){
	  if (total_fdomains_value!=population[pop_selected].total_fdomains)
	    Initialize_Fdomains(pop_selected,total_fdomains_value);
	}
	else{
	  myparent->text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	  wxLogMessage(wxT(">> WARNING: fdomains of population ") + int2wxStr(pop_selected) + wxT(" must be an integer greater than 0."));
	  myparent->text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	  error=-1;
	}
	if (error==0) {
	  population[pop_selected].total_fdomains=wxAtoi(Properties_grid->GetCellValue(2,pop_selected));
	  this->Enable(false);
	  NetBuilderDynamics_frame *Dynamics_frame=new NetBuilderDynamics_frame(this);
	  Dynamics_frame->Show();
	}
  }
  event.Skip();
}

void NetBuilderProperties_frame::Onbtn_pop_OKClick( wxCommandEvent& event )
{
  MainFrame *myparent=(MainFrame *) GetParent();
  
  int error = 0;
  wxChar *endptr;
  char rest[100];
  for (unsigned int i=0; i<total_populations; i++){
	int return_value;
	char size_label[MAXLABELSIZE];
	
	char old_label[MAXLABELSIZE];
	str_equal(population[i].label,old_label);
	strncpy(population[i].label,Properties_grid->GetCellValue(0,i).mb_str(),MAXLABELSIZE);
	population[i].label[MAXLABELSIZE-1]='\0';
	
	if (strcmp("",population[i].label)==0){
	  myparent->text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	  wxLogMessage(wxT(">> WARNING: label of population ") +  wxString::Format(wxT("%i"),i) + wxT(" is empty"));
	  myparent->text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	  str_equal(old_label,population[i].label);
	  error=-1;
	}
	
	wxStr2str(Properties_grid->GetCellValue(1,i),size_label);
    
	return_value=sscanf(size_label,"%ld*%ld*%ld)",&population[i].topology.dim_x,&population[i].topology.dim_y,&population[i].topology.dim_z);
	if (return_value!=3 || population[i].topology.dim_x < 1 || population[i].topology.dim_y < 1 || population[i].topology.dim_z < 1) 
	  {
		myparent->text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
		wxLogMessage(wxT(">> WARNING: size of population ") +  wxString::Format(wxT("%i"),i) + wxT(" is invalid"));
		myparent->text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
		population[i].topology.dim_x = 1;
		population[i].topology.dim_y = 1;
		population[i].topology.dim_z = 1;
		error=-1;
	  }
	
	for (unsigned int j=0; j<total_populations; j++){
	  if (dist_values[j][i][0]>population[i].topology.dim_x*population[i].topology.d_mean) 
		RANDOM_DIST[j][i][0]=1; //uniform
	  else
		RANDOM_DIST[j][i][0]=0; //gaussian
	  if (dist_values[j][i][1]>population[i].topology.dim_y*population[i].topology.d_mean) 
		RANDOM_DIST[j][i][1]=1; //uniform
	  else
		RANDOM_DIST[j][i][1]=0; //gaussian
	  if (dist_values[j][i][2]>population[i].topology.dim_z*population[i].topology.d_mean) 
		RANDOM_DIST[j][i][2]=1; //uniform
	  else
		RANDOM_DIST[j][i][2]=0; //gaussian
	}
	
	int total_fdomains_value= (int) wxStrtol(Properties_grid->GetCellValue(2,i),&endptr,10);
	strcpy(rest,wxString(endptr).mb_str());
	rest[100-1]='\0';
  
	if (total_fdomains_value <= 0 || strcmp(rest, "")!=0){
	  myparent->text_status->SetForegroundColour( wxColour( 247, 22, 10 ) );
	  wxLogMessage(wxT(">> WARNING: fdomains of population ") + int2wxStr(i) + wxT(" must be an integer greater than 0."));
	  myparent->text_status->SetForegroundColour( wxColour( 0, 0, 0 ) );
	  error=-1;
	}
	else{
	  if (total_fdomains_value!=population[i].total_fdomains){
		Initialize_Fdomains(i,total_fdomains_value);
		for (unsigned int j=0; j<total_populations; j++){
		  fdms_values[i][j]=0;
		}
	  }
	  population[i].total_fdomains=total_fdomains_value;   
	}
}
  
  if (error == 0){
      this->Close();
      myparent->Enable(true);
      wxLogMessage(wxT(">> Population properties (re)defined."));
	  if (summary_show==true)
		{
		  Summary->Destroy();
		  Summary = new NetBuilderSummary_frame(this);
		  Summary->Show(true);
		  summary_show=true;
		}
  }
}
