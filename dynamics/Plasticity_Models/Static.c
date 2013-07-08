/*
MODEL DESCRIPTION:
Static
	PARAM = 0:
	STATE = 0:
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


/** \file Static.c
 * This is a dummy model to implement a static synapses.
 * Parameters: none;
 * State variables: none;
 * The synaptic weight is constant under this model.
 * \author Paulo Aguiar pauloaguiar@fc.up.pt
 * \ingroup PlasticityModels
 */


//translate variables 

//global variables
extern struct POPULATION *population;
extern struct NEURON *neuron;
extern struct CONNECTION **connection;
extern double t;
extern double dt;



//function definitions:
//---------------------

///< No initializations required for the 'Static' plasticity model
void ModelInitialize_Static(long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  return;
}



///< The 'Static' plasticity model is a dummy model which supports the creation of static synapses
void ModelActivate_Static(double activation_time, long syn, long pre_nrn, long post_nrn, int target_fdm)
{
  return;
}
