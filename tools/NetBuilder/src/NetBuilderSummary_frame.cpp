#include "NetBuilderSummary_frame.h"

using namespace std;
extern unsigned int total_populations;
extern bool DivergentConnectivity;
extern bool Homogeneity;
extern bool Auto_connections;
extern bool Multiplicity;
extern struct POPULATION *population;
extern struct SYNAPSES **synapses;
extern int **conn_values;
extern int **fdms_values;
extern double ***dist_values;
extern bool summary_show;
extern int ***RANDOM_DIST;

NetBuilderSummary_frame::NetBuilderSummary_frame( wxWindow* parent )
:
Summary_frame( parent , wxID_ANY, wxT("Summary"), wxPoint(830,50), wxSize( -1,650 ))//470 
{

}

void NetBuilderSummary_frame::OnCloseSummary( wxCloseEvent& event )
{
	summary_show=false;
	this->Destroy();// TODO: Implement OnCloseSummary
}

void NetBuilderSummary_frame::OnInitSummary( wxInitDialogEvent& event )
{

    wxGridCellAttr *attr1;
    attr1 = new wxGridCellAttr;
    attr1->SetBackgroundColour(*wxWHITE);

    summary_show=true;
    int pop_number=total_populations;
    summary_grid->ClearGrid();
    int numbcols=summary_grid->GetNumberCols();

    wxGridCellAttr *attr;
    attr = new wxGridCellAttr;
    attr->SetFont( wxFont(10,wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
  
    if (pop_number<3)
        summary_grid->DeleteCols(0,numbcols-pop_number,true);
    else if (pop_number>3)
        summary_grid->AppendCols(pop_number-numbcols,true);
    
    summary_grid->SetDefaultColSize(95, true);
    
    char targets[255];
    for (int j=0; j<pop_number; j++)
    {   
	 summary_grid->SetColAttr(j,attr1);
	 summary_grid->SetColLabelValue(j, wxString(population[j].label,wxConvUTF8));
	 if (pop_number==1)
	    summary_grid->SetColSize(j,180);
	 else
	    summary_grid->SetColSize(j,100);
	 summary_grid->SetRowLabelValue(0,_T(""));
	 summary_grid->SetCellValue(0,0,_T("Population properties"));
	 summary_grid->SetCellSize(0, 0, 1, pop_number);
	 summary_grid->SetAttr(0,0, attr);
	 summary_grid->SetCellBackgroundColour(0, 0, *wxLIGHT_GREY);
	 summary_grid->SetReadOnly(0,0,true);
	 summary_grid->SetCellValue(1,j,wxString(population[j].label,wxConvUTF8));
	 summary_grid->SetReadOnly(1,j,true);
	 wxString size_pop =  int2wxStr(population[j].topology.dim_x * population[j].topology.dim_y * population[j].topology.dim_z);
	 summary_grid->SetCellValue(2,j,size_pop);
	 summary_grid->SetReadOnly(2,j,true);
	 wxString origin = wxT("(") + int2wxStr(population[j].topology.X0) + wxT(",") + int2wxStr(population[j].topology.Y0) + wxT(",") + int2wxStr(population[j].topology.Z0)+ wxT(")") ;
	 summary_grid->SetCellValue(3,j,origin);
	 summary_grid->SetReadOnly(3,j,true);
	 summary_grid->SetCellValue(4,j,dbl2wxStr(population[j].topology.d_mean));
	 summary_grid->SetReadOnly(4,j,true); 
	 summary_grid->SetCellValue(5,j,int2wxStr(population[j].total_fdomains));
	 summary_grid->SetReadOnly(5,j,true); 
	 str_equal("",targets);
	 for (int i=0;i<population[j].total_fdomains;i++){
	    if (strcmp(targets,"")==0)
		sprintf(targets,"%s:%s",population[j].fdomains[i].label,population[j].fdomains[i].dynamics.model_label);
	    else
		sprintf(targets,"%s\n%s:%s",targets,population[j].fdomains[i].label,population[j].fdomains[i].dynamics.model_label);
	  }
	 summary_grid->SetReadOnly(6,j,false);
	 summary_grid->SetRowSize( 6, 60 );
	 summary_grid->SetCellValue(6,j,wxString(targets,wxConvUTF8));
	 summary_grid->SetCellRenderer(6 , j, new wxGridCellAutoWrapStringRenderer());
	 summary_grid->SetCellEditor( 6,  j , new wxGridCellAutoWrapStringEditor());
	 summary_grid->SetCellValue(7,0,_T("Connectivity"));
	 summary_grid->SetCellSize(7, 0, 1, pop_number);
	 summary_grid->SetAttr(7,0, attr);
	 str_equal("",targets);
	 for (int i=0; i<pop_number; i++){
	    if (conn_values[j][i]!=0){
		if (strcmp(targets,"")==0)  
		    sprintf(targets,"%s:%d",population[i].label,conn_values[j][i]);
		else
		    sprintf(targets,"%s\n%s:%d",targets,population[i].label,conn_values[j][i]);
	    }
	  }
	 summary_grid->SetReadOnly(8,j,false);
	 summary_grid->SetRowSize( 8, 60 ); 
	 summary_grid->SetCellValue(8,j,wxString(targets,wxConvUTF8));
	 summary_grid->SetCellRenderer(8 , j, new wxGridCellAutoWrapStringRenderer());
	 summary_grid->SetCellEditor( 8,  j , new wxGridCellAutoWrapStringEditor());
	 wxString DivCov;
	 if (Auto_connections ==true){
	      if (DivergentConnectivity==true && Homogeneity==true && Multiplicity==true)
		  DivCov=_T("Divergent / Homogeneous / Auto-connections / Multiplicity");
	      else if (DivergentConnectivity==false && Homogeneity==true && Multiplicity==true)
		  DivCov=_T("Convergent / Homogeneous / Auto-connections / Multiplicity");
	      else if (DivergentConnectivity==true && Homogeneity==false && Multiplicity==true)
		  DivCov=_T("Divergent / Heterogeneous / Auto-connections / Multiplicity");
	      else if (DivergentConnectivity==false && Homogeneity==false && Multiplicity==true)
		  DivCov=_T("Convergent / Heterogeneous / Auto-connections / Multiplicity");
	      else if (DivergentConnectivity==true && Homogeneity==true && Multiplicity==false)
		  DivCov=_T("Divergent / Homogeneous / Auto-connections ");
	      else if (DivergentConnectivity==false && Homogeneity==true && Multiplicity==false)
		  DivCov=_T("Convergent / Homogeneous / Auto-connections ");
	      else if (DivergentConnectivity==true && Homogeneity==false && Multiplicity==false)
		  DivCov=_T("Divergent / Heterogeneous / Auto-connections ");
	      else if (DivergentConnectivity==false && Homogeneity==false && Multiplicity==false)
		  DivCov=_T("Convergent / Heterogeneous / Auto-connections ");
	 }
	 else{
	      if (DivergentConnectivity==true && Homogeneity==true && Multiplicity==true)
		  DivCov=_T("Divergent / Homogeneous / Multiplicity");
	      else if (DivergentConnectivity==false && Homogeneity==true && Multiplicity==true)
		  DivCov=_T("convergent / Homogeneous / Multiplicity");
	      else if (DivergentConnectivity==true && Homogeneity==false && Multiplicity==true)
		  DivCov=_T("Divergent / Heterogeneous / Multiplicity");
	      else if (DivergentConnectivity==false && Homogeneity==false && Multiplicity==true)
		  DivCov=_T("Convergent / Heterogeneous / Multiplicity");
	      else if (DivergentConnectivity==true && Homogeneity==true && Multiplicity==false)
		  DivCov=_T("Divergent / Homogeneous ");
	      else if (DivergentConnectivity==false && Homogeneity==true && Multiplicity==false)
		  DivCov=_T("Convergent / Homogeneous ");
	      else if (DivergentConnectivity==true && Homogeneity==false && Multiplicity==false)
		  DivCov=_T("Divergent / Heterogeneous ");
	      else if (DivergentConnectivity==false && Homogeneity==false && Multiplicity==false)
		  DivCov=_T("Convergent / Heterogeneous ");
	 }
	    
	 summary_grid->SetCellValue(9,0,DivCov);
	 summary_grid->SetCellSize(9, 0, 1, pop_number);
	 summary_grid->SetReadOnly(9,j,true); 
	 str_equal("",targets);
	 for (int i=0; i<pop_number; i++){
	      if (conn_values[j][i]!=0){
		 if (strcmp(targets,"")==0)
		      sprintf(targets,"%s:%d",population[i].label,fdms_values[j][i]);
		 else
		      sprintf(targets,"%s\n%s:%d",targets,population[i].label,fdms_values[j][i]);
	      }
	 }
	 summary_grid->SetReadOnly(10,j,false); 
	 summary_grid->SetRowSize( 10, 60 ); 
	 summary_grid->SetCellValue(10,j,wxString(targets,wxConvUTF8));
	 summary_grid->SetCellRenderer(10 , j, new wxGridCellAutoWrapStringRenderer());
	 summary_grid->SetCellEditor( 10,  j , new wxGridCellAutoWrapStringEditor());
	 str_equal("",targets);
	 char distrib[MAXLABELSIZE];
	 char x[3];
	 for (int i=0; i<pop_number; i++){
	   if (conn_values[i][j]!=0){
		 for (int k = 0; k<3; k++){
		   if ( RANDOM_DIST[i][j][k] == 0)
			 x[k]='N';
		   else
			 x[k]='U';
		 }
		 sprintf(distrib,"(%c,%c,%c)",x[0],x[1],x[2]);
		 
		 
		 if (strcmp(targets,"")==0)
		   sprintf(targets,"%s:%s",population[i].label,distrib);
		 else
		   sprintf(targets,"%s\n%s:%s",targets,population[i].label,distrib);
	   }
	 }
	 summary_grid->SetReadOnly(11,j,false);
	 summary_grid->SetRowSize( 11, 60 ); 
	 summary_grid->SetCellValue(11,j,wxString(targets,wxConvUTF8));
	 summary_grid->SetCellRenderer(11 , j, new wxGridCellAutoWrapStringRenderer());
	 summary_grid->SetCellEditor( 11,  j , new wxGridCellAutoWrapStringEditor());
	 summary_grid->SetCellValue(12,0,_T("Synapses"));
	 summary_grid->SetCellSize(12, 0, 1, pop_number);
	 summary_grid->SetAttr(12,0, attr);
	 str_equal("",targets);
	 for (int i=0; i<pop_number; i++){
	      if (conn_values[j][i]!=0){
		 if (strcmp(targets,"")==0) 
		    sprintf(targets,"%s:%s",population[i].label,synapses[j][i].conductances_dynamics.model_label);
		 else
		    sprintf(targets,"%s\n%s:%s",targets,population[i].label,synapses[j][i].conductances_dynamics.model_label);
	      }
	 }
	 summary_grid->SetReadOnly(13,j,false);
	 summary_grid->SetRowSize( 13, 60 ); 
	 summary_grid->SetCellValue(13,j,wxString(targets,wxConvUTF8));
	 summary_grid->SetCellRenderer(13 , j, new wxGridCellAutoWrapStringRenderer());
	 summary_grid->SetCellEditor( 13,  j , new wxGridCellAutoWrapStringEditor());
	 str_equal("",targets);
	 for (int i=0; i<pop_number; i++){
	      if (conn_values[j][i]!=0){
		  if (strcmp(targets,"")==0)
		      sprintf(targets,"%s:%s",population[i].label,synapses[j][i].plasticity_dynamics.model_label);
		  else
		      sprintf(targets,"%s\n%s:%s",targets,population[i].label,synapses[j][i].plasticity_dynamics.model_label);
	      }
	}
	 summary_grid->SetReadOnly(14,j,false);
	 summary_grid->SetRowSize( 14, 60 ); 
	 summary_grid->SetCellValue(14,j,wxString(targets,wxConvUTF8));
	 summary_grid->SetCellRenderer(14 , j, new wxGridCellAutoWrapStringRenderer());
	 summary_grid->SetCellEditor( 14,  j , new wxGridCellAutoWrapStringEditor());
	 for (int i=0; i<(summary_grid->GetNumberRows()); i++){
	      summary_grid->SetCellAlignment(i, j, wxALIGN_CENTRE, wxALIGN_CENTRE);
	  }
    }
    	if (pop_number<4){
	  GetSizer()->SetSizeHints(this);
	  this->SetPosition(wxPoint(830,50));
	}

}
