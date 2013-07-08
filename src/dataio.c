//************************************************************************
//*                                                                      *
//*               N E T W O R K   I N P U T / O U T P U T                *
//*                                                                      *
//************************************************************************

/**
 * \file dataio.c
 * \author Paulo Aguiar
 */

#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "general.h"
#include "models.h"
#include "populations.h"
#include "neurons.h"
#include "synapses.h"
#include "dataio.h"

extern struct POPULATION *population;
extern unsigned int total_populations;
extern struct NEURON *neuron;
extern unsigned long total_neurons;
extern unsigned long total_synapses;
extern struct CONNECTION **connection;
extern unsigned int total_connections;

extern struct FLAGS flags;

extern double t;
extern double dt;
extern double total_time;

//models
extern struct MODEL_DYNAMICS_NEURON *neuron_model;
extern struct MODEL_DYNAMICS_SYNAPSE *synapse_model;
extern struct MODEL_DYNAMICS_PLASTICITY *plasticity_model;


// VERY IMPORTANT: ANY CHANGE IN Read_Network() OR Write_Network() FUNCTIONS MUST BE MIRRORED IN THE CORRESPONDING
//                 FUNCTIONS IN NetBuilder (CreateNet.cpp, dataio.cpp) AND IN PyEditNetwork (Py_Edit_Network.py)


//------------------------------------------------------------------------

/**
 * Reads all data from a network model data file (.net) and updates all internal model structures
 * \param *network_file network model file name 
 * \return return 0 if the process goes well
 */

int Read_Network(char *network_file)
{
  FILE *file_in;
  unsigned long neuron_id, temp_nrn_sum, n_statevars, n_params;
  unsigned long nrn, syn, syn_index = 0;
  unsigned int i, pop, pop_i, pop_o, spk, n_spks, fdm, con, temp_int, m;
	int catch_err;

  char temp_string[MAX_STRING_SIZE];                     //holds temporary data
  char labelA[MAX_STRING_SIZE], labelB[MAX_STRING_SIZE]; //holds temporary data
  char temp_line[256];                                   //holds temporary data                  
  char *temp_ptr;                                        //holds temporary data

  struct MODEL_DYNAMICS_SYNAPSE *model_current;
  struct MODEL_DYNAMICS_PLASTICITY *model_plasticity;
  char model_name[MAX_STRING_SIZE];


  file_in = fopen(network_file, "r");
  if (file_in == NULL) {
    printf("\n\tERROR: unable to read neurons _network_ data file\n");
    exit(-1);
  }

  printf("[%s]:\n-> Reading _network_ file:", network_file);
  fflush(stdout);

  //Skip all comment lines. Begin at START keyword
  temp_ptr = fgets(temp_line, 256, file_in);
  while (strcmp(temp_line, "START\n") != 0) {
    if (fgets(temp_line, 256, file_in) == NULL) {
      printf("\n\tERROR: the keyword START was not found in the network file %s\n\n", network_file);
      fclose(file_in);
      exit(-1);
    }
  }


  // Start taking the data
  // ---------------------

	//MODELS
	catch_err = fscanf(file_in, "\nMODELS: %d\n", &temp_int);
	Check_fscanf(catch_err, 1, "MODELS", __LINE__ - 1, file_in);
	for( m=0; m<temp_int; m++) {
		temp_ptr = fgets(temp_line, 256, file_in); //goes directly to the trash bin: this information is used by the python functions to read the .net files
	}


  //POPULATIONS total_populations
  catch_err = fscanf(file_in, "\n\nPOPULATIONS: %d", &total_populations);
	Check_fscanf(catch_err, 1, "POPULATIONS", __LINE__ - 1, file_in);

  if (total_populations > 0) {
    population = calloc(total_populations, sizeof(struct POPULATION));
		Initialize_Populations( population, file_in );
  }
  else {
    printf("\nERROR: you do not have any populations defined in your network file\n\n");
    exit(-1);
  }

  catch_err = fscanf(file_in, "\n\tlabel [char*10]:");
  for (pop = 0; pop < total_populations; pop++) {
    catch_err = fscanf(file_in, "\t%s", population[pop].label);
		Check_fscanf(catch_err, 1, "POPULATIONS", __LINE__ - 1, file_in);
    population[pop].label[10] = '\0';	//force end of string; allows 10 chars in the string
  }

  catch_err = fscanf(file_in, "\n\tsize [#]:");
  for (pop = 0; pop < total_populations; pop++) {
    catch_err = fscanf(file_in, "\t%ld", &population[pop].n_neurons);
		Check_fscanf(catch_err, 1, "POPULATIONS", __LINE__ - 1, file_in);
    if (population[pop].n_neurons < 1) {
      printf("\n\tERROR: population %d contains less than 1 neuron; each population must contain at least 1 neuron", pop);
      printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
      fclose(file_in);
      exit(-1);
    }
  }
  
  catch_err = fscanf(file_in, "\n\tfdomains [#]:");
  for (pop = 0; pop < total_populations; pop++) {
		catch_err = fscanf(file_in, "\t%d", &population[pop].n_fdomains);
		Check_fscanf(catch_err, 1, "POPULATIONS", __LINE__ - 1, file_in);
		if (population[pop].n_fdomains > 0) {
			population[pop].fdomain = calloc(population[pop].n_fdomains, sizeof(struct FDOMAIN_GLOBAL));
			Initialize_FDomainGlobal( population[pop].fdomain, pop, file_in );
		}
		else {
			printf("\n\tERROR: you have a population of neurons [pop=%d] with 0 functional domains. All neurons must have at least 1 functional domain (fdm 0).", pop);
			printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			fclose(file_in);
			exit(-1);
		}
  }
	
	
  //FDOMAINS
  catch_err = fscanf(file_in, "\n\nFDOMAINS:");
  for (pop = 0; pop < total_populations; pop++) {

    catch_err = fscanf(file_in, "\n\t%s", temp_string); //take Pop00:
		Check_fscanf(catch_err, 1, "FDOMAINS", __LINE__ - 1, file_in);
    for (fdm = 0; fdm < population[pop].n_fdomains; fdm++) {
      catch_err = fscanf(file_in, "\t[\t%d\t%s\t%d\t(%lf,%lf,%lf,%lf)\t%s\t]", &temp_int, population[pop].fdomain[fdm].label, &population[pop].fdomain[fdm].parent_fdomain, &population[pop].fdomain[fdm].position.x, &population[pop].fdomain[fdm].position.y, &population[pop].fdomain[fdm].position.z, &population[pop].fdomain[fdm].position.r, model_name);
			Check_fscanf(catch_err, 8, "FDOMAINS", __LINE__ - 1, file_in);
			population[pop].fdomain[fdm].model = GetNeuronModelFromString( model_name );
			if( population[pop].fdomain[fdm].model == NULL ) {
				printf("\n\tERROR could not find model with the name %s", model_name);
				printf("\n\tERROR reports to FDOMAIN %d in POPULATION %d in the .net input file %s", fdm, pop, network_file);
				printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				fclose(file_in);
				exit(-1);
			}
		} //repeat for all fdomains

		Process_FDomainsGlobal_Childs( pop );			

  } //repeat for all populations
  

  //HOMOGENEOUS SYNAPSES
  catch_err = fscanf(file_in, "\n\nHOMOGENEOUS SYNAPSES:\t%s\n", temp_string);
	Check_fscanf(catch_err, 1, "HOMOGENEOUS SYNAPSES", __LINE__ - 1, file_in);
  if( strcasecmp(temp_string, "yes") == 0 ) {
    flags.homogeneous_synapses = 1;
  }
  else {
    flags.homogeneous_synapses = 0;
  }


  //CONNECTION PAIRS
  connection = calloc( total_populations, sizeof( struct CONNECTION * ) );
  for (pop = 0; pop < total_populations; pop++) {
		connection[pop] = calloc( total_populations, sizeof( struct CONNECTION ) );
  }
	Initialize_Connections( connection, file_in );

  catch_err = fscanf(file_in, "\nCONNECTION PAIRS:\t%d", &total_connections);
  Check_fscanf(catch_err, 1, "CONNECTION PAIRS", __LINE__ - 1, file_in);
  for( con=0; con<total_connections; con++) {
    catch_err = fscanf(file_in, "\n\tPAIR:\t%d\t[ %s %s ]\t%s\t[ %d %d ]\t%d", &temp_int, labelA, labelB, temp_string, &pop_i, &pop_o, &fdm);
		Check_fscanf(catch_err, 7, "CONNECTION PAIRS", __LINE__ - 1, file_in);
    if (temp_int != con) {
      printf("\n\tERROR: list of CONNECTION PAIRS out of order; Problem found in CONNECTION PAIR %d.\n\n", con);
      Exit_Error(2,file_in);
    }
		connection[pop_i][pop_o].set = 1;
		sprintf( temp_string, "%s>>%s", labelA, labelB );
		strcpy( connection[pop_i][pop_o].label, temp_string );
		connection[pop_i][pop_o].fdomain = fdm;
		
		catch_err = fscanf(file_in, "\n\t\t%s", model_name);
		Check_fscanf(catch_err, 1, "CONNECTION PAIRS - current model name", __LINE__ - 1, file_in);
		connection[pop_i][pop_o].current_model = GetSynapseModelFromString( model_name );
		if( connection[pop_i][pop_o].current_model == NULL ) {
			printf("\n\tERROR reports to CONNECTION PAIR %d in the .net input file %s", con, network_file);
			printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
      Exit_Error(2,file_in);
		}	
		if( flags.homogeneous_synapses == 1 ) {
			n_params = connection[pop_i][pop_o].current_model->n_params;
			connection[pop_i][pop_o].current_param = calloc( n_params, sizeof( double ) );
			if( connection[pop_i][pop_o].current_param == NULL ) {
				printf("\n\tERROR: unable to allocate memory for CONNECTION current model parameters. Failed at population pair (%d,%d).\n\n", pop_i, pop_o);
				Exit_Error(2,file_in);
			}
			for( i=0; i<n_params; i++ ) {
				catch_err = fscanf(file_in, "\t%lf", &connection[pop_i][pop_o].current_param[i]);	
				Check_fscanf(catch_err, 1, "CONNECTION PAIRS - current model parameters", __LINE__ - 1, file_in);
			}
		}
		
		catch_err = fscanf(file_in, "\n\t\t%s", model_name);
		Check_fscanf(catch_err, 1, "CONNECTION PAIRS - plasticity model name", __LINE__ - 1, file_in);
		connection[pop_i][pop_o].plasticity_model = GetPlasticityModelFromString( model_name );
		if( connection[pop_i][pop_o].plasticity_model == NULL ) {
			printf("\n\tERROR reports to CONNECTION PAIR %d in the .net input file %s", con, network_file);
			printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);		  
      Exit_Error(2,file_in);
		}
		if( flags.homogeneous_synapses == 1 ) {
			n_params = connection[pop_i][pop_o].plasticity_model->n_params;
			connection[pop_i][pop_o].plasticity_param = calloc( n_params, sizeof( double ) );
			if( connection[pop_i][pop_o].plasticity_param == NULL ) {
				printf("\n\tERROR: unable to allocate memory for CONNECTION plasticity model parameters. Failed at population pair (%d,%d).\n\n", pop_i, pop_o);
				Exit_Error(2,file_in);
			}
			for( i=0; i<n_params; i++ ) {
				catch_err = fscanf(file_in, "\t%lf", &connection[pop_i][pop_o].plasticity_param[i]);	
				Check_fscanf(catch_err, 1, "CONNECTION PAIRS - plasticity model parameters", __LINE__ - 1, file_in);
			}
		}
  }
  

  //NEURONS total_neurons
  catch_err = fscanf(file_in, "\n\nNEURONS: %ld", &total_neurons);
  Check_fscanf(catch_err, 1, "NEURONS (total)", __LINE__ - 1, file_in);
  if( total_neurons > 0 ) {
    neuron = calloc(total_neurons, sizeof(struct NEURON));
		Initialize_Neurons( neuron, file_in );
  }
  else {
    printf("\nERROR: you do not have any neurons defined in your network file.\n\n");
		Exit_Error(2,file_in);
  }
  

  //SYNAPSES total_synapses  
  catch_err = fscanf(file_in, "\nSYNAPSES: %ld\n", &total_synapses);
  Check_fscanf(catch_err, 1, "SYNAPSES (total)", __LINE__ - 1, file_in);




  // READ ALL NEURONS AND SYNAPSES DATA; here we go...

  //variables needed to keep track of population id within the nrn cycle
  pop = 0;
  temp_nrn_sum = population[pop].n_neurons;   
  printf("\nloading:\n%s ", population[pop].label);

  for (nrn = 0; nrn < total_neurons; nrn++) {  
  
    catch_err = fscanf(file_in, "\nNEURON\t%ld", &neuron_id);
		Check_fscanf(catch_err, 1, "NEURON", __LINE__ - 1, file_in);
    if (neuron_id != nrn) {
      printf("\n\tERROR: list of NEURONS out of order; Problem found in NEURON %ld.\n\n", nrn);
			Exit_Error(3,file_in);
    }

    //check to which population this neuron belongs
    if( nrn > temp_nrn_sum - 1 ) {
      pop++;
      temp_nrn_sum += population[pop].n_neurons;
			printf("\n%s ", population[pop].label);
    }
    printf(".");
    //fflush(stdout);
	
    catch_err = fscanf(file_in, "\n\t(%lf, %lf, %lf)", &neuron[nrn].position.x, &neuron[nrn].position.y, &neuron[nrn].position.z);
		Check_fscanf(catch_err, 3, "NEURON position", __LINE__ - 1, file_in);
	

		//create the FDOMAINS structures associated with this NEURON
		neuron[nrn].n_fdomains = population[pop].n_fdomains; //redundant but useful
    neuron[nrn].fdomain = calloc( neuron[nrn].n_fdomains, sizeof(struct FDOMAIN) );
		Initialize_FDomains( neuron[nrn].fdomain, nrn, file_in );
    for (fdm = 0; fdm < neuron[nrn].n_fdomains; fdm++) {	  
      catch_err = fscanf(file_in, "\n\t%lf\t%lf", &neuron[nrn].fdomain[fdm].Ra, &neuron[nrn].fdomain[fdm].V[0]);
			Check_fscanf(catch_err, 2, "NEURON fdomains", __LINE__ - 1, file_in);


			//state variables
			n_statevars = population[pop].fdomain[fdm].model->n_statevars;
			if( n_statevars > 0 ) {
				neuron[nrn].fdomain[fdm].model_statevar = calloc( n_statevars, sizeof( double * ) );
				if( neuron[nrn].fdomain[fdm].model_statevar == NULL ) {
					printf("\n\tERROR: unable to allocate memory for NEURON model state variables. Failed at neuron %ld, fdomain %d.\n\n", nrn, fdm);
					Exit_Error(3,file_in);
				}  
			}
			for (i = 0; i < n_statevars; i++) {	  
				neuron[nrn].fdomain[fdm].model_statevar[i] = calloc( NUMERICAL_ORDER, sizeof( double ) );
				if( neuron[nrn].fdomain[fdm].model_statevar[i] == NULL ) {
					printf("\n\tERROR: unable to allocate memory for NEURON model state variables. Failed at neuron %ld, fdomain %d.\n\n", nrn, fdm);
					Exit_Error(3,file_in);
				}  

				catch_err = fscanf(file_in, "\t%lf", &neuron[nrn].fdomain[fdm].model_statevar[i][0]);
				Check_fscanf(catch_err, 1, "NEURON fdomains", __LINE__ - 1, file_in);
			}

			
			//parameters
			n_params = population[pop].fdomain[fdm].model->n_params;
			if( n_params > 0 ) {
				neuron[nrn].fdomain[fdm].model_param = calloc( n_params, sizeof( double ) );
				if( neuron[nrn].fdomain[fdm].model_param == NULL ) {
					printf("\n\tERROR: unable to allocate memory for NEURON model parameters. Failed at neuron %ld, fdomain %d.\n\n", nrn, fdm);
					Exit_Error(3,file_in);
				}  
			}
			for (i = 0; i < n_params; i++) {	  
				catch_err = fscanf(file_in, "\t%lf", &neuron[nrn].fdomain[fdm].model_param[i]);		
				Check_fscanf(catch_err, 1, "NEURON fdomains", __LINE__ - 1, file_in);
			}
    }

	
    //SPIKES
    catch_err = fscanf(file_in, "\n\tSPIKES:\t%d", &n_spks);
		Check_fscanf(catch_err, 1, "SPIKES", __LINE__ - 1, file_in);
    if (n_spks > N_SPIKE_TIMES) {
      printf("\n\tIMPORTANT: the size of a list of recent spike times in your .net file for neuron %ld is larger than the predifined NeuralSyns allocation ring size (%d). Older spike times will be discarded!", nrn, N_SPIKE_TIMES);
    }
    //load data
    for (spk = 0; spk < n_spks; spk++) {
      catch_err = fscanf(file_in, "\t%lf", &neuron[nrn].spike_time[spk]);
			Check_fscanf(catch_err, 1, "SPIKES", __LINE__ - 1, file_in);
    }
    if( n_spks > 0 ) {
			neuron[nrn].last_spike_time_index = spk - 1;
		}
		else {
			neuron[nrn].last_spike_time_index = 0;
		}
	

    //SYNAPSES associated with this NEURON
    catch_err = fscanf(file_in, "\n\tSYNAPSES:\t%ld", &neuron[nrn].n_synapses);
		Check_fscanf(catch_err, 1, "NEURON's SYNAPSES list", __LINE__ - 1, file_in);
		if( neuron[nrn].n_synapses > 0 ) {
			neuron[nrn].synapse = calloc( neuron[nrn].n_synapses, sizeof(struct SYNAPSE) );
			Initialize_Synapses( neuron[nrn].synapse, nrn, file_in);
		}
		else {
			neuron[nrn].synapse = NULL;
		}
		
    //go through all synapses now...
    for (syn = 0; syn < neuron[nrn].n_synapses; syn++) {
			
      //all synaptic data      
      catch_err = fscanf(file_in, "\n\t\t@\t%ld\t%lf\t%lf", &neuron[nrn].synapse[syn].target_neuron, &neuron[nrn].synapse[syn].weight, &neuron[nrn].synapse[syn].delay);
			Check_fscanf(catch_err, 3, "NEURON synapses", __LINE__ - 1, file_in);
      catch_err = fscanf(file_in, "\n\t\t\t%d\t%hd\t%lf", &neuron[nrn].synapse[syn].spikes_cue, &neuron[nrn].synapse[syn].activated, &neuron[nrn].synapse[syn].psr_time);
			Check_fscanf(catch_err, 3, "NEURON synapses", __LINE__ - 1, file_in);
			

			//********************************

			//synapse current model state variables and parameters
      catch_err = fscanf(file_in, "\n\t\t\t%s", model_name);			
			Check_fscanf(catch_err, 1, "NEURON synapses models", __LINE__ - 1, file_in);
			model_current = GetSynapseModelFromString( model_name );
			if( model_current == NULL ) {
				printf("\n\tERROR reports to SYNAPSE %ld in NEURON %ld in the .net input file %s", syn_index, nrn, network_file);
				printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				fclose(file_in);
				exit(-1);
			} 

			//state variables for current model
			n_statevars = model_current->n_statevars;
			if( n_statevars > 0 ) { //this is needed otherwise, even with n_statevars=0, it allocates space to accomodate the values for all orders (NUMERICAL_ORDER)
				neuron[nrn].synapse[syn].current_statevar = calloc( n_statevars, sizeof( double * ) );  
				if( neuron[nrn].synapse[syn].current_statevar == NULL ) {
					printf("\n\tERROR: unable to allocate memory for SYNAPSES current model state variables for NEURON %ld", nrn);
					printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
					Exit_Error(3,file_in);
				}
			}
			for( i = 0; i < n_statevars; i++ ){
				neuron[nrn].synapse[syn].current_statevar[i] = calloc( NUMERICAL_ORDER, sizeof( double ) );  
				if( neuron[nrn].synapse[syn].current_statevar[i] == NULL ) {
					printf("\n\tERROR: unable to allocate memory for SYNAPSES current model state variables for NEURON %ld", nrn);
					printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
					Exit_Error(3,file_in);
				}

				catch_err = fscanf(file_in, "\t%lf", &neuron[nrn].synapse[syn].current_statevar[i][0]);	
				Check_fscanf(catch_err, 1, "NEURON synapses models", __LINE__ - 1, file_in);
			} 

			if( flags.homogeneous_synapses == 0 ) {
				n_params = model_current->n_params;
				neuron[nrn].synapse[syn].current_param = calloc( n_params, sizeof( double ) ); 
				if( neuron[nrn].synapse[syn].current_param == NULL ) {
					printf("\n\tERROR: unable to allocate memory for SYNAPSES current model parameters for NEURON %ld", nrn);
					printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
					Exit_Error(3,file_in);
				}
				for( i = 0; i < model_current->n_params; i++ ){
					catch_err = fscanf(file_in, "\t%lf", &neuron[nrn].synapse[syn].current_param[i]);	
					Check_fscanf(catch_err, 1, "NEURON synapses models", __LINE__ - 1, file_in);
				} 
			}

			//********************************

			//plasticity model state variables and parameters
      catch_err = fscanf(file_in, "\n\t\t\t%s", model_name);			
			Check_fscanf(catch_err, 1, "NEURON synapses models", __LINE__ - 1, file_in);
			model_plasticity = GetPlasticityModelFromString( model_name );
			if( model_plasticity == NULL ) {
				printf("\n\tERROR reports to SYNAPSE %ld in NEURON %ld in the .net input file %s", syn_index, nrn, network_file);
				printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				fclose(file_in);
				exit(-1);
			}  

			//state variables for plasticity model
			n_statevars = model_plasticity->n_statevars;
			if( n_statevars > 0 ) { //this is needed otherwise, even with n_statevars=0, it allocates space to accomodate the values for all orders (NUMERICAL_ORDER)
				neuron[nrn].synapse[syn].plasticity_statevar = calloc( n_statevars, sizeof( double * ) );  
				if( neuron[nrn].synapse[syn].plasticity_statevar == NULL ) {
					printf("\n\tERROR: unable to allocate memory for SYNAPSES plasticity model state variables for NEURON %ld", nrn);
					printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
					Exit_Error(3,file_in);
				}
			}
			for( i = 0; i < n_statevars; i++ ){
				neuron[nrn].synapse[syn].plasticity_statevar[i] = calloc( NUMERICAL_ORDER, sizeof( double ) );  
				if( neuron[nrn].synapse[syn].plasticity_statevar[i] == NULL ) {
					printf("\n\tERROR: unable to allocate memory for SYNAPSES plasticity model state variables for NEURON %ld", nrn);
					printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
					Exit_Error(3,file_in);
				}

				catch_err = fscanf(file_in, "\t%lf", &neuron[nrn].synapse[syn].plasticity_statevar[i][0]);	
				Check_fscanf(catch_err, 1, "NEURON synapses models", __LINE__ - 1, file_in);
			} 

			if( flags.homogeneous_synapses == 0 ) {
				n_params = model_plasticity->n_params;
				neuron[nrn].synapse[syn].plasticity_param = calloc( n_params, sizeof( double ) ); 
				if( neuron[nrn].synapse[syn].plasticity_param == NULL ) {
					printf("\n\tERROR: unable to allocate memory for SYNAPSES plasticity model parameters for NEURON %ld", nrn);
					printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
					Exit_Error(3,file_in);
				}
				for( i = 0; i < model_plasticity->n_params; i++ ){
					catch_err = fscanf(file_in, "\t%lf", &neuron[nrn].synapse[syn].plasticity_param[i]);	
					Check_fscanf(catch_err, 1, "NEURON synapses models", __LINE__ - 1, file_in);
				} 
			}

			//********************************

      catch_err = fscanf(file_in, "\n\n");
			
    }
		
  }
	
	
	catch_err = fscanf(file_in, "\n\n");
  temp_ptr = fgets(temp_line, 256, file_in);
	
  if (strcmp(temp_line, "END\n") != 0) {
    printf("\n\tERROR: Reading finished before END keyword\n\n");
		Exit_Error(3,file_in);
  }
	
  fclose(file_in);
	
  printf("\nOK, all data loaded!\n");

  return 0;
}





//------------------------------------------------------------------------

/**
 * Writes a network model data file (.net) containing all model information
 * \param *network_file network model file name 
 * \return return 0 if the process goes well
 */

int Write_Network(char *network_file)
{
  FILE *out;

  unsigned long nrn, syn;
  unsigned int i, pop, pop_i, pop_o, spk, n_spks, fdm, con, m, n_statevars, n_params;

  struct MODEL_DYNAMICS_SYNAPSE *model_current;
  struct MODEL_DYNAMICS_PLASTICITY *model_plasticity;


  out = fopen(network_file, "w+");
  if (out == NULL) {
    printf("\n\tERROR: unable to create and write the output network data file %s\n", network_file);
    exit(-1);
  }

  fprintf(out, "START\n");

  //MODELS
  fprintf(out, "\nMODELS: %d", N_NEURON_MODELS + N_SYNAPSE_MODELS + N_PLASTICITY_MODELS);
  for( m = 0; m < N_NEURON_MODELS; m++) {
		fprintf(out, "\n\t%s", neuron_model[m].model_label);
		n_statevars = neuron_model[m].n_statevars;
		fprintf(out, "\t%d", n_statevars);
		for( i=0; i<n_statevars; i++ ) {
			fprintf(out, "\t%s", neuron_model[m].statevar_label[i]);
		}
		n_params = neuron_model[m].n_params;
		fprintf(out, "\t%d", n_params);
		for( i=0; i<n_params; i++ ) {
			fprintf(out, "\t%s", neuron_model[m].param_label[i]);
		}
  }
  for( m = 0; m < N_SYNAPSE_MODELS; m++) {
		fprintf(out, "\n\t%s", synapse_model[m].model_label);
		n_statevars = synapse_model[m].n_statevars;
		fprintf(out, "\t%d", n_statevars);
		for( i=0; i<n_statevars; i++ ) {
			fprintf(out, "\t%s", synapse_model[m].statevar_label[i]);
		}
		n_params = synapse_model[m].n_params;
		fprintf(out, "\t%d", n_params);
		for( i=0; i<n_params; i++ ) {
			fprintf(out, "\t%s", synapse_model[m].param_label[i]);
		}
  }
  for( m = 0; m < N_PLASTICITY_MODELS; m++) {
		fprintf(out, "\n\t%s", plasticity_model[m].model_label);
		n_statevars = plasticity_model[m].n_statevars;
		fprintf(out, "\t%d", n_statevars);
		for( i=0; i<n_statevars; i++ ) {
			fprintf(out, "\t%s", plasticity_model[m].statevar_label[i]);
		}
		n_params = plasticity_model[m].n_params;
		fprintf(out, "\t%d", n_params);
		for( i=0; i<n_params; i++ ) {
			fprintf(out, "\t%s", plasticity_model[m].param_label[i]);
		}
  }
  

  //POPULATIONS total_populations
  fprintf(out, "\n\nPOPULATIONS: %d", total_populations);
	fprintf(out, "\n\tlabel [char*10]:");
  for (pop = 0; pop < total_populations; pop++) {
    population[pop].label[10] = '\0';	//force end of string; allows 10 chars in the string
    fprintf(out, "\t%s", population[pop].label);
  }
  fprintf(out, "\n\tsize [#]:");
  for (pop = 0; pop < total_populations; pop++) {
    fprintf(out, "\t%ld", population[pop].n_neurons);
  }
  fprintf(out, "\n\tfdomains [#]:");
  for (pop = 0; pop < total_populations; pop++) {
	fprintf(out, "\t%d", population[pop].n_fdomains);
  }

  
  //FDOMAINS
  fprintf(out, "\n\nFDOMAINS:");
  for (pop = 0; pop < total_populations; pop++) {
    fprintf(out, "\n\t%s", population[pop].label);
    for (fdm = 0; fdm < population[pop].n_fdomains; fdm++) {
      fprintf(out, "\t[\t%d\t%s\t%d\t(%.3lf,%.3lf,%.3lf,%.3lf)\t%s\t]", fdm, population[pop].fdomain[fdm].label, population[pop].fdomain[fdm].parent_fdomain, population[pop].fdomain[fdm].position.x, population[pop].fdomain[fdm].position.y, population[pop].fdomain[fdm].position.z, population[pop].fdomain[fdm].position.r, population[pop].fdomain[fdm].model->model_label);
		} //repeat for all fdomains
  } //repeat for all populations
  
  
  //HOMOGENEOUS SYNAPSES
  if( flags.homogeneous_synapses == 1 ) {
		fprintf(out, "\n\nHOMOGENEOUS SYNAPSES: YES");
  }
  else {
		fprintf(out, "\n\nHOMOGENEOUS SYNAPSES: NO");
  }
  

  //CONNECTION PAIRS
  fprintf(out, "\n\nCONNECTION PAIRS:\t%d", total_connections);
	con = 0;
	for( pop_i=0; pop_i<total_populations; pop_i++ ) {
	  for( pop_o=0; pop_o<total_populations; pop_o++ ) {
			if( connection[pop_i][pop_o].set == 1 ) {
				fdm = connection[pop_i][pop_o].fdomain;
				fprintf(out, "\n\tPAIR:\t%d\t[ %s %s ]\t%s\t[ %d %d ]\t%d", con, population[pop_i].label, population[pop_o].label, population[pop_o].fdomain[fdm].label, pop_i, pop_o, fdm);
				//current model
				fprintf(out, "\n\t\t%s", connection[pop_i][pop_o].current_model->model_label);
				if( flags.homogeneous_synapses == 1 ) {
					n_params = connection[pop_i][pop_o].current_model->n_params;
					for( i=0; i<n_params; i++ ) {
						fprintf(out, "\t%lg", connection[pop_i][pop_o].current_param[i]);	
					}
				}				
				//plasticity model
				fprintf(out, "\n\t\t%s", connection[pop_i][pop_o].plasticity_model->model_label);
				if( flags.homogeneous_synapses == 1 ) {
					n_params = connection[pop_i][pop_o].plasticity_model->n_params;
					for( i=0; i<n_params; i++ ) {
						fprintf(out, "\t%lg", connection[pop_i][pop_o].plasticity_param[i]);	
					}
				}		  

				con++;
			}
	  }
	}
	

  //NEURONS total_neurons
  fprintf(out, "\n\nNEURONS: %ld", total_neurons);
  //SYNAPSES total_synapses  
  fprintf(out, "\nSYNAPSES: %ld\n", total_synapses);


  // WRITE ALL NEURONS AND SYNAPSES DATA; here we go...
  //variables needed to keep track of population id within the nrn cycle
  printf("\nNeurons writen: ");
  for (nrn = 0; nrn < total_neurons; nrn++) {  

    fprintf(out, "\nNEURON\t%ld", nrn);
    printf(".");
    fflush(stdout);

		pop = neuron[nrn].population;
	
    fprintf(out, "\n\t(%.3lf, %.3lf, %.3lf)", neuron[nrn].position.x, neuron[nrn].position.y, neuron[nrn].position.z);

    for (fdm = 0; fdm < neuron[nrn].n_fdomains; fdm++) {	  
      fprintf(out, "\n\t%.3lf\t%.3lf", neuron[nrn].fdomain[fdm].Ra, neuron[nrn].fdomain[fdm].V[0]);
			n_statevars = population[pop].fdomain[fdm].model->n_statevars;
			for (i = 0; i < n_statevars; i++) {	  
				fprintf(out, "\t%lg", neuron[nrn].fdomain[fdm].model_statevar[i][0]);		
			}
			n_params = population[pop].fdomain[fdm].model->n_params;
			for (i = 0; i < n_params; i++) {	  
				fprintf(out, "\t%lg", neuron[nrn].fdomain[fdm].model_param[i]);		
			}
    }
		
    //SPIKES
		//start by checking how many recent spikes exist (avoid writing all the N_SPIKE_TIMES with the LONG_TIME_AGO time)
		n_spks = 0;
    for (spk = 0; spk < N_SPIKE_TIMES; spk++) {
			if( neuron[nrn].spike_time[spk] > LONG_TIME_AGO ) {
				n_spks++;
			}
		}		
    fprintf(out, "\n\tSPIKES:\t%d", n_spks);
    for (spk = 0; spk < n_spks; spk++) {
			i = ( neuron[nrn].last_spike_time_index + N_SPIKE_TIMES - n_spks + spk + 1 ) % N_SPIKE_TIMES;
      fprintf(out, "\t%lg", neuron[nrn].spike_time[i] - t);
    }

    //SYNAPSES associated with this NEURON
    fprintf(out, "\n\tSYNAPSES:\t%ld", neuron[nrn].n_synapses);
    for (syn = 0; syn < neuron[nrn].n_synapses; syn++) {

      fprintf(out, "\n\t\t@\t%ld\t%lg\t%.3lf", neuron[nrn].synapse[syn].target_neuron, neuron[nrn].synapse[syn].weight, neuron[nrn].synapse[syn].delay);
      fprintf(out, "\n\t\t\t%d\t%hd\t%.3lf", neuron[nrn].synapse[syn].spikes_cue, neuron[nrn].synapse[syn].activated, neuron[nrn].synapse[syn].psr_time);
			
			pop_i = neuron[nrn].population;
			pop_o = neuron[ neuron[nrn].synapse[syn].target_neuron ].population;
			
			model_current = connection[pop_i][pop_o].current_model;
			fprintf(out, "\n\t\t\t%s", model_current->model_label);			
			n_statevars = model_current->n_statevars;
			for( i = 0; i < n_statevars; i++ ){
				fprintf(out, "\t%lg", neuron[nrn].synapse[syn].current_statevar[i][0]);	
			} 
			if( flags.homogeneous_synapses == 0 ) {
				n_params = model_current->n_params;
				for( i = 0; i < model_current->n_params; i++ ){
					fprintf(out, "\t%lg", neuron[nrn].synapse[syn].current_param[i]);	
				} 
			}
			
			model_plasticity = connection[pop_i][pop_o].plasticity_model;
			fprintf(out, "\n\t\t\t%s", model_plasticity->model_label);			
			n_statevars = model_plasticity->n_statevars;
			for( i = 0; i < n_statevars; i++ ){
				fprintf(out, "\t%lg", neuron[nrn].synapse[syn].plasticity_statevar[i][0]);	
			} 
			if( flags.homogeneous_synapses == 0 ) {
				n_params = model_plasticity->n_params;
				for( i = 0; i < model_plasticity->n_params; i++ ){
					fprintf(out, "\t%lg", neuron[nrn].synapse[syn].plasticity_param[i]);	
				} 
			}
			
			fprintf(out, "\n");	  
		}
		
		if( neuron[nrn].n_synapses == 0 ) {
			fprintf(out, "\n");
		}
		
  }
  
  fprintf(out, "\nEND\n");

  fclose(out);
	
  printf(" OK!\n");

  return 0;
}





//------------------------------------------------------------------------

/**
 * Writes the present "network state" (membrane potentials in all fdomains) into a file
 * \param *state_file file name 
 * \param mode selects if all fdomains are written or just the soma
 */

void Write_State_File(FILE * state_file, short mode)
{
  int fdm;
  long nrn;

  fprintf(state_file, "\n%.4f ", t);

  for( nrn = 0; nrn < total_neurons; nrn++ ) {    
    fprintf( state_file, "%.4f ", neuron[nrn].fdomain[0].V[0] );    
    if( mode == 2 ) {
      for( fdm = 1; fdm < neuron[nrn].n_fdomains; fdm++ ) {
				fprintf(state_file, "%.4f ", neuron[nrn].fdomain[fdm].V[0]);      		
			}
		}
  }

  return;
}





//------------------------------------------------------------------------

/**
 * Checks if the number of successful conversions match the number of inputs; used with the fscanf function
 * \param n_conversions int returned by fscanf
 * \param n_inputs int with the desired number of inputs to be read
 * \param *message error message to provide
 * \param line source code file line number where the error occurred
 * \param *in file to be closed before program finishes
 */

void Check_fscanf(int n_conversions, int n_inputs, char *message, int line, FILE *in)
{
  if( n_conversions == n_inputs ) {
		return;
  }
  else {
		printf("\n\tERROR unable to properly read the section %s in the input network file", message);
		printf("\n\tERROR occured in dataio.c: %d\n\n", line);
		fclose(in);
		exit(-1);
  }
}
