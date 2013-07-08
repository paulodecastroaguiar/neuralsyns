//************************************************************************
//*                                                                      *
//*                       M Y   F U N C T I O N S                        *
//*                                                                      *
//************************************************************************

#if defined(MYFUNCTIONS_H)
#else
#define MYFUNCTIONS_H




// Prototypes for the two user functions

// function to manipulate or export user data to files before the simulation
// begins; although this function is called before the simulation begings,
// all network data is already loaded and accessible. (*)
void MyFunction_PreSimCycle_Gateway(void);


// function to export user data to a file called "output_data.dat"; this 
// function is called at every iteration allowing the user to access and
// export specific data at every stage of the analysis/simulation. (*)
void MyFunction_IntraSimCycle_Gateway(FILE * output_data);





// (*) the structures and variables describing the state of the network's
// POPULATIONS, SOMAS and SYNAPSES are shown in the myfunctions.c
// file for your reference.



#endif
