#include "NetBuilderSigma_frame.h"

extern NetBuilderDistributions_frame *Distributions_frame;
extern int **conn_values;
extern double ***dist_values;
extern int ***RANDOM_DIST;
extern int pop_row ;
extern int pop_col ;

NetBuilderSigma_frame::NetBuilderSigma_frame( wxWindow* parent )
:
Sigma( parent )
{

}
void NetBuilderSigma_frame::OnInitSigma( wxInitDialogEvent& event )
{
    int i=pop_row;
    int j=pop_col;
     m_textCtrl1->SetValue(dbl2wxStr(dist_values[i][j][0]));
     m_textCtrl2->SetValue(dbl2wxStr(dist_values[i][j][1]));
     m_textCtrl3->SetValue(dbl2wxStr(dist_values[i][j][2]));
}

void NetBuilderSigma_frame::Ok_Button_Click( wxCommandEvent& event )
{
    int error=0;
    int i=pop_row;
    int j=pop_col;
    (m_textCtrl1->GetValue()).ToDouble(&dist_values[i][j][0]);
    (m_textCtrl2->GetValue()).ToDouble(&dist_values[i][j][1]);
    (m_textCtrl3->GetValue()).ToDouble(&dist_values[i][j][2]);
    if ((m_textCtrl1->GetValue()).ToDouble(&dist_values[i][j][0])==false || dist_values[i][j][0]<0){
	  m_textCtrl1->SetForegroundColour( *wxRED );
	  error=-1;
    }
    if ((m_textCtrl2->GetValue()).ToDouble(&dist_values[i][j][1])==false || dist_values[i][j][1]<0){
	  m_textCtrl2->SetForegroundColour( *wxRED );
	  error=-1;
    }
    if ((m_textCtrl3->GetValue()).ToDouble(&dist_values[i][j][2])==false || dist_values[i][j][2]<0){
	  m_textCtrl3->SetForegroundColour(  *wxRED );
	  error=-1;	  
    }
    
    if (error==0){
      	  this->Close();
	  Distributions_frame->Enable();
	  Distributions_frame->Destroy();
	  Distributions_frame=new NetBuilderDistributions_frame(0L);
	  Distributions_frame->Show();
	  //Distributions_frame->SetPosition(wxPoint(200,200));
	  Distributions_frame->Center(wxCENTER_ON_SCREEN );

    }
}
