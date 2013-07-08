#include "data_structures.h"
#include <gsl/gsl_sort_long.h>



extern struct POPULATION *population;
extern struct DYNAMICS *Neuron_Models;
extern unsigned int total_neuron_models;
extern struct DYNAMICS *Conductances_Models;
extern unsigned int total_syn_models;
extern struct DYNAMICS *Plasticity_Models;
extern unsigned int total_plas_models;
extern struct SYNAPSES **synapses;
extern int **conn_values;
extern int **fdms_values;
extern double **delays_values;
extern double ***dist_values;
extern int ***RANDOM_DIST;


/// useful functions for convertions
///convert wxString to string
char* wxStr2str(wxString wxstr,char *str)
{
	strncpy(str,wxstr.mb_str(),MAXLABELSIZE);
	str[MAXLABELSIZE-1]='\0';
	return str;
}

///convert integers to wxString
wxString int2wxStr(int myint)
{
 wxString wxstr;
 wxstr=wxString::Format(wxT("%i"),myint);
 return wxstr;
}

///convert longs to wxString
wxString long2wxStr(long myint)
{
 wxString wxstr;
 wxstr=wxString::Format(wxT("%li"),myint);
 return wxstr;
}

///convert doubles to wxString
wxString dbl2wxStr(double mydbl)
{
 wxString wxstr;
 wxstr=wxString::Format(wxT("%lg"),mydbl);
 return wxstr;
}

///convert floats to wxString
wxString flt2wxStr(double myflt)
{
 wxString wxstr;
 wxstr=wxString::Format(wxT("%.1e"),myflt);
 return wxstr;
}

///operator = between two strings
char* str_equal(const char* strin, char* strout)
{
  strncpy(strout,strin,MAXLABELSIZE);
  strout[MAXLABELSIZE-1]='\0';
  return strout;
} 

double Clip_Dec_Cases(double x){
    int temp;
    temp = (int) round(x * 10.0);
    return temp/10.0;
}

void Free_Memory(void)
{
    free(Neuron_Models);
    free(Conductances_Models);
    free(Plasticity_Models);
    free(population);
    free(synapses);
    free(conn_values);
    free(fdms_values);
    free(dist_values);
    free(delays_values);

    printf("\n\nAll memory has been released.\n");
}

void ResizeGrid(wxSize panelsz, wxGrid *grid, int cols, int rows)
{
    int leftWidth = panelsz.GetWidth() - (grid->GetRowLabelSize() + cols * grid->GetColSize(0)) -20;
    int totalWidth = grid->GetRowLabelSize() + cols * grid->GetColSize(0) + leftWidth;
    int colWidth = (int)(totalWidth - grid->GetRowLabelSize())/cols;

    int leftH = panelsz.GetHeight() - (grid->GetColLabelSize() + rows * grid->GetRowSize(0)) -20;
    int totalH = grid->GetColLabelSize() + rows * grid->GetRowSize(0) + leftH;
    int colH = (int)(totalH - grid->GetColLabelSize())/rows;

    for (int col=0; col<cols;col++){
	grid->SetColSize(col,colWidth);
    }

    for (int row=0; row<rows;row++){
	grid->SetRowSize(row,colH);
    }
}
int Read_Neuron_Models(void){

    FILE *data_file;
    char teste_error[MAXLABELSIZE];
    char garbage[20];
		int foo;
    char *HOME_DIR, filedirectory[128];

    //get home directory; NeuralSyns should be installed in the HOME directory!
    HOME_DIR = getenv("HOME");
    sprintf( filedirectory,"%s%s", HOME_DIR, "/NeuralSyns/dynamics/Neuron_Models.lib");

    printf("\nReading Dynamic Models....");

    ///Initialize Neuron Models
    data_file=fopen(filedirectory, "r");
    if (data_file==NULL) {
      printf("\nYou need to have a valid Neuron_Models.lib file. This file must contain at least one model for neuron dynamics.\n\n");
      exit(-1);
      Free_Memory();
    }

    foo = fscanf(data_file,"total models = %d",&total_neuron_models);
    if (total_neuron_models<1){
      printf("\nYou need to define at least one model in your Neuron_Models file.\n\n");
      exit(-1);
      Free_Memory();
    }

    Neuron_Models=(DYNAMICS*) malloc(total_neuron_models * sizeof(struct DYNAMICS));
    char evt[10];
    printf("\n\nNeuron Models:");
    for (unsigned int m=0;m<total_neuron_models;m++){
      Neuron_Models[m].model_ID = m;
      foo = fscanf(data_file,"\n\n%s",Neuron_Models[m].model_label);
      printf("\n%s", Neuron_Models[m].model_label);
      foo = fscanf(data_file,"\n\t%s = %s",garbage, evt);
      
      if(strcasecmp(evt,"no")==0){
	Neuron_Models[m].evt_gen=0;
      }
      else if(strcasecmp(evt,"yes")==0){
	Neuron_Models[m].evt_gen=1;
      }
      else{
	 printf("\nYou need to define correctly if SPIKE = yes or no, in your Neuron_Models file.\n\n");
	 exit(-1);
	 Free_Memory();
      }
	
      
      foo = fscanf(data_file,"\n\tPARAM = %d:", &Neuron_Models[m].model_total_parameters);
      Neuron_Models[m].parameter=(double*) malloc(Neuron_Models[m].model_total_parameters * sizeof(double));
      Neuron_Models[m].parameter_label=(char** ) malloc(Neuron_Models[m].model_total_parameters * sizeof(char*));
      for (int p=0;p<Neuron_Models[m].model_total_parameters;p++){
	  Neuron_Models[m].parameter_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
	  foo = fscanf(data_file,"\n\t\t%s = %lf",Neuron_Models[m].parameter_label[p],&Neuron_Models[m].parameter[p]);
      }
      foo = fscanf(data_file,"\n\tSTATE = %d:", &Neuron_Models[m].n_state_vars);
      Neuron_Models[m].state_vars_val=(double* ) malloc(Neuron_Models[m].n_state_vars * sizeof(double));
      Neuron_Models[m].state_vars_label=(char** ) malloc(Neuron_Models[m].n_state_vars * sizeof(char*));
      for (int var=0;var<Neuron_Models[m].n_state_vars;var++){
	  Neuron_Models[m].state_vars_label[var]=(char*) malloc(MAXLABELSIZE * sizeof(char));
	  foo = fscanf(data_file,"\n\t\t%s = %lf", Neuron_Models[m].state_vars_label[var],  &Neuron_Models[m].state_vars_val[var]);
      }
     }
    // check if the number of models is correct in Models file
    if(fscanf(data_file,"\n%s",teste_error)!=EOF){
      printf("\nThe number of models must be equal to the variable total_models, in your Neuron_Models file.\n\n");
      exit(-1);
      Free_Memory();
    }
    fclose(data_file);
    printf("\n");
    fflush(stdout);
    return 0;
}

int Read_Synaptic_Models(void){
    
    FILE *data_file;
		int foo;
    char teste_error[MAXLABELSIZE];
    char *HOME_DIR, filedirectory[128];

    //get home directory; NeuralSyns should be installed in the HOME directory!
    HOME_DIR = getenv("HOME");
    sprintf( filedirectory,"%s%s", HOME_DIR, "/NeuralSyns/dynamics/Synapse_Models.lib");
    
    ///Initialize Conductances Models
    data_file=fopen(filedirectory, "r");

    if (data_file==NULL) {
	printf("\nYou need to have a valid Conductances_Models.lib file. This file must contain at least one model for neuron dynamics.\n\n");
	exit(-1);
	Free_Memory();
    }

    foo = fscanf(data_file,"total models = %d",&total_syn_models);
    if (total_syn_models<1){
	printf("\nYou need to define at least one model in your Conductances_Models file. \n\n");
	exit(-1);
	Free_Memory();
    }
    
    
    Conductances_Models=(DYNAMICS*) malloc(total_syn_models * sizeof(struct DYNAMICS));
    printf("\nConductances Models:");
    for (unsigned int m=0;m<total_syn_models;m++){
      Conductances_Models[m].model_ID = m;
      foo = fscanf(data_file,"\n\n%s",Conductances_Models[m].model_label);
      printf("\n%s", Conductances_Models[m].model_label);
      foo = fscanf(data_file,"\n\tPARAM = %d:", &Conductances_Models[m].model_total_parameters);
      Conductances_Models[m].parameter=(double*) malloc(Conductances_Models[m].model_total_parameters * sizeof(double));
      Conductances_Models[m].parameter_label=(char**) malloc(Conductances_Models[m].model_total_parameters * sizeof(char*));
      for (int p=0;p<Conductances_Models[m].model_total_parameters;p++)
	{
	  Conductances_Models[m].parameter_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
	  foo = fscanf(data_file,"\n\t\t%s = %lf",Conductances_Models[m].parameter_label[p],&Conductances_Models[m].parameter[p]);
	}

	foo = fscanf(data_file,"\n\tSTATE = %d:", &Conductances_Models[m].n_state_vars);
	Conductances_Models[m].state_vars_val=(double* ) malloc(Conductances_Models[m].n_state_vars * sizeof(double));
	Conductances_Models[m].state_vars_label=(char** ) malloc(Conductances_Models[m].n_state_vars * sizeof(char*));
	for (int var=0;var<Conductances_Models[m].n_state_vars;var++){
	    Conductances_Models[m].state_vars_label[var]=(char*) malloc(MAXLABELSIZE * sizeof(char));
	    foo = fscanf(data_file,"\n\t\t%s = %lf", Conductances_Models[m].state_vars_label[var], &Conductances_Models[m].state_vars_val[var]);
	}
    }
          // check if the number of models is correct in Models file
    if(fscanf(data_file,"\n%s",teste_error)!=EOF){
      printf("\nThe number of models must be equal to the variable total_models, in your Conductances_Models file.\n\n");
      exit(-1);
      Free_Memory();
    }

    fclose(data_file);
    printf("\n");
    fflush(stdout);
    return 0;
}

///Initialize the structures Dynamics reading values from file
int Read_Plasticity_Models(void)
{
    FILE *data_file;
		int foo;
    char teste_error[MAXLABELSIZE];
    char *HOME_DIR, filedirectory[128];

    //get home directory; NeuralSyns should be installed in the HOME directory!
    HOME_DIR = getenv("HOME");
    sprintf( filedirectory,"%s%s", HOME_DIR, "/NeuralSyns/dynamics/Plasticity_Models.lib"); 

    ///Initialize Plasticity Models
    data_file=fopen(filedirectory, "r");

    if (data_file==NULL) {
      printf("\nYou need to have a valid Plasticity_Models.lib file. This file must contain at least one model for neuron dynamics.\n\n");
      exit(-1);
      Free_Memory();
    }
    //TODO

    foo = fscanf(data_file,"total models = %d",&total_plas_models);
    if (total_plas_models<1){
	printf("\nYou need to define at least one model in your Plasticity_Models file.\n\n");
	exit(-1);
	Free_Memory();
    }

    Plasticity_Models=(DYNAMICS*) malloc(total_plas_models * sizeof(struct DYNAMICS));
    printf("\nPlasticity Models:");
    for (unsigned int m=0;m<total_plas_models;m++){
      Plasticity_Models[m].model_ID = m;
      foo = fscanf(data_file,"\n\n%s",Plasticity_Models[m].model_label);
      printf("\n%s", Plasticity_Models[m].model_label);
      foo = fscanf(data_file,"\n\tPARAM = %d:", &Plasticity_Models[m].model_total_parameters);
      Plasticity_Models[m].parameter=(double*) malloc(Plasticity_Models[m].model_total_parameters * sizeof(double));
      Plasticity_Models[m].parameter_label=(char**) malloc(Plasticity_Models[m].model_total_parameters * sizeof(char*));
      for (int p=0;p<Plasticity_Models[m].model_total_parameters;p++){
	  Plasticity_Models[m].parameter_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
	  foo = fscanf(data_file,"\n\t\t%s = %lf",Plasticity_Models[m].parameter_label[p],&Plasticity_Models[m].parameter[p]);
      }
      foo = fscanf(data_file,"\n\tSTATE = %d:", &Plasticity_Models[m].n_state_vars);
      Plasticity_Models[m].state_vars_val=(double* ) malloc(Plasticity_Models[m].n_state_vars * sizeof(double));
      Plasticity_Models[m].state_vars_label=(char** ) malloc(Plasticity_Models[m].n_state_vars * sizeof(char*));
      for (int var=0;var<Plasticity_Models[m].n_state_vars;var++){
	    Plasticity_Models[m].state_vars_label[var]=(char*) malloc(MAXLABELSIZE * sizeof(char));
	    foo = fscanf(data_file,"\n\t\t%s = %lf", Plasticity_Models[m].state_vars_label[var], &Plasticity_Models[m].state_vars_val[var]);
      }

    }
    // check if the number of models is correct in Models file
    if(fscanf(data_file,"\n%s",teste_error)!=EOF){
      printf("\nThe number of models must be equal to the variable total_models, in your Plasticity_Models file.\n\n");
      exit(-1);
      Free_Memory();
    }

    fclose(data_file);
    printf("\n");
    fflush(stdout);
    return 0;
}

int Test_Reading_Models(void){
 
     if (total_neuron_models<1){
      printf("\nYou need to define at least one model in your Neuron_Models file.\n\n");
      exit(-1);
      Free_Memory();
     }
     int k=0;
     int t=0;
     for (unsigned int m=0;m<total_neuron_models;++m){
	 if (Neuron_Models[m].evt_gen==0){
	    k=k+1;
	 }
	 else{
	    t=t+1;
	 }
     }
     if (k==0 || t==0){
        printf("\nYou need to define, in your Neuron_Models file, at least one model that generate spikes and one model that does not generate spikes.\n\n");
	exit(-1);
	Free_Memory();
     }
     return 0;
}


///Initialize all the elements from structure POPULATION
void Initialize_Populations(int total_populations){    
  char ind_label[MAXLABELSIZE];
  
  population=(POPULATION*) malloc(total_populations*sizeof(struct POPULATION));
  if (population == NULL) {
	printf("\n\tERROR: unable to allocate memory for list of POPULATIONS");
	printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	exit(-1);
	Free_Memory();
  }
  
  for (int pop = 0; pop < total_populations; pop++){
	if (pop<10)
	  snprintf(ind_label,MAXLABELSIZE,"pop0%d",pop);
	else
	  snprintf(ind_label,MAXLABELSIZE,"pop%d",pop);
	
	str_equal(ind_label,population[pop].label);
	population[pop].total_fdomains= 1;
	population[pop].topology.dim_x = 1;
	population[pop].topology.dim_y = 1;
	population[pop].topology.dim_z = 1;
	population[pop].topology.d_mean = 100.0;
	population[pop].topology.d_std = 0.0;
	population[pop].topology.X0 = 100*pop;
	population[pop].topology.Y0 = 0.0;
	population[pop].topology.Z0 = 0.0;


	
	population[pop].fdomains=(FDOMAINS*) malloc(1*sizeof(struct FDOMAINS));
	if (population[pop].fdomains == NULL){
	  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
	  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	  exit(-1);
	  Free_Memory();
	}
	
	population[pop].fdomains[0].fdm_ID=0;
	snprintf(ind_label,MAXLABELSIZE,"soma");
	str_equal(ind_label,population[pop].fdomains[0].label);
	population[pop].fdomains[0].x=0.0;
	population[pop].fdomains[0].y=0.0;
	population[pop].fdomains[0].z=0.0;
	population[pop].fdomains[0].r=10.0;
	population[pop].fdomains[0].Ra=10;
	str_equal("",population[pop].fdomains[0].parent_label);
	population[pop].fdomains[0].parent_ID=-1;
	str_equal(Neuron_Models[0].model_label,population[pop].fdomains[0].dynamics.model_label);
	
	
	for (unsigned int m=0; m<total_neuron_models; m++){
	    if (Neuron_Models[m].evt_gen==1){//select only a model that generates events
	   	population[pop].fdomains[0].dynamics.model_ID=m;
		population[pop].fdomains[0].dynamics.model_total_parameters=Neuron_Models[m].model_total_parameters;
		population[pop].fdomains[0].dynamics.parameter=(double*) malloc(Neuron_Models[m].model_total_parameters*sizeof(double));
		population[pop].fdomains[0].dynamics.parameter_label=(char**) malloc(Neuron_Models[m].model_total_parameters*sizeof(char*));
		if (population[pop].fdomains[0].dynamics.parameter == NULL || population[pop].fdomains[m].dynamics.parameter_label==NULL) {
		  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		  exit(-1);
		  Free_Memory();
		}
		
		for (int p=0;p<Neuron_Models[m].model_total_parameters;p++) {
		  population[pop].fdomains[0].dynamics.parameter_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
		  if (population[pop].fdomains[0].dynamics.parameter_label[p] == NULL) {
			printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			exit(-1);
			Free_Memory();
		  }
		  str_equal(Neuron_Models[m].parameter_label[p],population[pop].fdomains[0].dynamics.parameter_label[p]);
		  population[pop].fdomains[0].dynamics.parameter[p]= Neuron_Models[m].parameter[p];
		}
		population[pop].fdomains[0].dynamics.n_state_vars=Neuron_Models[m].n_state_vars;
		population[pop].fdomains[0].dynamics.state_vars_val=(double*) malloc(Neuron_Models[m].n_state_vars*sizeof(double));
		population[pop].fdomains[0].dynamics.state_vars_label=(char**) malloc(Neuron_Models[m].n_state_vars*sizeof(char*));
		if (population[pop].fdomains[0].dynamics.state_vars_label==NULL || population[pop].fdomains[0].dynamics.state_vars_val==NULL) {
		  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		  exit(-1);
		  Free_Memory();
		}
		
		for (int p=0;p<Neuron_Models[m].n_state_vars;p++) {
		  population[pop].fdomains[0].dynamics.state_vars_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
		  if (population[pop].fdomains[0].dynamics.state_vars_label[p] == NULL) {
			printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			exit(-1);
			Free_Memory();
		  }
		    str_equal(Neuron_Models[m].state_vars_label[p],population[pop].fdomains[0].dynamics.state_vars_label[p]);
		    population[pop].fdomains[0].dynamics.state_vars_val[p]=Neuron_Models[m].state_vars_val[p];
		}
		break;
	    }
	}
  }
  
  /// connectivity matrix
  conn_values=(int**) malloc(total_populations * sizeof(int*));
  if (conn_values== NULL) 	{
	printf("\n\tERROR: unable to allocate memory for matrix Connectivity");
	printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	exit(-1);
	Free_Memory();
  }
  for (int popi = 0; popi < total_populations; popi++) {
	conn_values[popi]=(int*) malloc(total_populations * sizeof(int));
	if (conn_values[popi]== NULL) {
	  printf("\n\tERROR: unable to allocate memory for matrix Connectivity");
	  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	  exit(-1);
	  Free_Memory();
	}
	for (int popj=0; popj<total_populations; popj++){
	  conn_values[popi][popj]=0;
	}
  }
  
  /// target fdms matrix
  
    fdms_values=(int**) malloc(total_populations * sizeof(int*));
    if (fdms_values== NULL) 	{
	  printf("\n\tERROR: unable to allocate memory for matrix FDMS");
	  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	  exit(-1);
	  Free_Memory();
    }
    for (int popi = 0; popi < total_populations; popi++) {
	  fdms_values[popi]=(int*) malloc(total_populations * sizeof(int));
	  if (fdms_values[popi]== NULL)  {
	    printf("\n\tERROR: unable to allocate memory for matrix FDMS");
	    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	    exit(-1);
	    Free_Memory();
	  }
	  for (int popj=0; popj<total_populations; popj++){
	    fdms_values[popi][popj]=0;
	  }
    }
  

 /// distribution matrix
  RANDOM_DIST=(int***) malloc(total_populations * sizeof(int**));
  for (int pop = 0; pop < total_populations; pop++) {
	RANDOM_DIST[pop]=(int**) malloc(total_populations * sizeof(int*));
  }
  for (int popi = 0; popi < total_populations; popi++) {
	for (int popj=0; popj<total_populations; popj++){
	  RANDOM_DIST[popi][popj]=(int*) malloc(3 * sizeof(int));
	  for (int k=0; k<3; k++){
		RANDOM_DIST[popi][popj][k]=0;
	  }
	}
  }
  
  dist_values=(double***) malloc(total_populations * sizeof(double**));
  if (dist_values== NULL) {
	printf("\n\tERROR: unable to allocate memory for matrix Distributions");
	printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	exit(-1);
	Free_Memory();
  }
  for (int popi = 0; popi < total_populations; popi++) {
	dist_values[popi]=(double**) malloc(total_populations * sizeof(double*));
	if (dist_values[popi]== NULL)   {
	  printf("\n\tERROR: unable to allocate memory for matrix Distributions");
	  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	  exit(-1);
	  Free_Memory();
	}
  }
  for (int popi = 0; popi < total_populations; popi++) {
	for (int popj=0; popj<total_populations; popj++){
	  dist_values[popi][popj]= (double*) malloc(3 * sizeof(double));
	  if (dist_values[popi][popj]== NULL) {
		printf("\n\tERROR: unable to allocate memory for matrix Distributions");
		printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		exit(-1);
		Free_Memory();
	  }
	  for (int k=0; k<3; k++){
		dist_values[popi][popj][k]=1000.0;
	  }
	}
  }
  
  ///delays matrix
  delays_values=(double**) malloc(total_populations * sizeof(double*));
    if (delays_values== NULL) 	{
	  printf("\n\tERROR: unable to allocate memory for matrix Delays");
	  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	  exit(-1);
	  Free_Memory();
    }
    for (int popi = 0; popi < total_populations; popi++) {
	  delays_values[popi]=(double*) malloc(total_populations * sizeof(double));
	  if (delays_values[popi]== NULL)  {
	    printf("\n\tERROR: unable to allocate memory for matrix Delays");
	    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	    exit(-1);
	    Free_Memory();
	  }
	  for (int popj=0; popj<total_populations; popj++){
	    delays_values[popi][popj]=5.0;
	  }
    }
  
  
  /// synaptic struture 
  
  synapses=(struct SYNAPSES**) malloc(total_populations*sizeof(struct SYNAPSES*));
  if (synapses== NULL)	{
	printf("\n\tERROR: unable to allocate memory for matrix Synapses");
	printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	exit(-1);
	Free_Memory();
  }
  for (int pop = 0; pop < total_populations; pop++) {
	synapses[pop]=(struct SYNAPSES*) malloc(total_populations*sizeof(struct SYNAPSES));
	if (synapses[pop]== NULL) {
	  printf("\n\tERROR: unable to allocate memory for matrix Synapses");
	  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	  exit(-1);
	  Free_Memory();
	}
  }
  
  int aux=0;
  for (unsigned int i=0; i<total_syn_models; i++){
      if (strcasecmp(Conductances_Models[i].model_label,"ExpSyn")==0)
	  aux=i;
  }
  
  int aux1=0;
  for (unsigned int i=0; i<total_plas_models; i++){
      if (strcasecmp(Plasticity_Models[i].model_label,"Static")==0)
	  aux1=i;
  }
  ///initialize synaptic conductances
  for (int popi = 0; popi < total_populations; popi++) {
	  for (int popj = 0; popj < total_populations; popj++) 	{
	      synapses[popi][popj].conductances_dynamics.model_ID=Conductances_Models[aux].model_ID;
	      str_equal(Conductances_Models[aux].model_label,synapses[popi][popj].conductances_dynamics.model_label);
	      synapses[popi][popj].conductances_dynamics.model_total_parameters=Conductances_Models[aux].model_total_parameters;
	      synapses[popi][popj].conductances_dynamics.parameter=(double*) malloc(Conductances_Models[aux].model_total_parameters*sizeof(double));
	      synapses[popi][popj].conductances_dynamics.parameter_label=(char**) malloc(Conductances_Models[aux].model_total_parameters*sizeof(char*));
	      if (synapses[popi][popj].conductances_dynamics.parameter == NULL || synapses[popi][popj].conductances_dynamics.parameter_label==NULL)  {
		  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		  exit(-1);
		  Free_Memory();
	      }
	      for (int p=0;p<Conductances_Models[aux].model_total_parameters;p++){
		  synapses[popi][popj].conductances_dynamics.parameter_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
		  if ( synapses[popi][popj].conductances_dynamics.parameter_label[p] == NULL) {
			printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			exit(-1);
			Free_Memory();
		  }
		  str_equal(Conductances_Models[aux].parameter_label[p], synapses[popi][popj].conductances_dynamics.parameter_label[p]);
		  synapses[popi][popj].conductances_dynamics.parameter[p]= Conductances_Models[aux].parameter[p];
	      }
	      
	      synapses[popi][popj].conductances_dynamics.n_state_vars=Conductances_Models[aux].n_state_vars;
	      synapses[popi][popj].conductances_dynamics.state_vars_val=(double*) malloc(Conductances_Models[aux].n_state_vars*sizeof(double));
	      synapses[popi][popj].conductances_dynamics.state_vars_label=(char**) malloc(Conductances_Models[aux].n_state_vars*sizeof(char*));
	      if (synapses[popi][popj].conductances_dynamics.state_vars_label==NULL || synapses[popi][popj].conductances_dynamics.state_vars_val==NULL) {
		printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		exit(-1);
		Free_Memory();
	      }
	      
	      for (int p=0;p<Conductances_Models[aux].n_state_vars;p++) {
		synapses[popi][popj].conductances_dynamics.state_vars_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
		if (synapses[popi][popj].conductances_dynamics.state_vars_label[p] == NULL) {
		      printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		      exit(-1);
		      Free_Memory();
		}
		str_equal(Conductances_Models[aux].state_vars_label[p],synapses[popi][popj].conductances_dynamics.state_vars_label[p]);
		synapses[popi][popj].conductances_dynamics.state_vars_val[p]=Conductances_Models[aux].state_vars_val[p];
	      }
	      
		///initialize synaptic plasticity
		
	      synapses[popi][popj].plasticity_dynamics.model_ID = Plasticity_Models[aux1].model_ID;
	      str_equal(Plasticity_Models[aux1].model_label,synapses[popi][popj].plasticity_dynamics.model_label);
	      synapses[popi][popj].plasticity_dynamics.model_total_parameters=Plasticity_Models[aux1].model_total_parameters;
	      synapses[popi][popj].plasticity_dynamics.parameter=(double*) malloc(Plasticity_Models[aux1].model_total_parameters*sizeof(double));
	      synapses[popi][popj].plasticity_dynamics.parameter_label=(char**) malloc(Plasticity_Models[aux1].model_total_parameters*sizeof(char*));
	      if (synapses[popi][popj].plasticity_dynamics.parameter == NULL || synapses[popi][popj].plasticity_dynamics.parameter_label==NULL)  {
		    printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		exit(-1);
		Free_Memory();
	      }
	      for (int p=0;p<Plasticity_Models[aux1].model_total_parameters;p++){
		    synapses[popi][popj].plasticity_dynamics.parameter_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
		    if ( synapses[popi][popj].plasticity_dynamics.parameter_label[p] == NULL) {
		      printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		      exit(-1);
		      Free_Memory();
		    }
		    str_equal(Plasticity_Models[aux1].parameter_label[p], synapses[popi][popj].plasticity_dynamics.parameter_label[p]);
		    synapses[popi][popj].plasticity_dynamics.parameter[p]= Plasticity_Models[aux1].parameter[p];
	      }
	      synapses[popi][popj].plasticity_dynamics.n_state_vars = Plasticity_Models[aux1].n_state_vars;
	      synapses[popi][popj].plasticity_dynamics.state_vars_val=(double*) malloc(Plasticity_Models[aux1].n_state_vars*sizeof(double));
	      synapses[popi][popj].plasticity_dynamics.state_vars_label=(char**) malloc(Plasticity_Models[aux1].n_state_vars*sizeof(char*));
	      if (synapses[popi][popj].plasticity_dynamics.state_vars_label==NULL || synapses[popi][popj].plasticity_dynamics.state_vars_val==NULL) {
		printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
		printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		exit(-1);
		Free_Memory();
	      }
	      
	      for (int p=0;p<Plasticity_Models[aux1].n_state_vars;p++) {
		  synapses[popi][popj].plasticity_dynamics.state_vars_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
		  if (synapses[popi][popj].plasticity_dynamics.state_vars_label[p] == NULL) {
			printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			exit(-1);
			Free_Memory();
		  }
		  str_equal(Plasticity_Models[aux1].state_vars_label[p],synapses[popi][popj].plasticity_dynamics.state_vars_label[p]);
		  synapses[popi][popj].plasticity_dynamics.state_vars_label[p]=Plasticity_Models[aux1].state_vars_label[p];
	      }
	  }
  }
}

///Initialize all the elements from structure FDOMAINS 
void Initialize_Fdomains(int pop,int fdms){
  char ind_label[MAXLABELSIZE];
  int old_n_fdomains;
  old_n_fdomains=population[pop].total_fdomains;

 
 
  if (old_n_fdomains<fdms){
    
	population[pop].fdomains=(FDOMAINS*) realloc(population[pop].fdomains,fdms*sizeof(struct FDOMAINS));
	for (int fdm = old_n_fdomains; fdm < fdms; fdm++){
	  
	    population[pop].fdomains[fdm].fdm_ID=fdm;
	    if (fdm<10)
	      snprintf(ind_label,MAXLABELSIZE,"fdm0%d",fdm);
	    else
	      snprintf(ind_label,MAXLABELSIZE,"fdm%d",fdm);
	    
	    str_equal(ind_label,population[pop].fdomains[fdm].label);
	    population[pop].fdomains[fdm].fdm_ID = fdm;
	    float teta = (fdm-old_n_fdomains)*2.0*Pi/(fdms-1)-Pi/2.0;
	    population[pop].fdomains[fdm].x=Clip_Dec_Cases(50.0*cos(teta));
	    population[pop].fdomains[fdm].y=Clip_Dec_Cases(50.0*sin(teta));
	    population[pop].fdomains[fdm].z=50.0*0;
	    population[pop].fdomains[fdm].r=3.0;
	    population[pop].fdomains[fdm].Ra=10;
    
            str_equal(population[pop].fdomains[0].label,population[pop].fdomains[fdm].parent_label);
	    population[pop].fdomains[fdm].parent_ID=0;
	    
	    for (unsigned int m=0; m<total_neuron_models; m++){
		if (Neuron_Models[m].evt_gen==0){//select only a model that generates events  
		    str_equal(Neuron_Models[m].model_label,population[pop].fdomains[fdm].dynamics.model_label);
		    population[pop].fdomains[fdm].dynamics.model_ID=m;
		    population[pop].fdomains[fdm].dynamics.model_total_parameters=Neuron_Models[m].model_total_parameters;
		    population[pop].fdomains[fdm].dynamics.parameter=(double*) malloc(Neuron_Models[m].model_total_parameters*sizeof(double));
		    population[pop].fdomains[fdm].dynamics.parameter_label=(char**) malloc(Neuron_Models[m].model_total_parameters*sizeof(char*));
		    if (population[pop].fdomains[fdm].dynamics.parameter == NULL || population[pop].fdomains[fdm].dynamics.parameter_label==NULL){
			  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			  exit(-1);
			  Free_Memory();
		    }
		    for (int p=0;p<Neuron_Models[m].model_total_parameters;p++){
			population[pop].fdomains[fdm].dynamics.parameter_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
			if (population[pop].fdomains[fdm].dynamics.parameter_label[p] == NULL) {
			  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			  exit(-1);
			  Free_Memory();
			}
			str_equal(Neuron_Models[m].parameter_label[p],population[pop].fdomains[fdm].dynamics.parameter_label[p]);
			population[pop].fdomains[fdm].dynamics.parameter[p]= Neuron_Models[m].parameter[p];
		    }
		    population[pop].fdomains[fdm].dynamics.n_state_vars=Neuron_Models[m].n_state_vars;
		    population[pop].fdomains[fdm].dynamics.state_vars_val=(double*) malloc(Neuron_Models[m].n_state_vars*sizeof(double));
		    population[pop].fdomains[fdm].dynamics.state_vars_label=(char**) malloc(Neuron_Models[m].n_state_vars*sizeof(char*));
		    if (population[pop].fdomains[fdm].dynamics.state_vars_label==NULL || population[pop].fdomains[fdm].dynamics.state_vars_val==NULL) {
			  printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			  printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			  exit(-1);
			  Free_Memory();
		    }

		    for (int p=0;p<Neuron_Models[m].n_state_vars;p++){
			  population[pop].fdomains[fdm].dynamics.state_vars_label[p]=(char*) malloc(MAXLABELSIZE * sizeof(char));
			  if (population[pop].fdomains[fdm].dynamics.state_vars_label[p] == NULL){
			    printf("\n\tERROR: unable to allocate memory for list of FDOMAINS");
			    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			    exit(-1);
			    Free_Memory();
			  }
			  str_equal(Neuron_Models[m].state_vars_label[p],population[pop].fdomains[fdm].dynamics.state_vars_label[p]);
			  population[pop].fdomains[fdm].dynamics.state_vars_val[p]=Neuron_Models[m].state_vars_val[p];
		    }
		    break;
	    }
	}
      }
    }
}
