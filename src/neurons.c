//************************************************************************
//*                                                                      *
//*                           N E U R O N S                              *
//*                                                                      *
//************************************************************************

/**
 * \file neurons.c
 * \author Paulo Aguiar
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_sf_trig.h>
#include <gsl/gsl_math.h>

#include "general.h"
#include "models.h"
#include "populations.h"
#include "neurons.h"
#include "synapses.h"

extern struct POPULATION *population;
extern unsigned int total_populations;
extern struct NEURON *neuron;
extern unsigned long total_neurons;
extern struct CONNECTION **connection;

extern struct MODEL_DYNAMICS_NEURON *neurons_model;
extern struct MODEL_DYNAMICS_SYNAPSE *synapses_model;
extern struct MODEL_DYNAMICS_PLASTICITY *plasticity_model;

extern double t;
extern double dt;

extern struct FLAGS flags;

extern double error;





//------------------------------------------------------------------------

/**
 * Initializes all pointers in the NEURON structures
 * \param *p pointer to NEURON structure
 * \param *file_in input .net file
 */

void Initialize_Neurons(struct NEURON *p, FILE *file_in)
{
	unsigned int spk;
	unsigned long nrn;

	if (p == NULL) {
		printf("\n\tERROR: unable to allocate memory for list of NEURONS");
		printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		Free_Connections();
		Free_Populations();
		fclose(file_in);
		exit(-1);
	}
	else {

		for( nrn = 0; nrn < total_neurons; nrn++ ) {

			p[nrn].spike_time = calloc( N_SPIKE_TIMES, sizeof( double ) );
			if( p[nrn].spike_time == NULL ) {
				printf("\n\tERROR: unable to allocate memory for list of recent spike times in NEURON %ld", nrn);
				Free_Neurons();
				Free_Connections();
				Free_Populations();		
				fclose(file_in);
				exit(-1);
			}
			//reset spike_time ring; be sure that they do not affect the freq estimate
			for (spk = 0; spk < N_SPIKE_TIMES; spk++) {
				neuron[nrn].spike_time[spk] = LONG_TIME_AGO;
			}

			p[nrn].synapse = NULL;
			p[nrn].fdomain = NULL;
		}
		
	}

  return;
}





//------------------------------------------------------------------------

/**
 * Initializes all pointers in the NEURON FDOMAIN structures
 * \param *p pointer to FDomains structure
 * \param *file_in input .net file
 * \param nrn neuron id
 */

void Initialize_FDomains(struct FDOMAIN *p, long nrn, FILE *file_in)
{
	unsigned int fdm;

	if( p == NULL ) {
		printf("\n\tERROR: unable to allocate memory for list of FDOMAINS inside neuron %ld", nrn);
		Free_Neurons();
		Free_Connections();
		Free_Populations();
		fclose(file_in);
		exit(-1);
	}
	else {

		for( fdm = 0; fdm < neuron[nrn].n_fdomains; fdm++ ) {

			p[fdm].V = calloc( NUMERICAL_ORDER, sizeof( double ) );
			p[fdm].I_total_syn = calloc( NUMERICAL_ORDER, sizeof( double ) );
			p[fdm].I_total_axial = calloc( NUMERICAL_ORDER, sizeof( double ) );
			p[fdm].I_total_electrode = calloc( NUMERICAL_ORDER, sizeof( double ) );

			if( p[fdm].V == NULL || p[fdm].I_total_syn == NULL || p[fdm].I_total_axial == NULL || p[fdm].I_total_electrode == NULL ) {
				printf("\n\tERROR: unable to allocate memory for core state variables in fdomain %d inside neuron %ld", fdm, nrn);
				Free_Neurons();
				Free_Connections();
				Free_Populations();
				fclose(file_in);
				exit(-1);
			}		

			p[fdm].model_param = NULL;
			p[fdm].model_statevar = NULL;
			p[fdm].gsl = NULL;
		}

	}

	return;
}





//------------------------------------------------------------------------

/**
 * Calculates and assigns all constants and initial values to NEURON structures
 */

void Prepare_Neurons(void)
{
  unsigned long nrn;

  for (nrn = 0; nrn < total_neurons; nrn++) {
		//prepare fdomain compartments 
		Prepare_FDomains(nrn);
		//set frequencies to 0.0 Hz
		neuron[nrn].firing_rate = 0.0;
		//prepare counter
		neuron[nrn].spikes_counter = 0;
		//clean all stim_fire flags
		neuron[nrn].spiketime_from_electrode = -1.0;
  }

  return;
}



//------------------------------------------------------------------------

/**
 * Calculates and assigns all the constants and initial values to FDOMAINS structures
 * \param nrn id of the NEURON structure
 */

void Prepare_FDomains(long nrn)
{
  unsigned int fdm, pop, order, n_statevars, s;

  neuron[nrn].n_fdomains = population[neuron[nrn].population].n_fdomains;

  //prepare all fdomain compartments
  for (fdm = 0; fdm < neuron[nrn].n_fdomains; fdm++) {
		//currents ( notice that calloc was used, so all data was initialized to 0 anyway...)
		for (order = 0; order < NUMERICAL_ORDER; order++) {
			neuron[nrn].fdomain[fdm].I_total_syn[order]   = 0.0;
			neuron[nrn].fdomain[fdm].I_total_axial[order] = 0.0;
			neuron[nrn].fdomain[fdm].I_total_electrode[order]  = 0.0;
		}
		//state variables
		pop = neuron[nrn].population;
		n_statevars = population[pop].fdomain[fdm].model->n_statevars;
		for( order = 1; order < NUMERICAL_ORDER; order++ ) {
			neuron[nrn].fdomain[fdm].V[order] = neuron[nrn].fdomain[fdm].V[0];
			for( s = 0; s < n_statevars; s++ ) {
				neuron[nrn].fdomain[fdm].model_statevar[s][order] = neuron[nrn].fdomain[fdm].model_statevar[s][0];
			}
		}
		
		//call the associated model initialization function
		population[pop].fdomain[fdm].model->Initialize(nrn, fdm);
		
  }

  return;
}





//------------------------------------------------------------------------

/**
 * Advances in time (updates) all NEURON structures using the dynamics defined in models.c. Attention: in multi-compartment models (number of fdomains > 1) the axial currents are calculated using a the Euler Method (O1).
 * \param *out_spike_times_file File where the spike times are written
 */

void Update_Neurons(FILE *out_spike_times_file)
{
  unsigned int fdm, parent_fdm, order;
  unsigned long pop, nrn;
  double I_axial;
  double spike_time;


  //reset populations data to be refilled with individual neuron info
  Prepare_Update_Populations();


  #pragma omp parallel for private( nrn, fdm, parent_fdm, order, I_axial, spike_time, pop )
  for (nrn = 0; nrn < total_neurons; nrn++) {

    //calculate axial currents in all fdomains
		//Attention: the axial currents are calculated using the Euler Method (O1).
    for( fdm = 1; fdm < neuron[nrn].n_fdomains; fdm++ ) {

			parent_fdm = population[ neuron[nrn].population ].fdomain[fdm].parent_fdomain;

			for( order = 0; order < NUMERICAL_ORDER; order++ ) {
				I_axial = ( neuron[nrn].fdomain[parent_fdm].V[order] - neuron[nrn].fdomain[fdm].V[order] ) / neuron[nrn].fdomain[fdm].Ra;
				neuron[nrn].fdomain[fdm].I_total_axial[order] += I_axial;
				neuron[nrn].fdomain[parent_fdm].I_total_axial[order] += -I_axial;
			}

    }

		
    //update membrane potentials at each functional compartment (fdomain) attached to the soma using the neuron dynamics model
    for (fdm = 1; fdm < neuron[nrn].n_fdomains; fdm++) {
      (population[ neuron[nrn].population ].fdomain[fdm].model)->Advance(nrn, fdm);
    }
    
		//update fdm = 0 and check if neuron fires
		spike_time = (population[ neuron[nrn].population ].fdomain[0].model)->Advance(nrn, 0); //fdm=0
		
		if( neuron[nrn].spiketime_from_electrode > 0.0 ) { //override dynamics if the fire electrode says so: electrode_fire_flag (fire due to input electrodes)
			spike_time = neuron[nrn].spiketime_from_electrode;
			neuron[nrn].spiketime_from_electrode = -1.0;	    //clear the spiketime_from_electrode flag
		}
		
		if( spike_time > t - dt ) {  
			//update list with the new spike time
			Update_Spike_Times(nrn, spike_time);	
			Add_Spike(nrn, spike_time);
			//update out_spike_times_file with spike times
			if (flags.store_spike_times_file == 1) {
				fprintf( out_spike_times_file, "%ld\t%.3f\n", nrn, spike_time);
			}		
		}
		
    //reset the total current input to the neuron - has to be after the model equations (called above)
    for (fdm = 0; fdm < neuron[nrn].n_fdomains; fdm++) {
			for (order = 0; order < NUMERICAL_ORDER; order++) {
				neuron[nrn].fdomain[fdm].I_total_syn[order]       = 0.0;
				neuron[nrn].fdomain[fdm].I_total_axial[order]     = 0.0;
				neuron[nrn].fdomain[fdm].I_total_electrode[order] = 0.0;
			}
		}
		
    //update population information
    pop = neuron[nrn].population;
    if( t - neuron[nrn].spike_time[ neuron[nrn].last_spike_time_index ] < ACTIVITY_LEVEL_TIME_WINDOW ) { 
      population[pop].activity_level += 1.0;
    }
    if( neuron[nrn].fdomain[0].V[0] < MEMB_POTENTIAL_HYPERPOLARIZED ) {
      population[pop].hyperpolarised += 1.0;
    }      
    if (neuron[nrn].fdomain[0].V[0] > MEMB_POTENTIAL_DEPOLARIZED) {
      population[pop].depolarised += 1.0;	
    }      
		
		//update firing-rate estimates      
    if (flags.enable_rate_estimation == 1) {
      Update_Firing_Rate(nrn);
      population[pop].mean_firing_rate += neuron[nrn].firing_rate;
      population[pop].stdev_firing_rate += neuron[nrn].firing_rate * neuron[nrn].firing_rate;
    }
		
  }
	
  return;
}





//------------------------------------------------------------------------

/**
 * Updates the spike_time (ring) array and the index to the most recent spike time for a specific neuron
 * \param nrn id of the NEURON structure
 * \param spike_time time of the most recent spike
 */

void Update_Spike_Times(long nrn, double spike_time)
{
  //update variable with index for most recent spike time
  neuron[nrn].last_spike_time_index = ( neuron[nrn].last_spike_time_index + 1 ) % N_SPIKE_TIMES;

  //store new spike time
  neuron[nrn].spike_time[neuron[nrn].last_spike_time_index] = spike_time;

  //update the total number of spikes in the population 
  population[ neuron[nrn].population ].total_spikes += neuron[nrn].n_synapses;

  return;
}





//------------------------------------------------------------------------

/**
 * Updates the estimated firing rate of a neuron
 * \param nrn id of the NEURON structure
 */

void Update_Firing_Rate(long nrn)
{
  // !!! CAREFUL !!! THIS IS A SEVERE BOTTLENECK !!!!
  //there are better ways of doing this (future work ...);

  unsigned long i;
  double ti, sum = 0.0;

  for (i = 0; i < N_SPIKE_TIMES; i++) {
    
    ti = t - neuron[nrn].spike_time[i];
    
    sum += ti * FREQ_ALPHA * FREQ_ALPHA * exp( - ti * FREQ_ALPHA );
    
  }
  
  neuron[nrn].firing_rate = 1000.0 * sum; //1000 is needed to convert kHz to Hz (ms to seconds)
  

  return;
}





//------------------------------------------------------------------------

/**
 * Free allocated space for NEURONS
 */

void Free_Neurons(void)
{	
	unsigned int fdm, i, pop_i, pop_o, n_statevars;
	unsigned long nrn, syn;

	for( nrn=0; nrn<total_neurons; nrn++ ) {

		// Spike Times array
		free( neuron[nrn].spike_time );

		// Synapses
		for( syn=0; syn<neuron[nrn].n_synapses; syn++ ) {

			pop_i = neuron[nrn].population;
			pop_o = neuron[ neuron[nrn].synapse[syn].target_neuron ].population;
		 
			if( flags.homogeneous_synapses == 0 ) {
				free( neuron[nrn].synapse[syn].current_param );
				free( neuron[nrn].synapse[syn].plasticity_param );
			}
			//current statevars
			if( neuron[nrn].synapse[syn].current_statevar != NULL ) {
				n_statevars = connection[pop_i][pop_o].current_model->n_statevars;
				for( i=0; i<n_statevars; i++ ) {
					free( neuron[nrn].synapse[syn].current_statevar[i] );
				}
				free( neuron[nrn].synapse[syn].current_statevar );
			}
			//plasticity statevars
			if( neuron[nrn].synapse[syn].plasticity_statevar != NULL ) {
				n_statevars = connection[pop_i][pop_o].plasticity_model->n_statevars;
				for( i=0; i<n_statevars; i++ ) {
					free( neuron[nrn].synapse[syn].plasticity_statevar[i] );
				}
				free( neuron[nrn].synapse[syn].plasticity_statevar );
			}	

		}
		free( neuron[nrn].synapse );


		// FDomains
		for( fdm=0; fdm<neuron[nrn].n_fdomains; fdm++ ) {

			free( neuron[nrn].fdomain[fdm].V );
			free( neuron[nrn].fdomain[fdm].I_total_syn );
			free( neuron[nrn].fdomain[fdm].I_total_axial );
			free( neuron[nrn].fdomain[fdm].I_total_electrode );

			free( neuron[nrn].fdomain[fdm].model_param );

			//model statevars
			if( neuron[nrn].fdomain[fdm].model_statevar != NULL ) {
				for( i=0; i<population[ neuron[nrn].population ].fdomain[fdm].model->n_statevars; i++ ) {
					free( neuron[nrn].fdomain[fdm].model_statevar[i] );
				}
				free( neuron[nrn].fdomain[fdm].model_statevar );
			}

			//gsl
			if( neuron[nrn].fdomain[fdm].gsl != NULL ) {
				free( neuron[nrn].fdomain[fdm].gsl->y );
				free( neuron[nrn].fdomain[fdm].gsl->y_err );
				free( neuron[nrn].fdomain[fdm].gsl->dydt_in );
				free( neuron[nrn].fdomain[fdm].gsl->dydt_out );
				//free( neuron[nrn].fdomain[fdm].gsl->T );
				free( neuron[nrn].fdomain[fdm].gsl->s );
				free( neuron[nrn].fdomain[fdm].gsl );
			}
		}
		free( neuron[nrn].fdomain );

	}
		
	free( neuron );

	return;
}
