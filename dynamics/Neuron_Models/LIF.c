/*
MODEL DESCRIPTION:
LIF
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


/** \file LIF.c
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

/// Initializations required for the 'LIF' model for fdomain dynamics
void ModelInitialize_LIF(long nrn, int fdm)
{
	neuron[nrn].fdomain[fdm].V[1] = neuron[nrn].fdomain[fdm].V[0];
  return;
}



/// Uses Heun's method (O2) to update membrane potential and linear interpolation to calculate spike time, if the threshold is crossed
double ModelAdvance_LIF(long nrn, int fdm)
{
  // RK2 (Heun's method)
  // dU/dt = F(U,t)
  // U_n+1 = U_n + dt/2 * ( k0 + k1 )
  // k0 = F(U_n  , t_n  )
  // k1 = F(U_n+1, t_n+1)

  double k0, k1;
  double Vm_previous, spike_time;
  struct FDOMAIN *ptr;

  ptr = &neuron[nrn].fdomain[fdm];

  Vm_previous = ptr->V[0];

	//Check if it is beyond the refractory period
	if( t - neuron[nrn].spike_time[ neuron[nrn].last_spike_time_index ] > ptr->RefracP ) {

		// F(U,t) = ( (V_rest - V)/Rm + I_total_syn + I_total_axial + I_total_electrode )/Cm
		k0 = ( (ptr->Vrest - ptr->V[0])/ptr->Rm + ptr->I_total_syn[0] + ptr->I_total_axial[0] + ptr->I_total_electrode[0] ) / ptr->Cm;
		ptr->V[1] = ptr->V[0] + dt * k0;
		
		k1 = ( (ptr->Vrest - ptr->V[1])/ptr->Rm + ptr->I_total_syn[1] + ptr->I_total_axial[1] + ptr->I_total_electrode[1] ) / ptr->Cm;
		ptr->V[0] = ptr->V[0] + 0.5 * dt * ( k0 + k1 );
		
		//Check if threshold was reached
		if( ptr->V[0] > ptr->Vthresh ) {
			//use linear interpolant to rescue O(2) and return the corrected spike time (must be a positive mumber)
			spike_time = t - dt * ( 1 - ( ptr->Vthresh - Vm_previous ) / ( ptr->V[0] - Vm_previous ) );
			//reset membrane potential
			ptr->V[0] = ptr->Vreset;
			ptr->V[1] = ptr->Vreset;
			return spike_time;
		}
		else {
			//no spike
			return NO_SPIKE;
		}

	}
	else {

		ptr->V[0] = ptr->Vreset;
		ptr->V[1] = ptr->Vreset;
		return NO_SPIKE;

	}

}
