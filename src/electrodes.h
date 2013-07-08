//************************************************************************
//*                                                                      *
//*                          E L E C T R O D E S                         *
//*                                                                      *
//************************************************************************

#if defined(ELECTRODES_H)
#else
#define ELECTRODES_H



/// holds electrodes data
struct ELECTRODE
{
  char type[MAX_STRING_SIZE]; ///< injFire                 = fire (input);
                              ///< injFirePeriodically     = periodically fire (input);
                              ///< injFireRandom           = fire random neurons (input);
                              ///< injFirePoissonAsync     = asynchronously fire (Poisson ISI) (input);
                              ///< injFirePoissonSync      = synchronously fire (Poisson ISI) (input);
                              ///< injCurrentConst         = constant current injection (input);
                              ///< injCurrentGaussianNoise = Gaussian current injection (input);
                              ///< injCurrentSine          = sinusoidal current injection (input);
                              ///< recV0                   = record fdm00 membrane potential (ouput);
                              ///< recVAll                 = record all fdomains membrane potentials (ouput);
                              ///< recStates0              = record all state variables for fdm00 (ouput);
                              ///< recStatesAll            = record all state variables for all fdomains (ouput);
                              ///< recSynStates            = record all state variables for the neuron's synapses (ouput);

  unsigned long neuron_first; ///< first neuron in the range to which the electrode is applyed
  unsigned long neuron_last;  ///< last neuron in the range to which the electrode is applyed    

  double period;              ///< electrode activation period ([ms] in Inj electrodes and [1] in Rec electrodes)
  double next_time;           ///< time for the next electrode injection [ms]
  double end_time;            ///< time at which the electrode finishes [ms]

  double param_A;             ///< available space for parameter used by the electrode
  double param_B;             ///< available space for parameter used by the electrode

	FILE *out_record_file;      ///< each recording electrode has it's own output file
};



// Prototypes of all functions that externaly stimulate the network

void Acquire_All_Electrodes(FILE * electrodes_file);

void Apply_Electrode(int elec);

void Electrode_Fire_Neurons(int elec);
void Electrode_Fire_Neurons_Periodically(int elec);
void Electrode_Fire_Neurons_PDF(int elec);
void Electrode_Fire_Neurons_Poisson(int elec);
void Electrode_Current_Constant(int elec);
void Electrode_Current_GaussianNoise(int elec);
void Electrode_Current_Sine(int elec);

void Add_Electrode(char *type, long nrn_first, long nrn_last, double next_time, double period, double end_time, double param_A, double param_B);
void Remove_Electrode(int elec);

void Produce_Electrodes_Template(void);

void Free_Electrodes(void);



#endif
