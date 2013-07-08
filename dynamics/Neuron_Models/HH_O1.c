/*
MODEL DESCRIPTION:
HH_O1
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


/** \file HH_O1.c
 * Implements the Hodgkin-Huxley model using the basic Euler method (O1) - modified from Traub and Miles (1991); see also Brette et al (2007).
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

/// No initializations required for the 'HH_O1' model for fdomain dynamics.
void ModelInitialize_HH_O1(long nrn, int fdm)
{
	return;
}



/// Uses basic Euler method (caution: inacurate and unstable) to update membrane potential and linear interpolation to calculate spike time. A time step dt<= 0.01 should be used. A threshold line of 0 mV is assumed (spike is emitted when threshold is crossed from below).
double ModelAdvance_HH_O1(long nrn, int fdm)
{
	double V0, V;
	double a, b, x_inf, tau_x;
	double tadj;
	struct FDOMAIN *ptr;

	ptr   = &neuron[nrn].fdomain[fdm];

	//keep previous V[0] in case it will be required for spike time interpolation
	V0 = ptr->V[0];


	V = ptr->V[0] - ptr->VTraub; // convert to traub convention
	tadj = pow(3.0 , (ptr->Temp - 36.0)/ 10.0 );

	//update state variables!
	// m
	a = 0.32 * (13.0 - V) / ( exp( (13.0 - V)/4.0) - 1.0 );
	b = 0.28 * (V - 40.0) / ( exp( (V - 40.0)/5.0) - 1.0);
	tau_x = 1.0 / ( a + b ) / tadj;
	x_inf = a / ( a + b );
	ptr->m[0] += ( 1.0 - exp(-dt/tau_x) ) * ( x_inf - ptr->m[0] );
	// h
	a = 0.128 * exp( (17.0 - V)/18.0 );
	b = 4.0 / ( 1.0 + exp( (40.0 - V)/5.0 ) );
	tau_x = 1.0 / ( a + b ) / tadj;
	x_inf = a / ( a + b );
	ptr->h[0] += ( 1.0 - exp(-dt/tau_x) ) * ( x_inf - ptr->h[0] );
	// n
	a = 0.032 * (15.0 - V) / ( exp( (15.0 - V)/5.0 ) - 1.0 );
	b = 0.5 * exp( (10.0 - V)/40.0 );
	tau_x = 1.0 / ( a + b ) / tadj;
	x_inf = a / ( a + b );
	ptr->n[0] += ( 1.0 - exp(-dt/tau_x) ) * ( x_inf - ptr->n[0] ); //blue
	// V
	ptr->V[0] += dt * ( ptr->gL * (ptr->EL - ptr->V[0]) + ptr->gNa_bar * ptr->m[0]*ptr->m[0]*ptr->m[0] * ptr->h[0] * (ptr->ENa - ptr->V[0]) + ptr->gK_bar * ptr->n[0]*ptr->n[0]*ptr->n[0]*ptr->n[0] * (ptr->EK - ptr->V[0]) +  ptr->I_total_syn[0] + ptr->I_total_axial[0] + ptr->I_total_electrode[0] ) / ptr->Cm;

	//check if spike is emitted
  if( ptr->V[0] > 0.0 && V0 < 0.0 ) {
		//use linear interpolant to improve the estimated spike time
		return t - dt * ( 1 + V0 / ( ptr->V[0] - V0 ) );
  }
  else {
		//no spike
		return NO_SPIKE;
  }
	
}
