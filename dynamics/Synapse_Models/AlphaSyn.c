/*
MODEL DESCRIPTION:
AlphaSyn	
	PARAM = 3:
		g_max[uS] = 0.001
		E_syn[mV] = 0.0
		tau[ms]	= 1.0
	STATE = 2:
		g_syn[1] = 0.0
		g_aux[1] = 0.0
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


/** \file AlphaSyn.c
 * Synapse with alpha conductance profile.
 * Parameters: g_max[uS], E_syn[mV], tau[ms];
 * State variables: g_syn[1], g_aux[1];
 * This synapse supports temporal summation.
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup SynapseModels
 */


//translate variables 
#define g_max     current_param[0]
#define E_syn     current_param[1]
#define tau 	  current_param[2]
#define	g_syn	  current_statevar[0]
#define	g_aux     current_statevar[1]


//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;


//function definitions:
//---------------------

///< Initializes state variables
void ModelInitialize_AlphaSyn(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  neuron[pre_nrn].synapse[syn].g_syn[1] = neuron[pre_nrn].synapse[syn].g_syn[0];
  neuron[pre_nrn].synapse[syn].g_aux[1] = neuron[pre_nrn].synapse[syn].g_aux[0];  

  return;
}



///< Synaptic conductance receives step increase when activated
void ModelActivate_AlphaSyn(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  neuron[pre_nrn].synapse[syn].g_aux[0] += neuron[pre_nrn].synapse[syn].g_max;
  neuron[pre_nrn].synapse[syn].g_aux[1] += neuron[pre_nrn].synapse[syn].g_max * exp( -dt / neuron[pre_nrn].synapse[syn].tau );

  return;
}



///< Conductance with dual exponential profile. This synapse is turned off after 5*tau
void ModelAdvance_AlphaSyn(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  double decay;

  decay = exp( -dt / neuron[pre_nrn].synapse[syn].tau );

	neuron[pre_nrn].synapse[syn].g_aux[0] *= decay;
  neuron[pre_nrn].synapse[syn].g_aux[1] *= decay;

  neuron[pre_nrn].synapse[syn].g_syn[0] += ( neuron[pre_nrn].synapse[syn].g_aux[0] - neuron[pre_nrn].synapse[syn].g_syn[0] ) * decay; // CONFIRMAR TUDO
  neuron[pre_nrn].synapse[syn].g_syn[1] += 0.0;



  //make the synaptic currents depend on the weight in order to allow possible plasticity mechanisms (which act solely in the weight variable)
  neuron[post_nrn].fdomain[target_fdm].I_total_syn[0] += neuron[pre_nrn].synapse[syn].weight * neuron[pre_nrn].synapse[syn].g_syn[0] * ( neuron[pre_nrn].synapse[syn].E_syn - neuron[post_nrn].fdomain[target_fdm].V[0] );

  neuron[post_nrn].fdomain[target_fdm].I_total_syn[1] += neuron[pre_nrn].synapse[syn].weight * neuron[pre_nrn].synapse[syn].g_syn[1] * ( neuron[pre_nrn].synapse[syn].E_syn - neuron[post_nrn].fdomain[target_fdm].V[1] );
	
  //shut-down synapse if its contribution is already too small
  if( neuron[pre_nrn].synapse[syn].psr_time > 5.0 * neuron[pre_nrn].synapse[syn].tau ) {
		neuron[pre_nrn].synapse[syn].activated = 0;
  }


  return;
}
