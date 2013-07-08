//************************************************************************
//*                                                                      *
//*                          E L E C T R O D E S                         *
//*                                                                      *
//************************************************************************

/**
 * \file electrodes.c
 * \author Paulo Aguiar
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_permutation.h>

#include "general.h"
#include "models.h"
#include "electrodes.h"
#include "populations.h"
#include "neurons.h"
#include "synapses.h"

extern struct POPULATION *population;
extern unsigned int total_populations;
extern struct NEURON *neuron;
extern unsigned long total_neurons;
extern struct CONNECTION **connection;
extern struct ELECTRODE *electrode;
extern unsigned long total_electrodes;

extern struct FLAGS flags;

extern double t;
extern double dt;

extern gsl_rng *rng;		//pointer to the global random number generator





//------------------------------------------------------------------------

/**
 * Goes through the electrodes file (.elec) and acquires all electrodes
 * \param elec_file file containing the electrodes types and parameters
 */

void Acquire_All_Electrodes(FILE *elec_file)
{
  int status, match;
  char elec_type[MAX_STRING_SIZE], temp_line[80];
  unsigned long nrn, neuron_first, neuron_last, sample_size, selected_nrn;
  double elec_time, duration, period, param_A, param_B, isi;
  gsl_permutation *perm = NULL;
  char *temp_ptr;   //holds temporary data to avoid fgets warning

  printf("\n-> Start reading electrodes file.");
  fflush(stdout);


  //Skip all the comment lines. Begin at START keyword
  temp_ptr = fgets(temp_line, 80, elec_file);

  while (strcmp(temp_line, "START\n") != 0) {
    if (fgets(temp_line, 80, elec_file) == NULL) {
      printf("\n\tERROR: the keyword START was not found in input electrodes file\n\n");
      fclose(elec_file);
			Free_Electrodes();
			Free_Neurons();
			Free_Connections();
			Free_Populations();
			exit(-1);
    }
  }

  printf("\n\tElectrode read:");

  while ((status = fscanf(elec_file, "\n%lf", &elec_time)) != EOF) {

		printf("\n\t[t=%.3f] > electrode type:", elec_time);
		
		status = fscanf(elec_file, "\t[%ld,%ld]\t%s", &neuron_first, &neuron_last, elec_type);
		
		if (neuron_first < 0 || neuron_last > total_neurons - 1) {
			//the electrodes file was written incorrectly... prevent "segmentation fault"      
			printf("\n\nProblems in the electrodes file! You applied a electrode to a non-existing NEURON\n%c\n", BELL);
			fclose(elec_file);
			Free_Electrodes();
			Free_Neurons();
			Free_Connections();
			Free_Populations();
			exit(-1);
		}
		
		// read information accordingly to the electrodes type and add a new electrode using:
		match = 0;
		
		
		// INJECT ELECTRODES
		// Add_Electrode(type, neuron_first, neuron_last, next_time, period, end_time, param_A, param_B)
		
		//fire the neuron
		if( strcmp( "injFire", elec_type ) == 0 ) {
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, 0.0, elec_time + dt, 0.0, 0.0 );
			match = 1;
		}
		
		//fire periodically the neuron
		if( strcmp( "injFirePeriodically", elec_type ) == 0 ) {
			status = fscanf( elec_file, "\t%lf\t%lf", &period, &duration );
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, period, elec_time + duration, 0.0, 0.0 );
			match = 1;
		}
		
		//fire the neuron according to given Probability Density Function
		if( strcmp( "injFirePDF", elec_type ) == 0 ) {
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, 0.0, -LONG_TIME_AGO, 0.0, 0.0 );
			match = 1;
		}

		//fire random neurons from given range
		if( strcmp( "injFireRandom", elec_type ) == 0 ) {
			status = fscanf(elec_file, "\t%ld", &sample_size);
			perm = gsl_permutation_calloc(  neuron_last - neuron_first );	//create a permutation vector with as many entries as the neurons in the range
			gsl_ran_shuffle (rng, perm->data, neuron_last - neuron_first, sizeof(size_t));
			for( nrn = 0; nrn < sample_size; nrn++ ) {
				selected_nrn = neuron_first + gsl_permutation_get(perm, nrn);
				Add_Electrode( "injFire", selected_nrn, selected_nrn, elec_time, 0.0, elec_time + dt, 0.0, 0.0 );
			}
			gsl_permutation_free(perm);
			match = 1;
		}
		
		//fire neurons asynchronously - Poisson Process
		if( strcmp( "injFirePoissonAsync", elec_type ) == 0 ) {
			status = fscanf(elec_file, "\t%lf\t%lf", &period, &duration);
			for( nrn = neuron_first; nrn < neuron_last + 1; nrn++ ) {
				isi = RND_Exponential(period);
				while( isi < dt ) {
					isi = RND_Exponential(period);
				}
				Add_Electrode( "injFirePoissonSync", nrn, nrn, elec_time + isi, period, elec_time + duration, 0.0, 0.0 );
			}
			match = 1;
		}
		
		//fire neurons synchronously - Poisson Process
		if( strcmp( "injFirePoissonSync", elec_type ) == 0 ) {	
			status = fscanf(elec_file, "\t%lf\t%lf", &period, &duration);
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, period, elec_time + duration, 0.0, 0.0 );
			match = 1;
		}
		
		//constant current injection  			
		if( strcmp( "injCurrentConstant", elec_type ) == 0 ) {	
			status = fscanf(elec_file, "\t%lf\t%lf", &param_A, &duration);
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, 0.0, elec_time + duration, param_A, 0.0 );
			match = 1;
		}
		
		//noise current injection  			
		if( strcmp( "injCurrentGaussianNoise", elec_type ) == 0 ) {	
			status = fscanf(elec_file, "\t%lf\t%lf\t%lf", &param_A, &param_B, &duration);
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, 0.0, elec_time + duration, param_A, param_B );
			match = 1;
		}
		
		//sinusoidal current injection  			
		if( strcmp( "injCurrentSine", elec_type ) == 0 ) {	
			status = fscanf(elec_file, "\t%lf\t%lf\t%lf", &param_A, &param_B, &duration);
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, 1.0 / param_B, elec_time + duration, param_A, elec_time );
			match = 1;
		}
		
		
		// RECORD ELECTRODES
		// Add_Electrode(type, neuron_first, neuron_last, next_time, period, end_time, param_A, param_B)
		
		//record fdm00 membrane potential (ouput);
		if( strcmp( "recV0", elec_type ) == 0 ) {
			status = fscanf( elec_file, "\t%lf\t%lf", &period, &duration );
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, period, elec_time + duration, 0.0, 0.0 );
			match = 1;
		}
		
		//record all fdomains membrane potentials (ouput);
		if( strcmp( "recVAll", elec_type ) == 0 ) {
			status = fscanf( elec_file, "\t%lf\t%lf", &period, &duration );
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, period, elec_time + duration, 0.0, 0.0 );
			match = 1;
		}
		
		//record all state variables for fdm00 (ouput);
		if( strcmp( "recStates0", elec_type ) == 0 ) {
			status = fscanf( elec_file, "\t%lf\t%lf", &period, &duration );
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, period, elec_time + duration, 0.0, 0.0 );
			match = 1;
		}
		
		//record all state variables for all fdomains (ouput);
		if( strcmp( "recStatesAll", elec_type ) == 0 ) {
			status = fscanf( elec_file, "\t%lf\t%lf", &period, &duration );
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, period, elec_time + duration, 0.0, 0.0 );
			match = 1;
		}
		
		//record all state variables for the neuron's synapses (ouput);
		if( strcmp( "recSynStates", elec_type ) == 0 ) {
			status = fscanf( elec_file, "\t%lf\t%lf", &period, &duration );
			Add_Electrode( elec_type, neuron_first, neuron_last, elec_time, period, elec_time + duration, 0.0, 0.0 );
			match = 1;
		}
		
		
		// CHECK FOR UNRECOGNIZED ELECTRODE TYPE
		if( match == 0 ) {
			printf("\nUnrecognized electrode command in the electrodes file\n");
			fclose(elec_file);
			Free_Electrodes();
			Free_Neurons();
			Free_Connections();
			Free_Populations();
			exit(-1);
		}
		
		printf(" %s", elec_type);    
		
	} // while != EOF
	
  printf("\n\tA total of %ld electrodes initiation times have been acquired.", total_electrodes);


  return;
}





//------------------------------------------------------------------------

/**
 * Adds a new ELECTRODE structure
 * \param type id for the type of ELECTRODE
 * \param neuron_first id of the first NEURON in the range affected by this electrode
 * \param neuron_last id of last NEURON in the range affected by this electrode
 * \param next_time futute time at which the electrode will be activated
 * \param period period of the electrode
 * \param end_time time at which the electrode will be terminated
 * \param param_A mean current injected by the electrode
 * \param param_B standard deviation of the current injected by the electrode
 */

void Add_Electrode(char *type, long neuron_first, long neuron_last, double next_time, double period, double end_time, double param_A, double param_B)
{
	char filename[MAX_STRING_SIZE];
	unsigned int pop, pop_i, pop_o, fdm, i;
	unsigned long nrn, syn, elec;

  total_electrodes += 1;

  electrode = (struct ELECTRODE *) realloc(electrode, total_electrodes * sizeof(struct ELECTRODE));

  if (electrode == NULL) {
    printf("\n\tERROR: unable to re-allocate memory for list of ELECTRODES");
    printf("\n\tERROR occured in function Add_Electrode, %s: %d\n\n", __FILE__, __LINE__);
		Free_Neurons();
		Free_Connections();
		Free_Populations();					
    exit(-1);
  }

	elec = total_electrodes - 1;
  strcpy( electrode[elec].type, type );
  electrode[elec].neuron_first = neuron_first;
  electrode[elec].neuron_last = neuron_last;
  electrode[elec].next_time = next_time;
  electrode[elec].period = period;
  electrode[elec].end_time = end_time;
  electrode[elec].param_A = param_A;
  electrode[elec].param_B = param_B;

	//check if it's an input or output electrode and open a file for output electrodes
	if( type[0] == 'i' ) {
		electrode[elec].out_record_file = NULL;
	}
	else {
		//build filename
		sprintf( filename, "out_elec_%ld_%s.dat", total_electrodes, type);

		//fopen
		electrode[elec].out_record_file = fopen( filename, "w" );

		if( electrode[elec].out_record_file == NULL ) {
			printf("\n\tERROR: unable to create new file to store electrode %ld readings", elec);
			printf("\n\tERROR occured in function Add_Electrode, %s: %d\n\n", __FILE__, __LINE__);
			Free_Neurons();
			Free_Connections();
			Free_Populations();					
			exit(-1);
		}

		//CREATE HEADERS - comment to ease import of this data (to MATLAB for example)
		//common component
		fprintf( electrode[elec].out_record_file, "Readings from electrode %ld, type = %s, applied in neurons [%ld, %ld], during the time window [%.3lf, %.3lf]", elec, type, neuron_first, neuron_last, next_time, end_time );
		
		//record fdm00 membrane potential (ouput);
		if( strcmp( "recV0", electrode[elec].type ) == 0 ) {
			fprintf( electrode[elec].out_record_file, "\nt[ms]" );
			for( nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++ ) {
				fprintf( electrode[elec].out_record_file, "\tnrn(%ld).fdm(0).V", nrn );
			}
		}
	
		//record all fdomains membrane potentials (ouput);
		if( strcmp( "recVAll", electrode[elec].type ) == 0 ) {
			fprintf( electrode[elec].out_record_file, "\nt[ms]" );
			for( nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++ ) {
				for( fdm = 0; fdm < population[ neuron[nrn].population ].n_fdomains; fdm++ ) {
					fprintf( electrode[elec].out_record_file, "\tnrn(%ld).fdm(%d).V", nrn, fdm );
				}
			}
		}
			
		//record all state variables for fdm00 (ouput);
		if( strcmp( "recStates0", electrode[elec].type ) == 0 ) {
			fprintf( electrode[elec].out_record_file, "\nt[ms]" );
			for( nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++ ) {
				fprintf( electrode[elec].out_record_file, "\tnrn(%ld).fdm(0).V", nrn );
				pop = neuron[nrn].population;
				for( i = 0; i < population[ neuron[nrn].population ].fdomain[0].model->n_statevars; i++ ) {
					fprintf( electrode[elec].out_record_file, "\tnrn(%ld).fdm(0).%s", nrn, population[pop].fdomain[0].model->statevar_label[i] );
				}
			}
		}
			
		//record all state variables for all fdomains (ouput);
		if( strcmp( "recStatesAll", electrode[elec].type ) == 0 ) {
			fprintf( electrode[elec].out_record_file, "\nt[ms]" );
			for( nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++ ) {
				pop = neuron[nrn].population;
				for( fdm = 0; fdm < population[ neuron[nrn].population ].n_fdomains; fdm++ ) {
					fprintf( electrode[elec].out_record_file, "\tnrn(%ld).fdm(%d).V", nrn, fdm );
					for( i = 0; i < population[ neuron[nrn].population ].fdomain[fdm].model->n_statevars; i++ ) {
						fprintf( electrode[elec].out_record_file, "\t\tnrn(%ld).fdm(%d).%s", nrn, fdm, population[pop].fdomain[0].model->statevar_label[i] );
					}
				}
			}
		}
		
		//record all state variables for the neuron's synapses (ouput);
		if( strcmp( "recSynStates", electrode[elec].type ) == 0 ) {
			fprintf( electrode[elec].out_record_file, "\nt[ms]" );
			for( nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++ ) {
				for( syn = 0; syn < neuron[nrn].n_synapses; syn++ ) { //please be pacient with the next line... (I really wanted to put it in a single line)
					fprintf( electrode[elec].out_record_file, "\tweigth[1]" );
					pop_i = neuron[nrn].population;
					pop_o = neuron[ neuron[nrn].synapse[syn].target_neuron ].population;
					for( i = 0; i < connection[pop_i][pop_o].current_model->n_statevars; i++ ) {
						fprintf( electrode[elec].out_record_file, "\t%s", connection[pop_i][pop_o].current_model->statevar_label[i] );
					}
					for( i = 0; i < connection[pop_i][pop_o].plasticity_model->n_statevars; i++ ) {
						fprintf( electrode[elec].out_record_file, "\t%s", connection[pop_i][pop_o].plasticity_model->statevar_label[i] );
					}
				}
			}
		}		

		//end of types of recording electrodes

	}//closes "else" for recording (rec) electrodes

  return;
}





//------------------------------------------------------------------------

/**
 * Starts a electrode 
 * \param elec id of the ELECTRODE structure
 */

void Apply_Electrode(int elec)
{
	unsigned int i, fdm;
	unsigned long nrn, syn;

  //inform the user that a ELECTRODE is being applyed
	/*
	if (flags.text == 1) {
    printf("\nelectrode %s applied at t=%.2f", electrode[elec].type, t);
  }
	*/


	// INJECT ELECTRODES

	//fire the neuron
	if( strcmp( "injFire", electrode[elec].type ) == 0 ) {
    Electrode_Fire_Neurons(elec);
		return;
	}
			
	//fire periodically the neuron
	if( strcmp( "injFirePeriodically", electrode[elec].type ) == 0 ) {
    Electrode_Fire_Neurons_Periodically(elec);
		return;
	}

	//fire the neuron according to given Probability Density Function
	if( strcmp( "injFirePDF", electrode[elec].type ) == 0 ) {
    Electrode_Fire_Neurons_PDF(elec);
		return;
	}
					
	//fire random neurons from given range
	//REDEFINED IN TERMS OF injFire ELECTRODES BEFORE Add_Electrode()
	//if( strcmp( "injFireRandom", electrode[elec].type ) == 0 ) {	
	//}
			
	//fire neurons asynchronously with Poisson Process
	//REDEFINED IN TERMS OF injFirePoissonSync ELECTRODES BEFORE Add_Electrode()
	//if( strcmp( "injFirePoissonAsync", electrode[elec].type ) == 0 ) {
	//}

	//fire neurons synchronously with Poisson Process
	if( strcmp( "injFirePoissonSync", electrode[elec].type ) == 0 ) {	
    Electrode_Fire_Neurons_Poisson(elec);
		return;
	}

	//constant current injection  			
	if( strcmp( "injCurrentConstant", electrode[elec].type ) == 0 ) {	
    Electrode_Current_Constant(elec);
		return;
	}

	//noise current injection  			
	if( strcmp( "injCurrentGaussianNoise", electrode[elec].type ) == 0 ) {	
    Electrode_Current_GaussianNoise(elec);
		return;
	}

	//sinusoidal current injection  			
	if( strcmp( "injCurrentSine", electrode[elec].type ) == 0 ) {	
		Electrode_Current_Sine(elec);
		return;
	}


	// RECORD ELECTRODES

	//record fdm00 membrane potential (ouput);
	if( strcmp( "recV0", electrode[elec].type ) == 0 ) {
		fprintf( electrode[elec].out_record_file, "\n%.3lf", t );
	  for( nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++ ) {
			fprintf( electrode[elec].out_record_file, "\t%.3lf", neuron[nrn].fdomain[0].V[0] );
		}
		electrode[elec].next_time = t + electrode[elec].period * dt;
		return;
	}
	
	//record all fdomains membrane potentials (ouput);
	if( strcmp( "recVAll", electrode[elec].type ) == 0 ) {
		fprintf( electrode[elec].out_record_file, "\n%.3lf", t );
	  for( nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++ ) {
			for( fdm = 0; fdm < population[ neuron[nrn].population ].n_fdomains; fdm++ ) {
				fprintf( electrode[elec].out_record_file, "\t%.3lf", neuron[nrn].fdomain[fdm].V[0] );
			}
		}
		electrode[elec].next_time = t + electrode[elec].period * dt;
		return;
	}
			
	//record all state variables for fdm00 (ouput);
	if( strcmp( "recStates0", electrode[elec].type ) == 0 ) {
		fprintf( electrode[elec].out_record_file, "\n%.3lf", t );
	  for( nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++ ) {
			fprintf( electrode[elec].out_record_file, "\t%.3lf", neuron[nrn].fdomain[0].V[0] );
			for( i = 0; i < population[ neuron[nrn].population ].fdomain[0].model->n_statevars; i++ ) {
				fprintf( electrode[elec].out_record_file, "\t%.3lg", neuron[nrn].fdomain[0].model_statevar[i][0] );
			}
		}
		electrode[elec].next_time = t + electrode[elec].period * dt;
		return;
	}
			
	//record all state variables for all fdomains (ouput);
	if( strcmp( "recStatesAll", electrode[elec].type ) == 0 ) {
		fprintf( electrode[elec].out_record_file, "\n%.3lf", t );
	  for( nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++ ) {
			for( fdm = 0; fdm < population[ neuron[nrn].population ].n_fdomains; fdm++ ) {
				fprintf( electrode[elec].out_record_file, "\t%.3lf", neuron[nrn].fdomain[fdm].V[0] );
				for( i = 0; i < population[ neuron[nrn].population ].fdomain[fdm].model->n_statevars; i++ ) {
					fprintf( electrode[elec].out_record_file, "\t%.3lg", neuron[nrn].fdomain[fdm].model_statevar[i][0] );
				}
			}
		}
		electrode[elec].next_time = t + electrode[elec].period * dt;
		return;
	}
			
	//record all state variables for the neuron's synapses (ouput);
	if( strcmp( "recSynStates", electrode[elec].type ) == 0 ) {

		fprintf( electrode[elec].out_record_file, "\n%.3lf", t );

	  for( nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++ ) {
			for( syn = 0; syn < neuron[nrn].n_synapses; syn++ ) { //please be pacient with the next line... (I really wanted to put it in a single line)
				fprintf( electrode[elec].out_record_file, "\t%.3lg", neuron[nrn].synapse[syn].weight );
				for( i = 0; i < connection[   neuron[nrn].population   ][   neuron[ neuron[nrn].synapse[syn].target_neuron ].population   ].current_model->n_statevars; i++ ) {
					fprintf( electrode[elec].out_record_file, "\t%.3lg", neuron[nrn].synapse[syn].current_statevar[i][0] );
				}
				for( i = 0; i < connection[   neuron[nrn].population   ][   neuron[ neuron[nrn].synapse[syn].target_neuron ].population   ].plasticity_model->n_statevars; i++ ) {
					fprintf( electrode[elec].out_record_file, "\t%.3lg", neuron[nrn].synapse[syn].plasticity_statevar[i][0] );
				}
			}
		}
		electrode[elec].next_time = t + electrode[elec].period * dt;
		return;
	}		

	// IT'S NOT GOOD IF YOU ARRIVED HERE EMPTY HANDED...
	printf("\nUnrecognized electrode type %s in the electrodes file. Check the available electrode types in the template.elec file (which you can create by running NeuralSyns without arguments).\n", electrode[elec].type);
	fclose(electrode[elec].out_record_file);
	Free_Electrodes();
	Free_Neurons();
	Free_Connections();
	Free_Populations();
	exit(-1);


  return;
}





//------------------------------------------------------------------------

/** Removes a ELECTRODE structure from the array containing all electrodes
 * \param elec id of the ELECTRODE structure
 */

void Remove_Electrode(int elec)
{
  unsigned long i;

	if( electrode[elec].out_record_file != NULL ) {
		fclose( electrode[elec].out_record_file );
	}

  total_electrodes -= 1;

  for (i = elec; i < total_electrodes; i++) {
    electrode[i] = electrode[i + 1];
	}

	/*
	char type[MAX_STRING_SIZE]; 
  unsigned long neuron_first;
  unsigned long neuron_last;

  double period;            
  double next_time;         
  double end_time;          

  double param_A;           
  double param_B;           

	FILE *out_record_file;    
	*/


  electrode = (struct ELECTRODE *) realloc(electrode, total_electrodes * sizeof(struct ELECTRODE));

  if (electrode == NULL && total_electrodes > 0) {
    printf("\n\tERROR: unable to re-allocate memory for list of ELECTRODE");
    printf("\n\tERROR occured in function Add_Electrode, %s: %d\n\n", __FILE__, __LINE__);
		Free_Neurons();
		Free_Connections();
		Free_Populations();					
    exit(-1);
  }

  return;
}





//------------------------------------------------------------------------

/**
 * Produces a electrodes data file template for future use (template.elec)
 */

void Produce_Electrodes_Template(void)
{
  FILE *out_elect_template_file;

  out_elect_template_file = fopen("template.elec", "w");

  if (out_elect_template_file == NULL) {
    printf("\n\tERROR: unable to write input electrodes template file 'elecdata_template.elec'\n");
    exit(-1);
  }

  fprintf(out_elect_template_file, "# This is a template for an input electrodes protocol file.\n");
  fprintf(out_elect_template_file, "# The commands start only after the uncommented 'START' keyword.\n");
  fprintf(out_elect_template_file, "# The structure is:\n#\n# START\n#\n");
  fprintf(out_elect_template_file, "# <electrode_00 start time [ms]>\t[<first neuron id>,<last neuron id>]\t<electrode type> <parameters> ...\n");
  fprintf(out_elect_template_file, "# <electrode_01 start time [ms]>\t[<first neuron id>,<last neuron id>]\t<electrode type> <parameters> ...\n");
  fprintf(out_elect_template_file, "# ...\n#\n");
  fprintf(out_elect_template_file, "# Possible electrode types, and respective parameters, are:\n");
  fprintf(out_elect_template_file, "# (stimulation electrodes have the 'inj' prefix while recording electrodes have the 'rec' prefix)\n");
  fprintf(out_elect_template_file, "# fire neurons in range                       : injFire\n");
  fprintf(out_elect_template_file, "# fire neurons periodically                   : injFirePeriodically <period [ms]> <duration [ms]>\n");
  fprintf(out_elect_template_file, "# fires a selected number of random neurons   : injFireRandom <sample size [1]>\n");
  fprintf(out_elect_template_file, "# fire neurons independently, Poisson process : injFirePoissonAsync <mean value [ms]> <duration [ms]>\n");
  fprintf(out_elect_template_file, "# fire neurons synchronously, Poisson process : injFirePoissonSync <mean value [ms]> <duration [ms]>\n");
  fprintf(out_elect_template_file, "# inject constant current                     : injCurrentConstant <current [nA]> <duration [ms]>\n");
  fprintf(out_elect_template_file, "# inject random current (Gaussian)            : injCurrentGaussianNoise <current mean [nA]> <current stdev [nA]> <duration [ms]>\n");
	fprintf(out_elect_template_file, "# inject sinusoidal current                   : injCurrentSine <current amplitude [nA]> <frequency [Hz]> <duration [ms]>\n");
	fprintf(out_elect_template_file, "# record membrane potential from fdm00        : recV0 <record dt periodicity [1] (integer>0)> <duration [ms]>\n");
	fprintf(out_elect_template_file, "# record membrane potential from all fdomais  : recVAll <record dt periodicity [1] (integer>0)> <duration [ms]>\n");
	fprintf(out_elect_template_file, "# record state variables from fdm00           : recStates0 <record dt periodicity [1] (integer>0)> <duration [ms]>\n");
	fprintf(out_elect_template_file, "# record state variables from all fdomains    : recStatesAll <record dt periodicity [1] (integer>0)> <duration [ms]>\n");
	fprintf(out_elect_template_file, "# record state variables from all synapses    : recSynStates <record dt periodicity [1] (integer>0)> <duration [ms]>\n");

  fprintf(out_elect_template_file, "#\n# Example:\n#\n#\n# START\n#\n");
  fprintf(out_elect_template_file, "# 100.0\t[100,200]\trecVAll 1 200.0\n");
  fprintf(out_elect_template_file, "# 150.0\t[0,100]\t\tinjCurrentGaussianNoise\t0.5\t0.1\t100.0\n");
  fprintf(out_elect_template_file, "# 200.0\t[1,1]\t\tinjFire\n");
  fprintf(out_elect_template_file, "# 230.0\t[400,500]\tinjCurrentSine\t1.0\t8.0\t100.0\n");

  fprintf(out_elect_template_file, "\n\n\nSTART\n\n");
  fprintf(out_elect_template_file, "0.0\t[0,0]\tinjFire\n");

  fclose(out_elect_template_file);

  return;
}





//------------------------------------------------------------------------

/**
 * Free memory used by ELECTRODE
 */

void Free_Electrodes(void)
{
	unsigned int elec;

	for( elec=0; elec<total_electrodes; elec++) {
		if( electrode[elec].out_record_file != NULL ) {
			fclose( electrode[elec].out_record_file );
		}
	}

	free( electrode );	

	return;
}





//------------------------------------------------------------------------

/**
 * Fires all neurons within the given range
 * \param elec id of the ELECTRODE structure
 */

void Electrode_Fire_Neurons(int elec)
{
  unsigned long nrn;

  for (nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++) {
    neuron[nrn].spiketime_from_electrode = electrode[elec].next_time;
  }

  return;
}





//------------------------------------------------------------------------

/**
 * Fires periodically all neurons within the given range
 * \param elec id of the ELECTRODE structure
 */

void Electrode_Fire_Neurons_Periodically(int elec)
{
  unsigned long nrn;

  for (nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++) {
    neuron[nrn].spiketime_from_electrode = electrode[elec].next_time;
  }

  electrode[elec].next_time = t + electrode[elec].period;

  return;
}





//------------------------------------------------------------------------

/**
 * Fires all neurons within the given range using a Poisson Prossess
 * \param elec id of the ELECTRODE structure
 */

void Electrode_Fire_Neurons_Poisson(int elec)
{
  unsigned long nrn;
  double isi;

  for (nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++) {
    neuron[nrn].spiketime_from_electrode = electrode[elec].next_time;
  }

  isi = RND_Exponential(electrode[elec].period);
  while( isi < dt ) {
    isi = RND_Exponential(electrode[elec].period);
  }
  
  electrode[elec].next_time = t + isi;


  return;
}





//------------------------------------------------------------------------

/**
 * Injects constant current [nA] into neurons in range
 * \param elec id of the ELECTRODE structure
 */

void Electrode_Current_Constant(int elec)
{
  unsigned long nrn;
	unsigned short order;

  for( nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++ ) {
		for( order = 0; order < NUMERICAL_ORDER; order++ ) {
			neuron[nrn].fdomain[0].I_total_electrode[order] += electrode[elec].param_A;
		}
  }

  electrode[elec].next_time = t + dt;

  return;
}





//------------------------------------------------------------------------

/**
 * Injects Gaussian current noise [nA] into neurons in range
 * \param elec id of the ELECTRODE structure
 */

void Electrode_Current_GaussianNoise(int elec)
{
  unsigned long nrn;
  unsigned int order;
  double rn;

  for (nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++) {
		rn = RND_Normal( electrode[elec].param_A, electrode[elec].param_B);
		for( order = 0; order < NUMERICAL_ORDER; order++ ) {
			neuron[nrn].fdomain[0].I_total_electrode[order] += rn; //well, is this fair...? it applies the same "noise" to all orders.
		}
  }

  electrode[elec].next_time = t + dt;

  return;
}





//------------------------------------------------------------------------

/**
 * Injects a sinusoidal current [nA] into neurons in range
 * \param elec id of the ELECTRODE structure
 */

void Electrode_Current_Sine(int elec)
{
  unsigned long nrn;
  unsigned int order;
	double Inj;

	for( order = 0; order < NUMERICAL_ORDER; order++ ) {

		Inj = electrode[elec].param_A * sin( 0.002 * PI / electrode[elec].period * ( t + dt * order ) ); //0.001=ms to s; PHASE IS ZERO!

		for (nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++) {
			neuron[nrn].fdomain[0].I_total_electrode[order] += Inj;
		}
  }

  electrode[elec].next_time = t + dt;

  return;
}





//------------------------------------------------------------------------

/*
Fires all neurons according to a user defined Probability Density Function. 
\param elec id of the ELECTRODE structure

This function does not appear automatically in the documentation
*/

void Electrode_Fire_Neurons_PDF(int elec)
{
	double f_min = 0.5;       //maximum spike frequency [Hz]
	double f_max = 5.0;       //maximum spike frequency [Hz]
	double t_center = 1000.0; //time when center of the place field is reached [ms]
	double sigma = 400.0;     //standard dev of the Gaussian profile [ms]
	double f_mod = 10.0;      //modulating frequency [Hz] (theta)
	double A_mod = 0.2;       //fraction of modulation [0,1] [1]

  unsigned long nrn;
	double f_spike, p0, p;


	if( 2.0 * f_max * dt / 1000.0 > 1.0 ) {
    printf("WARNING: given dt and maximum spike frequency, the probability of spike occuring in a bin may exceed 1!");
	}


	// USER DEFINED PROBABILITY DENSITY FUNCTION
	f_spike = f_min + (f_max-f_min) * exp( -( t - t_center ) * ( t - t_center ) / ( sigma * sigma ) );
	p0 = f_spike * dt / 1000.0;
	p = p0 * ( 1 + A_mod * cos( 2.0*PI*f_mod * t/1000.0) );
	
  for (nrn = electrode[elec].neuron_first; nrn <= electrode[elec].neuron_last; nrn++) {

		// USER DEFINED PROBABILITY DENSITY FUNCTION
		if( gsl_rng_uniform(rng) < p ) {
			neuron[nrn].spiketime_from_electrode = t;
		}

  }

  electrode[elec].next_time = t + dt;

  return;
}


