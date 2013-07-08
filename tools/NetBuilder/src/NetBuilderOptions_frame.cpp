#include "NetBuilderOptions_frame.h"

extern bool DivergentConnectivity;
extern bool Homogeneity;
extern bool Auto_connections;
extern bool Multiplicity;

NetBuilderOptions_frame::NetBuilderOptions_frame( wxDialog* parent )
:
Options_frame( parent )
{

}

void NetBuilderOptions_frame::OnInitOptions( wxInitDialogEvent& event )
{
	if (DivergentConnectivity==true)
	  conn_choices->SetSelection( 0 );
	else
	  conn_choices->SetSelection( 1 );

	if (Homogeneity==true)
	  syn_choices->SetSelection( 0 );
	else
	  syn_choices->SetSelection( 1 );

	if (Auto_connections==true)
	  auto_choices->SetSelection( 1 );
	else
	  auto_choices->SetSelection( 0 );

	if (Multiplicity==true){
	  mult_choices->SetSelection( 1 );
	}
	else
	  mult_choices->SetSelection( 0 );

}

void NetBuilderOptions_frame::On_syn_choices_Selection( wxCommandEvent& event )
{
	  if ((syn_choices->GetCurrentSelection())==1)
	    {Homogeneity=false;}
	  else
	    {Homogeneity=true;}
}

void NetBuilderOptions_frame::On_conn_choices_Selection( wxCommandEvent& event )
{
	  if ((conn_choices->GetCurrentSelection())==1)
	    {DivergentConnectivity=false;}
	  else
	    {DivergentConnectivity=true;}
}

void NetBuilderOptions_frame::On_auto_choices_Selection( wxCommandEvent& event )
{
	if (auto_choices->GetCurrentSelection()==1)
	  Auto_connections=true;
	else
	  Auto_connections=false;
}

void NetBuilderOptions_frame::On_mult_choices_Selection( wxCommandEvent& event )
{
	if (mult_choices->GetCurrentSelection()==1)
	  Multiplicity=true;
	else
	  Multiplicity=false;
}

void NetBuilderOptions_frame::On_btn_options_OKClick( wxCommandEvent& event )
{
	Connectivity_frame* myparent=(Connectivity_frame *) GetParent();
	myparent->Enable( true );
// 	myparent->btn_conn_OK->Enable( true );
// 	myparent->btn_conn_Options->Enable( true );
	this->Destroy();// TODO: Implement On_btn_options_OKClick
}
