/** @brief Functions to create the network and store it in a .net file
    
    @author Sousa, M.
    @date October 2009
*/

#include <time.h>
#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h> 
#include <gsl/gsl_permute.h>

#include "CreateNet.h"
#include <stdio.h>
#include <math.h>
#define N_CONN_ATTEMPTS 100  // Maximum number of attempts to find an appropriate connection target (used in stochastic connections)

extern struct POPULATION *population;
extern unsigned int total_populations;
extern int **conn_values;
extern int **fdms_values;
extern double **delays_values;
extern double ***dist_values;
extern bool Auto_connections;
extern bool DivergentConnectivity;
extern bool Homogeneity;
extern bool Multiplicity;
extern struct SYNAPSES **synapses;
extern struct DYNAMICS *Neuron_Models;
extern unsigned int total_neuron_models;
extern struct DYNAMICS *Conductances_Models;
extern unsigned int total_syn_models;
extern struct DYNAMICS *Plasticity_Models;
extern unsigned int total_plas_models;
struct Position *nrn_position;
int **converg_conn_values = NULL;
unsigned int *total_neurons;
int *total_pop_synapses;
unsigned int *n_first;
gsl_rng *rng;
long **TargetsArray=NULL;
long *Targets_counter=NULL;
long *target_array_length;
extern int ***RANDOM_DIST;
int RND_fix=-1;


/**
  * GSL Normal Random Number Generator
  * @param sigma
  * @return return r the random number
*/
double RandGaussian(double sigma){  
  double r = gsl_ran_gaussian(rng,sigma);
  return r;
}

/**
  * GSL Uniform Random Number Generator
  * @param min 
  * @param max\						
*/

long RND_Uniform_Int(long min, long max){
    return (min + gsl_rng_uniform_int(rng, max-min));
}

/**
  * Determines the Triplet position (i,j,k) given a neuron ID
  * @param pop population number 
  * @param nrn neuron ID
  * @return return triplet struct
*/
struct Triplet ID2Triplet(int pop, long nrn, long first_nrn){

    long nrn_id = nrn - first_nrn; 
    long dim_x=population[pop].topology.dim_x ;
    long dim_y=population[pop].topology.dim_y;	
    
    struct Triplet triplet;
    triplet.i= nrn_id%dim_x;
    triplet.j= (int)((nrn_id%( dim_x*dim_y))/dim_x);
    triplet.k= (int)(nrn_id/ (dim_x*dim_y) );

    return triplet;
}

/**
  * Calculates neuron's ID given a Triplet position (i,j,k) 
  * @param i i coordinate
  * @param j j coordinate
  * @param k k coordinate 
  * @param dim_x number os neurons in x coordinate
  * @param dim_y number os neurons in y coordinate
  * @return ID neuron's ID
*/
int Triple2ID(int i, int j, int k, int dim_x, int dim_y){
    
    return k*(dim_x*dim_y) + j*dim_x + i;
}

/**
  * Choose the Random Target Position for each population
  * @param pop_pre
  * @param pop_pos
  * @param dim
  * @param sigma
  * @param target_center
  * @return rand_target (neurons' ID)
*/
int Choose_Random_Target_Position(int pop_pre, int pop_pos, int coord, int dim, double sigma, double target_center ) {

    int rand_target;
    int count=0;
    if (sigma < dim * population[pop_pos].topology.d_mean){
	  rand_target = (int)round( RandGaussian( sigma / population[pop_pos].topology.d_mean ) + target_center );
	  //RANDOM_DIST[pop_pre][pop_pos][coord]=0;//Gaussian Distribution
    }
    else{
	  rand_target = RND_Uniform_Int(0, dim );
	  //RANDOM_DIST[pop_pre][pop_pos][coord]=1;//Uniform Distribution
	}
    while( rand_target < 0 || rand_target >= dim ) {
	  rand_target = (int)round( RandGaussian( sigma / population[pop_pos].topology.d_mean ) + target_center );
	  count++;
	  if( count > N_CONN_ATTEMPTS ){ 
		rand_target = -1;
		break;
	  }
    }
	return rand_target;
}

/**
  * Calculate 3D geometric position of all neurons in each population 
  * @param pop
  * @param first
  * @param n_nrn
  * @return position struct
*/
struct Position* Neurons_Position(int pop, unsigned int first, unsigned int n_nrn){

      nrn_position[first].x_pos=population[pop].topology.X0;
      nrn_position[first].y_pos=population[pop].topology.Y0;
      nrn_position[first].z_pos=population[pop].topology.Z0;
      nrn_position[first].ID=first;
      nrn_position[first].pop_id=pop;
      double r, r1, r2;
      struct Triplet triplet;
      for (unsigned long nrn=first; nrn<n_nrn; nrn++){
  
		  triplet=ID2Triplet(pop, nrn, first);
	  
		  r = RandGaussian(population[pop].topology.d_std );
		  r1 = RandGaussian(population[pop].topology.d_std );
		  r2 = RandGaussian(population[pop].topology.d_std );
		  nrn_position[nrn].x_pos= nrn_position[first].x_pos + triplet.i*population[pop].topology.d_mean + r;
		  nrn_position[nrn].y_pos= nrn_position[first].y_pos + triplet.j*population[pop].topology.d_mean + r1;
		  nrn_position[nrn].z_pos= nrn_position[first].z_pos + triplet.k*population[pop].topology.d_mean + r2;

		  nrn_position[nrn].ID=nrn;
		  nrn_position[nrn].pop_id=pop;
	    }	
    return nrn_position;

}

/**
  * Calculate Targets IDs for each neuron and stores them at TargetsArray (neurons X targets)
  * @param pop_pre
  * @param pop_pos
  * @return error 0 if the targets are calculated
*/
int Neuron_Targets_ID(unsigned int pop_pre,unsigned int pop_pos){

  int error=0;
  struct Triplet triplet_source;
  int rand_target_i, rand_target_j, rand_target_k ;
  double target_center_x, target_center_y, target_center_z;
  unsigned int target_ID;

  int pop_source, pop_target;

  if (DivergentConnectivity==true){
	pop_source=pop_pre;
	pop_target=pop_pos;
  }
  else {
	pop_source=pop_pos;
	pop_target=pop_pre;
  }

  int dimx=population[pop_target].topology.dim_x;
  int dimy=population[pop_target].topology.dim_y;
  int dimz=population[pop_target].topology.dim_z;

  double sigma_x = dist_values[pop_pre][pop_pos][0];
  double sigma_y = dist_values[pop_pre][pop_pos][1];
  double sigma_z = dist_values[pop_pre][pop_pos][2];

  int n=0;
  
  for (unsigned long nrn=n_first[pop_source];nrn<(total_neurons[pop_source]+n_first[pop_source]);nrn++){

      //Multiplicity Disable
      if(Multiplicity==false && RANDOM_DIST[pop_source][pop_target][0]*RANDOM_DIST[pop_source][pop_target][1]*RANDOM_DIST[pop_source][pop_target][2]==1){
	    long* target_array;
	    long target_size=0;
	    int j=-1;
	    if (Auto_connections==false && pop_source==pop_target){
	      target_size=total_neurons[pop_target]-1;
	      target_array = (long*) malloc(target_size * sizeof(long));
	      for (unsigned long i=0; i< total_neurons[pop_target]; i++){
		  if ((n_first[pop_target]+i)!=nrn){
		      j=j+1;
		      target_array[j]=n_first[pop_target]+i;
		      
		  }
	      }
	    }
	    else{
	      target_size= total_neurons[pop_target];
	      target_array = (long*) malloc(target_size * sizeof(long));
	      for (unsigned long i=0; i<total_neurons[pop_target]; i++){
		      target_array[i]=n_first[pop_target]+i;
	      }
	    }
	    
	     //TODO verificar se não é melhor alterar a matrix de connectividade
	    // total_synapses must be less than the number of target population neurons (-1 to prevent no auto_connections)
	    // permute target neurons array and fill the targets of each neuron in Targets_Array(neuronsxtargets)
	    if(conn_values[pop_pre][pop_pos]<=target_size){
		  RND_fix=RND_fix+1;
		  gsl_ran_shuffle (rng, target_array, target_size, sizeof(size_t)); 
		  for(int target=0; target< conn_values[pop_pre][pop_pos]; target++) {
			if (DivergentConnectivity==true){
			  TargetsArray[n][Targets_counter[n]]=target_array[target];
			  Targets_counter[n]= Targets_counter[n] + 1;
			}
			else{
			  if(Targets_counter[target_array[target]-n_first[pop_pre]]>=target_array_length[target_array[target]-n_first[pop_pre]]){//Convergent connectivity 
				target_array_length[target_array[target]-n_first[pop_pre]]=target_array_length[target_array[target]-n_first[pop_pre]]+1;
				TargetsArray[target_array[target]-n_first[pop_pre]]=(long*) realloc(TargetsArray[target_array[target] -n_first[pop_pre]], target_array_length[target_array[target]-n_first[pop_pre]] * sizeof(long));
			  }
			  TargetsArray[target_array[target]-n_first[pop_pre]][Targets_counter[target_array[target]-n_first[pop_pre]]]=nrn;
			  Targets_counter[target_array[target]-n_first[pop_pre]]=Targets_counter[target_array[target]-n_first[pop_pre]]+1;
			}
		  }
	    }
	    else{
		
		  // fill the targets os each neuron in Targets_Array(neuronsxtargets)
		  for(int target=0; target< target_size; target++) {
		    if (DivergentConnectivity==true){///Divergent connectivity 
		      TargetsArray[n][Targets_counter[n]]=target_array[target];
		      Targets_counter[n]= Targets_counter[n] + 1;
		    }
		    else{//Convergent connectivity 
			  if (Targets_counter[target_array[target]-n_first[pop_pre]]>=target_array_length[target_array[target]-n_first[pop_pre]]){//realloc TargetArray
				target_array_length[target_array[target]-n_first[pop_pre]]=target_array_length[target_array[target]-n_first[pop_pre]]+1;
				TargetsArray[target_array[target]-n_first[pop_pre]]=(long*) realloc(TargetsArray[target_array[target] -n_first[pop_pre]], target_array_length[target_array[target]-n_first[pop_pre]] * sizeof(long));
		      }
		      TargetsArray[target_array[target]-n_first[pop_pre]][Targets_counter[target_array[target]-n_first[pop_pre]]]=nrn;
		      Targets_counter[target_array[target]-n_first[pop_pre]]=Targets_counter[target_array[target]-n_first[pop_pre]]+1;
		    }
		  }
	    }
	delete [] target_array;

	}//Multiplicity Enable
	else {
	  RND_fix=RND_fix + 1;
	  // find the center of the target population
	  triplet_source = ID2Triplet(pop_source,nrn,n_first[pop_source]);
	  
	  if (population[pop_source].topology.dim_x==1){
	      target_center_x =  0.5 * (population[pop_target].topology.dim_x-1.0) ;
	  }
	  else{
	      target_center_x = (population[pop_target].topology.dim_x-1.0)/(population[pop_source].topology.dim_x-1.0) * triplet_source.i;
	  }
	  if (population[pop_source].topology.dim_y==1){  
	      target_center_y = 0.5 * (population[pop_target].topology.dim_y-1.0);
	  }
	  else{
	      target_center_y = (population[pop_target].topology.dim_y-1.0)/(population[pop_source].topology.dim_y-1.0) * triplet_source.j;
	  }
	  if (population[pop_source].topology.dim_z==1){  
	      target_center_z = 0.5 * (population[pop_target].topology.dim_z-1.0);
	  }
	  else{
	      target_center_z = (population[pop_target].topology.dim_z-1.0)/(population[pop_source].topology.dim_z-1.0) * triplet_source.k;
	  }

	  for(int target=0; target< conn_values[pop_pre][pop_pos]; target++) {
		rand_target_i = Choose_Random_Target_Position(pop_source,pop_target, 0, dimx,sigma_x, target_center_x);
		rand_target_j = Choose_Random_Target_Position(pop_source,pop_target, 1, dimy,sigma_y, target_center_y);
		rand_target_k = Choose_Random_Target_Position(pop_source,pop_target, 2, dimz,sigma_z, target_center_z);
	      if (rand_target_i==-1 || rand_target_j==-1 || rand_target_k==-1){
		  error = -1;
		  break;
	      }
	      target_ID = n_first[pop_target] + Triple2ID( rand_target_i, rand_target_j, rand_target_k, population[pop_target].topology.dim_x, 												population[pop_target].topology.dim_y);
	      int count=0;
	      if (Auto_connections==false){//Auto-connections Disable
		    while (target_ID==nrn){
			count++;
			rand_target_i = Choose_Random_Target_Position(pop_source,pop_target, 0, dimx,sigma_x, target_center_x);
			rand_target_j = Choose_Random_Target_Position(pop_source,pop_target, 1, dimy,sigma_y, target_center_y);
			rand_target_k = Choose_Random_Target_Position(pop_source,pop_target, 2, dimz,sigma_z, target_center_z);
			      
			target_ID = n_first[pop_target] + Triple2ID( rand_target_i, rand_target_j, rand_target_k, population[pop_target].topology.dim_x, 													population[pop_target].topology.dim_y);
			if( count > N_CONN_ATTEMPTS ) {
			    error= -1;
			    break;
			}
		    }
	      }
	      if (DivergentConnectivity==true){//Divergent connectivity 
		  TargetsArray[n][Targets_counter[n]]=target_ID;
		  Targets_counter[n]= Targets_counter[n] + 1;
	      }
	      else{//Convergent connectivity 
		  if (Targets_counter[target_ID-n_first[pop_pre]]>=target_array_length[target_ID-n_first[pop_pre]]){//realloc Target_Array
		      target_array_length[target_ID-n_first[pop_pre]]=target_array_length[target_ID-n_first[pop_pre]]+1;
		      TargetsArray[target_ID-n_first[pop_pre]]=(long*) realloc(TargetsArray[target_ID-n_first[pop_pre]],target_array_length[target_ID-n_first[pop_pre]] * sizeof(long));
		  }
		  TargetsArray[target_ID-n_first[pop_pre]][Targets_counter[target_ID-n_first[pop_pre]]]=nrn;
		  Targets_counter[target_ID-n_first[pop_pre]]=Targets_counter[target_ID-n_first[pop_pre]]+1;
	      }
	    }
	}
	n=n+1;
    }
    return error;
}


/**
  * Checks some connectivity options before the network is created (multiplicity and auto-connections)
  * @return error 0 if all options are in agree
*/
int CheckError(void){

  int error=0;
  int k = 0;
  
  for (unsigned int pop_source=0; pop_source<total_populations;pop_source++){
	for (unsigned int pop_target=0; pop_target<total_populations;pop_target++){
	 
	    int size_pop_source=population[pop_source].topology.dim_x*population[pop_source].topology.dim_y*population[pop_source].topology.dim_z;
	    int size_pop_target=population[pop_target].topology.dim_x*population[pop_target].topology.dim_y*population[pop_target].topology.dim_z;
	 
	    if (conn_values[pop_source][pop_target]!=0){
	      //test for multiplicity versus Uniform distribution + #CONNECTIONS
	      if (Multiplicity==false){
		    if (RANDOM_DIST[pop_source][pop_target][0]*RANDOM_DIST[pop_source][pop_target][1]*RANDOM_DIST[pop_source][pop_target][2] == 1){//UNIFORM
			  if (Auto_connections == TRUE ){
				k = 1;
			  }
			  //Divergent connectivity      
			  if (DivergentConnectivity == TRUE) {
				if (conn_values[pop_source][pop_target] >= size_pop_target - k){
				  wxLogMessage(wxT(">> WARNING: For the selected target population ") + wxString(population[pop_target].label,wxConvUTF8) + _(" multiplicity option must be Enable."));
				  error = 1;
				  break;
				}
			  }
			  //Convergent connectivity
			  if (DivergentConnectivity == FALSE) {
				if (conn_values[pop_source][pop_target] >= size_pop_source - k){
				  wxLogMessage(wxT(">> WARNING: For the selected target population ") + wxString(population[pop_target].label,wxConvUTF8) + _(" multiplicity must be Enable."));
				  error = 1;
				  break;
				}
			  }
		      }
		    else {//GAUSSIAN
		      wxLogMessage(wxT(">> WARNING: For the inserted sigma's values in population ") + wxString(population[pop_target].label,wxConvUTF8) + _(" (which assume a non-uniform distribution) multiplicity option must be Enable (to allow multiple synapses to/from the same neuron)."));
		      error = 1;
		      break;
		    }
		}
	      
		//Take care of auto_connections problems
		if (size_pop_target==1 && pop_target==pop_source && Auto_connections==false){ //size_pop_target=size_pop_source
		      wxLogMessage(wxT(">> WARNING: The selected target population ") + wxString(population[pop_target].label,wxConvUTF8) + _(" has only one unit therefore auto-connections must be Enable."));
		      error = 1;
		      break;
		}
	   }
      }
  }
  return error;
}


/**
  * Main core to create .Net File
  * @param *network_file the file where will contain the network
  * @return error 0 if all options are in agree
*/
int Main_Core(char* network_file){

  int error = 0;
  double end_time; 
  int start_time;
  // initialize the pseudo-random number generator
  const gsl_rng_type * T;
  gsl_rng_env_setup();
  
  T = gsl_rng_default;
  rng = gsl_rng_alloc (T);
  
  if (rng == NULL) {
    printf("\n\tERROR: unable to initialize the pseudo-random number generator.\n");
    exit(-1);
  }
  start_time = time(NULL);

	// ****************************************************
	// REMOVE TO ALLOW DIFFERENT RANDOM STREAMS
	// gsl_rng_set( rng, (long) start_time );
	// ****************************************************

  // create/open .net file  
  FILE *out_file;
  out_file=fopen(network_file,"wb+"); 
  
  fprintf(out_file, "# File created with NetBuilder 1.0\n");
  fprintf(out_file, "# Mafalda Sousa\tmafsousa@ibmc.up.pt\n# Paulo Castro Aguiar\tpauloaguiar@fc.up.pt\n#");

  // Recall template structure in case modification is needed
  fprintf(out_file, "\n# The template for the structure of .net files is available at ~/NeuralSyns/doc/DotNet_Template.pdf.");

  
  // THE CORE CODE STARTS HERE
  fprintf(out_file, "\nSTART");
  fprintf(out_file, "\n");
  int total_models=total_neuron_models+total_syn_models+total_plas_models;
  fprintf(out_file, "\nMODELS: %d",total_models);
  for (unsigned int n=0;n<total_neuron_models;n++){
     fprintf(out_file,"\n\t%s\t%d",Neuron_Models[n].model_label,Neuron_Models[n].n_state_vars);
     for (int var=0;var<Neuron_Models[n].n_state_vars;var++){
	  fprintf(out_file,"\t%s",Neuron_Models[n].state_vars_label[var]);
     }
     fprintf(out_file,"\t%d",Neuron_Models[n].model_total_parameters);
     for (int par=0;par<Neuron_Models[n].model_total_parameters;par++){
	  fprintf(out_file,"\t%s",Neuron_Models[n].parameter_label[par]);
     }
  }
  for (unsigned int s=0;s<total_syn_models;s++){
     fprintf(out_file,"\n\t%s\t%d",Conductances_Models[s].model_label,Conductances_Models[s].n_state_vars);
     for (int var=0;var<Conductances_Models[s].n_state_vars;var++){
	  fprintf(out_file,"\t%s",Conductances_Models[s].state_vars_label[var]);
     }
     fprintf(out_file,"\t%d",Conductances_Models[s].model_total_parameters);
     for (int par=0;par<Conductances_Models[s].model_total_parameters;par++){
	  fprintf(out_file,"\t%s",Conductances_Models[s].parameter_label[par]);
     }
  }
  for (unsigned int p=0;p<total_plas_models;p++){
     fprintf(out_file,"\n\t%s\t%d",Plasticity_Models[p].model_label,Plasticity_Models[p].n_state_vars);
     for (int var=0;var<Plasticity_Models[p].n_state_vars;var++){
	  fprintf(out_file,"\t%s",Plasticity_Models[p].state_vars_label[var]);
     }
     fprintf(out_file,"\t%d",Plasticity_Models[p].model_total_parameters);
     for (int par=0;par<Plasticity_Models[p].model_total_parameters;par++){
	  fprintf(out_file,"\t%s",Plasticity_Models[p].parameter_label[par]);
     }
  }
  
  fprintf(out_file, "\n"); 
  fprintf(out_file, "\nPOPULATIONS:  %d",total_populations);
  fprintf(out_file, "\n\tlabel [char*10]:");
  for (unsigned int pop=0;pop<total_populations;pop++){
      fprintf(out_file, "\t%s",population[pop].label);
  }
  fprintf(out_file, "\n\tsize [#]:\t");
  for (unsigned int pop=0;pop<total_populations;pop++){
      int size_pop =  population[pop].topology.dim_x * population[pop].topology.dim_y * population[pop].topology.dim_z;
      fprintf(out_file, "\t%d",size_pop);
  }
  fprintf(out_file, "\n\tfdomains [#]:\t");
  for (unsigned int pop=0;pop<total_populations;pop++){
      fprintf(out_file, "\t%d",population[pop].total_fdomains);
  }
  fprintf(out_file, "\n ");

  char topology[MAXLABELSIZE];
  long parent_ID;
  fprintf(out_file, "\nFDOMAINS:");
  for (unsigned int pop=0;pop<total_populations;pop++){
	  fprintf(out_file, "\n\t%s", population[pop].label);
	  for (int fdm=0; fdm<population[pop].total_fdomains;fdm++){
		snprintf(topology,MAXLABELSIZE,"(%.2lf,%.2lf,%.2lf,%.2lf)", population[pop].fdomains[fdm].x, population[pop].fdomains[fdm].y, population[pop].fdomains[fdm].z, population[pop].fdomains[fdm].r);
		if (fdm==0)
		    parent_ID=-1;
		else{
		    for (int i = 0; i < population[pop].total_fdomains; i++){
			if (strcmp(population[pop].fdomains[fdm].parent_label,population[pop].fdomains[i].label)==0)
			      parent_ID=population[pop].fdomains[i].fdm_ID;	    
		    }
		}
		fprintf(out_file, "\t[ %ld\t%s\t%ld\t%s\t%s ]",
				    population[pop].fdomains[fdm].fdm_ID,
				    population[pop].fdomains[fdm].label,
				    parent_ID, 
				    topology, 
				    population[pop].fdomains[fdm].dynamics.model_label);
	  }
  }
  


  ///fill RANDOM_DIST matrix
  for (unsigned int pop_pre=0;pop_pre<total_populations;pop_pre++){
      for (unsigned int pop_pos=0;pop_pos<total_populations;pop_pos++){
	  if (conn_values[pop_pre][pop_pos]!=0){
	      int dim[3];
	      dim[0]=population[pop_pos].topology.dim_x ;
	      dim[1]=population[pop_pos].topology.dim_y ;
	      dim[2]=population[pop_pos].topology.dim_z ;
	      for(unsigned int coord=0;coord<3;coord++){
		  if (dist_values[pop_pre][pop_pos][coord] <= dim[coord] * population[pop_pos].topology.d_mean)
		      RANDOM_DIST[pop_pre][pop_pos][coord]=0;//Gaussian Distribution
		  else
		      RANDOM_DIST[pop_pre][pop_pos][coord]=1;//Uniform Distribution
	      }
	  }
      }
  }
  
  error=CheckError();
  if (error==1) return error;

   
  fprintf(out_file, "\n ");

  fprintf(out_file, "\nHOMOGENEOUS SYNAPSES:");
  if (Homogeneity==true)
	fprintf(out_file, "\tYES");
  else 
	fprintf(out_file, "\tNO");


  //connection pairs
  int total_connection_pairs=0;
  for (unsigned int pop=0;pop<total_populations;pop++){
      for (unsigned int popi=0;popi<total_populations;popi++){
	  if (conn_values[pop][popi]!=0){
	      total_connection_pairs=total_connection_pairs + 1;
	  }
      }
  }
  
  int n=-1;
  fprintf(out_file,"\n\nCONNECTION PAIRS: %d", total_connection_pairs);
  for (unsigned int pop_pre=0;pop_pre<total_populations;pop_pre++){
      for (unsigned int pop_pos=0;pop_pos<total_populations;pop_pos++){
	  if (conn_values[pop_pre][pop_pos]!=0){
	 	n=n+1;
		fprintf(out_file,"\n\tPAIR: %d\t[ %s %s ]\t%s\t[ %d %d ]\t%ld",
			n,
			population[pop_pre].label,
			population[pop_pos].label,
			population[pop_pos].fdomains[fdms_values[pop_pre][pop_pos]].label,
			pop_pre,
			pop_pos,
			population[pop_pos].fdomains[fdms_values[pop_pre][pop_pos]].fdm_ID);
		//conductances
		fprintf(out_file,"\n\t\t%s",  synapses[pop_pre][pop_pos].conductances_dynamics.model_label);
		if (Homogeneity==true){
		    for (int par=0; par<synapses[pop_pre][pop_pos].conductances_dynamics.model_total_parameters; par++){
			fprintf(out_file,"\t%lg",synapses[pop_pre][pop_pos].conductances_dynamics.parameter[par]);
		    }
		}
		//plasticity
		fprintf(out_file,"\n\t\t%s",  synapses[pop_pre][pop_pos].plasticity_dynamics.model_label);
		if (Homogeneity==true){
		    for (int par=0; par<synapses[pop_pre][pop_pos].plasticity_dynamics.model_total_parameters; par++){
			fprintf(out_file,"\t%lg",synapses[pop_pre][pop_pos].plasticity_dynamics.parameter[par]);
		    }
		}
	    }
	}
  }
  
  total_neurons=(unsigned int*) malloc(total_populations * sizeof(unsigned int));
  n_first=(unsigned int*) malloc(total_populations * sizeof(unsigned int));

  unsigned int n_nrn=0;
  nrn_position=(Position*) malloc(sizeof(struct Position));

  //Place the NEURON in a 3D-box
  for (unsigned int pop=0; pop<total_populations; pop++){
      long dim_x=population[pop].topology.dim_x ;
      long dim_y=population[pop].topology.dim_y;	
      long dim_z=population[pop].topology.dim_z;
      total_neurons[pop]= dim_x*dim_y*dim_z;
      n_first[pop]=n_nrn;
      n_nrn=n_nrn+total_neurons[pop];
      nrn_position=(Position*) realloc(nrn_position,n_nrn * sizeof(struct Position));

      Neurons_Position(pop,n_first[pop],n_nrn); // Neurons position
  }

  fprintf(out_file, "\n");
  fprintf(out_file, "\nNEURONS: %d", n_nrn);

  int total_synapses_div=0;
  int total_synapses_conv=0;
  
  total_pop_synapses=(int*) malloc (total_populations * sizeof(int));
  for (unsigned int i=0; i<total_populations;i++){
	  int syn=0;
	  for (unsigned int j=0; j<total_populations;j++){	  
		total_synapses_div= total_synapses_div + total_neurons[i]*conn_values[i][j];
		total_synapses_conv= total_synapses_conv + total_neurons[i]*conn_values[j][i];
		syn=syn + conn_values[i][j];
	  }
	  total_pop_synapses[i]= syn;
  }

  if (DivergentConnectivity==true)
      fprintf(out_file, "\nSYNAPSES: %d\n", total_synapses_div);
  else
      fprintf(out_file, "\nSYNAPSES: %d\n", total_synapses_conv);

  


  wxProgressDialog progressDlg(_("Please wait"), _(" The network model file is now being created"), n_nrn, NULL, wxPD_AUTO_HIDE); 

  int nrn_ID;
  for (unsigned int pop_pre= 0;pop_pre<total_populations;pop_pre++){
	TargetsArray=(long**) malloc(total_neurons[pop_pre] * sizeof(long*));
	Targets_counter=(long*) malloc(total_neurons[pop_pre]*sizeof(long));
	target_array_length=(long*) malloc(total_neurons[pop_pre]*sizeof(long));
	if (TargetsArray== NULL){
	    printf("\n\tERROR: unable to allocate memory for Targets Array");
	    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
	    exit(-1);
	    Free_Memory();
	}
	for (unsigned int nrn = 0; nrn < total_neurons[pop_pre]; nrn++) {
	    Targets_counter[nrn]=0;
	}
	if (total_pop_synapses[pop_pre]!=0 ){
	    for (unsigned int nrn = 0; nrn < total_neurons[pop_pre]; nrn++) {
		TargetsArray[nrn]=(long*) malloc(total_pop_synapses[pop_pre] * sizeof(long));
		target_array_length[nrn]=total_pop_synapses[pop_pre];
		if (TargetsArray[nrn]== NULL) {
		    printf("\n\tERROR: unable to allocate memory for Targets Array");
		    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		    exit(-1);
		    Free_Memory();
		  } 
	  }
	}

	// For each source population calculate all neuron targets 
	for(unsigned int pop_pos=0; pop_pos<total_populations; pop_pos++){
	      if (conn_values[pop_pre][pop_pos]!=0){    
		error=Neuron_Targets_ID(pop_pre,pop_pos);
		if (error!=0) 
		      break;
		
		char **x;
		x=(char**) malloc(3*sizeof(char*));
		for (int k = 0; k<3; k++){
		    x[k]=(char*) malloc(MAXLABELSIZE*sizeof(char));
		    if ( RANDOM_DIST[pop_pre][pop_pos][k] == 0)
			  str_equal("Normal",x[k]);
		    else
			  str_equal("Uniform",x[k]);
		}
		wxLogMessage(wxT(">> The receptive field from population ") + wxString(population[pop_pre].label,wxConvUTF8) +  _(" to population ") + wxString(population[pop_pos].label,wxConvUTF8) + _(" follows a distribution with components (") + wxString(x[0],wxConvUTF8) + _(",") + wxString(x[1],wxConvUTF8) + _(",") + wxString(x[2],wxConvUTF8) + _(")"));
	      }
	}

	//print to file the information for each neuron
	for (unsigned int nrn=0; nrn<total_neurons[pop_pre];nrn++){ 
		nrn_ID=n_first[pop_pre] + nrn;
		if (nrn_ID%(n_nrn/100 + 1)==0)
		    progressDlg.Update(nrn_ID);
		
		fprintf(out_file, "\nNEURON %d", nrn_ID);
		fprintf(out_file, "\n\t(%.3lf, %.3lf, %.3lf)", nrn_position[nrn_ID].x_pos,nrn_position[nrn_ID].y_pos,nrn_position[nrn_ID].z_pos);
		//repeat for all fdomains
		for (int fdm=0; fdm<population[nrn_position[nrn_ID].pop_id].total_fdomains;fdm++){
			fprintf(out_file, "\n\t%lg\t-70.0", population[nrn_position[nrn_ID].pop_id].fdomains[fdm].Ra);
		    for (int var=0; var <population[nrn_position[nrn_ID].pop_id].fdomains[fdm].dynamics.n_state_vars; var++){
			fprintf(out_file, "\t%lg",population[pop_pre].fdomains[fdm].dynamics.state_vars_val[var]);
		    }
		    for (int par=0;par<population[nrn_position[nrn_ID].pop_id].fdomains[fdm].dynamics.model_total_parameters;par++){
			fprintf(out_file, "\t%lg",population[pop_pre].fdomains[fdm].dynamics.parameter[par]);
		    }
		}
		      
		fprintf(out_file, "\n\tSPIKES: 0");//<#fired spikes><time>...<time>	
		fprintf(out_file, "\n\tSYNAPSES: %ld",Targets_counter[nrn]);
		//repeat for all synapses

		    
		for (int syn=0; syn< Targets_counter[nrn]; syn++){
			int target_id=TargetsArray[nrn][syn];
			fprintf(out_file, "\n\t\t@ %ld\t1.0\t%.3lf", TargetsArray[nrn][syn],delays_values[pop_pre][nrn_position[target_id].pop_id]);
		    fprintf(out_file, "\n\t\t\t 0\t0\t1e09 ");//<total spikes in cue><pcsc><time last release>  

		    fprintf(out_file, "\n\t\t\t %s", synapses[pop_pre][nrn_position[target_id].pop_id].conductances_dynamics.model_label);
		    for (int stv=0; stv < synapses[pop_pre][nrn_position[target_id].pop_id].conductances_dynamics.n_state_vars;stv++){
					fprintf(out_file,"\t%.2lf",synapses[pop_pre][nrn_position[target_id].pop_id].conductances_dynamics.state_vars_val[stv]);
		    }
		    if (Homogeneity==false){
					for (int par=0; par<synapses[pop_pre][nrn_position[target_id].pop_id].conductances_dynamics.model_total_parameters; par++){
						fprintf(out_file,"\t%lg",synapses[pop_pre][nrn_position[target_id].pop_id].conductances_dynamics.parameter[par]);
					}
		    }      

		    fprintf(out_file,"\n\t\t\t %s",  synapses[pop_pre][nrn_position[target_id].pop_id].plasticity_dynamics.model_label);
		    for (int stv=0; stv < synapses[pop_pre][nrn_position[target_id].pop_id].plasticity_dynamics.n_state_vars;stv++){
					fprintf(out_file,"\t%lf",synapses[pop_pre][nrn_position[target_id].pop_id].plasticity_dynamics.state_vars_val[stv]);
		    }
		    if (Homogeneity==false){
					for (int par=0; par<synapses[pop_pre][nrn_position[target_id].pop_id].plasticity_dynamics.model_total_parameters; par++)
			      fprintf(out_file,"\t%lg",synapses[pop_pre][nrn_position[target_id].pop_id].plasticity_dynamics.parameter[par]);
		    }




				fprintf(out_file, "\n");
		}
		if (Targets_counter[nrn]==0)
			fprintf(out_file, "\n");
	}
  

  free(TargetsArray);
  free(Targets_counter);
  free(target_array_length);	    
  }
  fprintf(out_file,"\nEND\n");
  fclose(out_file);
  end_time=time(NULL);
  double dife=end_time-start_time;

  printf("\ntime=%.3f\n\n",dife);
  fflush(stdout);
  gsl_rng_free(rng); 
  return error;
}
