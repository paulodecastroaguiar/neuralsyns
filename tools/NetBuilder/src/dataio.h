#ifndef DATAIO_H
#define DATAIO_H

/** @brief Functions to read and write the network state

    Detailed description follows here.
    @author Sousa, M.
    @date October 2009
    */

#ifdef WX_GCH  
#include "wx/wx_pch.h"  
#else  
#include "wx/wx.h"  
#endif

#include "data_structures.h"

using namespace std;


/// Functions to read and write the network state

int Save_Netbuilder_File(char* network_file);	///< function that writes "network" file
int Load_Netbuilder_File(char *network_file);	///<function that reads "network" file

#endif //DATAIO_H