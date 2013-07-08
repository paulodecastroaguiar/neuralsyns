#include "NetBuilderVisualization_frame.h"

extern int pop_selected;
extern unsigned int total_populations;
extern struct POPULATION *population;
extern struct DYNAMICS *Neuron_Models;
extern bool visual_show;
extern struct COORDINATES *coordinates;
int tt_fdomains;

NetBuilderVisualization_frame::NetBuilderVisualization_frame( wxDialog* parent )
:
Visualization_frame(parent, wxID_ANY,  wxT("Visualization"), wxPoint(500,300), wxSize(200,200))
{

}

void NetBuilderVisualization_frame::OnInitVisualization( wxInitDialogEvent& event )
{
	visual_show=true;
	this->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	Dynamics_frame *myparent= (Dynamics_frame*) GetParent();
	tt_fdomains=population[pop_selected].total_fdomains;
	coordinates = (COORDINATES*) malloc(tt_fdomains*sizeof(struct COORDINATES));
  
	int error=0;
	coordinates[0].x = 0.0;
	coordinates[0].y = 0.0;
	coordinates[0].z = 0.0;
	coordinates[0].parent_id = -1; 
	for (int fdms=0; fdms<tt_fdomains;fdms++){
		(myparent->dynamics_grid->GetCellValue(2,fdms)).ToDouble(&coordinates[fdms].radius);
		if (coordinates[fdms].radius <=0.0){
			coordinates[fdms].radius = population[pop_selected].fdomains[fdms].r;
			myparent->dynamics_grid->SetCellTextColour(2,fdms,*wxRED);
			myparent->dynamics_grid->Refresh();
			error=-1;
			break;
		}
	}
	int return_value;
	char position[50];
	char par_label[MAXLABELSIZE];
	for (int fdm = 1; fdm < tt_fdomains; fdm++)
		{
	    wxStr2str(myparent->dynamics_grid->GetCellValue(1,fdm),position);
	    return_value=sscanf(position,"(%lf,%lf,%lf)", &coordinates[fdm].x, &coordinates[fdm].y, &coordinates[fdm].z);
	    if (return_value!=3) 
	      {
					coordinates[fdm].x=population[pop_selected].fdomains[fdm].x;
					coordinates[fdm].y=population[pop_selected].fdomains[fdm].y;
					coordinates[fdm].z=population[pop_selected].fdomains[fdm].z;
					myparent->dynamics_grid->SetCellTextColour(1,fdm,*wxRED);
					myparent->dynamics_grid->Refresh();
					error=-1;
					break;
	      }
			
	    
	    wxStr2str(myparent->dynamics_grid->GetCellValue(4,fdm),par_label);
	    int pid = 0;
	    int n=0;
	    for (int i = 0; i < tt_fdomains; i++){
				if (strncmp(par_label,population[pop_selected].fdomains[i].label,MAXLABELSIZE)==0){
			    pid=i;
				}
				else{
			    n=n+1;
				}
	    }
	    
	    coordinates[fdm].parent_id=pid;
	    if (n!=tt_fdomains-1 || strncmp(par_label,population[pop_selected].fdomains[fdm].label,MAXLABELSIZE)==0){
				coordinates[fdm].parent_id=population[pop_selected].fdomains[fdm].parent_ID;
				myparent->dynamics_grid->SetCellTextColour(4,fdm,*wxRED);
				myparent->dynamics_grid->Refresh();
				error=-1;
				break;
	    }
		}
	
	if (error==0){
	  wxClientDC dc(this); //dependent on drawpane.h
	  drawPane->render(dc, coordinates, tt_fdomains); //draw fdomains in a basic draw panel
	}
	else{
		free(coordinates);
		visual_show=false;
		Destroy();
	}
}

void NetBuilderVisualization_frame::OnClose( wxCloseEvent& event )
{
      free(coordinates);
      visual_show=false;	
      Destroy();
}



