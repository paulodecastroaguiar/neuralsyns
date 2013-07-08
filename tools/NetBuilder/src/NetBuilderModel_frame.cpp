#include "NetBuilderModel_frame.h"

extern struct POPULATION *population;
extern unsigned int total_populations;
extern struct DYNAMICS *Neuron_Models;
extern unsigned int total_neuron_models;
extern struct DYNAMICS *Conductances_Models;
extern unsigned int total_syn_models;
extern struct DYNAMICS *Plasticity_Models;
extern unsigned int total_plas_models;
extern struct SYNAPSES **synapses;
extern int pop_selected;
extern int fdms_selected;
extern int pop_pre;
extern int pop_pos;
extern wxString dynamic_model;
extern int model_table;


NetBuilderModel_frame::NetBuilderModel_frame( wxDialog* parent )
:
Model_frame( parent )
{
}

void NetBuilderModel_frame::OnInitModel( wxInitDialogEvent& event )
{
  char model[MAXLABELSIZE];
  wxStr2str(dynamic_model,model);  
  Dynamics_frame* myparent1=(Dynamics_frame *) GetParent();
  Conductances_frame* myparent2=(Conductances_frame *) GetParent();
  Plasticity_frame* myparent3=(Plasticity_frame *) GetParent();

  wxGridCellAttr *attr;
  attr = new wxGridCellAttr;
  attr->SetBackgroundColour(*wxWHITE);

  int n_state_var;

  switch (model_table)
  {
    case 1: /// Dynamic model for fdomains population 
    	
	myparent1->Enable(false);
	for (unsigned int i=0;i<total_neuron_models;i++)
	  {
	    if (strcmp(Neuron_Models[i].model_label,model) == 0)
	      line=i;
	  }
	total_par=Neuron_Models[line].model_total_parameters;
	n_state_var=Neuron_Models[line].n_state_vars;
	model_grid->ClearGrid();
	if (total_par<4)
	    model_grid->DeleteRows(0,model_grid->GetNumberRows()-total_par,true);
	else if (total_par>4)
	    model_grid->AppendRows(total_par-model_grid->GetNumberRows(),true);

	
	if (population[pop_selected].fdomains[fdms_selected].dynamics.model_ID!=Neuron_Models[line].model_ID)
	{
	      free(population[pop_selected].fdomains[fdms_selected].dynamics.parameter_label); //free memory to alocate a different model
	      free(population[pop_selected].fdomains[fdms_selected].dynamics.parameter);
	      free(population[pop_selected].fdomains[fdms_selected].dynamics.state_vars_label);
     	      population[pop_selected].fdomains[fdms_selected].dynamics.parameter=(double*) malloc(total_par * sizeof(double));
	      population[pop_selected].fdomains[fdms_selected].dynamics.parameter_label=(char**) malloc(total_par * sizeof(char*));
	      if (population[pop_selected].fdomains[fdms_selected].dynamics.parameter == NULL || population[pop_selected].fdomains[fdms_selected].dynamics.parameter_label == NULL ) 
		    {
		      printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		      exit(-1);
		      Free_Memory();
		    }

	      population[pop_selected].fdomains[fdms_selected].dynamics.model_ID=Neuron_Models[line].model_ID;
	      str_equal(Neuron_Models[line].model_label,population[pop_selected].fdomains[fdms_selected].dynamics.model_label);
	      population[pop_selected].fdomains[fdms_selected].dynamics.model_total_parameters=total_par;

	      model_grid->SetColLabelValue(0,wxString(Neuron_Models[line].model_label,wxConvUTF8));
	      for (int j = 0; j < total_par; j++)
		{
		  population[pop_selected].fdomains[fdms_selected].dynamics.parameter_label[j]=(char*) malloc( MAXLABELSIZE * sizeof(char));
	    	  if (population[pop_selected].fdomains[fdms_selected].dynamics.parameter_label[j] == NULL) 
		    {
		      printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		      exit(-1);
		      Free_Memory();
		    }
		  model_grid->SetRowLabelValue(j,wxString(Neuron_Models[line].parameter_label[j],wxConvUTF8));
		  model_grid->SetCellValue(j,0,dbl2wxStr(Neuron_Models[line].parameter[j]));
		  model_grid->SetRowAttr(j,attr);
		}
	      population[pop_selected].fdomains[fdms_selected].dynamics.state_vars_label=(char**) malloc( n_state_var * sizeof(char*));
	      if ( population[pop_selected].fdomains[fdms_selected].dynamics.state_vars_label == NULL ) 
		    {
		      printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		      exit(-1);
		      Free_Memory();
		    }

	      population[pop_selected].fdomains[fdms_selected].dynamics.n_state_vars=n_state_var;
	      for (int j = 0; j < n_state_var; j++)
		{
		  population[pop_selected].fdomains[fdms_selected].dynamics.state_vars_label[j]=(char*) malloc( MAXLABELSIZE * sizeof(char));
	    	  if (population[pop_selected].fdomains[fdms_selected].dynamics.state_vars_label[j] == NULL) 
		    {
		      printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		      exit(-1);
		      Free_Memory();
		    }
		  str_equal(Neuron_Models[line].state_vars_label[j],population[pop_selected].fdomains[fdms_selected].dynamics.state_vars_label[j]);
		}
	  }
	  else
	   {
	      model_grid->SetColLabelValue(0,wxString(population[pop_selected].fdomains[fdms_selected].dynamics.model_label,wxConvUTF8));
	      for (int j = 0; j < total_par; j++)
		  {
		    model_grid->SetRowLabelValue(j,wxString(population[pop_selected].fdomains[fdms_selected].dynamics.parameter_label[j],wxConvUTF8));
		    model_grid->SetCellValue(j,0,dbl2wxStr(population[pop_selected].fdomains[fdms_selected].dynamics.parameter[j]));
		    model_grid->SetRowAttr(j,attr);
		  }
	    }
	    
	break;
   
    case 2: ///Dynamic model for synaptic conductances 
	
	myparent2->Enable(false);
	for (unsigned int i=0;i<total_syn_models;i++)
	  {
	  if (strcmp(model,Conductances_Models[i].model_label) == 0)
	    line=i;
	  }
	  
	total_par=Conductances_Models[line].model_total_parameters;
	model_grid->ClearGrid();
	if (total_par<4)
	      model_grid->DeleteRows(0,model_grid->GetNumberRows()-total_par,true);
	else if (total_par>4)
	      model_grid->AppendRows(total_par-model_grid->GetNumberRows(),true);

	if ( synapses[pop_pre][pop_pos].conductances_dynamics.model_ID!=Conductances_Models[line].model_ID)
	  {
	      free(synapses[pop_pre][pop_pos].conductances_dynamics.parameter_label); //free memory to alocate a different model
	      free(synapses[pop_pre][pop_pos].conductances_dynamics.parameter);
	      synapses[pop_pre][pop_pos].conductances_dynamics.parameter=(double*) malloc(total_par * sizeof(double));
	      synapses[pop_pre][pop_pos].conductances_dynamics.parameter_label=(char**) malloc(total_par * sizeof(char*));
	      if (synapses[pop_pre][pop_pos].conductances_dynamics.parameter_label == NULL || synapses[pop_pre][pop_pos].conductances_dynamics.parameter==NULL) 
		{
		   printf("\n\tERROR: unable to allocate memory for list of SYNAPSES");
		   printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		   exit(-1);
		   Free_Memory();
		 }
	      
	      free(synapses[pop_pre][pop_pos].conductances_dynamics.state_vars_label); //free memory to alocate a different model
	      free(synapses[pop_pre][pop_pos].conductances_dynamics.state_vars_val);
	      synapses[pop_pre][pop_pos].conductances_dynamics.state_vars_val=(double*) malloc(Conductances_Models[line].n_state_vars * sizeof(double));
	      synapses[pop_pre][pop_pos].conductances_dynamics.state_vars_label=(char**) malloc(Conductances_Models[line].n_state_vars * sizeof(char*));
	      if (synapses[pop_pre][pop_pos].conductances_dynamics.state_vars_label == NULL || synapses[pop_pre][pop_pos].conductances_dynamics.state_vars_val==NULL) 
		{
		   printf("\n\tERROR: unable to allocate memory for list of SYNAPSES");
		   printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		   exit(-1);
		   Free_Memory();
		 }
		 
	      synapses[pop_pre][pop_pos].conductances_dynamics.model_ID=Conductances_Models[line].model_ID;
	      str_equal(Conductances_Models[line].model_label,synapses[pop_pre][pop_pos].conductances_dynamics.model_label);
	      synapses[pop_pre][pop_pos].conductances_dynamics.model_total_parameters=total_par;
	      synapses[pop_pre][pop_pos].conductances_dynamics.n_state_vars=Conductances_Models[line].n_state_vars;
	      model_grid->SetColLabelValue(0,wxString(Conductances_Models[line].model_label,wxConvUTF8));
	      for (int j = 0; j < total_par; j++)
		{
		  synapses[pop_pre][pop_pos].conductances_dynamics.parameter_label[j]=(char*) malloc(MAXLABELSIZE * sizeof(char));
		  if (synapses[pop_pre][pop_pos].conductances_dynamics.parameter_label[j] == NULL) 
		    {
		      printf("\n\tERROR: unable to allocate memory for list of SYNAPSES");
		      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		      exit(-1);
		      Free_Memory();
		    }
		  model_grid->SetRowLabelValue(j,wxString(Conductances_Models[line].parameter_label[j],wxConvUTF8));
		  model_grid->SetCellValue(j,0,dbl2wxStr(Conductances_Models[line].parameter[j]));
		  model_grid->SetRowAttr(j,attr);
		}
	      for (int var=0; var<synapses[pop_pre][pop_pos].conductances_dynamics.n_state_vars;var++){
		  synapses[pop_pre][pop_pos].conductances_dynamics.state_vars_label[var]=(char*) malloc(MAXLABELSIZE * sizeof(char));
		  synapses[pop_pre][pop_pos].conductances_dynamics.state_vars_val[var]=Conductances_Models[line].state_vars_val[var];
		  str_equal(Conductances_Models[line].state_vars_label[var],synapses[pop_pre][pop_pos].conductances_dynamics.state_vars_label[var]);
	      }
	    }
	  else
	    {
	      model_grid->SetColLabelValue(0,wxString(synapses[pop_pre][pop_pos].conductances_dynamics.model_label,wxConvUTF8));
	      for (int j = 0; j < total_par; j++){
		  model_grid->SetRowLabelValue(j,wxString(synapses[pop_pre][pop_pos].conductances_dynamics.parameter_label[j],wxConvUTF8));
		  model_grid->SetCellValue(j,0,dbl2wxStr(synapses[pop_pre][pop_pos].conductances_dynamics.parameter[j]));
		  model_grid->SetRowAttr(j,attr);
		}
	      }
	  break;
  
    case 3: /// Dynamic model for synaptic plasticity 
	
	  myparent3->Enable(false);
	  for (unsigned int i=0;i<total_plas_models;i++)
	    {
	    if (strcmp(model,Plasticity_Models[i].model_label) == 0)
	    line=i;
	    }
	  total_par=Plasticity_Models[line].model_total_parameters;
	  model_grid->ClearGrid();
	  
	  if (total_par==0){  // in the case the model (Static) has no parameters
	    model_grid->Hide();
	    wxBoxSizer* bSizer11;
	    bSizer11 = new wxBoxSizer( wxVERTICAL );
	    text_title = new wxStaticText( m_panel9, wxID_ANY, wxT("No parameters required"), wxDefaultPosition, wxDefaultSize, 0 );
	    text_title->Wrap( -1 );
	    text_title->SetFont( wxFont( 10, 70, 90, 90, false, wxEmptyString ) );
	    bSizer11->Add( text_title, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	    m_panel9->SetSizer( bSizer11 );
	    GetSizer()->SetSizeHints(this);
	  }
	  
	  if (total_par<4)
	    model_grid->DeleteRows(0,model_grid->GetNumberRows()-total_par,true);
	  else if (total_par>4)
	    model_grid->AppendRows(total_par-model_grid->GetNumberRows(),true);
	  
	  if ( synapses[pop_pre][pop_pos].plasticity_dynamics.model_ID!=Plasticity_Models[line].model_ID)
	    {
	      free(synapses[pop_pre][pop_pos].plasticity_dynamics.parameter_label); //free memory to alocate a different model
	      free(synapses[pop_pre][pop_pos].plasticity_dynamics.parameter);
	      synapses[pop_pre][pop_pos].plasticity_dynamics.parameter=(double*) malloc(total_par * sizeof(double));
	      synapses[pop_pre][pop_pos].plasticity_dynamics.parameter_label=(char**) malloc(total_par * sizeof(char*));
	      
	      if (synapses[pop_pre][pop_pos].plasticity_dynamics.parameter_label == NULL || synapses[pop_pre][pop_pos].plasticity_dynamics.parameter == NULL) {
		  printf("\n\tERROR: unable to allocate memory for list of SYNAPSES");
		  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		  exit(-1);
		  Free_Memory();
	      }
		
	      free(synapses[pop_pre][pop_pos].plasticity_dynamics.state_vars_label); //free memory to alocate a different model
	      free(synapses[pop_pre][pop_pos].plasticity_dynamics.state_vars_val);
	      synapses[pop_pre][pop_pos].plasticity_dynamics.state_vars_val=(double*) malloc(Plasticity_Models[line].n_state_vars * sizeof(double));
	      synapses[pop_pre][pop_pos].plasticity_dynamics.state_vars_label=(char**) malloc(Plasticity_Models[line].n_state_vars * sizeof(char*));
	      if (synapses[pop_pre][pop_pos].plasticity_dynamics.state_vars_label == NULL || synapses[pop_pre][pop_pos].plasticity_dynamics.state_vars_val==NULL) {
		   printf("\n\tERROR: unable to allocate memory for list of SYNAPSES");
		   printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		   exit(-1);
		   Free_Memory();
	      }
		 
	      synapses[pop_pre][pop_pos].plasticity_dynamics.model_ID=Plasticity_Models[line].model_ID;
	      str_equal(Plasticity_Models[line].model_label,synapses[pop_pre][pop_pos].plasticity_dynamics.model_label);
	      synapses[pop_pre][pop_pos].plasticity_dynamics.model_total_parameters=total_par;
	      synapses[pop_pre][pop_pos].plasticity_dynamics.n_state_vars=Plasticity_Models[line].n_state_vars;
	      model_grid->SetColLabelValue(0,wxString(Plasticity_Models[line].model_label,wxConvUTF8));
	      for (int j = 0; j < total_par; j++){
		  synapses[pop_pre][pop_pos].plasticity_dynamics.parameter_label[j]=(char*) malloc(MAXLABELSIZE * sizeof(char));
		  if (synapses[pop_pre][pop_pos].plasticity_dynamics.parameter_label[j] == NULL) 
		    {
		      printf("\n\tERROR: unable to allocate memory for list of SYNAPSES");
		      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		      exit(-1);
		      Free_Memory();
		    }
		  model_grid->SetRowLabelValue(j,wxString(Plasticity_Models[line].parameter_label[j],wxConvUTF8));
		  model_grid->SetCellValue(j,0,dbl2wxStr(Plasticity_Models[line].parameter[j]));
		  model_grid->SetRowAttr(j,attr);
	      }
	      for (int var=0; var<synapses[pop_pre][pop_pos].plasticity_dynamics.n_state_vars;var++){
		  synapses[pop_pre][pop_pos].plasticity_dynamics.state_vars_label[var]=(char*) malloc(MAXLABELSIZE * sizeof(char));
		  synapses[pop_pre][pop_pos].plasticity_dynamics.state_vars_val[var]=Plasticity_Models[line].state_vars_val[var];
		  str_equal(Plasticity_Models[line].state_vars_label[var],synapses[pop_pre][pop_pos].plasticity_dynamics.state_vars_label[var]);
	      }
	    }
	   else
	    {
	      model_grid->SetColLabelValue(0,wxString(synapses[pop_pre][pop_pos].plasticity_dynamics.model_label,wxConvUTF8));
	      for (int j = 0; j < total_par; j++)
		{
		  model_grid->SetRowLabelValue(j,wxString(synapses[pop_pre][pop_pos].plasticity_dynamics.parameter_label[j],wxConvUTF8));
		  model_grid->SetCellValue(j,0,dbl2wxStr(synapses[pop_pre][pop_pos].plasticity_dynamics.parameter[j]));
		  model_grid->SetRowAttr(j,attr);
		}
	    }
      break;
  }
GetSizer()->SetSizeHints(this);

}
void NetBuilderModel_frame::Onbtn_Model_OKClick( wxCommandEvent& event )
{
  int error=0;
  Dynamics_frame* myparent1=(Dynamics_frame *) GetParent();
  Conductances_frame* myparent2=(Conductances_frame *) GetParent();
  Plasticity_frame* myparent3=(Plasticity_frame *) GetParent(); 

 switch (model_table)
  {
    case 1: // Dynamic model for fdomains population
	
	myparent1->Enable(true);
	for (int j = 0; j < total_par; j++)
	  {
	    if (!(model_grid->GetCellValue(j,0)).ToDouble(&population[pop_selected].fdomains[fdms_selected].dynamics.parameter[j])){
		model_grid->SetCellTextColour(j,0,*wxRED);
		model_grid->Refresh();
		error=-1;
	    }
		
	    wxStr2str(model_grid->GetRowLabelValue(j),population[pop_selected].fdomains[fdms_selected].dynamics.parameter_label[j]);
	  }
	break;
 
    case 2: //Dynamic model for synaptic conductances 
	  
	myparent2->Enable(true);
	  for (int j = 0; j < total_par; j++)
	    {
	      if (!(model_grid->GetCellValue(j,0)).ToDouble(&synapses[pop_pre][pop_pos].conductances_dynamics.parameter[j])){;
		model_grid->SetCellTextColour(j,0,*wxRED);
		model_grid->Refresh();
		error=-1;
	      }
	       wxStr2str(model_grid->GetRowLabelValue(j),synapses[pop_pre][pop_pos].conductances_dynamics.parameter_label[j]);
	    }
	  
	  break;

    case 3: // Dynamic model for synaptic plasticity 
	 
	 myparent3->Enable(true);
	  for (int j = 0; j < total_par; j++)
	    {
	      if(!(model_grid->GetCellValue(j,0)).ToDouble(&synapses[pop_pre][pop_pos].plasticity_dynamics.parameter[j])){
		model_grid->SetCellTextColour(j,0,*wxRED);
		model_grid->Refresh();
		error=-1;
	      }
	      wxStr2str(model_grid->GetRowLabelValue(j),synapses[pop_pre][pop_pos].plasticity_dynamics.parameter_label[j]);
	    }
      break;
  }

  if (error==0)
    this->Destroy();
}
