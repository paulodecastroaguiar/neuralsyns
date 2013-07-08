/*
MODEL DESCRIPTION:
IExpSyn	
	PARAM = 2:
		I_max[nA] = 0.1
		tau_decay[ms] = 5.0
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


/** \file IExpSyn.c
 * Synapse with exponential current injection. Synapse is deactivated after 3.0*tau_decay.
 * Parameters: I_max[nA], tau_decay[ms];
 * State variables: t0[ms];
 * This synapse supports temporal summation in the following way: amplitude is not increased but duration is extended.
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup SynapseModels
 */


//translate variables 
#define I_max  current_param[0]
#define tau_decay    current_param[1]

//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;


//function definitions:
//---------------------

///< No initializations required for this model
void ModelInitialize_IExpSyn(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  return;
}



///< No changes upon activation either
void ModelActivate_IExpSyn(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  return;
}



///< Exponential current. Current is turned off after 3.0*tau_decay ms
void ModelAdvance_IExpSyn(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
	double syncur;

	syncur = neuron[pre_nrn].synapse[syn].weight * neuron[pre_nrn].synapse[syn].I_max * exp( -neuron[pre_nrn].synapse[syn].psr_time / neuron[pre_nrn].synapse[syn].tau_decay );

  //make the synaptic currents depend on the weight in order to allow possible plasticity mechanisms (which act solely in the weight variable)
  neuron[post_nrn].fdomain[target_fdm].I_total_syn[0] += syncur;

	if( neuron[pre_nrn].synapse[syn].psr_time + dt < neuron[pre_nrn].synapse[syn].tau_decay ) {
		neuron[post_nrn].fdomain[target_fdm].I_total_syn[1] += syncur;
	}
	
  //shut-down synapse if its contribution is already too small
  if( neuron[pre_nrn].synapse[syn].psr_time > 3.0 * neuron[pre_nrn].synapse[syn].tau_decay ) {
		neuron[pre_nrn].synapse[syn].activated = 0;
  }


  return;
}
