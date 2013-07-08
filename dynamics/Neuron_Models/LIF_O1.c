/*
MODEL DESCRIPTION:
LIF_O1
	SPIKE = yes	
	PARAM = 6:
		Cm[nF] = 1.0
		Rm[Mohm] = 20.0
		Vrest[mV] = -70.0
		Vthresh[mV]	= -50.0
		Vreset[mV] = -70.0
		RefracP[ms] = 5.0
	STATE = 0:
END
*/

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

#include "../../src/general.h"
#include "../../src/models.h" 
#include "../../src/populations.h"
#include "../../src/neurons.h"
#include "../../src/synapses.h"


/** \file LIF_O1.c
 * Implements the canonical leaky integrate-and-fire model.
 * Parameters: Cm[nF], Rm[Mohm], Vrest[mV], Vthresh[mV], Vreset[mV], RefracP[ms] = 5.0;
 * State variables: none;
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup NeuronModels
 */


//translate variables 
#define Cm      model_param[0]
#define Rm      model_param[1]
#define Vrest   model_param[2]
#define Vthresh model_param[3]
#define Vreset  model_param[4]
#define RefracP model_param[5]


//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;



//function definitions:
//---------------------

/// No initializations required for the 'LIF_O1' model for fdomain dynamics
void ModelInitialize_LIF_O1(long nrn, int fdm)
{
  return;
}



/// Uses the basic Euler method (O1) (caution: inacurate and unstable) to update membrane potential and linear interpolation to calculate spike time, if the threshold is crossed
double ModelAdvance_LIF_O1(long nrn, int fdm)
{
  // Euler's method

  double Vm_previous, spike_time;
  struct FDOMAIN *ptr;

  ptr = &neuron[nrn].fdomain[fdm];

  Vm_previous = ptr->V[0];

	//Check if it is beyond the refractory period
	if( t - neuron[nrn].spike_time[ neuron[nrn].last_spike_time_index ] > ptr->RefracP ) {

		ptr->V[0] = ptr->V[0] + dt * ( (ptr->Vrest - ptr->V[0])/ptr->Rm + ptr->I_total_syn[0] + ptr->I_total_axial[0] + ptr->I_total_electrode[0] ) / ptr->Cm ;

		//Check if threshold was reached
		if( ptr->V[0] > ptr->Vthresh ) {
			//use linear interpolant and return the corrected spike time (must be a positive mumber)
			spike_time = t - dt * ( 1 - ( ptr->Vthresh - Vm_previous ) / ( ptr->V[0] - Vm_previous ) );
			//reset membrane potential
			ptr->V[0] = ptr->Vreset;
			return spike_time;
		}
		else {
			//no spike
			return NO_SPIKE;
		}
	}
	else {

		ptr->V[0] = ptr->Vreset;
		return NO_SPIKE;

  }

}
