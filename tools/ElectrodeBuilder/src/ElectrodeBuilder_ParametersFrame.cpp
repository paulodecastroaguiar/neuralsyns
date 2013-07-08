#include "ElectrodeBuilder_ParametersFrame.h"
#include "main.h"

extern char elect[25];
extern double *params;
extern int params_number;

ElectrodeBuilder_ParametersFrame::ElectrodeBuilder_ParametersFrame( wxWindow* parent )
:
ParametersFrame( parent )
{

}


void ElectrodeBuilder_ParametersFrame::On_Init(wxInitDialogEvent& event )
{
   ElectrodeBuilder_MainFrame *myparent=(ElectrodeBuilder_MainFrame *) GetParent();
   int position = myparent->electrode_comboBox->GetCurrentSelection();
    
   wxGridCellAttr *attr1;
   attr1 = new wxGridCellAttr;
   attr1->SetBackgroundColour(*wxWHITE);
   switch (position)
   {
     case 1://Periodic
       strcpy(elect,"injFirePeriodically");
       table->SetColLabelValue( 0, wxT("Periodically") );
       table->SetRowLabelValue(0 , _T("Period [ms]"));//wxString(labels[i],wxConvUTF8) );
       table->SetRowLabelValue(1 , _T("Duration [ms]"));//wxString(labels[i],wxConvUTF8) );
       table->SetCellValue(0, 0, dbl2wxStr(100.0));
       table->SetCellValue(1, 0, dbl2wxStr(100.0));
       table->SetRowAttr(0, attr1);
       table->SetRowAttr(1, attr1);
       break;
     case 2: //fires a selected number of random neurons  
       strcpy(elect,"injFireRandom");
       table->SetColLabelValue( 0, wxT("Random") );
       table->DeleteRows(1,true);
       table->SetRowLabelValue(0 , _T("Sample size [1]"));
       table->SetCellValue(0, 0, dbl2wxStr(20.0));
       table->SetRowAttr(0, attr1);
       break;
     case 3://Poisson independ
       strcpy(elect,"injFirePoissonAsync");
       table->SetColLabelValue( 0, wxT("Poisson") );
       table->SetRowLabelValue(0 , _T("Mean [ms]"));
       table->SetRowLabelValue(1 , _T("Duration [ms]"));
       table->SetCellValue(0, 0, dbl2wxStr(200.0));
       table->SetCellValue(1, 0, dbl2wxStr(50.0));
       table->SetRowAttr(0, attr1);
       table->SetRowAttr(1, attr1);
       break;
     case 4://Poisson synchronously
       strcpy(elect,"injFirePoissonSync");
       table->SetColLabelValue( 0, wxT("Poisson") );
       table->SetRowLabelValue(0 , _T("Mean [ms]"));
       table->SetRowLabelValue(1 , _T("Duration [ms]"));
       table->SetCellValue(0, 0, dbl2wxStr(200.0));
       table->SetCellValue(1, 0, dbl2wxStr(50.0));
       table->SetRowAttr(0, attr1);
       table->SetRowAttr(1, attr1);
       break;
     case 5://Constant current
       strcpy(elect,"injCurrentConstant");
       table->SetColLabelValue( 0, wxT("Constant") );
       table->SetRowLabelValue(0 , _T("Current [nA]"));
       table->SetRowLabelValue(1 , _T("Duration [ms]"));
       table->SetCellValue(0, 0, dbl2wxStr(2.0));
       table->SetCellValue(1, 0, dbl2wxStr(200.0));
       table->SetRowAttr(0, attr1);
       table->SetRowAttr(1, attr1);
       break;
     case 6://Gaussian current
       strcpy(elect,"injCurrentGaussianNoise");
       table->SetColLabelValue( 0, wxT("Gaussian") );
       table->AppendRows(1,true);
       table->SetRowLabelValue(0 , _T("Mean [nA]"));
       table->SetRowLabelValue(1 , _T("Stdev [nA]"));
       table->SetRowLabelValue(2 , _T("Duration [ms]"));
       table->SetCellValue(0, 0, dbl2wxStr(1.5));
       table->SetCellValue(1, 0, dbl2wxStr(0.2));
       table->SetCellValue(2, 0, dbl2wxStr(500.0));
       table->SetRowAttr(0, attr1);
       table->SetRowAttr(1, attr1);
       table->SetRowAttr(2, attr1);
       break;
    case 7: //inject sinusoidal current
       strcpy(elect,"injCurrentSine");
       table->SetColLabelValue( 0, wxT("Sinusoidal") );
       table->AppendRows(1,true);
       table->SetRowLabelValue(0 , _T("Amplitude [nA]")); 
       table->SetRowLabelValue(1 , _T("Frequency [Hz]"));
       table->SetRowLabelValue(2 , _T("Duration [ms]"));
       table->SetCellValue(0, 0, dbl2wxStr(2.0));
       table->SetCellValue(1, 0, dbl2wxStr(10.0));
       table->SetCellValue(2, 0, dbl2wxStr(500.0));
       table->SetRowAttr(0, attr1);
       table->SetRowAttr(1, attr1);
       table->SetRowAttr(2, attr1);
       break;
     case 8:// record membrane potential from fdm00 
       strcpy(elect,"recV0");
       table->SetColLabelValue( 0, wxT("Record") );
       table->SetRowLabelValue(0 , _T("Periodicity [1]"));
       table->SetRowLabelValue(1 , _T("Duration [ms]"));
       table->SetCellValue(0, 0, wxString::Format(wxT("%i"),1));
       table->SetCellValue(1, 0, dbl2wxStr(500.0));
       table->SetRowAttr(0, attr1);
       table->SetRowAttr(1, attr1);
       break;
     case 9: //record membrane potential from all fdomais
       strcpy(elect,"recVAll");
       table->SetColLabelValue( 0, wxT("Record") );
       table->SetRowLabelValue(0 , _T("Periodicity [1]"));
       table->SetRowLabelValue(1 , _T("Duration [ms]"));
       table->SetCellValue(0, 0, wxString::Format(wxT("%i"),1));
       table->SetCellValue(1, 0, dbl2wxStr(500.0));
       table->SetRowAttr(0, attr1);
       table->SetRowAttr(1, attr1);
       break;
     case 10: //record state variables from fdm00
       strcpy(elect,"recStates0");
       table->SetColLabelValue( 0, wxT("Record") );
       table->SetRowLabelValue(0 , _T("Periodicity [1]"));
       table->SetRowLabelValue(1 , _T("Duration [ms]"));
       table->SetCellValue(0, 0, wxString::Format(wxT("%i"),1));
       table->SetCellValue(1, 0, dbl2wxStr(500.0));
       table->SetRowAttr(0, attr1);
       table->SetRowAttr(1, attr1);
       break;
     case 11: //record state variables from all fdomains
       strcpy(elect,"recStatesAll");
       table->SetColLabelValue( 0, wxT("Record") );
       table->SetRowLabelValue(0 , _T("Periodicity [1]"));
       table->SetRowLabelValue(1 , _T("Duration [ms]"));
       table->SetCellValue(0, 0, wxString::Format(wxT("%i"),1));
       table->SetCellValue(1, 0, dbl2wxStr(500.0));
       table->SetRowAttr(0, attr1);
       table->SetRowAttr(1, attr1);
       break;
     case 12: //record state variables from all synapses
       strcpy(elect,"recSynStates");
       table->SetColLabelValue( 0, wxT("Record") );
       table->SetRowLabelValue(0 , _T("Periodicity [1]"));
       table->SetRowLabelValue(1 , _T("Duration [ms]"));
       table->SetCellValue(0, 0,wxString::Format(wxT("%i"),1));
       table->SetCellValue(1, 0, dbl2wxStr(500.0));
       table->SetRowAttr(0, attr1);
       table->SetRowAttr(1, attr1);
       break;
    }
    
    GetSizer()->SetSizeHints(this);
}

void ElectrodeBuilder_ParametersFrame::On_Ok_Click( wxCommandEvent& event )
{
  
    params_number=table->GetNumberRows();
    params = (double*) malloc(params_number * sizeof(double));
    for (int par=0;par<params_number;par++){
        (table->GetCellValue(par,0)).ToDouble(&params[par]);
    }
 
    ElectrodeBuilder_MainFrame *myparent=(ElectrodeBuilder_MainFrame *) GetParent();
    myparent->Enable(true);
    this->Close();
}
