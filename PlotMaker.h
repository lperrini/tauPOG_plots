#ifndef PLOTMAKER_H
#define PLOTMAKER_H

#include "TCanvas.h"
#include "TFile.h"
#include "TKey.h"
#include "TBenchmark.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TFrame.h"
#include "TF1.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"
#include "TLegend.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>

using namespace std;

class PlotMaker{

  public:
    //Constructors
    PlotMaker();
    PlotMaker(string File, string EvT, string Eff, int Rebin);
    //virtual ~PlotMaker(); //class destructor

    //Methods
    void ReadFiles(string EvT);
    vector<string> GetDiscriminators(string EvT,
				     TFile *f);
    //void PrintDiscriminators(vector<string> list);
    void Initialize(string EvT);
    void InitializeStyle();
    TPaveText* CreatePaveTextDM(double d1,double d2, double d3, double d4, string s);    
    vector<TCanvas*> CreateCanvases(int nc, string name);
    //
    vector<double> RetrieveScatterPerc(TH2F* h1);
    vector< vector<double> > RetrieveScatterInfo();
    void StyleScatterHisto(TH2F* s1);
    //void SetOptimalRange(TH1F* h1);
    TPaveText* GetCMSTitle();
    void DrawScatterHisto(string Cname, string File);
    //
    pair<TH1F*,TH1F*> RetrieveGenHisto(string File);
    void StyleGenHisto(TH1F* h1, TH1F* h2);
    void DrawGenHisto(string File);
    TH1F* RebinHisto(TH1F* h, int n);
    //
    vector< vector<TH1F*> > RetrievePtRatioHisto();
    vector<TH1F*> AddHistos(vector<TH1F*> v1, vector<TH1F*> v2);
    TH1F* MultiplyHistos(TH1F* h1, TH1F* h2);
    int FindMaximum(vector<TH1F*> vh); //WIP
    vector<vector<TH1F*> > ScaleHistograms(vector<vector<TH1F*> > vvh);
    void DrawPtRatioHisto(string Cname, string File);
    void StylePtRatioHisto(vector<TH1F*> v1);
    TLegend* SetHistoLegend(vector<TH1F*> vh, vector<string> vs, vector<double>, string s);
    //
    vector<TH1F*> GetHistosForEff(string s, vector<string> vs, bool useRef=false);
    TH1F* DivideHisto(TH1F* h1, TH1F* h2);
    void DrawEffSignal(string File, vector<string> vsHN, vector<string> vsN, string HD, vector<string> vsD, vector<string> vsLeg, vector<double> vd, string Cname, int Rebin, bool useRef=false, bool range=false);
    void FinalDrawEffSignal(string File, int Rebin);
    void StyleEffHisto(vector<TH1F*>,string s);
    vector<vector< TH1F* > > GetEffSignal(vector<string> vsHN, vector<string> vsN, string HD, vector<string> vsD, int Rebin, bool useRef=false);
    //vector<vector< TH1F* > > GetCompositeEffSignal(vector<string> vsHN, vector<string> vsN, string HD, vector<string> vsD, int Rebin, bool useRef=false);

  protected:
    string m_File;
    string m_EvT;
    string m_Eff;
    int    m_Rebin;

    TFile *file;
    vector<int> colors;
    vector<int> markers;
    vector<string> discrList;
    vector<string> dmLabels;
    vector<string> dmLabelsShort;
    vector<string> dmHistos;
    string dir;
    string hpsPFTau;
    string refCollection;
    vector<string> iso3Hits;
    vector<string> eleRej;
    vector<string> muonRej;
    vector<string> MVAeleRej;
    vector<string> MVAmuonRej;
    vector<string> MVAoldLT;
    vector<string> forEff; 
    vector<string> forEffRef;
    vector<string> forEffRefFull;
    vector<string> typeLeg;
    vector<string> stringLegPtRatio;
    vector<string> stringLegEff3Hits;
    vector<string> stringLegEffMVAIso;
    vector<string> stringLegEffEleRej;
    vector<string> stringLegEffMuonRej;
    vector<string> stringLegEffMVAEleRej;
    vector<string> stringLegEffMVAMuonRej;
    vector<string> nameHisto;


};
#endif
