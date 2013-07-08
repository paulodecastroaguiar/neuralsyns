/*
MODEL DESCRIPTION:
Hebb
	PARAM = 3:
		w_max[1] = 1.0
		delta_w[1] = 0.1
		timewindow[ms] = 50.0
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


/** \file Hebb.c
 * Implements a standard Hebbian plasticity model.
 * Parameters: w_max[1], delta_w[1], timewindow[ms];
 * State variables: none;
 * The synaptic weight is incremented by delta_w, if the time difference between pre- and post-synaptic spikes is within timewindow. The weight saturates at w_max (hard-bounds [0, w_max]).
 * Notice that the synaptic weigth acts as an multiplication factor which is applied to the synapse model.
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup PlasticityModels
 */


//translate variables 
#define w_max      plasticity_param[0]
#define delta_w    plasticity_param[1]
#define timewindow plasticity_param[2]

//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;



//function definitions:
//---------------------

/// No initializations required for the 'Hebb' plasticity model
void ModelInitialize_Hebb(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  return;
}



/// The 'Hebb' plasticity model implements a basic Hebbian learning rule: if pre- and post-synaptic activations occur within a small time window, plasticity takes place
void ModelActivate_Hebb(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  //looks at the previous synaptic activation and checks if there was a co-activation of the post-synaptic neuron inside the plasticity window
  double t_pre, t_post, w;
  long i;

  if( neuron[pre_nrn].synapse[syn].weight > neuron[pre_nrn].synapse[syn].w_max - neuron[pre_nrn].synapse[syn].delta_w ) {
		//no point in spending more time here: synapse is already fully potentiated
		return;
  }

  t_pre = neuron[pre_nrn].synapse[syn].last_release_time;
  t_post = neuron[post_nrn].spike_time[ neuron[post_nrn].last_spike_time_index ];

  //find the post-synaptic spike which is closest to the previous synaptic activation
  for( i=1; i<N_SPIKE_TIMES; i++ ) {
		w = fabs( t_pre - t_post );
		t_post = neuron[post_nrn].spike_time[ (neuron[post_nrn].last_spike_time_index - 1) % N_SPIKE_TIMES ]; //go back one value in the spike times ring
		if( fabs( t_pre - t_post ) > w ) {
			break;
		}
  }

  //check if the closest pair is inside the plasticity window
  if( w < neuron[pre_nrn].synapse[syn].timewindow ) {
		neuron[pre_nrn].synapse[syn].weight += neuron[pre_nrn].synapse[syn].delta_w;
		if( neuron[pre_nrn].synapse[syn].weight > neuron[pre_nrn].synapse[syn].w_max ) {
			neuron[pre_nrn].synapse[syn].weight = neuron[pre_nrn].synapse[syn].w_max;
		}
  }

  return;
}
