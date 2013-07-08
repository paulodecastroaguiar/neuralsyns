/*
MODEL DESCRIPTION:
STP
	PARAM = 3:
		U1[1] = 1.0
		tau_fac[ms] = 0.1
		tau_rec[ms] = 0.1
	STATE = 2:
		Use[1] = 0.0
		P[1] = 1.0
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


/** \file STP.c
 * Implements a short-term synaptic plasticity model - modified from Fuhrmann et al (2001).
 * Parameters: U1[1], tau_fac[ms], tau_rec[ms];
 * State variables: Use[1], P[1];
 * The synaptic weight is incremented by delta_w, if the time difference between pre- and post-synaptic spikes is within timewindow. The weight saturates at w_max.
 * Notice that the synaptic weigth acts as an multiplication factor which is applied to the synapse model.
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup PlasticityModels
 */


//translate variables 
#define U1      plasticity_param[0]
#define tau_fac plasticity_param[1]
#define tau_rec plasticity_param[2]
#define Use     plasticity_statevar[0][0] //exact equations: no need to hold higher order values
#define P       plasticity_statevar[1][0] //exact equations: no need to hold higher order values

//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;



//function definitions:
//---------------------

/// No initializations required for the 'STP' plasticity model
void ModelInitialize_STP(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  return;
}



/// The 'STP' plasticity model implements a basic short-term synaptic plasticity learning rule based on the probabilistic model of Fuhrmann et al (2002). This model uses exact solution for the dynamics.
void ModelActivate_STP(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm)
{
	double delta_t;
	struct SYNAPSE *ptr;

	ptr = &neuron[pre_nrn].synapse[syn];
	delta_t = t - ptr->last_release_time;

	ptr->Use    = ptr->Use * exp( -delta_t/ptr->tau_fac );
	ptr->Use    = ptr->Use + ptr->U1 * ( 1.0 - ptr->Use ); 
	ptr->P      = 1.0 - ( 1.0 - ptr->P ) * exp( -delta_t/ptr->tau_rec );
	ptr->weight = ptr->Use * ptr->P;
	ptr->P      = ptr->P - ptr->weight;

  return;
}
