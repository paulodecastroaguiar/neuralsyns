/**
 * \file main.c
 * \author Paulo Aguiar
 */

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include <gsl/gsl_rng.h>
#include <GL/freeglut.h>

#include "general.h"
#include "models.h"
#include "populations.h"
#include "neurons.h"
#include "synapses.h"
#include "electrodes.h"
#include "dataio.h"
#include "myfunctions.h"
#include "graphics.h"



//GLOBAL VARIABLES
struct POPULATION *population;
unsigned int total_populations = 0;
struct NEURON *neuron;
unsigned long total_neurons = 0;
unsigned long total_synapses = 0;
struct CONNECTION **connection;
unsigned int total_connections = 0;

struct MODEL_DYNAMICS_NEURON *neuron_model;         ///< N_NEURON_MODELS is defined in models.h
struct MODEL_DYNAMICS_SYNAPSE *synapse_model;       ///< N_SYNAPSE_MODELS is defined in models.h
struct MODEL_DYNAMICS_PLASTICITY *plasticity_model; ///< N_PLASTICITY_MODELS is defined in models.h

struct ELECTRODE *electrode;
unsigned long total_electrodes = 0;

struct FLAGS flags;
struct GRAPHICS graphics;

char *HOME_DIR;

double t = 0.0;	             //ms
double dt = 0.01;            //ms user-defined; could be 1/64=0.0156: finite binary representation
double total_time;           //ms; total simulation time
unsigned long sim_step = 0;  //simulation step

gsl_rng *rng;	               //pointer to the global random number generator



//----------------------------------------------------------------

int
main(int argc, char *argv[])
{
  unsigned int c, pop, fdm;
  unsigned long write_state_periodicity = 1, write_rate_periodicity = 1;
  unsigned long nrn;
	long elec;
  long seed;

  clock_t sim_time_start, sim_time_end, time_update;
  double cpu_time_used;


  double spontaneous_fire_period = 0.0, sys_reset_period = 0.0;

  char *in_electrodes_filename = NULL;
  char *out_network_filename = NULL;

  FILE *out_Vstates_file = NULL;
  FILE *in_electrodes_file = NULL;
  FILE *out_activity_levels_file = NULL;
  FILE *out_spike_times_file = NULL;
  FILE *out_user_data_file = NULL;


  struct option longopts[] = {
    {"total-time", required_argument, NULL, 't'},
    {"dt", required_argument, NULL, 'd'},
    {"output-netfile", required_argument, NULL, 'o'},
    {"textinfo", no_argument, NULL, 'i'},
    {"store-Vstates", required_argument, NULL, 'V'},
    {"store-all-Vstates", required_argument, NULL, 'W'},
    {"store-activity-levels", no_argument, NULL, 'A'},
    {"store-spike-times", no_argument, NULL, 'K'},
    {"graphinfo", no_argument, NULL, 'g'},
    {"graphinfo-paused", no_argument, NULL, 'G'},
    {"elecfile", required_argument, NULL, 's'},
    {"myfunctions", no_argument, NULL, 'u'},
    {"disable-plasticity", no_argument, NULL, 'y'},
    {"enable-rate-estimation", no_argument, NULL, 'f'},
    {"initial-reset", no_argument, NULL, 'r'},
    {"periodic-reset", required_argument, NULL, 'R'},
    {"input-spontaneous-rate", required_argument, NULL, 'S'},
    {0, 0, 0, 0}
  };


  if (argc < 2) {
    printf("\n\nNeuralSyns simulation environment.");
    printf("\n--------------------------------");
    printf("\n[Paulo Aguiar, 2010 - pauloaguiar@fc.up.pt]");
    printf("\n");
    printf("\nThis program is provided to you AS IS and comes with NO WARRANTY.");
    //printf("\nSee the file LICENSE for details.");
    printf("\nIf you publish results which were obtained using NeuralSyns, please cite appropriately NeuralSyns in your publication. This software is distributed under GNU General Public License.");
    printf("\n");

    printf("\nSyntax:");
    printf("\n$ NeuralSyns 'network data file' '--options'");
    printf("\n\noptions:\n--------\n");

    printf("-t, --total_time=TIME\n\ttotal simulation time in ms (default = 1 time step)\n\n");
    printf("-d, --dt=STEP\n\tsimulation time step in ms (default = 0.125=1/8 ms)\n\n");
    printf("-o, --output-netfile=FILE\n\toutput network file\n\n");
    printf("-i, --textinfo\n\tprovides console text information\n\n");
    printf("-V, --store-Vstates=n\n\tstores soma's Vm in 'out_membrane_potentials.dat' every n sim-steps\n\n");
    printf("-W, --store-all-Vstates=n\n\tuse instead of -V to store all compartments Vm every n sim-steps\n\n");
    printf("-A, --store-activity-levels\n\tstores activity levels in 'out_activity_levels.dat' at each time step\n\n");
    printf("-K, --store-spike-times\n\tstores neuron's spike times in 'out_spike_times.dat'\n\n");
    printf("-g, --graphinfo\n\tprovides graphical information (slows down simulation)\n\n");
    printf("-G, --graphinfo-paused\n\tuse instead of -g to start the graphical window in PAUSE mode\n\n");
    printf("-e, --electrodes=FILE\n\telectrodes input file (a template file was created in your working dir)\n\n");
    printf("-u, --myfunctions\n\tenables Manipulate_Data and Output_Data functions (myfunctions.c)\n\n");    
    printf("-y, --disable-plasticity\n\tdisables synaptic plasticity mechanisms\n\n");    
    printf("-f, --enable-rate-estimation\n\tenables firing rate estimation (slows down simulation)\n\n");
    printf("-F, --store-rate-estimation\n\tuse instead of -f to store firing rate estimations (slows down simulation)\n\n");
    printf("-r, --initial-reset\n\tresets system activity initially (erases all spikes and pscs)\n\n");
    printf("-R, --periodic-reset=PERIOD\n\tresets the system activity periodically with PERIOD in ms\n\n");    
    printf("-S, --input-spontaneous-rate=RATE\n\tneurons in Pop00 fire spontaneously with mean firing RATE Hz\n\n");

    //produce a template to help the user       
    Produce_Electrodes_Template();
    printf("\n\tAn electrode-file template has been created: template.elec\n\n");

    exit(0);
  }


	//get home directory; NeuralSyns should be installed in the HOME directory!
	HOME_DIR = getenv("HOME");


  //set default values
  flags.text = 0;
  flags.graphics = 0;
  flags.store_activity_levels_file = 0;
  flags.store_spike_times_file = 0;
  flags.store_network_file = 0;
  flags.store_Vstates_file = 0;
  flags.read_electrodes_file = 0;
  flags.disable_plasticity = 0;
  flags.enable_rate_estimation = 0;
  flags.sys_initial_reset = 0;
  flags.sys_periodic_reset = 0;
  flags.spontaneous_fire = 0;
  flags.single_step = 0;
  flags.myfunctions = 0;
  flags.finish_earlier = 0;

  total_time = 0.0;

  while ((c = getopt_long_only(argc, argv, ":e:o:t:V:W:d:iuAKygGfF:rR:S:", longopts, NULL)) != EOF) {
    switch (c) {
    case 't':
      total_time = atof(optarg);
      break;
    case 'd':
      dt = atof(optarg);
      if (dt>1.0) {
				printf("\nWARNING: you should not use an integration time step larger than 1.0 ms\n");
      }
      break;
    case 'i':
      flags.text = 1;
      break;
    case 'u':
      flags.myfunctions = 1;
      break;
    case 'A':
      flags.store_activity_levels_file = 1;
      break;
    case 'K':
      flags.store_spike_times_file = 1;
      break;
    case 'y':
      flags.disable_plasticity = 1;
      break;
    case 'f':
      flags.enable_rate_estimation = 1;
      break;
    case 'F':
      flags.enable_rate_estimation = 2;
      write_rate_periodicity = atol(optarg);
      if(write_rate_periodicity < 1) {
        printf("\n\tERROR: You have used a bad parameter for the -F option.");
        printf("\n\t       Use an integer larger than 0.\n");
				exit(-1);
      }
      break;
    case 'r':
      flags.sys_initial_reset = 1;
      break;
    case 'R':
      sys_reset_period = atof(optarg);
	  if( sys_reset_period < 0.0 ) {
        printf("\n\tERROR: You have used a bad parameter for the -R option.");
        printf("\n\t       Use a number larger than 0.0\n");
				exit(-1);
      }
      flags.sys_periodic_reset = 1;
      break;
    case 'g':
      flags.graphics = 1;
      break;
    case 'G':
      flags.graphics = 2;
      break;
    case 'o':
      out_network_filename = optarg;
      flags.store_network_file = 1;
      break;
    case 'e':
      in_electrodes_filename = optarg;
      flags.read_electrodes_file = 1;
      break;
    case 'V':
      flags.store_Vstates_file = 1;
      write_state_periodicity = atol(optarg);
      if(write_state_periodicity < 1) {
        printf("\n\tERROR: You have used a bad parameter for the -V option.");
        printf("\n\t       Use an integer larger than 0.\n");
				exit(-1);
      }
      break;
    case 'W':
      flags.store_Vstates_file = 2;
      write_state_periodicity = atol(optarg);
      if(write_state_periodicity < 1) {
        printf("\n\tERROR: You have used a bad parameter for the -W option.");
        printf("\n\t       Use an integer larger than 0.\n");
				exit(-1);
      }
      break;
    case 'S':
      flags.spontaneous_fire = 1;
      spontaneous_fire_period = 1000.0 / atof(optarg); //firing period [ms]
      break;
    default:
      printf("\n");
      break;
    }
  }



  // Inform about simulation settings:
  // ---------------------------------
  printf("\nList of options:\n");
  printf("----------------\n");
  printf("simulation time = %.3f (ms)\n", total_time);
  printf("time step dt = %.5f (ms)\n", dt);
  if (flags.text == 1)
    printf("text information ON\n");
  else
    printf("text information OFF\n");
  if (flags.graphics > 0)
    printf("graphical information ON\n");
  else
    printf("graphical information OFF\n");
  if (flags.read_electrodes_file == 1)
    printf("input electrodes file defined: %s\n", in_electrodes_filename);
  if (flags.store_network_file == 1)
    printf("output network file defined: %s\n", out_network_filename);
  if (flags.store_Vstates_file > 0)
    printf("file 'out_membrane_potentials.dat' stores V_m every %.3f (ms) [%ld*dt]\n", write_state_periodicity * dt, write_state_periodicity);
  if (flags.enable_rate_estimation == 2)
    printf("file 'out_rates.dat' stores firing rates every %.3f (ms) [%ld*dt]\n", write_state_periodicity * dt, write_state_periodicity);

  //*************************************************************************
  // WARNINGS:
  //*************************************************************************
  if (flags.disable_plasticity == 1) {
    printf("\nWarning: simulation with synaptic plasticity mechanisms disabled.");
  }
  if (flags.enable_rate_estimation > 0) {
    printf("\nWarning: simulation with rate estimation enabled. Expect long simulation time.");
  }
  if (flags.myfunctions == 1) {
    printf("\nWarning: user functions will be executed.");
  }
  //*************************************************************************


  printf("\n\nPress Return key");
  //getchar();


  // Initialize Pseudo-Random Number Generator
  // -----------------------------------------
  rng = gsl_rng_alloc(gsl_rng_ranlxs0);
  if (rng == NULL) {
    printf("\n\tERROR: unable to initialize the pseudo-random number generator.\n");
    exit(-1);
  }
  seed = time(NULL);
  gsl_rng_set(rng, seed);

  // Initialize Models Library
  // -------------------------
  SetupModelsLibrary();


  // Read network input file
  // -----------------------
  printf("\n");
  if (Read_Network(argv[optind]) != 0) {
    printf("\n\tERROR: Problems in reading network file");
    printf("\n\tSimulation cannot proceed... exiting");
    exit(-1);
  }
	//IMPORTANT: the input network file is closed [fclose] inside the Read_Network() function


  // Prepare data structures
  // ----------------------------
  Prepare_Populations();
  Prepare_Connections();
  Prepare_Neurons();
  Prepare_Synapses();



  //*************************************************************************
  //--------- USE THIS TO MANIPULATE VARIABLES OR OUTPUT MORE DATA ----------
  //*************************************************************************
  if (flags.myfunctions == 1) {
    MyFunction_PreSimCycle_Gateway();
    //open file output_data to be used in function Output_Data
    out_user_data_file = fopen("out_myfunctions_user_data.dat", "w");
    if (out_user_data_file == NULL) {
      printf("\n\tERROR: unable to open 'out_user_data.dat' file for writting.\n");
			Free_Neurons();
			Free_Connections();
			Free_Populations();
      exit(-1);
    }
  }
  //*************************************************************************



  // Take care of user requests:
  // ---------------------------

	//OUT_ACTIVITY_LEVELS
  //open file activity_levels case it was requested
  if (flags.store_activity_levels_file == 1) {
    out_activity_levels_file = fopen("out_activity_levels.dat", "w");
    if (out_activity_levels_file == NULL) {
      printf("\n\tERROR: unable to open 'activity_levels.dat' file for writting.\n\n");
			Free_Neurons();
			Free_Connections();
			Free_Populations();
      exit(-1);
    }
		//create HEADER - comment to ease import of this data (to MATLAB for example)
    fprintf(out_activity_levels_file, "Activity levels, measured in a time window of %.1f ms, for a total of %ld neurons in a simulation of %.2f ms total time.\nt[ms]", ACTIVITY_LEVEL_TIME_WINDOW, total_neurons, total_time);
    for (pop = 0; pop < total_populations; pop++) {
      fprintf(out_activity_levels_file, "\t%s", population[pop].label); //this can be taken out to allow automatic load of data in matlab
    }
  }

	//OUT_SPIKE_TIMES
  //open file out_spike_times case it was requested
  if (flags.store_spike_times_file == 1) {
    out_spike_times_file = fopen("out_spike_times.dat", "w");
    if (out_spike_times_file == NULL) {
      printf("\n\tERROR: unable to open 'out_spike_times.dat' file for writting.\n\n");
			Free_Neurons();
			Free_Connections();
			Free_Populations();
      exit(-1);
    }
		//create HEADER - comment to ease import of this data (to MATLAB for example)
    fprintf(out_spike_times_file,"Spike times for a total of %ld neurons in a simulation of %.2f ms total time.\nnrn_id[1]\tspike_time[ms]\n", total_neurons, total_time);
  }

	//OUT_Vm_STATES
  //open file out_membrane_potentials case it was requested
  if (flags.store_Vstates_file > 0) {
    out_Vstates_file = fopen("out_membrane_potentials.dat", "w");
    if (out_Vstates_file == NULL) {
      printf("\n\tERROR: unable to open 'out_membrane_potentials.dat' file for writting.\n\n");
			Free_Neurons();
			Free_Connections();
			Free_Populations();
      exit(-1);
    }
  	//create HEADER - comment to ease import of this data (to MATLAB for example)
		fprintf(out_Vstates_file, "Membrane potentials, for a total of %ld neurons in a simulation of %.2f ms total time.", total_neurons, total_time);
		if (flags.store_Vstates_file == 1) {
			fprintf(out_Vstates_file, " Data for fdomain 0 only.\nt[ms]");
			for (nrn = 0; nrn < total_neurons; nrn++) {
				fprintf(out_Vstates_file, "\tnrn(%ld)", nrn);
			}
		}
		else {
			fprintf(out_Vstates_file, " Data for all fdomains.\nt[ms]");
			for (nrn = 0; nrn < total_neurons; nrn++) {
				for (fdm = 0; fdm < neuron[nrn].n_fdomains; fdm++) {
					fprintf(out_Vstates_file, "\tnrn(%ld).fdm(%d)", nrn, fdm);
				}
			}			
		}
		
	}
	


	// ELECTRODES
	// ----------
  //open file in_electrodes_file case it was requested
  if (flags.read_electrodes_file == 1) {
    in_electrodes_file = fopen(in_electrodes_filename, "r");
    if (in_electrodes_file == NULL) {
      printf("\n\tERROR: unable to open input electrodes file\n\n");
			Free_Neurons();
			Free_Connections();
			Free_Populations();
      exit(-1);
    }
    //read all electrodes
    Acquire_All_Electrodes(in_electrodes_file);
		fclose(in_electrodes_file);
  }  
  //set Poisson spikers in Pop00 in case it was requested
  if ( flags.spontaneous_fire == 1 ) {
    for( nrn=0; nrn<population[0].n_neurons; nrn++ ) {
      Add_Electrode("injFirePoissonSync", nrn, nrn, RND_Exponential( spontaneous_fire_period), spontaneous_fire_period, total_time, 0.0, 0.0);
    }
  }



  //inital Reset
  if (flags.sys_initial_reset == 1) {
    Reset_System();
  }



  //open graphics mode in case it was requested
  if (flags.graphics > 0) {
    Prepare_Graphics(argc, argv);
    glutMainLoopEvent();
  }





  printf("\n\nSimulation starting...\n");
  printf("\n-------------------------------------------------------\n");
  fflush(stdout);

  sim_time_start = clock();
	time_update = clock();

  //*************************************************************************
  //
  //   - - - - -   S I M U L A T I O N   M A I N   C Y C L E   - - - - -
  //
  //*************************************************************************

  while (t < total_time + 0.5*dt && flags.finish_earlier == 0) {


    // PERIODIC RESET of the System (resets state variables, clean spikes and clean pscs). Useful in some learning models
    if (flags.sys_periodic_reset == 1) {
      if (fabs(fmod(t + dt, sys_reset_period)) < dt) {
				printf("\nReseting system (period=%.1f ms): resets state variables, clean spikes and clean active pscs\n", sys_reset_period);
				Reset_System();
      }
    }
    

    // CALLS USER FUNCTION, DEFINED IN myfunctions.c
    if (flags.myfunctions == 1) {
      MyFunction_IntraSimCycle_Gateway(out_user_data_file);	//Function defined by the user; among other things may be used to export specific data
    }


    // ELECTRODES
		for( elec = total_electrodes - 1; elec >= 0; elec-- ) {	//Process all electrodes entries
			if( fabs( t - electrode[elec].next_time ) < 0.5 * dt ) {
				Apply_Electrode(elec);
			}
			if( t > electrode[elec].end_time - 0.5 * dt ) {
				Remove_Electrode(elec);
			}
		}

  
    //the order in the updates is: SPIKEs->SYNAPSEs->PSCs->NEURONs
  
    // SYNAPSES    
    Update_Synapses();


    // NEURONS (needs to be after SYNAPSES)
    Update_Neurons(out_spike_times_file);


    // POPULATIONS (needs to be after NEURONS)
    Update_Populations();


    // GRAPHICS
    if (flags.graphics > 0 ) {	//&& (long)(rint(t/dt)) % 10 == 0 ) {
      Update_Graphics();
      glutMainLoopEvent();

      // take care of the Pause condition
      while( graphics.pause_graphics == 1 && graphics.close_graphics != 1) {
				Update_Graphics();
				glutMainLoopEvent();
      }

      // take care of the Step condition
      if( graphics.step1_graphics == 1 ) {
				graphics.step1_graphics = 0;
				graphics.pause_graphics = 1;
      }

    }


    // TEXT INFORMATION
    if (flags.text == 1 ) {	//&& ((long)rint(t/dt)) % 10 == 0 ) {      
      printf("\nt=%.2fms AL:", t);
      for (pop = 0; pop < total_populations; pop++) {
				printf(" %s=%.3f%\%;", population[pop].label, population[pop].activity_level);
      }
    }
		if( (clock() - time_update) / CLOCKS_PER_SEC > 1.0 ) {
			printf("\nt = %.3f\t[%.0f%%]", t, 100.0*t/total_time);
			time_update = clock();
			fflush(stdout);
		}


    // ACTIVITY LEVELS: send information to activity_levels.dat file
    if (flags.store_activity_levels_file == 1) {
      fprintf(out_activity_levels_file, "\n%.3f", t);
      for (pop = 0; pop < total_populations; pop++) {
				fprintf(out_activity_levels_file, "\t%.3f", population[pop].activity_level);
      }
    }


    // STATES FILE
    if (flags.store_Vstates_file > 0) {
      if (sim_step % write_state_periodicity == 0) {
				Write_State_File(out_Vstates_file, flags.store_Vstates_file);
      }
    }



    sim_step += 1;
    t = sim_step * dt;

  }

  //*************************************************************************
  //
  //     - - - - -   S I M U L A T I O N   F I N I S H E D   - - - - -
  //
  //*************************************************************************

  sim_time_end = clock();
  cpu_time_used = ((double) (sim_time_end - sim_time_start)) / CLOCKS_PER_SEC;
  printf("\n\n-------------------------------------------------------\n");
	printf("\nSimulation finished. Total CPU time %.3f seconds.",  cpu_time_used );


  printf("\n\nNow closing all output files.\n");

  //close the user's output_data file 
  if (flags.myfunctions == 1) {
    fclose(out_user_data_file);
  }
  //close 'activity_levels.dat' file case it was open
  if (flags.store_activity_levels_file == 1) {
    fclose(out_activity_levels_file);
  }
  //close 'out_spike_times.dat' file case it was open
  if (flags.store_spike_times_file == 1) {
    fclose(out_spike_times_file);
  }
  //close 'out_membrane_potentials.dat' file case it was open
  if (flags.store_Vstates_file > 0) {
    fclose(out_Vstates_file);
  }
  //write network file case requested
  if (flags.store_network_file == 1) {
    if (Write_Network(out_network_filename) != 0) {
      printf("\n\tERROR: Problems writting output network file. Sorry!");
      printf("\n\tInformation was NOT stored...");
    }
  }
  //take care of graphical window
  if( flags.graphics > 0 ) {
    glutSetWindowTitle("NeuralSyns - simulation completed");
    printf("\nPress 'Q' to finish ");
    fflush(stdout);
    while( graphics.close_graphics != 1 ) {
      Update_Graphics();
      glutMainLoopEvent();
    }
  }


  //free memory occupied by NEURONS and SYNAPSES and POPULATIONS and ELECTRODE
  Free_Mem();

  gsl_rng_free(rng);


  printf("\nAll processes finished!\n%c\n", BELL);
  fflush(stdout);



  return 0;
}
