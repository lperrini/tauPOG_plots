#include "PlotMaker.h"
#include "TH1F.h"
#include <sstream>
#include <iostream>
#include <vector>
#include "TApplication.h"
#include "TRint.h"

using namespace std;

int main(int argc, char* argv[]){
  
  //cout<< "argc " << argc<<endl; //ti serve per contare di quanti elementi necessari hai bisogno
  if(argc<11){
    cout<<"PlotMaker: TauID plots producer!"<<endl;
    cout<<"  Usage   : ./Plotter "<<endl;
    cout<<"  Needed Options : "<<endl;
    cout<<"                   --File  [Channel Name]"<<endl;
    cout<<"                   --Event [Channel Name]"<<endl;
    cout<<"                   --Effi [Channel Name]"<<endl;
    cout<<"                   --Rebin [Rebinning Factor]"<<endl;
    cout<<"                   --Dir   [Folder Name]"<<endl;
    cout<<"  Other Options  :"<<endl;
    //generalmente qui si mettono quelle opzioni che se non le passi vengono settate di defult dal codice
    cout<<"                   --path /home/salvucci/Lists/ [path to Data/MC list: default is ./]"<<endl;
    exit(0);
  };
  
  string File,EvT,Eff,TmpRebin,Folder;
  int Rebin;
  
  for(int a=1; a<argc; a++){
    if(!strcmp(argv[a],"--File")){
      File = argv[a+1];
    }
    else if(!strcmp(argv[a],"--Event")){
      EvT = argv[a+1];
    }
    else if(!strcmp(argv[a],"--Effi")){
      Eff = argv[a+1];
    }
    else if(!strcmp(argv[a],"--Rebin")){
      TmpRebin = argv[a+1];
      istringstream buffer(TmpRebin);
      buffer>>Rebin;
    }
    else if(!strcmp(argv[a],"--Dir")){
      Folder = argv[a+1];
    }
  }
  
  /*debug*/
  cout<< " Options Passed:" <<endl;
  cout<< "           File  = " << File   << endl;
  cout<< "           Event = " << EvT    << endl;
  cout<< "           Effi  = " << Eff    << endl;
  cout<< "           Rebin = " << Rebin  << endl;
  cout<< "           Dir   = " << Folder << endl;
  
#ifdef WITHRINT
  TRint *myApp = new TRint("RootSession",&argc,argv,NULL,0);
  cout<<" Using Rint: Opening ROOT ..." << endl;
#else
  TApplication *myApp = new TApplication("myApp",0,0);
  cout<<" Using TApplication: Gui will be used ... " << endl;
#endif
    
  PlotMaker *PM = new PlotMaker(File,EvT,Eff,Rebin,Folder);
  PM->ReadFiles();
  PM->Initialize();
  PM->InitializeStyle();
  if(EvT=="ZpTT" || EvT=="ZTT"){
    vector< vector<double> > scatter = PM->RetrieveScatterInfo();
    PM->DrawScatterHisto("recoDM_vs_genDM");
    PM->DrawGenHisto();
    PM->RetrievePtRatioHisto();
    PM->DrawPtRatioHisto("ptratio");
  }
  PM->FinalDrawEffSignal(Rebin);
  //PM->MakeTables(File);
  
  /*Running GUI*/
  myApp->Run();
  return 0;
}
