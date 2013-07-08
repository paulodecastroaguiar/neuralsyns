/*
MODEL DESCRIPTION:
Izhikevich
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


/** \file Izhikevich.c
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

/// Dynamics used in the GSL ODE Solver
int ModelFunc_Izhikevich (double t, const double y[], double f[], void *params)
{
	double *model_param;

	model_param = (double *)params;

	f[0] = 0.04 * y[0]*y[0] + 5.0 * y[0] + 140.0 - y[1] + y[2];
	f[1] = a * ( b * y[0] - y[1] );
	f[2] = 0.0; //the 2th state variable is used only to send I_total to the ODE Solver

	return GSL_SUCCESS;
}



/// Initializations required for the 'Izhikevich' model for fdomain dynamics. The total current appears in the model as an additional state variable.
void ModelInitialize_Izhikevich(long nrn, int fdm)
{
	struct FDOMAIN *ptr;
	struct GSL_ODESOLVER_DATA *gsl_p;
	int dim = 3;

	ptr = &neuron[nrn].fdomain[fdm];

	//initialize GSL_ODESOLVER_DATA
	ptr->gsl = malloc( sizeof( struct GSL_ODESOLVER_DATA ) );
	if( ptr->gsl == NULL ) {
		printf("\n\tERROR: unable to allocate memory for GSL data arrays in Izhikevich model initialization (neuron %ld, fdomain %d). This is an unclean exit!", nrn, fdm);
		exit(-1);
	}

	gsl_p = ptr->gsl;

	gsl_p->T = gsl_odeiv_step_rk2;                    //stepping algorithm
	gsl_p->s = gsl_odeiv_step_alloc( gsl_p->T, dim ); //fixed time step algorithm, dim dimension
	gsl_p->sys.function = ModelFunc_Izhikevich;       //function with the model dynamics
	gsl_p->sys.jacobian = NULL;                       //Jacobian; not used
	gsl_p->sys.dimension = dim;                       //number of dimensions in the system
	gsl_p->sys.params = ptr->model_param;             //model parameters       
	
	gsl_p->y        = calloc( dim, sizeof(double) );
	gsl_p->y_err    = calloc( dim, sizeof(double) );
	gsl_p->dydt_in  = calloc( dim, sizeof(double) );
	gsl_p->dydt_out = calloc( dim, sizeof(double) );

	//copy state variables from FDOMAIN structure to GSL structure
	gsl_p->y[0] = ptr->V[0];
	gsl_p->y[1] = ptr->u[0];
	gsl_p->y[2] = ptr->I_total_syn[0] + ptr->I_total_axial[0] + ptr->I_total_electrode[0];

	//initialise dydt_in from system parameters
	GSL_ODEIV_FN_EVAL( &(gsl_p->sys), t, gsl_p->y, gsl_p->dydt_in );

	return;
}



/// Uses GSL ODE Solver (RK2 - O2) to update membrane potential and linear interpolation to calculate spike time. A spike is triggered when V=+30mV (crossed from below).
double ModelAdvance_Izhikevich(long nrn, int fdm)
{
	unsigned int i;
	double V0;
	double tspike;
	struct FDOMAIN *ptr;
	struct GSL_ODESOLVER_DATA *gsl_p;

	ptr   = &neuron[nrn].fdomain[fdm];
	gsl_p = ptr->gsl;

	V0 = ptr->V[0];

	//inform about new I_total
	gsl_p->y[2]       = ptr->I_total_syn[0] + ptr->I_total_axial[0] + ptr->I_total_electrode[0];
	gsl_p->dydt_in[2] = ( ptr->I_total_syn[1] + ptr->I_total_axial[1] + ptr->I_total_electrode[1] - gsl_p->y[2] ) / dt;
	
	//update state variables
	if( gsl_odeiv_step_apply( gsl_p->s, t, dt, gsl_p->y, gsl_p->y_err, gsl_p->dydt_in, gsl_p->dydt_out, &(gsl_p->sys) ) != GSL_SUCCESS )  {
		printf("\n\tERROR: problem in GSL ODE Solver in Izhikevich model (neuron %ld, fdomain %d). This is an unclean exit!", nrn, fdm);
		exit(-1);
	}

	for( i=0;	i<gsl_p->sys.dimension-1; i++ ) { 
		gsl_p->dydt_in[i] = gsl_p->dydt_out[i];
	}

	ptr->V[0] = gsl_p->y[0];
	ptr->V[1] = gsl_p->y[0] + gsl_p->dydt_out[0] * dt;
	ptr->u[0] = gsl_p->y[1]; //this is used only by recState electrodes

	//check if spike is emitted
  if( ptr->V[0] >= 30.0 ) {

		//use linear interpolant to rescue O(2) and return the corrected spike time
		tspike = t - dt * ( 1 - ( 30.0 - V0 ) / ( ptr->V[0] - V0 ) );

		//reset operations
		ptr->u[0]   += ptr->d;
		ptr->u[1]   += ptr->d;
		gsl_p->y[1] += ptr->d;
		ptr->V[0]   = ptr->c;
		ptr->V[1]   = ptr->c;
		gsl_p->y[0] = ptr->c;

		//initialise dydt_in from system parameters
		GSL_ODEIV_FN_EVAL( &(gsl_p->sys), t, gsl_p->y, gsl_p->dydt_in );
		
		return tspike;
  }
  else {
		//no spike
		return NO_SPIKE;
  }
	
}
