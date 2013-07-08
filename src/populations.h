//************************************************************************
//*                                                                      *
//*                         P O P U L A T I O N S                        *
//*                                                                      *
//************************************************************************

#if defined(POPULATIONS_H)
#else
#define POPULATIONS_H



#define MEMB_POTENTIAL_HYPERPOLARIZED -80.0
#define MEMB_POTENTIAL_DEPOLARIZED -50.0



/// holds the 3D geometrical location of a fdomain, relative to first fdm: (x,y,z,r)
struct POSITION
{
  double x;  ///< x position [um]
  double y;  ///< y position [um]
  double z;  ///< z position [um]
  double r;  ///< r radius   [um]
};



/// holds data which is common to all fdomains inside the same population
struct FDOMAIN_GLOBAL
{
  char *label;                         ///< fdomain label
  int n_child_fdomains;                ///< number of child fdomains attached to this fdomain
  int *child_fdomain;                  ///< list of child fdomain id's (ending fdomains have child id -1)
  int parent_fdomain;                  ///< id of the parent fdomain (first fdomains has parent id -1)
  struct POSITION position;            ///< compartment position, relative to first fdm, and radius - used for visualization purposes

  struct MODEL_DYNAMICS_NEURON *model; ///< pointer to structure holding model and labels for the fdomain dynamics
};



/// holds data for populations
struct POPULATION
{
  char *label;	                   ///< population name
  unsigned long n_neurons;	       ///< number of NEURONs in this population
  unsigned long first_neuron;	     ///< NEURON id of first unit in this population

  unsigned int n_fdomains;	       ///< number of functional domains
  struct FDOMAIN_GLOBAL *fdomain;  ///< list of the functional domains defining the type of neuron in this population

  double activity_level;	         ///< percentage of NEURONS in the population that fired in the last 1 ms
  double hyperpolarised;	         ///< percentage of NEURONS in the population with V < MEMB_POTENTIAL_HYPERPOLARIZED
  double depolarised;	             ///< percentage of NEURONS in the population with V > MEMB_POTENTIAL_DEPOLARIZED

  double mean_firing_rate;	       ///< mean of the population's firing rates
  double stdev_firing_rate;	       ///< standard deviation of the population's firing rates

  unsigned long total_spikes;	     ///< total number of active spikes in the population
  unsigned long total_pscs;	       ///< total number of active pscs in the population
};





// Prototypes of all general purpose functions

void Initialize_Populations(struct POPULATION *p, FILE *file_in);
void Initialize_FDomainGlobal(struct FDOMAIN_GLOBAL *p, int pop, FILE *file_in);
void Process_FDomainsGlobal_Childs( int pop );

void Prepare_Populations(void);

void Prepare_Update_Populations(void);

void Update_Populations(void);

void Reset_System(void);

void Free_Populations(void);



#endif
