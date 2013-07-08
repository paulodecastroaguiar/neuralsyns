#ifndef CREATENET_H
#define CREATENET_H

/** @brief Functions to create the network and store it in a .net file
    
    @author Sousa, M.
    @date October 2009
*/

#ifdef WX_GCH  
#include "wx/wx_pch.h"  
#else  
#include "wx/wx.h"  
#endif

#include "data_structures.h"
#include "gui.h"
#include <wx/progdlg.h>

using namespace std;
#define MAXLABELSIZE 24


///Triplet structure
struct Triplet{
  int i;		///< i spatial coordinate
  int j;		///< i spatial coordinate
  int k;		///< i spatial coordinate
};

///Position structure 
struct Position{
  int ID;		    ///< Neuron's position ID
  int pop_id;		///< Neuron's population ID
  double x_pos;  	///< Neuron's x position 
  double y_pos;	    ///< Neuron's y position 
  double z_pos;	    ///< Neuron's z position 
};

double RandGaussian(double sigma);					/// Normal-random generator

long RND_Uniform_Int(long min, long max); 				/// Uniform-random generator

struct Triplet ID2Triplet(int pop, int nrn);				/// Triplet (i.j.k) from neuron ID

int Triplet2ID(int i, int j, int k, int dimx, int dimy); 		/// Neuron ID from Triplet (i.j.k)

int Main_Core(char* network_file);					/// Main Core to create .net file

int Neuron_Targets_ID(unsigned int pop_pre,unsigned int pop_pos);	/// Targets IDs for each neuron

struct Position* Neurons_Position(int pop, int n_first, int n_nrn);	/// 3D geometric position of all neurons in each population

int Choose_Random_Target_Position(int pop_pre, int pop_pos, int coord,int dim, double sigma, double target_center );  /// Choose the random target position

double TotalDelay(struct Position pos_in,struct Position pos_out, double v);	/// Total delay between 2 neurons

#endif //CREATENET_H
