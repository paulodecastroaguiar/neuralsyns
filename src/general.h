//************************************************************************
//*                                                                      *
//*                   G E N E R A L   F U N C T I O N S                  *
//*                                                                      *
//************************************************************************

#if defined(GENERAL_H)
#else
#define GENERAL_H


#define PI 3.14159265
#define BELL 7
#define ZERO 1.0e-9                     ///< values below this are considered zero
#define MAX_STRING_SIZE 64+1            ///< maximum number of characters allowed for strings
#define FREQ_ALPHA 0.01	                ///< time resolution for firing rate estimation (1/FREQ_ALPHA=100ms)
#define ACTIVITY_LEVEL_TIME_WINDOW 10.0 ///< time window [ms], used to calculate the activity level
#define LONG_TIME_AGO -1.0e+9           ///< used, among other things, to initialize spike times

#define NUMERICAL_ORDER 2               ///< possible numerical order in the ODE solvers (DO NOT CHANGE UNLESS YOU _REALLY_ KNOW WHAT YOU ARE DOING)
#define NO_SPIKE -1.0                   ///< used on the neuron models to signal that a spike was not produced



// General purpose structures

///holds flags setting simulation conditions
struct FLAGS
{
  short text;
  short graphics;
  short store_spike_times_file;
  short store_activity_levels_file;
  short store_network_file;
  short store_Vstates_file;
  short read_electrodes_file;
  short disable_plasticity;
  short enable_rate_estimation;
  short sys_initial_reset;
  short sys_periodic_reset;
  short spontaneous_fire;  
  short single_step;
  short myfunctions;
  short finish_earlier;
  short homogeneous_synapses;
};



// Prototypes of all general purpose functions
void Free_Mem(void);
void Free_Models(void);
void Exit_Error(int level, FILE *in_file);

unsigned int RND_Poisson(double lambda);
double RND_Exponential(double lambda);
double RND_Normal(double miu, double sigma);
double RND_Uniform(double min, double max);
long RND_Discrete(long min, long max);





#endif
