/*
MODEL DESCRIPTION:
Izhikevich_O1
	SPIKE = yes	
	PARAM = 4:
		a[1/ms] = 0.02
		b[1] = 0.2
		c[mV]	= -50.0
		d[mV] = 2.0
	STATE = 1:
		u[mV] = 0.0
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


/** \file Izhikevich_O1.c
 * Implements the Ihzikevich model.
 * Parameters: a[1/ms], b[1], c[mV], d[mV]
 * State variables:	u[mV]
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup NeuronModels
 */


//translate variables 
#define a model_param[0]
#define b model_param[1]
#define c model_param[2]
#define d model_param[3]
#define u model_statevar[0]

//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;



//function definitions:
//---------------------

/// No initializations required for the 'Izhikevich' model for fdomain dynamics. The total current appears in the model as an additional state variable.
void ModelInitialize_Izhikevich_O1(long nrn, int fdm)
{
	return;
}



/// Uses Euler method (O1) (caution: inacurate and unstable) to update membrane potential and linear interpolation to calculate spike time. A time step dt<= 0.01 should be used. A spike is triggered when V=+30mV (crossed from below).
double ModelAdvance_Izhikevich_O1(long nrn, int fdm)
{
	double V0;
	double tspike;
	struct FDOMAIN *ptr;

	ptr   = &neuron[nrn].fdomain[fdm];

	//keep previous V[0] in case it will be required for spike time interpolation
	V0 = ptr->V[0];

	//update state variables
	ptr->u[0] += dt * ptr->a * ( ptr->b * ptr->V[0] - ptr->u[0] );
	ptr->V[0] += dt * ( 0.04 * ptr->V[0]*ptr->V[0] + 5.0 * ptr->V[0] + 140.0 - ptr->u[0] + ptr->I_total_syn[0] + ptr->I_total_axial[0] + ptr->I_total_electrode[0] );

	//check if spike is emitted
  if( ptr->V[0] >= 30.0 ) {
		//use linear interpolant to rescue O(2) and return the corrected spike time
		tspike = t - dt * ( 1 - ( 30.0 - V0 ) / ( ptr->V[0] - V0 ) );
		//reset operations
		ptr->u[0] += ptr->d;
		ptr->V[0] = ptr->c;
		
		return tspike;
  }
  else {
		//no spike
		return NO_SPIKE;
  }
	
}
