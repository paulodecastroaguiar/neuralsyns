//************************************************************************
//*                                                                      *
//*                   D A T A   I N P U T / O U T P U T                  *
//*                                                                      *
//************************************************************************

#if defined(DATAIO_H)
#else
#define DATAIO_H





// Prototypes of all functions read and write the network state

int Read_Network(char *network_file);	                                                 //function that reads "network" file

void Check_fscanf(int n_conversions, int n_inputs, char *message, int line, FILE *in); //function that checks if fscanf worked fine

int Write_Network(char *network_file);	                                               //function that writes "network" file

void Write_State_File(FILE * state_file, short mode);                                	 //function that writes present "network_state" into file



#endif
