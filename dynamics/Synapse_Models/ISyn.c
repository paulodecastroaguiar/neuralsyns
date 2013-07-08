/*
MODEL DESCRIPTION:
ISyn	
	PARAM = 2:
		I_max[nA] = 0.1
		dur[ms] = 5.0
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


/** \file ISyn.c
 * Synapse with constant current injection, during a predefined time window.
 * Parameters: I_max[nA], dur[ms];
 * State variables: t0[ms];
 * This synapse supports temporal summation in the following way: amplitude is not increased but duration is extended.
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup SynapseModels
 */


//translate variables 
#define I_max  current_param[0]
#define dur    current_param[1]

//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;


//function definitions:
//---------------------

///< No initializations required for this model
void ModelInitialize_ISyn(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  return;
}



///< No changes upon activation either
void ModelActivate_ISyn(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  return;
}



///< Square wave current. Current is turned off after dur ms
void ModelAdvance_ISyn(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  //make the synaptic currents depend on the weight in order to allow possible plasticity mechanisms (which act solely in the weight variable)
  neuron[post_nrn].fdomain[target_fdm].I_total_syn[0] += neuron[pre_nrn].synapse[syn].weight * neuron[pre_nrn].synapse[syn].I_max;

	if( neuron[pre_nrn].synapse[syn].psr_time + dt < neuron[pre_nrn].synapse[syn].dur ) {
		neuron[post_nrn].fdomain[target_fdm].I_total_syn[1] += neuron[pre_nrn].synapse[syn].weight * neuron[pre_nrn].synapse[syn].I_max;
	}
	
  //shut-down synapse if its contribution is already too small
  if( neuron[pre_nrn].synapse[syn].psr_time > neuron[pre_nrn].synapse[syn].dur ) {
		neuron[pre_nrn].synapse[syn].activated = 0;
  }


  return;
}
