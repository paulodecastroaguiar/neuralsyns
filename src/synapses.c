//************************************************************************
//*                                                                      *
//*                          S Y N A P S E S                             *
//*                                                                      *
//************************************************************************

/**
 * \file synapses.c
 * \author Paulo Aguiar
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "general.h"
#include "models.h"
#include "dataio.h"
#include "populations.h"
#include "neurons.h"
#include "synapses.h"

extern struct POPULATION *population;
extern unsigned int total_populations;
extern struct NEURON *neuron;
extern unsigned long total_neurons;
extern unsigned long total_synapses;
extern struct CONNECTION **connection;

extern struct MODEL_DYNAMICS_NEURON *neurons_model;
extern struct MODEL_DYNAMICS_SYNAPSE *synapses_model;
extern struct MODEL_DYNAMICS_PLASTICITY *plasticity_model;

extern  struct FLAGS flags;

extern double t;
extern double dt;

extern double error;





//------------------------------------------------------------------------

/**
 * Initializes all pointers in the SYNAPSES associated with a specific NEURON
 * \param *p pointer to SYNAPSE structure
 * \param nrn neuron id
 * \param *file_in input .net file
 */

void Initialize_Synapses( struct SYNAPSE *p, long nrn, FILE *file_in)
{
	unsigned long syn;

	if( p == NULL ) {
		printf("\n\tERROR: unable to allocate memory for list of SYNAPSES for NEURON %ld", nrn);
		Free_Neurons();
		Free_Connections();
		Free_Populations();
		fclose(file_in);
		exit(-1);
	}
	else {

		for( syn = 0; syn < neuron[nrn].n_synapses; syn++ ) {
			p[syn].current_param = NULL;
			p[syn].current_statevar = NULL;
			p[syn].plasticity_param = NULL;
			p[syn].plasticity_statevar = NULL;
		}

	}


	return;
}





//------------------------------------------------------------------------

/**
 * Calculates and assigns all the constants and initial values
 */

void Prepare_Synapses(void)
{
  unsigned long nrn, syn, index;
  unsigned int pop_i, pop_o;
  double t1;

  for (nrn = 0; nrn < total_neurons; nrn++) {
		for (syn = 0; syn < neuron[nrn].n_synapses; syn++) {

			//set synapse's last_release_time and next_release_time; NOTICE that neuron[nrn].spike_time has LONG_TIME_AGO in entries without a recent spike time!
			index = neuron[nrn].last_spike_time_index;
			t1 = neuron[nrn].spike_time[index];
			neuron[nrn].synapse[syn].next_release_time = t1 + neuron[nrn].synapse[syn].delay;
			
			index = (N_SPIKE_TIMES + index - 1) % N_SPIKE_TIMES;
			t1 = neuron[nrn].spike_time[index];
			neuron[nrn].synapse[syn].last_release_time = t1 + neuron[nrn].synapse[syn].delay;
		
			pop_i = neuron[nrn].population;
			pop_o = neuron[ neuron[nrn].synapse[syn].target_neuron ].population;

			//point the parameters pointer to CONNECTION if the synapse is homogeneous
			if( flags.homogeneous_synapses == 1 ) {
				neuron[nrn].synapse[syn].current_param = connection[pop_i][pop_o].current_param;
				neuron[nrn].synapse[syn].plasticity_param = connection[pop_i][pop_o].plasticity_param;
			}

			//initialize synapse and plasticity models
			connection[pop_i][pop_o].current_model    -> Initialize( syn, nrn, neuron[nrn].synapse[syn].target_neuron, connection[pop_i][pop_o].fdomain );
			connection[pop_i][pop_o].plasticity_model -> Initialize( syn, nrn, neuron[nrn].synapse[syn].target_neuron, connection[pop_i][pop_o].fdomain );

		}
  }


  return;
}





//------------------------------------------------------------------------

/**
 * Updates all SYNAPSE structures
 */

void Update_Synapses(void)
{
  int pop_i, pop_o;
  unsigned long nrn, syn;

	//this is _THE_ bottleneck

  #pragma omp parallel for private( nrn, syn, pop_i, pop_o,  )
  for( nrn = 0; nrn < total_neurons; nrn++ ) {
		for( syn = 0; syn < neuron[nrn].n_synapses; syn++ ) {
	  
			//UPDATE SPIKEs
			if( neuron[nrn].synapse[syn].spikes_cue > 0 ) {				

				if (t >= neuron[nrn].synapse[syn].next_release_time) { //check if spike has reached the synapse

					Remove_Spike(nrn, syn);	//remove spike from cue
									
					//ACTIVATE Plasticity
					pop_i = neuron[nrn].population;
					pop_o = neuron[ neuron[nrn].synapse[syn].target_neuron ].population;
					connection[pop_i][pop_o].plasticity_model -> Activate( neuron[nrn].synapse[syn].next_release_time, syn, nrn, neuron[nrn].synapse[syn].target_neuron, connection[pop_i][pop_o].fdomain );

					//ACTIVATE Currents
					Add_PSC(nrn, syn, neuron[nrn].synapse[syn].next_release_time);
					pop_i = neuron[nrn].population;
					pop_o = neuron[ neuron[nrn].synapse[syn].target_neuron ].population;
					connection[pop_i][pop_o].current_model -> Activate( neuron[nrn].synapse[syn].next_release_time, syn, nrn, neuron[nrn].synapse[syn].target_neuron, connection[pop_i][pop_o].fdomain );
					
				}
			}

			//ADVANCE Currents
			if( neuron[nrn].synapse[syn].activated == 1 ) {
				pop_i = neuron[nrn].population;
				pop_o = neuron[ neuron[nrn].synapse[syn].target_neuron ].population;
				connection[pop_i][pop_o].current_model -> Advance( syn, nrn, neuron[nrn].synapse[syn].target_neuron, connection[pop_i][pop_o].fdomain );				
				if ( neuron[nrn].synapse[syn].activated == 0 ) { //remove PSCs that have been signaled by the Advance function as "too small"
					Remove_PSC(nrn, syn);
				}
				else {
					neuron[nrn].synapse[syn].psr_time += dt;	  	 //move PSCs in time
				}
			}
			
		}
  }

  return;
}





//------------------------------------------------------------------------

/**
 * Add a spike to the synapse's cue
 * \param nrn id of the NEURON structure
 * \param time present simulation time
 */

void Add_Spike(long nrn, double time)
{
  unsigned long syn;

  for( syn=0; syn<neuron[nrn].n_synapses; syn++ ) {
		if( neuron[nrn].synapse[syn].spikes_cue == 0 ) {
			neuron[nrn].synapse[syn].next_release_time = time + neuron[nrn].synapse[syn].delay;
		}
		neuron[nrn].synapse[syn].spikes_cue++;  
  }
	
  //update the total number of spikes in the population:
  //this is done in Update_Spike_Times where, whenever a neuron fires, n_synapses spikes are added
  
  //update the total number of spikes in the neuron:
  neuron[nrn].spikes_counter++;		
    
  return;
}





//------------------------------------------------------------------------

/**
 * Remove a reached spike from the synapse's cue
 * \param nrn id of the NEURON structure
 * \param syn id of the SYNAPSE structure
 */

void Remove_Spike(long nrn, long syn)
{
  unsigned int index;

  neuron[nrn].synapse[syn].spikes_cue--;

  if (neuron[nrn].synapse[syn].spikes_cue > 0 ) {
    
    index = ( N_SPIKE_TIMES + neuron[nrn].last_spike_time_index - neuron[nrn].synapse[syn].spikes_cue + 1) % N_SPIKE_TIMES;

    neuron[nrn].synapse[syn].next_release_time = neuron[nrn].spike_time[index] + neuron[nrn].synapse[syn].delay;
    
  }
  
  //update the total number of spikes in the population 
  population[ neuron[nrn].population ].total_spikes--;

  return;
}





//------------------------------------------------------------------------

/**
 * Add a psc to the synapse's cue
 * \param nrn id of the NEURON structure
 * \param syn id of the SYNAPSE structure
 * \param time present simulation time
 */

void Add_PSC(long nrn, long syn, double time)
{
  neuron[nrn].synapse[syn].activated = 1;
  neuron[nrn].synapse[syn].psr_time = 0.0;

  neuron[nrn].synapse[syn].last_release_time = time;

  //update the total number of pscs in the population 
  population[ neuron[nrn].population ].total_pscs++;

  return;
}





//------------------------------------------------------------------------

/**
 * Remove a very week psc from the synapse's cue
 * \param nrn id of the NEURON structure
 * \param syn id of the SYNAPSE structure
 */

void Remove_PSC(long nrn, long syn)
{
  neuron[nrn].synapse[syn].activated = 0;
  neuron[nrn].synapse[syn].psr_time = 0.0;

  //update the total number of pscs in the population 
  population[ neuron[nrn].population ].total_pscs--;

  return;
}





//------------------------------------------------------------------------

/**
 * Initializes all pointers in the CONNECTION structures
 * \param *p pointer to CONNECTION structure
 * \param *file_in input .net file
 */

void Initialize_Connections(struct CONNECTION **p, FILE *file_in)
{
  unsigned int pop_i, pop_o;
  

  for( pop_i=0; pop_i<total_populations; pop_i++ ) {
	  
		if( p[pop_i] == NULL ) {
			printf("\n\tERROR: unable to allocate memory for list of CONNECTION. Failed at population array %d", pop_i);
			fclose(file_in);
			Free_Populations();
			exit(-1);
		}
		else {
			
			for( pop_o=0; pop_o<total_populations; pop_o++ ) {

				p[pop_i][pop_o].label = calloc( MAX_STRING_SIZE, sizeof( char ) );
				if( p[pop_i][pop_o].label == NULL ) {
					printf("\n\tERROR: unable to allocate memory for CONNECTION label. Failed at population pair (%d,%d)", pop_i, pop_o);
					fclose(file_in);
					Free_Populations();
					exit(-1);
				}
				p[pop_i][pop_o].set = 0;
				p[pop_i][pop_o].label[0] = '\0';
				p[pop_i][pop_o].current_model = NULL;
				p[pop_i][pop_o].current_param = NULL;
				p[pop_i][pop_o].plasticity_model = NULL;
				p[pop_i][pop_o].plasticity_param = NULL;
			}
			
		}
		
	}
	
  return;
}





//------------------------------------------------------------------------

/**
 * Calculates and assigns all the constants and initial values
 */

void Prepare_Connections(void)
{
  unsigned int pop_i, pop_o;
  
  //CHECK IF ALL SYNAPSES CONNECT TO EXISTING FDOMAINS
  for( pop_i=0; pop_i<total_populations; pop_i++ ) {
		for( pop_o=0; pop_o<total_populations; pop_o++ ) {
	  
			if( connection[pop_i][pop_o].label != NULL ) {
				if( connection[pop_i][pop_o].fdomain > population[pop_o].n_fdomains - 1 ) {
					printf("\n\tERROR: Synaptic connections to unexistent fdomains have been detected!");
					printf("\n\t       Check in your .net file, the CONNECTION between population %s and population %s\n\n", population[pop_i].label, population[pop_o].label);
					Free_Mem();
					exit(-1);
				}				
			}
			
		}
  }
	
  return;
}





//------------------------------------------------------------------------

/**
 * Free allocated space for CONNECTION
 */

void Free_Connections(void)
{
	unsigned int pop_i, pop_o;

	for( pop_i=0; pop_i<total_populations; pop_i++ ) {

		for( pop_o=0; pop_o<total_populations; pop_o++ ) {

			free( connection[pop_i][pop_o].label );

			//models
			if( flags.homogeneous_synapses == 1 ) {
				free( connection[pop_i][pop_o].current_param );
				free( connection[pop_i][pop_o].plasticity_param );
			}
			else {
				connection[pop_i][pop_o].current_param = NULL;     //sorry, can't help it...
				connection[pop_i][pop_o].plasticity_param = NULL;
			}
			connection[pop_i][pop_o].current_model = NULL; 
			connection[pop_i][pop_o].plasticity_model = NULL; 

		}
		free( connection[pop_i] );

	}

	free( connection );

	return;
}
