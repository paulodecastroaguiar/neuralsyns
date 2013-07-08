//************************************************************************
//*                                                                      *
//*                   G E N E R A L   F U N C T I O N S                  *
//*                                                                      *
//************************************************************************

/**
 * \file general.c
 * \author Paulo Aguiar
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "general.h"
#include "models.h"
#include "populations.h"
#include "neurons.h"
#include "synapses.h"
#include "electrodes.h"
#include "graphics.h"
#include "dataio.h"
#include "myfunctions.h"

extern struct POPULATION *population;
extern unsigned int total_populations;
extern struct NEURON *neuron;
extern unsigned long total_neurons;
extern struct CONNECTION **connection;

extern struct MODEL_DYNAMICS_NEURON *neuron_model;         // N_NEURON_MODELS is defined in models.h
extern struct MODEL_DYNAMICS_SYNAPSE *synapse_model;       // N_SYNAPSE_MODELS is defined in models.h
extern struct MODEL_DYNAMICS_PLASTICITY *plasticity_model; // N_PLASTICITY_MODELS is defined in models.h

extern struct ELECTRODE *electrode;
extern unsigned long total_electrodes;

extern struct FLAGS flags;

extern double t;
extern double dt;

extern gsl_rng *rng;		//pointer to the global random number generator





//------------------------------------------------------------------------

/**
 * Poisson Random number generator (uses GSL gsl_ran_poisson)
 * \param lambda mean of the Poisson distribution
 * \return random integer from the Poisson distribution with mean lambda
 */

unsigned int RND_Poisson(double lambda)
{
  return gsl_ran_poisson(rng, lambda);
}





//------------------------------------------------------------------------

/**
 * Exponential Random number generator (uses GSL gsl_ran_exponential)
 * \param lambda mean of the Exponential distribution
 * \return random variate from the Exponential distribution with mean lambda
 */

double RND_Exponential(double lambda)
{
  return gsl_ran_exponential(rng, lambda);
}





//------------------------------------------------------------------------

/**
 * Normal (Gaussian) Random number generator (uses GSL gsl_ran_gaussian)
 * \param miu mean of the Normal distribution
 * \param sigma standard deviation of the Normal distribution
 * \return random variate, with mean miu and standard deviation sigma
 */

double RND_Normal(double miu, double sigma)
{
  return gsl_ran_gaussian(rng, sigma) + miu;
}





//------------------------------------------------------------------------

/**
 * Uniform Random number generator (uses GSL gsl_ran_uniform)
 * \param min minimum value of the Uniform distribution
 * \param max maximum value of the Uniform distribution
 * \return random long, with Uniform distribuition between min and max
 */

double RND_Uniform(double min, double max)
{
  return min + (max - min) * gsl_rng_uniform(rng);
}





//------------------------------------------------------------------------

/**
 * Discrete Uniform Random number generator (uses GSL gsl_ran_uniform_int)
 * \param min minimum value of the Discrete Uniform distribution
 * \param max maximum value of the Discrete Uniform distribution
 * \return random long, with Discrete Uniform distribuition between min and max
 */


long RND_Discrete(long min, long max)
{
  return min + gsl_rng_uniform_int(rng, max - min + 1);
}





//------------------------------------------------------------------------

/**
 * Forces release of all memory used by NEURON, SYNAPSE, POPULATION, ELECTRODE and graphics structures
 */

void Free_Mem(void)
{
	//inverse order of creation; this order is necessary

	//graphics
  if(flags.graphics == 1) {
    Free_GraphicsMem();
  }

	//electrode
  Free_Electrodes();

	//neuron & synapse
  Free_Neurons();

	//connection
  Free_Connections();

	//population
  Free_Populations();

	//models
	Free_Models();

  printf("\nUsed memory has been released\n");

  return;
}





//------------------------------------------------------------------------

/**
 * Releases allocated space, taking into account the type of error that occured. Used frequently during network file read, in case something goes wrong.
 */

void Exit_Error( int level, FILE *in_file )
{
	switch (level) {

	case 4:
		Free_Electrodes();

	case 3:
		Free_Neurons();

	case 2:
		Free_Connections();

	case 1:
		Free_Populations();					

	}

	Free_Models();
	
	fclose(in_file);
	exit(-1);
	
	return; //yeah...	return... 
}





//------------------------------------------------------------------------

/**
 * Free memory allocated for MODELS
 */

void Free_Models( void )
{
	unsigned int m, i;

	// neuron models
	for( m=0; m<N_NEURON_MODELS; m++ ) {

		free( neuron_model[m].model_label );

		for( i=0; i<neuron_model[m].n_params; i++ ) {
			free( neuron_model[m].param_label[i] );
		}
		free( neuron_model[m].param_label );

		for( i=0; i<neuron_model[m].n_statevars; i++ ) {
			free( neuron_model[m].statevar_label[i] );
		}
		free( neuron_model[m].statevar_label );
		
	}
	free( neuron_model );
 
	// synapse models
	for( m=0; m<N_SYNAPSE_MODELS; m++ ) {

		free( synapse_model[m].model_label );

		for( i=0; i<synapse_model[m].n_params; i++ ) {
			free( synapse_model[m].param_label[i] );
		}
		free( synapse_model[m].param_label );

		for( i=0; i<synapse_model[m].n_statevars; i++ ) {
			free( synapse_model[m].statevar_label[i] );
		}
		free( synapse_model[m].statevar_label );
		
	}
	free( synapse_model );
 
	// plasticity models
	for( m=0; m<N_PLASTICITY_MODELS; m++ ) {

		free( plasticity_model[m].model_label );

		for( i=0; i<plasticity_model[m].n_params; i++ ) {
			free( plasticity_model[m].param_label[i] );
		}
		free( plasticity_model[m].param_label );

		for( i=0; i<plasticity_model[m].n_statevars; i++ ) {
			free( plasticity_model[m].statevar_label[i] );
		}
		free( plasticity_model[m].statevar_label );
		
	}
	free( plasticity_model );


	return;
}
