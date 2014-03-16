#include "PlotMaker.h"
#include "TH1F.h"

using namespace std;

void plotter(string File,
             string EvT,
             string Eff,
             int    Rebin){

    PlotMaker *PM = new PlotMaker(File,EvT,Eff,Rebin);
    PM->ReadFiles(EvT);
    PM->Initialize(EvT);
    PM->InitializeStyle();
    vector< vector<double> > scatter = PM->RetrieveScatterInfo();
    PM->DrawScatterHisto("recoDM_vs_genDM", File);
    pair<TH1F*,TH1F*> gHistos = PM->RetrieveGenHisto(File);
    PM->DrawGenHisto(File);
    PM->RetrievePtRatioHisto();
    PM->DrawPtRatioHisto("ptratio", File);
    PM->FinalDrawEffSignal(File, Rebin);

}
