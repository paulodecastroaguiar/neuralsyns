#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

/** @brief Definition of all data structures and some usefull functions

    Detailed description follows here.
    @author Sousa, M.
    @date October 2009
    */

#ifdef WX_GCH  
#include "wx/wx_pch.h"  
#else  
#include "wx/wx.h"  
#endif

#include <stdlib.h>
#include <vector>
#include "wx/grid.h"

using namespace std;
#define MAXLABELSIZE 24
#define Pi 3.1415926535897932384626433832795

//#define Pi 3.14159265

///Usefull functions
wxString int2wxStr(int myint); 				///<convert integer to wsString
wxString long2wxStr(long myint);			///<convert longs to wsString
wxString dbl2wxStr(double mydbl); 			///<convert double to wsString
wxString flt2wxStr(double myflt); 			///<convert double to wsString
char* wxStr2str(wxString wxstr, char *str); 		///<convert wxString to string
char * str_equal(const char* strin, char* strout); 		///<operator= between strings

double Clip_Dec_Cases(double x);

void Initialize_Populations(int total_populations); 	///<Initialize structure POPULATION
void Initialize_Fdomains(int pop,int fdms); 		///<Initialize structure FDOMAINS
int Read_Neuron_Models(void);				///<Read neuron and synaptic models from .lib file
int Read_Synaptic_Models(void);
int Read_Plasticity_Models(void);
int Test_Reading_Models(void);
void Free_Memory(void);					///<Frees memory
void ResizeGrid(wxSize panelsz, wxGrid *grid, int cols, int rows);	///<Resizes grid

///TOPOLOGY structure
struct TOPOLOGY
{
  long dim_x;						///< x dimension [space units]
  long dim_y;						///< y dimension [space units]
  long dim_z;						///< z dimension [space units]
  double X0;						///< initial coordinate x [um]
  double Y0;						///< initial coordinate y [um]
  double Z0;						///< initial coordinate z [um]
  double d_mean;					///< mean distance [um]
  double d_std;						///< standard deviation distance [um]
};

///DYNAMICS structure
struct DYNAMICS
{
  char model_label[MAXLABELSIZE];			///<  dynamical model label
  int model_ID;						///<  dynamical model identification
  int evt_gen;						///< =1 if dynamic model generates events
  int model_total_parameters;				///< total parameters of the model
  char **parameter_label;				///< array of model parameters labels
  double *parameter;					///< array of model parameters
  int n_state_vars;					///< total state variables of the model
  double *state_vars_val; 				///< array of state variables 
  char **state_vars_label; 				///< array of state variables labels
};


/// FDOMAINS structure
struct FDOMAINS
{ 
  long fdm_ID;						///< fdomains ID
  char label[MAXLABELSIZE];			///< fdomain name
  char parent_label[MAXLABELSIZE];	///< fdomain parent label
  long parent_ID;					///< fdomain parent ID
  double x;						///< position x [um]
  double y;						///< position y [um]
  double z;						///< position z [um] distributed in trigonometric circle
  double r; 						///< sphere/cylinder radius
  double Ra;						///< Axial resistence [ohm.cm]
  struct DYNAMICS dynamics;				///< dynamic fdms model
};

/// POPULATION structure
struct POPULATION
{
  char label[MAXLABELSIZE];					///< population name
  int total_fdomains;					///< number of functional domains
  struct TOPOLOGY topology; 				///< topology structure
  struct FDOMAINS *fdomains;				///< fdomains structure
};

    
/// SYNAPSES structure
struct SYNAPSES
{
  struct DYNAMICS conductances_dynamics;		///< dynamic conductances model
  struct DYNAMICS plasticity_dynamics;			///< dynamic plasticity model
};

#endif //DATA_STRUCTURES_H
