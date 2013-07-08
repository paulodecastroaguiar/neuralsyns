//************************************************************************
//*                                                                      *
//*                         P O P U L A T I O N S                        *
//*                                                                      *
//************************************************************************

/**
 * \file populations.c
 * \author Paulo Aguiar
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "general.h"
#include "models.h"
#include "dataio.h"
#include "populations.h"
#include "neurons.h"
#include "synapses.h"

extern struct POPULATION *population;
extern unsigned int total_populations;
extern struct NEURON *neuron;
extern unsigned long total_neurons;
extern struct CONNECTION **connection;

extern struct FLAGS flags;

extern double t;
extern double dt;

/*
//empirical check if axial resistance values lead to numerical instability
	for( fdm = 1; fdm < population[pop].n_fdomains; fdm++ ) { //starts at fdm=1 since fdm0 has no Ra associated (has no parent)
	  if(population[pop].fdomain[fdm].Ra < MIN_Ra) { //should have  dt << Cm * Ra; assuming that Cm ~ 1.0 [nF];
		//axial resistant is too low for the used dt: numerical instability will occur!!
		//anyway, if Ra is too small, this compartment should be considered as part of the soma
		printf("\n\nYou are using very small fdomain axial resistances [population %d; fdm %d; Ra = %lf Mohm].\nIncrease Ra in your network file data otherwise numerical instability may occurs in the axial currents.\nNotice that fdomains with very low Ra are not electrotonically distinct anyway.\n\n", pop, fdm, population[pop].fdomain[fdm].Ra);
		Free_Mem();
		exit(-1);
	  }
	}*/


//------------------------------------------------------------------------

/**
 * Initializes all pointers in the POPULATION structures
 * \param *p pointer to POPULATION structure
 * \param *file_in input .net file
 */

void Initialize_Populations(struct POPULATION *p, FILE *file_in)
{
	unsigned int pop;

	if (p == NULL) {
		printf("\n\tERROR: unable to allocate memory for list of POPULATIONS");
		fclose(file_in);
		exit(-1);
	}
	else {

		for( pop = 0; pop < total_populations; pop++ ) {
			p[pop].label = calloc( MAX_STRING_SIZE, sizeof( char ) );
			if( p[pop].label == NULL ) {
				printf("\n\tERROR: unable to allocate memory for labels in list of POPULATIONS");
				fclose(file_in);
				exit(-1);
			}
			p[pop].label[0] = '\0';
			p[pop].fdomain = NULL;
		}
		
	}
	
  return;
}





//------------------------------------------------------------------------

/**
 * Initializes all pointers in the FDOMAIN_GLOBAL structures
 * \param *p pointer to FDOMAIN_GLOBAL structure
 * \param pop population id
 * \param *file_in input .net file
 */

void Initialize_FDomainGlobal(struct FDOMAIN_GLOBAL *p, int pop, FILE *file_in)
{
	unsigned int fdm;

	if( p == NULL ) {
		printf("\n\tERROR: unable to allocate memory for list of FDOMAINS inside population %d", pop);
		fclose(file_in);
		exit(-1);
	}
	else {

		for( fdm = 0; fdm < population[pop].n_fdomains; fdm++ ) {

			p[fdm].label = calloc( MAX_STRING_SIZE, sizeof( char ) );

			if( p[fdm].label == NULL ) {
				printf("\n\tERROR: unable to allocate memory for label for FDOMAIN %d inside population %d", fdm, pop);
				fclose(file_in);
				exit(-1);
			}

			p[fdm].n_child_fdomains = 0;
			p[fdm].child_fdomain = NULL;

			p[fdm].label[0] = '\0';
			p[fdm].model = NULL;

		}

	}

	return;
}





//------------------------------------------------------------------------

/**
 * Produces the child fdomains id information from the parent fdomains id in the FDOMAIN_GLOBAL structures
 * \param pop population id
 */

void Process_FDomainsGlobal_Childs( int pop )
{
	int pid;
	unsigned int fdm;

	for( fdm=0; fdm<population[pop].n_fdomains; fdm++ ) {
		
		pid = population[pop].fdomain[fdm].parent_fdomain;
		if( pid >= 0 ) {			
			population[pop].fdomain[pid].n_child_fdomains++;
			population[pop].fdomain[pid].child_fdomain = realloc( population[pop].fdomain[pid].child_fdomain, population[pop].fdomain[pid].n_child_fdomains * sizeof(int) );
			population[pop].fdomain[pid].child_fdomain[ population[pop].fdomain[pid].n_child_fdomains - 1 ] = fdm;
		}

	}
	
	return;
}





//------------------------------------------------------------------------

/**
 * Prepares the POPULATION structures for simulation
 */

void Prepare_Populations(void)
{
  unsigned int pop;
  unsigned long nrn = 0, syn;


  for( pop = 0; pop < total_populations; pop++ ) {

    population[pop].activity_level = 0.0;
    population[pop].hyperpolarised = 0.0;
    population[pop].depolarised    = 0.0;

    population[pop].mean_firing_rate = 0.0;
    population[pop].stdev_firing_rate = 0.0;

    population[pop].total_spikes = 0;
    population[pop].total_pscs = 0;


    population[pop].first_neuron = nrn;
    nrn += population[pop].n_neurons;

    for (nrn = population[pop].first_neuron; nrn < population[pop].first_neuron + population[pop].n_neurons; nrn++) {
      neuron[nrn].population = pop;
      //count total number of spikes and pscs in this population
      for (syn = 0; syn < neuron[nrn].n_synapses; syn++) {
		population[pop].total_spikes += neuron[nrn].synapse[syn].spikes_cue;
		population[pop].total_pscs   += neuron[nrn].synapse[syn].activated;
      }
    }	

  } //cycle over pop

  return;
}





//------------------------------------------------------------------------

/**
 * Update all information variables in the POPULATION structure using the
 * data that was made available by each neuron during Update_Neurons()
 */

void Update_Populations(void)
{
  unsigned int pop;

  // NOTE: information regarding each individual neuron is provided to POPULATION structures
  // during Update_Neurons() for preformance reasons

  for (pop = 0; pop < total_populations; pop++) {

    //results in percentage
    population[pop].activity_level = 100.0 * population[pop].activity_level / population[pop].n_neurons;
    population[pop].hyperpolarised = 100.0 * population[pop].hyperpolarised / population[pop].n_neurons;
    population[pop].depolarised    = 100.0 * population[pop].depolarised    / population[pop].n_neurons;

    population[pop].mean_firing_rate = population[pop].mean_firing_rate / population[pop].n_neurons;

    if ( population[pop].n_neurons > 1 ) {
      population[pop].stdev_firing_rate = sqrt( population[pop].stdev_firing_rate / population[pop].n_neurons - population[pop].mean_firing_rate * population[pop].mean_firing_rate );
    }
    else {
      population[pop].stdev_firing_rate = 0.0;
    }
    
  }

  return;
}





//------------------------------------------------------------------------

/**
 * Prepare all population data to be updated
 */

void Prepare_Update_Populations(void)
{
  unsigned int pop;

  for (pop = 0; pop < total_populations; pop++) {

    population[pop].activity_level = 0.0;
    population[pop].hyperpolarised = 0.0;
    population[pop].depolarised    = 0.0;

    population[pop].mean_firing_rate = 0.0;
    population[pop].stdev_firing_rate = 0.0;

  }

  return;
}





//------------------------------------------------------------------------

/**
 * Resets activity: clear all spikes and pscs
 */

void Reset_System(void)
{
  unsigned int pop;
  unsigned long nrn, syn;

  for (nrn = 0; nrn < total_neurons; nrn++) {
	for (syn = 0; syn < neuron[nrn].n_synapses; syn++) {
	  //clear all PSCs
	  neuron[nrn].synapse[syn].activated = 0;
	  neuron[nrn].synapse[syn].psr_time = -LONG_TIME_AGO; //the '-' sign is correct as we want a positive time (LONG_TIME_AGO is negative)!
	  //clear all Spikes
	  neuron[nrn].synapse[syn].spikes_cue = 0;
	}	
  }

  for( pop = 0; pop < total_populations; pop++ ) {
	population[pop].total_spikes = 0;
	population[pop].total_pscs = 0;
  }

  return;
}





//------------------------------------------------------------------------

/**
 * Free all memory associated with POPULATIONS
 */

void Free_Populations(void)
{
	unsigned int pop, fdm;

	for( pop=0; pop<total_populations; pop++ ) {

		free( population[pop].label );

		for( fdm=0; fdm<population[pop].n_fdomains; fdm++ ) {
			free( population[pop].fdomain[fdm].label );
			population[pop].fdomain[fdm].model = NULL; 
			if( population[pop].fdomain[fdm].n_child_fdomains > 0 ) {
				free( population[pop].fdomain[fdm].child_fdomain );
			}
		}

		free( population[pop].fdomain );

	}

	free( population );

	return;
}
