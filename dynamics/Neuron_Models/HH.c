/*
MODEL DESCRIPTION:
HH
	SPIKE = yes	
	PARAM = 9:
		Cm[nF] = 1.0
		gL[uS] = 0.05
		EL[mV] = -70.0
		gNa_bar[uS] = 100.0
		ENa[mV] = 50.0
		gK_bar[uS] = 30.0
		EK[mV] = -90.0
		Temp[Celsius]	= 36.0
		VTraub[mV] = -63.0
	STATE = 3:
		m[1] = 0.0
		h[1] = 1.0
		n[1] = 0.0	
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


/** \file HH.c
 * Implements the Hodgkin-Huxley model - modified from Traub and Miles (1991); see also Brette et al (2007).
 * Parameters: Cm[nF], gL[uS], EL[mV], gNa_bar[uS],	ENa[mV], gK_bar[uS], EK[mV], Temp[Celsius],	VTraub[mV]
 * State variables:	m[1], h[1], n[1]
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup NeuronModels
 */


//translate variables 
#define Cm      model_param[0]
#define gL      model_param[1]
#define EL      model_param[2]
#define gNa_bar model_param[3]
#define ENa     model_param[4]
#define gK_bar  model_param[5]
#define EK      model_param[6]
#define Temp    model_param[7]
#define VTraub  model_param[8]
#define m       model_statevar[0]
#define h       model_statevar[1]
#define n       model_statevar[2]

//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;



//function definitions:
//---------------------

/// Dynamics used in the GSL ODE Solver
int ModelFunc_HH (double t, const double y[], double f[], void *params)
{
	double a_m, b_m, a_h, b_h, a_n, b_n;
	double V;
	double tadj;
	double *model_param;

	model_param = (double *)params;

	V = y[0] - VTraub; // convert to traub convention
	tadj = pow(3.0 , (Temp - 36.0)/ 10.0 );

	a_m = 0.32 * (13.0 - V) / ( exp( (13.0 - V)/4.0) - 1.0 );
	b_m = 0.28 * (V - 40.0) / ( exp( (V - 40.0)/5.0) - 1.0);
	
	a_h = 0.128 * exp( (17.0 - V)/18.0 );
	b_h = 4.0 / ( 1.0 + exp( (40.0 - V)/5.0 ) );
  
	a_n = 0.032 * (15.0 - V) / ( exp( (15.0 - V)/5.0 ) - 1.0 );
	b_n = 0.5 * exp( (10.0 - V)/40.0 );

	f[0] = ( gL * (EL - y[0]) + gNa_bar * y[1]*y[1]*y[1] * y[2] * (ENa - y[0]) + gK_bar * y[3]*y[3]*y[3]*y[3] * (EK - y[0]) + y[4] ) / Cm;
	f[1] = ( a_m * (1.0 - y[1]) - b_m * y[1] ) * tadj;
	f[2] = ( a_h * (1.0 - y[2]) - b_h * y[2] ) * tadj;
	f[3] = ( a_n * (1.0 - y[3]) - b_n * y[3] ) * tadj;
	f[4] = 0.0; //the 4th state variable is used only to send I_total to the ODE Solver

	return GSL_SUCCESS;
}



/// Initializations required for the 'HH' model for fdomain dynamics. The total current appears in the model as an additional state variable.
void ModelInitialize_HH(long nrn, int fdm)
{
	struct FDOMAIN *ptr;
	struct GSL_ODESOLVER_DATA *gsl_p;
	int dim = 5;

	ptr = &neuron[nrn].fdomain[fdm];

	//initialize GSL_ODESOLVER_DATA
	ptr->gsl = malloc( sizeof( struct GSL_ODESOLVER_DATA ) );
	if( ptr->gsl == NULL ) {
		printf("\n\tERROR: unable to allocate memory for GSL data arrays in HH model initialization (neuron %ld, fdomain %d). This is an unclean exit!", nrn, fdm);
		exit(-1);
	}

	gsl_p = ptr->gsl;

	gsl_p->T = gsl_odeiv_step_rk2;                    //stepping algorithm
	gsl_p->s = gsl_odeiv_step_alloc( gsl_p->T, dim ); //fixed time step algorithm, dim dimension
	gsl_p->sys.function = ModelFunc_HH;               //function with the model dynamics
	gsl_p->sys.jacobian = NULL;                       //Jacobian; not used
	gsl_p->sys.dimension = dim;                       //number of dimensions in the system
	gsl_p->sys.params = ptr->model_param;             //model parameters       
	
	gsl_p->y        = calloc( dim, sizeof(double) );
	gsl_p->y_err    = calloc( dim, sizeof(double) );
	gsl_p->dydt_in  = calloc( dim, sizeof(double) );
	gsl_p->dydt_out = calloc( dim, sizeof(double) );

	//copy state variables from FDOMAIN structure to GSL structure
	gsl_p->y[0] = ptr->V[0];
	gsl_p->y[1] = ptr->m[0];
	gsl_p->y[2] = ptr->h[0];
	gsl_p->y[3] = ptr->n[0];
	gsl_p->y[4] = ptr->I_total_syn[0] + ptr->I_total_axial[0] + ptr->I_total_electrode[0];

	//initialise dydt_in from system parameters
	GSL_ODEIV_FN_EVAL( &(gsl_p->sys), t, gsl_p->y, gsl_p->dydt_in );

	return;
}



/// Uses GSL ODE Solver (RK2 - O2) to update membrane potential and linear interpolation to calculate spike time. A threshold line of 0 mV is assumed (spike is emitted when threshold is crossed from below).
double ModelAdvance_HH(long nrn, int fdm)
{
	unsigned int i;
	double V0;
	struct FDOMAIN *ptr;
	struct GSL_ODESOLVER_DATA *gsl_p;

	ptr   = &neuron[nrn].fdomain[fdm];
	gsl_p = ptr->gsl;


	//inform about new I_total
	gsl_p->y[4]       = ptr->I_total_syn[0] + ptr->I_total_axial[0] + ptr->I_total_electrode[0];
	gsl_p->dydt_in[4] = ( ptr->I_total_syn[1] + ptr->I_total_axial[1] + ptr->I_total_electrode[1] - gsl_p->y[4] ) / dt;
	
	//keep previous V[0] in case it will be required for spike time interpolation
	V0 = ptr->V[0];

	//update state variables!
	if( gsl_odeiv_step_apply( gsl_p->s, t, dt, gsl_p->y, gsl_p->y_err, gsl_p->dydt_in, gsl_p->dydt_out, &(gsl_p->sys) ) != GSL_SUCCESS )  {
		printf("\n\tERROR: problem in GSL ODE Solver in HH model (neuron %ld, fdomain %d). This is an unclean exit!", nrn, fdm);
		exit(-1);
	}

	//speed up the next step 
	for( i=0;	i<gsl_p->sys.dimension-1; i++ ) { 
		gsl_p->dydt_in[i] = gsl_p->dydt_out[i];
	}

	//transfer results to FDOMAIN data structures (... needs improvent)
	ptr->V[0] = gsl_p->y[0];
	ptr->V[1] = gsl_p->y[0] + gsl_p->dydt_out[0] * dt;
	ptr->m[0] = gsl_p->y[1]; //this is used by recState electrodes 
	ptr->h[0] = gsl_p->y[2]; //this is used by recState electrodes
	ptr->n[0] = gsl_p->y[3]; //this is used by recState electrodes

	//check if spike is emitted
  if( ptr->V[0] > 0.0 && V0 < 0.0 ) {
		//use linear interpolant to rescue O(2) and return the corrected spike time
		return t - dt * ( 1 + V0 / ( ptr->V[0] - V0 ) );
  }
  else {
		//no spike
		return NO_SPIKE;
  }
	
}
