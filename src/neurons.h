//************************************************************************
//*                                                                      *
//*                            N E U R O N S                             *
//*                                                                      *
//************************************************************************

#if defined(NEURONS_H)
#else
#define NEURONS_H


#define N_SPIKE_TIMES 50        ///< number of spike times to keep track of
#define MIN_Ra 10.0             ///< minimum axial resistant between fdomains (Mohm)



/// holds data regarding a specific compartment
struct FDOMAIN
{
  // NUMERICAL_ORDER is defined in 'general.h'
  double *V;	                    ///< functional domain's voltage [mV] (V[0] is the present value, V[i>=0] hold future estimations)

  double *I_total_syn;            ///< total synaptic current [nA] (i=0 is the present value, i>=0 hold future estimations)
  double *I_total_axial;          ///< total axial currents [nA] (i=0 is the present value, i>=0 hold future estimations)
  double *I_total_electrode;      ///< total input stimulation current [nA] (i=0 is the present value, i>=0 hold future estimations)

  double Ra;		                  ///< axial resistance between this functional domain and its parent [Mohm]

  double *model_param;            ///< pointer to array holding parameters for the fdomain dynamics
  double **model_statevar;        ///< pointer to array holding state variables for the fdomain dynamics (membrane potential NOT included)

  struct GSL_ODESOLVER_DATA *gsl; ///< pointer to structure holding gsl specifications to use, if required, the gsl ode solver
};


/// holds all data describing a neuron unit
struct NEURON
{
  struct POSITION position;           ///< 3D geometrical location of neuron

  unsigned long n_synapses;           ///< number of synapses established by this neuron
  struct SYNAPSE *synapse;            ///< array of SYNAPSES established by this neuron

  unsigned int population;	          ///< id of neuron's host population (POPULATION structure)
 
  unsigned int n_fdomains;            ///< number of functional domains [R]
  struct FDOMAIN *fdomain;            ///< array of the functional domains associated with this neuron

  double *spike_time;                 ///< array (ring) with recently fired spike times [ms];
  unsigned int last_spike_time_index; ///< index in the spike_time array of the most recent spike time 

  double firing_rate;  	              ///< estimated instantaneous spiking rate [Hz]
  unsigned long spikes_counter;       ///< amount of fired spikes so far

  double spiketime_from_electrode;    ///< signals if neuron must fire due to a user-defined stimuli; if > 0.0 it contains the spike time

  long n_active_synapses;             ///< total number of active synapses; if 0 simulation can be speedup by bypassing synapse update in this neuron
};





// Prototypes of all the functions that manage the neurons structures
// ------------------------------------------------------------------

void Initialize_Neurons(struct NEURON *p, FILE *file_in);
void Initialize_FDomains(struct FDOMAIN *p, long nrn, FILE *file_in);

void Prepare_Neurons(void);

void Prepare_FDomains(long nrn);

void Update_Neurons(FILE *out_spike_times_file);

void Update_Spike_Times(long nrn, double spike_time);

void Update_Firing_Rate(long nrn);

void Free_Neurons(void);



#endif
