/*
MODEL DESCRIPTION:
Exp2Syn	
	PARAM = 4:
		g_max[uS] = 0.001
		E_syn[mV] = 0.0
		tau_rise[ms]	= 1.0
		tau_decay[ms]	= 5.0
	STATE = 3:
		norm[1] = 1.0
		g_syn_A[1] = 0.0
		g_syn_B[1] = 0.0
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


/** \file Exp2Syn.c
 * Synapse with dual exponential conductance profile.
 * Parameters: g_max[uS], E_syn[mV], tau_rise[ms], tau_decay[ms];
 * State variables: norm[1], g_syn_A[1], g_syn_B[1];
 * This synapse supports temporal summation.
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup SynapseModels
 */


//translate variables 
#define g_max     current_param[0]
#define E_syn     current_param[1]
#define tau_rise  current_param[2]
#define tau_decay current_param[3]
#define	norm      current_statevar[0]
#define	g_syn_A   current_statevar[1]
#define	g_syn_B   current_statevar[2]


//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;


//function definitions:
//---------------------

///< Initializes state variables
void ModelInitialize_Exp2Syn(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
	double tp, t_r, t_d;

  neuron[pre_nrn].synapse[syn].g_syn_A[1] = neuron[pre_nrn].synapse[syn].g_syn_A[0];
  neuron[pre_nrn].synapse[syn].g_syn_B[1] = neuron[pre_nrn].synapse[syn].g_syn_B[0];  

	//initialize the normalization factor (it is quite useful to have this value precalculated as it is computationally demanding)
	t_r = neuron[pre_nrn].synapse[syn].tau_rise;
	t_d =	neuron[pre_nrn].synapse[syn].tau_decay;
	tp = (t_r * t_d) / (t_d - t_r) * log(t_d / t_r);
	neuron[pre_nrn].synapse[syn].norm[0] = 1.0 / ( -exp( -tp / t_r ) + exp( -tp / t_d ) );
	neuron[pre_nrn].synapse[syn].norm[1] = neuron[pre_nrn].synapse[syn].norm[0]; //this is perfectly redundant but fits better In the Advance functions

  return;
}



///< Synaptic conductance receives step increase when activated
void ModelActivate_Exp2Syn(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  neuron[pre_nrn].synapse[syn].g_syn_A[0] += neuron[pre_nrn].synapse[syn].g_max;
  neuron[pre_nrn].synapse[syn].g_syn_A[1] += neuron[pre_nrn].synapse[syn].g_max * exp( -dt / neuron[pre_nrn].synapse[syn].tau_rise );
  neuron[pre_nrn].synapse[syn].g_syn_B[0] += neuron[pre_nrn].synapse[syn].g_max;
  neuron[pre_nrn].synapse[syn].g_syn_B[1] += neuron[pre_nrn].synapse[syn].g_max * exp( -dt / neuron[pre_nrn].synapse[syn].tau_decay );

  return;
}



///< Conductance with dual exponential profile. This synapse is turned off after 5*tau_decay
void ModelAdvance_Exp2Syn(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  double decay_A, decay_B;

  decay_A = exp( -dt / neuron[pre_nrn].synapse[syn].tau_rise );
  decay_B = exp( -dt / neuron[pre_nrn].synapse[syn].tau_decay );

  neuron[pre_nrn].synapse[syn].g_syn_A[0] *= decay_A;
  neuron[pre_nrn].synapse[syn].g_syn_A[1] *= decay_A;
 
	neuron[pre_nrn].synapse[syn].g_syn_B[0] *= decay_B;
  neuron[pre_nrn].synapse[syn].g_syn_B[1] *= decay_B;


  //make the synaptic currents depend on the weight in order to allow possible plasticity mechanisms (which act solely in the weight variable)
  neuron[post_nrn].fdomain[target_fdm].I_total_syn[0] += neuron[pre_nrn].synapse[syn].weight * neuron[pre_nrn].synapse[syn].norm[0] * ( neuron[pre_nrn].synapse[syn].g_syn_B[0] - neuron[pre_nrn].synapse[syn].g_syn_A[0] ) * ( neuron[pre_nrn].synapse[syn].E_syn - neuron[post_nrn].fdomain[target_fdm].V[0] );

  neuron[post_nrn].fdomain[target_fdm].I_total_syn[1] += neuron[pre_nrn].synapse[syn].weight * neuron[pre_nrn].synapse[syn].norm[1] * ( neuron[pre_nrn].synapse[syn].g_syn_B[1] - neuron[pre_nrn].synapse[syn].g_syn_A[1] ) * ( neuron[pre_nrn].synapse[syn].E_syn - neuron[post_nrn].fdomain[target_fdm].V[1] );
	
  //shut-down synapse if its contribution is already too small
  if( neuron[pre_nrn].synapse[syn].psr_time > 5.0 * neuron[pre_nrn].synapse[syn].tau_decay ) {
		neuron[pre_nrn].synapse[syn].activated = 0;
  }


  return;
}
