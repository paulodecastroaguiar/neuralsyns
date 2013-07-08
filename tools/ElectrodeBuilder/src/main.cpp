#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "main.h"
#include <string>
#include <vector>

using namespace std;

struct ELECTRODE *electrode=NULL;
int number_models=7;
char elect[25];
double *params;
int params_number=0;
int number_elect=-1;


/// Initialize application

IMPLEMENT_APP(ElectrodeBuilder);


bool ElectrodeBuilder::OnInit()
{
   strcpy(elect,"injFire");
   params = (double*) malloc(params_number * sizeof(double));
   electrode=(ELECTRODE*) malloc (sizeof(struct ELECTRODE) );
   //Load_variables(); 
   
   ElectrodeBuilder_MainFrame* frame = new ElectrodeBuilder_MainFrame(0L);
   frame->Show(true);
   return true;
}

///convert doubles to wxString
wxString dbl2wxStr(double mydbl)
{
 wxString wxstr;
 wxstr=wxString::Format(wxT("%.1lf"),mydbl);
 return wxstr;
}

  
