#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "main.h"

//Global variables, strutures and frames
using namespace std;

struct POPULATION *population = NULL;
unsigned int total_populations=0;
struct DYNAMICS *Neuron_Models = NULL;
unsigned int total_neuron_models=0;
struct DYNAMICS *Conductances_Models = NULL;
unsigned int total_syn_models=0;
struct DYNAMICS *Plasticity_Models = NULL;
unsigned int total_plas_models=0;
struct SYNAPSES **synapses = NULL;
int **conn_values = NULL;
int **fdms_values = NULL;
double **delays_values = NULL;
double ***dist_values = NULL;
//struct NEURON *neuron = NULL;

bool DivergentConnectivity = true;
bool Homogeneity = false;
bool Auto_connections = false;
bool Multiplicity = true;
bool summary_show = false;
bool visual_show = false;
int model_table=0;
int fdms_check = 0;
int ***RANDOM_DIST=NULL;

NetBuilderMainFrame* frame =NULL;
NetBuilderSummary_frame* Summary = NULL;


/// Initialize application

IMPLEMENT_APP(NetBuilder);


bool NetBuilder::OnInit()
{
   Read_Neuron_Models();
   Read_Synaptic_Models();
   Read_Plasticity_Models();
   Test_Reading_Models();
   frame = new NetBuilderMainFrame(0L);
   frame->Show(true);
   Summary = new NetBuilderSummary_frame(frame);
   return true;
}
