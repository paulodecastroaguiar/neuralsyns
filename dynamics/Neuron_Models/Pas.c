/*
MODEL DESCRIPTION:
Pas
	SPIKE = no
	PARAM = 3:
		Cm[nF] = 1.0
		g_pas[uS] = 0.0001
		E_pas[mV] = -70.0
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


/** \file Pas.c
 * Implements the basic membrane passive properties (capacitance and resistance).
 * Parameters: Cm[nF], g_pas[uS], E_pas[mV];
 * State variables: none;
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup NeuronModels
 */


//translate variables 
#define Cm    model_param[0]
#define g_pas model_param[1]
#define E_pas model_param[2]

//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;


//function definitions:
//---------------------

/// No initializations required for the 'PAS' model for fdomain dynamics
void ModelInitialize_Pas(long nrn, int fdm)
{
  return;
}



///< Implements a standard passive model
double ModelAdvance_Pas(long nrn, int fdm)
{
  // RK2 (Heun's method)
  // dU/dt = F(U,t)
  // U_n+1 = U_n + dt/2 * ( k0 + k1 )
  // k0 = F(U_n  , t_n  )
  // k1 = F(U_n+1, t_n+1)

  double k0, k1;
  struct FDOMAIN *ptn;

  ptn = &neuron[nrn].fdomain[fdm];

  // F(U,t) = ( g_pas * (E_pas - V) + I_total_syn + I_total_axial + I_total_electrode )/Cm
  k0 = ( ptn->g_pas * (ptn->E_pas - ptn->V[0]) + ptn->I_total_syn[0] + ptn->I_total_axial[0] + ptn->I_total_electrode[0] ) / ptn->Cm;
  ptn->V[1] = ptn->V[0] + dt * k0;

  k1 = ( ptn->g_pas * (ptn->E_pas - ptn->V[1]) + ptn->I_total_syn[1] + ptn->I_total_axial[1] + ptn->I_total_electrode[1] ) / ptn->Cm;
  ptn->V[0] = ptn->V[0] + 0.5 * dt * ( k0 + k1 );


  //This model does not generate spikes
  return NO_SPIKE;
}
