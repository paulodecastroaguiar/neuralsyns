//************************************************************************
//*                                                                      *
//*                          S Y N A P S E S                             *
//*                                                                      *
//************************************************************************

#if defined(SYNAPSES_H)
#else
#define SYNAPSES_H


/// holds data for synapses
struct SYNAPSE
{
  double weight;                ///< synaptic efficacy (adimensional); plasticity acts on this state variable
  double delay;                 ///< time measuring axonal + synaptic delay (ms)  ["R" if homogeneous]

  unsigned long target_neuron;  ///< id of the post-synaptic neuron (output)

  double next_release_time;     ///< next synaptic release time (ms)
  double last_release_time;     ///< last synaptic release time (ms) [R]
  unsigned int spikes_cue;      ///< number of spikes in cue (travelling in the axon)

  unsigned short activated;	    ///< boolean informing if synapse currents are active or not
  double psr_time;	            ///< time since initiation of the post-synaptic potential (ms) [R]

  double *current_param;        ///< pointer to parameters for the synaptic conductance dynamics (if HOMOGENEOUS, points to CONNECTION *param array)
  double **current_statevar;    ///< array of state variables for the synaptic current dynamics

  double *plasticity_param;     ///< pointer to parameters for the synaptic plasticity dynamics (if HOMOGENEOUS, points to CONNECTION *param array)
  double **plasticity_statevar; ///< array of state variables for the synaptic plasticity dynamics
};



/// holds data for connections
struct CONNECTION
{
	short set;                                          ///< flags if this connection is set
  char *label;                	                      ///< connection name
  unsigned int fdomain;	                              ///< id of the target functional domain in the post-synaptic neuron
  struct MODEL_DYNAMICS_SYNAPSE *current_model;       ///< pointer to structure holding model dynamics and labels for the synaptic current dynamics
  double *current_param;                              ///< SYNAPSES *model_param point here if the HOMOGENEOUS flag is set
  struct MODEL_DYNAMICS_PLASTICITY *plasticity_model; ///< pointer to structure holding model dynamics and labels for the synaptic plasticity dynamics
  double *plasticity_param;                           ///< SYNAPSES *plasticity_param point here if the HOMOGENEOUS flag is set
};





// Prototypes of all functions that manage the synapse structure


void Initialize_Synapses( struct SYNAPSE *p, long nrn, FILE *file_in);
void Initialize_Connections(struct CONNECTION **p, FILE *file_in);

void Prepare_Synapses(void);

void Update_Synapses(void);

void Add_Spike(long nrn, double time);
void Remove_Spike(long nrn, long syn);

void Add_PSC(long nrn, long syn, double time);
void Remove_PSC(long nrn, long syn);

void Prepare_Connections(void);

void Free_Connections(void);



#endif
