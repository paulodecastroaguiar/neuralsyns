/*
MODEL DESCRIPTION:
AMPA_NMDA
	PARAM = 8:
		g_max_AMPA[uS] = 0.001
		g_max_NMDA[uS] = 0.001
		E_syn[mV] = 0.0
		AMPA_rise[ms] = 1.0
		AMPA_decay[ms] = 5.0
		NMDA_rise[ms] = 1.0
		NMDA_decay[ms] = 5.0
		Mgo[mM] = 1.0
	STATE = 6:
		norm_AMPA[1] = 1.0
		g_AMPA_A[1] = 0.0
		g_AMPA_B[1] = 0.0
		norm_NMDA[1] = 1.0
		g_NMDA_A[1] = 0.0
		g_NMDA_B[1] = 0.0
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


/** \file AMPA_NMDA.c
 * AMPA and NMDA receptors combined. Both have dual exponential conductance profiles and NMDA is subject to magnesium block dynamics (from Jahr & Stevens 1990).
 * Parameters: g_max_AMPA[uS], g_max_NMDA[uS], E_syn[mV], AMPA_rise[ms], AMPA_decay[ms], NMDA_rise[ms], NMDA_decay[ms], Mgo[mM];
 * State variables: norm[1], g_AMPA_A[1], g_AMPA_B[1], g_NMDA_A[1], g_NMDA_B[1];
 * This synapse supports temporal summation.
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup SynapseModels
 */


//translate variables 
#define g_max_AMPA  current_param[0]
#define g_max_NMDA  current_param[1]
#define E_syn       current_param[2]
#define AMPA_rise   current_param[3]
#define AMPA_decay  current_param[4]
#define NMDA_rise   current_param[5]
#define NMDA_decay  current_param[6]
#define Mgo         current_param[7]
#define	norm_AMPA   current_statevar[0]
#define	g_AMPA_A    current_statevar[1]
#define	g_AMPA_B    current_statevar[2]
#define	norm_NMDA   current_statevar[3]
#define	g_NMDA_A    current_statevar[4]
#define	g_NMDA_B    current_statevar[5]


//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;


//function definitions:
//---------------------

///< Initializes state variables
void ModelInitialize_AMPA_NMDA(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
	double tp, t_r, t_d;

	//AMPA
  neuron[pre_nrn].synapse[syn].g_AMPA_A[1] = neuron[pre_nrn].synapse[syn].g_AMPA_A[0];
  neuron[pre_nrn].synapse[syn].g_AMPA_B[1] = neuron[pre_nrn].synapse[syn].g_AMPA_B[0];  
	//initialize the normalization factor (it is useful to have this value precalculated as it is computationally demanding)
	t_r = neuron[pre_nrn].synapse[syn].AMPA_rise;
	t_d =	neuron[pre_nrn].synapse[syn].AMPA_decay;
	tp = (t_r * t_d) / (t_d - t_r) * log(t_d / t_r);
	neuron[pre_nrn].synapse[syn].norm_AMPA[0] = 1.0 / ( -exp( -tp / t_r ) + exp( -tp / t_d ) );
	neuron[pre_nrn].synapse[syn].norm_AMPA[1] = neuron[pre_nrn].synapse[syn].norm_AMPA[0]; //this is perfectly redundant but fits better in the Advance functions

	//NMDA
  neuron[pre_nrn].synapse[syn].g_NMDA_A[1] = neuron[pre_nrn].synapse[syn].g_NMDA_A[0];
  neuron[pre_nrn].synapse[syn].g_NMDA_B[1] = neuron[pre_nrn].synapse[syn].g_NMDA_B[0];  
	//initialize the normalization factor (it is useful to have this value precalculated as it is computationally demanding)
	t_r = neuron[pre_nrn].synapse[syn].NMDA_rise;
	t_d =	neuron[pre_nrn].synapse[syn].NMDA_decay;
	tp = (t_r * t_d) / (t_d - t_r) * log(t_d / t_r);
	neuron[pre_nrn].synapse[syn].norm_NMDA[0] = 1.0 / ( -exp( -tp / t_r ) + exp( -tp / t_d ) );
	neuron[pre_nrn].synapse[syn].norm_NMDA[1] = neuron[pre_nrn].synapse[syn].norm_NMDA[0]; //this is perfectly redundant but fits better in the Advance functions

  return;
}



///< Synaptic conductance receives step increase when activated
void ModelActivate_AMPA_NMDA(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm)
{
	//AMPA
  neuron[pre_nrn].synapse[syn].g_AMPA_A[0] += neuron[pre_nrn].synapse[syn].g_max_AMPA;
  neuron[pre_nrn].synapse[syn].g_AMPA_A[1] += neuron[pre_nrn].synapse[syn].g_max_AMPA * exp( -dt / neuron[pre_nrn].synapse[syn].AMPA_rise );
  neuron[pre_nrn].synapse[syn].g_AMPA_B[0] += neuron[pre_nrn].synapse[syn].g_max_AMPA;
  neuron[pre_nrn].synapse[syn].g_AMPA_B[1] += neuron[pre_nrn].synapse[syn].g_max_AMPA * exp( -dt / neuron[pre_nrn].synapse[syn].AMPA_decay );

	//NMDA
  neuron[pre_nrn].synapse[syn].g_AMPA_A[0] += neuron[pre_nrn].synapse[syn].g_max_AMPA;
  neuron[pre_nrn].synapse[syn].g_AMPA_A[1] += neuron[pre_nrn].synapse[syn].g_max_AMPA * exp( -dt / neuron[pre_nrn].synapse[syn].AMPA_rise );
  neuron[pre_nrn].synapse[syn].g_AMPA_B[0] += neuron[pre_nrn].synapse[syn].g_max_AMPA;
  neuron[pre_nrn].synapse[syn].g_AMPA_B[1] += neuron[pre_nrn].synapse[syn].g_max_AMPA * exp( -dt / neuron[pre_nrn].synapse[syn].AMPA_decay );

  return;
}



///< Two independent conductance with dual exponential profiles and Mg2+ dynamics in NMDA. This synapse is turned off after 5*NMDA_decay
void ModelAdvance_AMPA_NMDA(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  double mgblock, decay_A, decay_B;

	//AMPA
  decay_A = exp( -dt / neuron[pre_nrn].synapse[syn].AMPA_rise );
  decay_B = exp( -dt / neuron[pre_nrn].synapse[syn].AMPA_decay );
  neuron[pre_nrn].synapse[syn].g_AMPA_A[0] *= decay_A;
  neuron[pre_nrn].synapse[syn].g_AMPA_A[1] *= decay_A; 
	neuron[pre_nrn].synapse[syn].g_AMPA_B[0] *= decay_B;
  neuron[pre_nrn].synapse[syn].g_AMPA_B[1] *= decay_B;
  //make the synaptic currents depend on the weight in order to allow possible plasticity mechanisms (which act solely in the weight variable)
  neuron[post_nrn].fdomain[target_fdm].I_total_syn[0] += neuron[pre_nrn].synapse[syn].weight * neuron[pre_nrn].synapse[syn].norm_AMPA[0] * ( neuron[pre_nrn].synapse[syn].g_AMPA_B[0] - neuron[pre_nrn].synapse[syn].g_AMPA_A[0] ) * ( neuron[pre_nrn].synapse[syn].E_syn - neuron[post_nrn].fdomain[target_fdm].V[0] );
  neuron[post_nrn].fdomain[target_fdm].I_total_syn[1] += neuron[pre_nrn].synapse[syn].weight * neuron[pre_nrn].synapse[syn].norm_AMPA[1] * ( neuron[pre_nrn].synapse[syn].g_AMPA_B[1] - neuron[pre_nrn].synapse[syn].g_AMPA_A[1] ) * ( neuron[pre_nrn].synapse[syn].E_syn - neuron[post_nrn].fdomain[target_fdm].V[1] );

	//NMDA
  decay_A = exp( -dt / neuron[pre_nrn].synapse[syn].NMDA_rise );
  decay_B = exp( -dt / neuron[pre_nrn].synapse[syn].NMDA_decay );
  neuron[pre_nrn].synapse[syn].g_NMDA_A[0] *= decay_A;
  neuron[pre_nrn].synapse[syn].g_NMDA_A[1] *= decay_A; 
	neuron[pre_nrn].synapse[syn].g_NMDA_B[0] *= decay_B;
  neuron[pre_nrn].synapse[syn].g_NMDA_B[1] *= decay_B;
  //make the synaptic currents depend on the weight in order to allow possible plasticity mechanisms (which act solely in the weight variable)
	mgblock = 1.0 / ( 1.0 + exp( -0.062  * neuron[post_nrn].fdomain[target_fdm].V[0] ) * ( neuron[pre_nrn].synapse[syn].Mgo / 3.57 ) );
  neuron[post_nrn].fdomain[target_fdm].I_total_syn[0] += neuron[pre_nrn].synapse[syn].weight * neuron[pre_nrn].synapse[syn].norm_NMDA[0] * ( neuron[pre_nrn].synapse[syn].g_NMDA_B[0] - neuron[pre_nrn].synapse[syn].g_NMDA_A[0] ) * ( neuron[pre_nrn].synapse[syn].E_syn - neuron[post_nrn].fdomain[target_fdm].V[0] ) * mgblock;
	mgblock = 1.0 / ( 1.0 + exp( -0.062  * neuron[post_nrn].fdomain[target_fdm].V[0] ) * ( neuron[pre_nrn].synapse[syn].Mgo / 3.57 ) );
  neuron[post_nrn].fdomain[target_fdm].I_total_syn[1] += neuron[pre_nrn].synapse[syn].weight * neuron[pre_nrn].synapse[syn].norm_NMDA[1] * ( neuron[pre_nrn].synapse[syn].g_NMDA_B[1] - neuron[pre_nrn].synapse[syn].g_NMDA_A[1] ) * ( neuron[pre_nrn].synapse[syn].E_syn - neuron[post_nrn].fdomain[target_fdm].V[1] ) * mgblock;
	

  //shut-down synapse if its contribution is already too small
  if( neuron[pre_nrn].synapse[syn].psr_time > 5.0 * neuron[pre_nrn].synapse[syn].NMDA_decay ) {
		neuron[pre_nrn].synapse[syn].activated = 0;
  }

  return;
}
