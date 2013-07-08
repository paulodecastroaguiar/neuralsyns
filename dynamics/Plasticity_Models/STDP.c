/*
MODEL DESCRIPTION:
STDP
	PARAM = 5:
		tau_pre[ms] = 20.0
		tau_post[ms] = 20.0
		D_pre[1] = 0.1
		D_post[1] = 0.1
		w_max[1] = 1.0
	STATE = 0:
END
*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_rng.h>

#include "../../src/general.h"
#include "../../src/models.h" 
#include "../../src/populations.h"
#include "../../src/neurons.h"
#include "../../src/synapses.h"


/** \file STDP.c
 * Implements a nearest neighbor spike-timing dependent plasticity model.
 * Parameters: tau_pre[ms], tau_post[ms], D_pre[1], D_post[1], w_max[1]
 * State variables: none;
 * Consider Dt as the difference between the closest post-synaptic spike time and the closest pre-synaptic spike time, evaluated at synapse activation time. The synaptic weight is incremented by D_pre*exp(-Dt/tau_pre) if Dt >= 0, and is reduced by D_post*exp(-Dt/tau_post) if Dt < 0. The synaptic weight has hard-bounds [0,w_max].
 * Notice that the synaptic weight acts as an multiplication factor which is applied to the synapse model.
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup PlasticityModels
 */


//translate variables 
#define tau_pre  plasticity_param[0]
#define tau_post plasticity_param[1]
#define D_pre    plasticity_param[2]
#define D_post   plasticity_param[3]
#define w_max    plasticity_param[4]

//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;



//function definitions:
//---------------------

/// No initializations required for the 'STDP' plasticity model
void ModelInitialize_STDP(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  return;
}



/// The 'STDP' plasticity model implements a nearest neighbor spike-timing dependent plasticity model. This model uses exact solution for the dynamics.
void ModelActivate_STDP(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm)
{
	unsigned int last_post_nrn_spike_time_index;
	double Delta_t;
	struct SYNAPSE *ptr;
	
	last_post_nrn_spike_time_index = neuron[post_nrn].last_spike_time_index;
	ptr = &neuron[pre_nrn].synapse[syn];

	Delta_t = neuron[post_nrn].spike_time[last_post_nrn_spike_time_index] - ptr->last_release_time;

	if( Delta_t >= 0.0 ) {
		ptr->weight += ptr->D_pre * exp( -Delta_t / ptr->tau_pre );
		if( ptr->weight > ptr->w_max ) { //hard bounds
			ptr->weight = ptr->w_max;
		}
	}
	else {
		ptr->weight -= ptr->D_post * exp( -Delta_t / ptr->tau_post );
		if( ptr->weight < 0.0 ) { //hard bounds
			ptr->weight = 0.0;
		}
	}

  return;
}
