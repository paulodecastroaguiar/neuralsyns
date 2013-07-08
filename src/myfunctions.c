//************************************************************************
//*                                                                      *
//*                       M Y   F U N C T I O N S                        *
//*                                                                      *
//************************************************************************

/**
 * \file myfunctions.c
 * \author Paulo Aguiar
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <gsl/gsl_rng.h>

#include "general.h"
#include "models.h"
#include "graphics.h"
#include "populations.h"
#include "myfunctions.h"
#include "neurons.h"
#include "synapses.h"


extern struct POPULATION *population;
extern unsigned int total_populations;
extern struct NEURON *neuron;
extern unsigned long total_neurons;
extern unsigned long total_synapses;
extern struct CONNECTION **connection;

extern double t;
extern double dt;

extern gsl_rng *rng;		//pointer to the global random number generator


extern struct GRAPHICS graphics;





//------------------------------------------------------------------------

/**
 * Manipulates network model data _before_ simulation is started
 *
 * This function is called in NeuralSyns after all variables are loaded from the network file, and _before_ the simulation is started. Change this function to manipulate or output any data you want. Check the documentation, in directory ~/NeuralSyns/doc, for information regarding the available variables and data structures.
 * Use NeuralSyns with the -o flag and no simulation time -t 0.0, if you want to create a network with these changes permanently applyed.
 * IMPORTANT: note that to execute the code included in this functions you need to call the compilation script ~/bin/NeuralSyns/NeuralSyns_update.sh AND run NeuralSyns with the -u flag.
 */

void MyFunction_PreSimCycle_Gateway(void)
{
  
  // EXAMPLE:
  // This code changes the axon delay according to the distance between the neurons
  /*
  unsigned long syn;
  unsigned long nrn_i, nrn_o;
  double spike_velocity = 20.0; // [space units/ms]

  for (syn = 0; syn < total_synapses; syn++) {

    nrn_i = synapse[syn].neuron_i;
    nrn_o = synapse[syn].neuron_o;

    synapse[syn].total_delay = 1.0 / spike_velocity * 
      sqrt( ( neuron[nrn_i].position.x - neuron[nrn_o].position.x ) * 
	    ( neuron[nrn_i].position.x - neuron[nrn_o].position.x ) +
	    ( neuron[nrn_i].position.y - neuron[nrn_o].position.y ) *
	    ( neuron[nrn_i].position.y - neuron[nrn_o].position.y ) +
	    ( neuron[nrn_i].position.z - neuron[nrn_o].position.z ) *
	    ( neuron[nrn_i].position.z - neuron[nrn_o].position.z ) );

  }
  */

  return;
}





//------------------------------------------------------------------------

/**
 * Gateway to the simulation's main cycle
 *
 * This function is called within the main cycle, _before_ any of the dynamics update functions are called. The available network simulation data corresponds therefore to the computed data from the previous time step. Change this function to manipulate or output any data you want. A file, with pointer output_data, is created for you and is closed after the simulation finishes; you can use it to export data. Check the documentation, in directory ~/NeuralSyns/doc, for information regarding the available variables and data structures. Naturally, use this function with special care...
 * IMPORTANT: note that to execute the code included in this functions you need to call the compilation script ~/bin/NeuralSyns/NeuralSyns_update.sh AND run NeuralSyns with the -u flag.
 */

void MyFunction_IntraSimCycle_Gateway(FILE *output_data)
{

  // EXAMPLE:
  // This code exports the total amount of spikes and post-synaptic currents
  //  traveling/activated in the network, at each time step

  
  unsigned int pop;
  unsigned long total_spikes = 0, total_pscs = 0;

  for (pop=0; pop<total_populations; pop++) {
    total_spikes += population[pop].total_spikes;
    total_pscs   += population[pop].total_pscs;
  }

  fprintf(output_data, "%.3f\t%ld\t%ld\n", t, total_spikes, total_pscs);
  

  return;
}



// Suggestion: if you have different proceadures that you consistently use,
// write them as functions and, inside MyFunction_PreSimCycle_Gateway() or 
// MyFunction_IntraSimCycle_Gateway(), simply call the specific function you
// want to use for that moment. This will prevent you from commenting / 
// uncommenting large amounts of code each time you want to use a different 
// proceadure. Just remember to place the functions prototypes in myfunctions.h.


/*For your reference:
//------------------------------------------------------------------------
//
// IMPORTANT DATA STRUCTURES AND VARIABLES YOU CAN USE:
//
//------------------------------------------------------------------------


// POPULATION structure

// populations.h

//------------------------------------------------------------------------


// NEURON structure

// neurons.h

//------------------------------------------------------------------------


// FDOMAIN structure

// neurons.h

//------------------------------------------------------------------------


// SYNAPSE structure

// synapses.h
                                                  
//-----------------------------------------------------------------------


// CONNECTION structure

// synapses.h
                                                  
*///-----------------------------------------------------------------------
