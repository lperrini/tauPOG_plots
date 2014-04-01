#include "PlotMaker.h"

PlotMaker::PlotMaker(){
}

PlotMaker::PlotMaker(string File,
                     string EvT,
                     string Eff,
                     int    Rebin,
                     string Folder){
	gROOT->ProcessLine(".L tdrstyle.C");
	gROOT->ProcessLine("setTDRStyle()");

    m_File    = File;
    m_EvT     = EvT;
    m_Eff     = Eff;
    m_Rebin   = Rebin;
    m_Folder  = Folder;
 
    CreateDir();
}

void PlotMaker::ReadFiles(){

    file = new TFile( (m_File).c_str() );
    discrList = GetDiscriminators(file);
    //PrintDiscriminators(discrList);
}
    
//*********************************************************************************************************
//Create Dir to organize all the plots
//*********************************************************************************************************
void PlotMaker::CreateDir(){
	struct stat st;
	if(stat(m_Folder.c_str(),&st) == 0){
		cout<<"Out Directory "<< m_Folder <<" already present!"<<endl;
	}else{
		cout<<"Creating Directory "<< m_Folder <<" ... "<<endl;
		int outD = system( ("mkdir "+m_Folder).c_str() );
		if(outD!=0)
			cout<<"Directory "<< m_Folder <<" could not be created!"<< endl;
	}
}

//*********************************************************************************************************
//Get the list of discriminators
//*********************************************************************************************************
vector<string> PlotMaker::GetDiscriminators(TFile *f){


    string prefix   = "hpsPFTauProducer"+m_EvT+"_hpsPFTauDiscriminationBy";
    int prefix_size = prefix.size();
    string discrim = " "; 
    int discrim_size;
    vector<string> list_discrim;

    TIter next(f->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)next())) {
        TClass *cl = gROOT->GetClass(key->GetClassName());
        if (cl->InheritsFrom("TDirectory")){
            TDirectory* dir1 = (TDirectory*)key->ReadObj();
            TIter next2(dir1->GetListOfKeys());
            TKey *key2;
            while ((key2 = (TKey*)next2())) {
                TClass *cl2 = gROOT->GetClass(key2->GetClassName());
                if (cl2->InheritsFrom("TDirectory")){
                    TDirectory* dir2 = (TDirectory*)key2->ReadObj();
                    TIter next3(dir2->GetListOfKeys());
                    TKey *key3;
                    while ((key3 = (TKey*)next3())) {
                        TClass *cl3 = gROOT->GetClass(key3->GetClassName());
                        if (cl3->InheritsFrom("TDirectory")){
                            TDirectory* dir3 = (TDirectory*)key3->ReadObj();
                            string namedir = (string)dir3->GetName();
                            if(namedir.find("Matched") != string::npos) 
                                list_discrim.push_back("Matched");
                            else if(namedir.find("Reference") != string::npos)
                                list_discrim.push_back("ReferenceCollection");
                            else{
                                discrim_size = namedir.size() - prefix_size;
                                discrim      = namedir.substr(prefix_size,discrim_size);
                                list_discrim.push_back(discrim);
                                discrim = " ";
                            }
                        }
                    }
                }
            }
        }
    }
    return list_discrim;
}

//void PlotMaker::PrintDiscriminators(vector<string> list){
//    for(uint i=0;i<list.size();i++)
//        cout << list.at(i) << endl;
//}



//*********************************************************************************************************
//Initialize what you need!
//*********************************************************************************************************
void PlotMaker::Initialize(){

	dmLabels.clear();
	dmHistos.clear();
	iso3Hits.clear();
	MVAoldLT.clear();
        eleRej.clear();
        muonRej.clear();
        MVAeleRej.clear();
        MVAmuonRej.clear();
        forEff.clear();
        forEffRef.clear();
        forEffRefFull.clear();
	//1 bin 
        forEff1bin.clear();
        forEffRef1bin.clear();

        dir = ""; hpsPFTau = ""; refCollection = "";  
	
	dir       	 	 = "DQMData/RecoTauV/";
	hpsPFTau  	 	 = "hpsPFTauProducer"+m_EvT+"_";
	refCollection 	 	 = "ReferenceCollection/";
        dmLabels.push_back("generated #tau^{-} #rightarrow h^{-} #nu_{#tau}"); 
        dmLabels.push_back("generated #tau^{-} #rightarrow h^{-} #pi^{0}s #nu_{#tau}"); 
        dmLabels.push_back("generated #tau^{-} #rightarrow h^{-} h^{+} h^{-} #nu_{#tau}"); 
        dmLabelsShort.push_back("#pi"); 
        dmLabelsShort.push_back("#pi#pi^{0}s"); 
        dmLabelsShort.push_back("#pi#pi#pi"); 
	dmHistos.push_back("oneProng0Pi0"); 
        dmHistos.push_back("oneProng1Pi0"); 
        dmHistos.push_back("oneProng2Pi0"); 
        dmHistos.push_back("threeProng0Pi0");
	iso3Hits.push_back("hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits");
	iso3Hits.push_back("hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits");
	iso3Hits.push_back("hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits");
        MVAoldLT.push_back("hpsPFTauDiscriminationByVLooseIsolationMVA3oldDMwLT");
        MVAoldLT.push_back("hpsPFTauDiscriminationByLooseIsolationMVA3oldDMwLT");
        MVAoldLT.push_back("hpsPFTauDiscriminationByMediumIsolationMVA3oldDMwLT");
        MVAoldLT.push_back("hpsPFTauDiscriminationByTightIsolationMVA3oldDMwLT");
        MVAoldLT.push_back("hpsPFTauDiscriminationByVTightIsolationMVA3oldDMwLT");
        MVAoldLT.push_back("hpsPFTauDiscriminationByVVTightIsolationMVA3oldDMwLT");
        eleRej.push_back("hpsPFTauDiscriminationByLooseElectronRejection"); 
        eleRej.push_back("hpsPFTauDiscriminationByMediumElectronRejection"); 
        eleRej.push_back("hpsPFTauDiscriminationByTightElectronRejection");
        muonRej.push_back("hpsPFTauDiscriminationByLooseMuonRejection3"); 
        muonRej.push_back("hpsPFTauDiscriminationByTightMuonRejection3");
        MVAmuonRej.push_back("hpsPFTauDiscriminationByMVALooseMuonRejection");   
        MVAmuonRej.push_back("hpsPFTauDiscriminationByMVAMediumMuonRejection"); 
        MVAmuonRej.push_back("hpsPFTauDiscriminationByMVATightMuonRejection");  
        MVAeleRej.push_back("hpsPFTauDiscriminationByMVA5VLooseElectronRejection");     
        MVAeleRej.push_back("hpsPFTauDiscriminationByMVA5LooseElectronRejection");     
        MVAeleRej.push_back("hpsPFTauDiscriminationByMVA5MediumElectronRejection");    
        MVAeleRej.push_back("hpsPFTauDiscriminationByMVA5TightElectronRejection");     
        MVAeleRej.push_back("hpsPFTauDiscriminationByMVA5VTightElectronRejection");    

        forEff.push_back("_vs_etaTauVisible");
        forEff.push_back("_vs_phiTauVisible");
        forEff.push_back("_vs_pileupTauVisible");
        forEff.push_back("_vs_ptTauVisible");
        forEffRef.push_back("nRef_Taus_vs_etaTauVisible");
        forEffRef.push_back("nRef_Taus_vs_phiTauVisible");
        forEffRef.push_back("nRef_Taus_vs_pileupTauVisible");
        forEffRef.push_back("nRef_Taus_vs_ptTauVisible");
        //1 bin
        forEff1bin.push_back("_vs_etaTauVisible_1bin");
        forEff1bin.push_back("_vs_phiTauVisible_1bin");
        forEff1bin.push_back("_vs_pileupTauVisible_1bin");
        forEff1bin.push_back("_vs_ptTauVisible_1bin");
        forEffRef1bin.push_back("nRef_Taus_vs_etaTauVisible_1bin");
        forEffRef1bin.push_back("nRef_Taus_vs_phiTauVisible_1bin");
        forEffRef1bin.push_back("nRef_Taus_vs_pileupTauVisible_1bin");
        forEffRef1bin.push_back("nRef_Taus_vs_ptTauVisible_1bin");
        //these next are useful for Zprime plots. We need a different 
        //range wrt the one used for efficiencies
        forEffRefFull.push_back("nRef_Taus_vs_ptTauVisible_full");

        typeLeg.clear();
        typeLeg.push_back("l"); typeLeg.push_back("p"); typeLeg.push_back("f");
	stringLegPtRatio.clear();
        stringLegPtRatio.push_back(" 3 < N_{vtx} <  7"); 
        stringLegPtRatio.push_back(" 8 < N_{vtx} < 12"); 
        stringLegPtRatio.push_back("13 < N_{vtx} < 17"); 
        stringLegPtRatio.push_back("18 < N_{vtx} < 22"); 
        stringLegPtRatio.push_back("23 < N_{vtx} < 27"); 
	//Combined Isolation DB Sum Pt Correct - 3 Hits
        stringLegEff3Hits.clear();
        stringLegEff3Hits.push_back("HPS #delta#beta 3-hit Loose");
        stringLegEff3Hits.push_back("HPS #delta#beta 3-hit Medium");
        stringLegEff3Hits.push_back("HPS #delta#beta 3-hit Tight");
	//MVA isolation with LT info included
	stringLegEffMVAIso.clear();
        stringLegEffMVAIso.push_back("MVA incl. LT vLoose");
        stringLegEffMVAIso.push_back("MVA incl. LT Loose");
        stringLegEffMVAIso.push_back("MVA incl. LT Medium");
        stringLegEffMVAIso.push_back("MVA incl. LT Tight");
        stringLegEffMVAIso.push_back("MVA incl. LT vTight");
        stringLegEffMVAIso.push_back("MVA incl. LT vvTight");
	//Cut-based anti electron discriminators
	stringLegEffEleRej.clear();
        stringLegEffEleRej.push_back("Cut-based anti-e Discriminator Loose");
        stringLegEffEleRej.push_back("Cut-based anti-e Discriminator Medium");
        stringLegEffEleRej.push_back("Cut-based anti-e Discriminator Tight");
	//MVA anti electron discriminators
	stringLegEffMVAEleRej.clear();
        stringLegEffMVAEleRej.push_back("MVA anti-e Discriminator Loose");
        stringLegEffMVAEleRej.push_back("MVA anti-e Discriminator Medium");
        stringLegEffMVAEleRej.push_back("MVA anti-e Discriminator Tight");
        stringLegEffMVAEleRej.push_back("MVA anti-e Discriminator VLoose");
        stringLegEffMVAEleRej.push_back("MVA anti-e Discriminator VTight");
	//Cut-based anti muon discriminators
	stringLegEffMuonRej.clear();
        stringLegEffMuonRej.push_back("Cut-based anti-#mu Discriminator Loose");
        stringLegEffMuonRej.push_back("Cut-based anti-#mu Discriminator Tight");
	//MVA anti muon discriminators
	stringLegEffMVAMuonRej.clear();
        stringLegEffMVAMuonRej.push_back("MVA anti-#mu Discriminator Loose");
        stringLegEffMVAMuonRej.push_back("MVA anti-#mu Discriminator Medium");
        stringLegEffMVAMuonRej.push_back("MVA anti-#mu Discriminator Tight");
 
}

//*********************************************************************************************************
//Initialize the style
//*********************************************************************************************************
void PlotMaker::InitializeStyle(){
        colorsCB.clear();
        colorsCB.push_back(418); colorsCB.push_back(807); colorsCB.push_back(618); 
        colorsMVA.clear();
        colorsMVA.push_back(856); colorsMVA.push_back(418); colorsMVA.push_back(807); 
        colorsMVA.push_back(618); colorsMVA.push_back(596); colorsMVA.push_back(14); 
        markersCB.clear();
        markersCB.push_back(25); markersCB.push_back(26); markersCB.push_back(32); 
        markersMVA.clear();
        markersMVA.push_back(24); markersMVA.push_back(25); markersMVA.push_back(26); 
        markersMVA.push_back(32); markersMVA.push_back(27); markersMVA.push_back(30); 
}

//*********************************************************************************************************
//Create canvases
//*********************************************************************************************************
vector<TCanvas*> PlotMaker::CreateCanvases(int nc, string name){
	TCanvas *c;
	vector<TCanvas* > Cans;
	stringstream Cname;
	string CName;
	for(int i=0; i<nc; i++){
		Cname.clear();
		Cname<<name<<i;
		Cname>>CName;
		c = new TCanvas(CName.c_str(),CName.c_str());
		Cans.push_back(c);
	}
	return Cans;
}

//*********************************************************************************************************
//Create Pave text
//*********************************************************************************************************
TPaveText* PlotMaker::CreatePaveTextDM(double d1,double d2, double d3, double d4, string s){
     TPaveText *pavt = new TPaveText(d1, d2, d3, d4, "NDC");
     pavt->SetTextSize(0.037);
     pavt->SetTextFont(62);
     pavt->SetFillColor(0);
     pavt->SetBorderSize(0);
     pavt->SetMargin(0.01);
     pavt->SetTextAlign(12); // align left
     pavt->AddText(0.01,0.4,s.c_str());
     return pavt;
}

//*********************************************************************************************************
//Retrieve percentage to fill the "reconstructed decay mode vs generated decay mode" plot
//*********************************************************************************************************
vector<double> PlotMaker::RetrieveScatterPerc(TH2F* h1){
        vector<double> percentage;
        percentage.clear();
        int sum1,sum2,sum3;
        sum1=h1->GetBinContent(1,1)+h1->GetBinContent(1,2)+h1->GetBinContent(1,3);
        sum2=h1->GetBinContent(2,1)+h1->GetBinContent(2,2)+h1->GetBinContent(2,3);
        sum3=h1->GetBinContent(3,1)+h1->GetBinContent(3,2)+h1->GetBinContent(3,3);
        percentage.push_back( h1->GetBinContent(1,1)/sum1 ); percentage.push_back( h1->GetBinContent(1,2)/sum1 ); percentage.push_back( h1->GetBinContent(1,3)/sum1 );  
        percentage.push_back( h1->GetBinContent(2,1)/sum2 ); percentage.push_back( h1->GetBinContent(2,2)/sum2 ); percentage.push_back( h1->GetBinContent(2,3)/sum2 );  
        percentage.push_back( h1->GetBinContent(3,1)/sum3 ); percentage.push_back( h1->GetBinContent(3,2)/sum3 ); percentage.push_back( h1->GetBinContent(3,3)/sum3 );  
        sum1=0,sum2=0,sum3=0;
        return percentage;
}

vector< vector<double> > PlotMaker::RetrieveScatterInfo(){
	vector<string> Nvtx;
        Nvtx.push_back("_RecoDM_vs_GenDM"); Nvtx.push_back("_3to7_RecoDM_vs_GenDM"); Nvtx.push_back("_8to12_RecoDM_vs_GenDM");
        Nvtx.push_back("_13to17_RecoDM_vs_GenDM"); Nvtx.push_back("_18to22_RecoDM_vs_GenDM"); Nvtx.push_back("_23to27_RecoDM_vs_GenDM");
	vector<TH2F*> s1;
	vector< vector<double> > percs;
	for(uint j=0;j<Nvtx.size();j++){
		s1.push_back( (TH2F*)file->Get( (dir+hpsPFTau+iso3Hits.at(0)+"/"+iso3Hits.at(0)+Nvtx.at(j) ).c_str() ) ); 
		percs.push_back( RetrieveScatterPerc(s1.at(j)) );
	}
        return percs;
}

void PlotMaker::StyleScatterHisto(TH2F* s1){
     s1->SetMarkerSize(3);
     s1->GetXaxis()->SetNdivisions(300);
     s1->GetYaxis()->SetNdivisions(300);
     s1->GetXaxis()->SetTitle( "generated #tau decay mode" );
     s1->GetYaxis()->SetTitle( "reconstructed #tau decay mode" );
     
     if( dmLabels.size()==0 ) cout << "WARNING! No decay mode labels found!" << endl;
     
     for(uint i=0;i<3;i++){ 
	     s1->GetXaxis()->SetBinLabel(i+1,dmLabelsShort[i].c_str());
	     s1->GetYaxis()->SetBinLabel(i+1,dmLabelsShort[i].c_str());
     }
}

TH1F* PlotMaker::RebinHisto(TH1F* h, int n){
        if(nameHisto.size()==0){
		h->Rebin(n);
	        nameHisto.push_back( (string)h->GetName() );
        	return h;
	}
        else{
                bool found = false;
		for(uint i=0;i<nameHisto.size();i++){
                   if( (string)h->GetName() == nameHisto.at(i) ){
			   found = true;
                           break;}
		}
                if(found) return h;
                else{
			h->Rebin(n);
			nameHisto.push_back( (string)h->GetName() );
			return h;
                }
	} 
}


TPaveText* PlotMaker::GetCMSTitle(){
     TPaveText *ll = new TPaveText(0.16, 0.96, 0.96, 0.99, "NDC");
     ll->SetTextSize(0.037);
     ll->SetTextFont(62);
     ll->SetFillColor(0);
     ll->SetBorderSize(0);
     ll->SetMargin(0.01);
     ll->SetTextAlign(12); // align left
     TString text = "CMS Simulation 2012, #sqrt{8} TeV ";
     ll->AddText(0.01,0.4,text);
     return ll;
}

void PlotMaker::DrawScatterHisto(string Cname){
        vector<TH2F*>  finalScatter;
	vector<string> forName;
        forName.push_back("3to7"),forName.push_back("8to12"),forName.push_back("13to17"),forName.push_back("18to22"),forName.push_back("23to27");
	stringstream streamname; 
	string histname; 
	TH2F * tmp; 
        vector<TCanvas*> canvas =  CreateCanvases( (int)forName.size(), Cname );
        for (uint i=0; i<forName.size(); i++) {
		streamname.clear();
		histname="";
                streamname<<forName[i]<<"_Nvts";
                streamname>>histname;
                tmp =  new TH2F(histname.c_str(),histname.c_str(),3,0.,3.,3,0.,3.);
                ( finalScatter ).push_back( tmp );
                ( finalScatter.at(i) )->SetBinContent(1,1,( (RetrieveScatterInfo()).at(i) ).at(0)  );
                ( finalScatter.at(i) )->SetBinContent(1,2,( (RetrieveScatterInfo()).at(i) ).at(1) );
                ( finalScatter.at(i) )->SetBinContent(1,3,( (RetrieveScatterInfo()).at(i) ).at(2) );
                ( finalScatter.at(i) )->SetBinContent(2,1,( (RetrieveScatterInfo()).at(i) ).at(3) );
                ( finalScatter.at(i) )->SetBinContent(2,2,( (RetrieveScatterInfo()).at(i) ).at(4) );
                ( finalScatter.at(i) )->SetBinContent(2,3,( (RetrieveScatterInfo()).at(i) ).at(5) );
                ( finalScatter.at(i) )->SetBinContent(3,1,( (RetrieveScatterInfo()).at(i) ).at(6) );
                ( finalScatter.at(i) )->SetBinContent(3,2,( (RetrieveScatterInfo()).at(i) ).at(7) );
                ( finalScatter.at(i) )->SetBinContent(3,3,( (RetrieveScatterInfo()).at(i) ).at(8) );
        }
        TPaveText* l1 = GetCMSTitle();
	for(uint i=0;i<finalScatter.size();i++){
		canvas[i]->cd();
                StyleScatterHisto(finalScatter.at(i));
                TPaveText* pavet = CreatePaveTextDM( 0.17, 0.90, 0.27, 0.94, (stringLegPtRatio.at(i)).c_str() );
                finalScatter.at(i)->Draw("TEXT");    
		l1->Draw("same");
		pavet->SetTextColor(kRed);
		pavet->Draw("same");
		canvas[i]->SaveAs(((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+forName[i]+"_"+m_Folder+".pdf").c_str());
		canvas[i]->SaveAs(((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+forName[i]+"_"+m_Folder+".png").c_str());
		canvas[i]->SaveAs(((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+forName[i]+"_"+m_Folder+".eps").c_str());
	}
}

pair<TH1F*,TH1F*> PlotMaker::RetrieveGenHisto(){
	pair<TH1F*, TH1F*> gHistos;
	TH1F* histoCheck = (TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(3) ).c_str() );
	double min = histoCheck->GetXaxis()->GetXmin();
	double max = histoCheck->GetXaxis()->GetXmax();

	if( m_File.find("ZpTT") != string::npos ){
		if( (max - min) > 150. ){
			gHistos = make_pair(  RebinHisto( (TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRefFull.at(0) ).c_str() ), 2), 
				(TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(0) ).c_str() ) );
		}else{
			gHistos = make_pair(  RebinHisto( (TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(3) ).c_str() ), 2), 
				(TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(0) ).c_str() ) );
		}
	}else if( m_File.find("ZTT") != string::npos ){
			gHistos = make_pair(    (TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(3) ).c_str() ), 
				(TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(0) ).c_str() ) );
			}
	return gHistos;
}

void PlotMaker::StyleGenHisto(TH1F* h1, TH1F* h2){
     h1->SetMarkerStyle( markersCB.at(0) );
     h2->SetMarkerStyle( markersCB.at(0) );
     h1->SetLineWidth( 2 );
     h2->SetLineWidth( 2 );
     h1->SetMarkerColor( colorsCB.at(0) );
     h2->SetMarkerColor( colorsCB.at(0) );
     h1->SetLineColor( colorsCB.at(0) );
     h2->SetLineColor( colorsCB.at(0) );
     h1->GetYaxis()->SetTitle( "Relative yield" );
     h2->GetYaxis()->SetTitle( "Relative yield" );
     h1->GetYaxis()->SetTitleOffset(1.70);
     h2->GetYaxis()->SetTitleOffset(1.80);
     h1->GetXaxis()->SetTitle( "generated p_{T}^{#tau} [GeV/c]" );
     h2->GetXaxis()->SetTitle( "generated #eta_{#tau}" );
     h1->GetXaxis()->SetTitleOffset(1.30);
     h2->GetXaxis()->SetTitleOffset(1.30);
}

void PlotMaker::DrawGenHisto(){
	TH1F *h1 = (TH1F*)RetrieveGenHisto().first;
	TH1F *h2 = (TH1F*)RetrieveGenHisto().second;
        TPaveText* l1 = GetCMSTitle();
        StyleGenHisto(h1,h2);
	TCanvas *c1 = new TCanvas();
        c1->cd();
        TH1F *h1C = (TH1F*)h1->Clone();
        h1C->Sumw2();
        h1C->Scale(1./h1C->Integral());    
        h1C->Draw("E1P");    
        l1->Draw("same");
        c1->SaveAs((m_Folder+"/GenPt_"+m_Folder+".pdf").c_str());
        c1->SaveAs((m_Folder+"/GenPt_"+m_Folder+".png").c_str());
        c1->SaveAs((m_Folder+"/GenPt_"+m_Folder+".eps").c_str());
	TCanvas *c2 = new TCanvas();
        c2->cd();
        TH1F *h2C = (TH1F*)h2->Clone();
        h2C->Sumw2();
        h2C->Rebin(6);
        h2C->Scale(1./h2C->Integral());    
        h2C->GetYaxis()->SetRangeUser(0.,0.3);    
	h2C->Draw("E1P");    
        l1->Draw("same");
        c2->SaveAs((m_Folder+"/GenEta_"+m_Folder+".pdf").c_str());
        c2->SaveAs((m_Folder+"/GenEta_"+m_Folder+".png").c_str());
        c2->SaveAs((m_Folder+"/GenEta_"+m_Folder+".eps").c_str());
}

vector<TH1F*> PlotMaker::AddHistos(vector<TH1F*> v1, vector<TH1F*> v2){
	vector<TH1F*> merge;
	for(uint i=0; i<v1.size(); i++){
                TH1F *tmp1 = v1.at(i);
                TH1F *tmp2 = v2.at(i);
                tmp1->Add(tmp2);
		merge.push_back( tmp1 );
	}	
	return merge;
}

TH1F* PlotMaker::MultiplyHistos(TH1F* h1, TH1F* h2){
	TH1F *tmp1 = h1;
	TH1F *tmp2 = h2;
	TH1F* multiply;
	tmp1->Multiply(tmp2);
        multiply = tmp1;
        return multiply;
}

vector< vector<TH1F*> > PlotMaker::RetrievePtRatioHisto(){
	vector<string> Nvtx;
        Nvtx.push_back("_Nvtx3to7"); Nvtx.push_back("_Nvtx8to12"); Nvtx.push_back("_Nvtx13to17"); 
        Nvtx.push_back("_Nvtx18to22"); Nvtx.push_back("_Nvtx23to27"); 

        vector< vector<TH1F*> > histo;
        vector< vector<TH1F*> > finalHisto;
        vector<TH1F*> tmpVec;
        
        if( dmHistos.size()==0 ) cout << "WARNING! No decay mode histos found!" << endl;

	for(uint i=0;i<dmHistos.size();i++){
		tmpVec.clear();
		for(uint j=0;j<Nvtx.size();j++){
			tmpVec.push_back( (TH1F*)file->Get( (dir+hpsPFTau+iso3Hits.at(0)+"/"+iso3Hits.at(0)+"_pTRatio_"+dmHistos.at(i)+Nvtx.at(j) ).c_str() ) );
		}
			histo.push_back(tmpVec);
	}
       
        finalHisto.push_back( histo.at(0) );
	finalHisto.push_back( AddHistos(histo.at(1),histo.at(2)) );
	finalHisto.push_back( histo.at(3) );

        //cout << "final histo size " << finalHisto.size() << endl;	
	for(uint i=0;i<finalHisto.size();i++){
		for(uint j=0;j<(finalHisto.at(i)).size();j++){
		}
	}

	return finalHisto;
}

TLegend* PlotMaker::SetHistoLegend(vector<TH1F*> vh, vector<string> vs, vector<double> vd, string s){
	TLegend* leg = new TLegend(vd.at(0),vd.at(1),vd.at(2),vd.at(3),NULL,"brNDC");
	leg->SetFillColor(0);
	leg->SetTextSize(0.029);
	leg->SetTextFont(42);
        for(uint i=0; i<vs.size(); i++)
		leg->AddEntry(vh.at(i),(vs.at(i)).c_str(),s.c_str());
        return leg;        
}

void PlotMaker::StylePtRatioHisto(vector<TH1F*> v1){
        for(uint i=0; i<v1.size(); i++){
                v1.at(i)->GetYaxis()->SetTitleOffset(1.70);
		v1.at(i)->GetXaxis()->SetTitleOffset(1.30);
		v1.at(i)->SetLineColor(colorsMVA.at(i));
		v1.at(i)->SetLineWidth(2);
	}
}

vector<vector<TH1F*> > PlotMaker::ScaleHistograms(vector<vector<TH1F*> > vvh){
	vector<TH1F*> tmpH;
	vector<vector<TH1F*> > vvh2;
	for(uint i=0; i < vvh.size(); i++){
                tmpH.clear();
		for(uint j=0; j < vvh.at(i).size(); j++){
		vvh.at(i).at(j)->Scale(1. / vvh.at(i).at(j)->Integral() );
		tmpH.push_back( vvh.at(i).at(j) ) ;
		}
		vvh2.push_back(tmpH);
	}
return vvh2;
}



void PlotMaker::DrawPtRatioHisto(string Cname){
	vector< vector<TH1F*> > histos = RetrievePtRatioHisto();
	vector<TH1F*> histoScaled;
        vector<TCanvas*> canvas = CreateCanvases( (int)histos.size(), Cname );
        TPaveText* l1 = GetCMSTitle();

	for(uint i=0;i<histos.size();i++){
                histoScaled.clear();
		canvas[i]->cd();
                TPaveText* pavet = CreatePaveTextDM( 0.21, 0.85, 0.31, 0.92, (dmLabels.at(i)+" decay mode").c_str() );
                StylePtRatioHisto( histos.at(i) );
		histos.at(i).at(0)->Scale( 1./ histos.at(i).at(0)->Integral() );
		if( m_File.find("ZpTT") != string::npos)
			histos.at(i).at(0)->GetYaxis()->SetRangeUser(0.,0.18);
		else if( m_File.find("ZTT") != string::npos){
			histos.at(0).at(0)->GetYaxis()->SetRangeUser(0.,0.4);
			histos.at(1).at(0)->GetYaxis()->SetRangeUser(0.,0.2);
			histos.at(2).at(0)->GetYaxis()->SetRangeUser(0.,0.4);
                }
                RebinHisto( histos.at(i).at(0), 2)->Draw();
		for(uint j=0; j<histos.at(i).size() ;j++){
                        if(j != 0){
				histos.at(i).at(j)->Scale( 1./ histos.at(i).at(j)->Integral() );
				RebinHisto(histos.at(i).at(j),2)->Draw("same");
			}
                }
		l1->Draw("same");
                vector<double> posLegPtRatio;
                posLegPtRatio.push_back(0.22); posLegPtRatio.push_back(0.67); posLegPtRatio.push_back(0.52); posLegPtRatio.push_back(0.84);
                SetHistoLegend( histos.at(i), stringLegPtRatio, posLegPtRatio, typeLeg.at(0) )->Draw("same");
                pavet->Draw("same");
                canvas[i]->SaveAs( ((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+m_Folder+".pdf").c_str() );
                canvas[i]->SaveAs( ((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+m_Folder+".png").c_str() );
                canvas[i]->SaveAs( ((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+m_Folder+".eps").c_str() );
	}
}


TH1F* PlotMaker::DivideHisto(TH1F* h1, TH1F* h2){
        TH1F *ratio=(TH1F*)h1->Clone();
        ratio->Sumw2();
        ratio->Divide(h2);
        return ratio;            
}

vector<TH1F*> PlotMaker::GetHistosForEff(string s, vector<string> vs, bool useRef){
	vector< TH1F*> histosForEff;
        if(useRef){
                histosForEff.push_back(  (TH1F*)file->Get( (dir+hpsPFTau+s+"/"+vs.at(3)).c_str() ) );
		histosForEff.push_back(  (TH1F*)file->Get( (dir+hpsPFTau+s+"/"+vs.at(2)).c_str() ) );
		histosForEff.push_back(  (TH1F*)file->Get( (dir+hpsPFTau+s+"/"+vs.at(0)).c_str() ) );
        }else{
		histosForEff.push_back(  (TH1F*)file->Get( (dir+hpsPFTau+s+"/"+s+vs.at(3)).c_str() ) );
		histosForEff.push_back(  (TH1F*)file->Get( (dir+hpsPFTau+s+"/"+s+vs.at(2)).c_str() ) );
		histosForEff.push_back(  (TH1F*)file->Get( (dir+hpsPFTau+s+"/"+s+vs.at(0)).c_str() ) );
        }
        
        return histosForEff;
}

void PlotMaker::StyleEffHisto( vector<TH1F *> vh, string s, string Cname ){
	for(uint i=0; i<vh.size(); i++){
                
		vh.at(i)->GetXaxis()->SetTitleOffset(1.30);
		if(s=="pu") 
			vh.at(0)->GetXaxis()->SetTitle("N_{vtx}");
		if(m_File.find("ZpTT")!=string::npos || m_File.find("ZTT")!=string::npos){
			if(s=="pt") 
				vh.at(0)->GetXaxis()->SetTitle("generated P_{T}^{#tau} [GeV/c]");
			if(s=="eta") 
				vh.at(0)->GetXaxis()->SetTitle("generated #eta_{#tau}");
			vh.at(i)->GetYaxis()->SetTitle("Expected #tau efficiency");
		}else if(m_File.find("QCD")!=string::npos || m_File.find("WJETS")!=string::npos){
			if(s=="pt") 
				vh.at(0)->GetXaxis()->SetTitle("P_{T}^{jet} [GeV/c]");
			if(s=="eta") 
				vh.at(0)->GetXaxis()->SetTitle("#eta_{jet}");
			vh.at(i)->GetYaxis()->SetTitle("Expected jet #rightarrow #tau fake rate");
		}else if(m_File.find("ZEE")!=string::npos || m_File.find("ZMM")!=string::npos){
			if(s=="pt") 
				vh.at(0)->GetXaxis()->SetTitle("generated P_{T} [GeV/c]");
			if(s=="eta") 
				vh.at(0)->GetXaxis()->SetTitle("generated #eta");
			vh.at(i)->GetYaxis()->SetTitle("Expected lepton #rightarrow #tau fake rate");
                }
		
                //Set Range
		if(m_File.find("ZTT")!=string::npos || m_File.find("ZpTT")!=string::npos){
			if(Cname.find("Muon")!=string::npos){
				vh.at(i)->GetYaxis()->SetNdivisions(508);
				vh.at(i)->GetYaxis()->SetRangeUser(0.7,1.2);
			}
			if(Cname.find("Ele")!=string::npos){
				vh.at(i)->GetYaxis()->SetRangeUser(0.2,1.4);
			}		
			if((Cname.find("MVA")!=string::npos || Cname.find("Iso")!=string::npos) && Cname.find("Ele")==string::npos && Cname.find("Muon")==string::npos){
				vh.at(i)->GetYaxis()->SetRangeUser(0.,1.1);
			}		
		}
		else if(m_File.find("ZEE")!=string::npos)
			vh.at(i)->GetYaxis()->SetRangeUser(0.0001,10.);
		else if(m_File.find("ZMM")!=string::npos)
			vh.at(i)->GetYaxis()->SetRangeUser(0.00001,0.1);

                if(m_File.find("QCD")!=string::npos || m_File.find("WJETS")!=string::npos)
			vh.at(i)->GetYaxis()->SetRangeUser(0.0000001,1);
		vh.at(i)->GetYaxis()->SetTitleOffset(1.50);
		if(Cname.find("MVA")!=string::npos){
			vh.at(i)->SetMarkerStyle( markersMVA.at(i) );
			vh.at(i)->SetMarkerColor( colorsMVA.at(i) );
			vh.at(i)->SetLineColor( colorsMVA.at(i) );
		}else{
			vh.at(i)->SetMarkerStyle( markersCB.at(i) );
			vh.at(i)->SetMarkerColor( colorsCB.at(i) );
			vh.at(i)->SetLineColor( colorsCB.at(i) );
		}
                vh.at(i)->SetMarkerSize( 1.2 );
		vh.at(i)->SetLineWidth( 2 );
	}
}

vector<vector< TH1F* > > PlotMaker::GetEffSignal(vector<string> vsHN, vector<string> vsN, string HD, vector<string> vsD, int Rebin, bool useRef){
	vector<vector <TH1F*> > efficiency;
	vector <TH1F*> effpt;
	vector <TH1F*> effpu;
	vector <TH1F*> effeta;

	for(uint i=0; i<vsHN.size(); i++){
          	effpt.push_back(  DivideHisto( RebinHisto( GetHistosForEff( vsHN.at(i), vsN).at(0), Rebin), 
					       RebinHisto( (GetHistosForEff( HD, vsD, useRef )).at(0), Rebin) ));
		effpu.push_back(  DivideHisto( RebinHisto( GetHistosForEff( vsHN.at(i), vsN ).at(1), 4),     
				               RebinHisto( (GetHistosForEff( HD, vsD, useRef )).at(1), 4) ));
		effeta.push_back( DivideHisto( RebinHisto( GetHistosForEff( vsHN.at(i), vsN ).at(2), 6),     
				               RebinHisto( (GetHistosForEff( HD, vsD, useRef )).at(2), 6) ));
	}
	efficiency.push_back( effpt );
	efficiency.push_back( effpu );
	efficiency.push_back( effeta );

	return efficiency;
}

/*vector<vector< TH1F* > > PlotMaker::GetCompositeEffSignal(vector<string> vsHN, vector<string> vsN, string HD, vector<string> vsD, int Rebin, bool useRef){

	vector<vector <TH1F*> > efficiencyLoose3Hits = GetEffSignal( iso3Hits, forEff, refCollection, forEffRef, Rebin, true );
	vector< vector< TH1F* > > effHistos = GetEffSignal( vsHN, vsN, HD, vsD, Rebin, useRef );
	vector<vector <TH1F*> > efficiency;
	vector <TH1F*> effpt;
	vector <TH1F*> effpu;

        cout << "eff size " << effHistos.size() << endl;
  
	for(uint j=0; j<effHistos.at(0).size(); j++){
		effpt.push_back( MultiplyHistos(effHistos.at(0).at(j),efficiencyLoose3Hits.at(0).at(0)) ) ;

       }    
	for(uint j=0; j<effHistos.at(1).size(); j++)
		effpu.push_back( MultiplyHistos(effHistos.at(1).at(j),efficiencyLoose3Hits.at(1).at(0)) );
	efficiency.push_back( effpt );
	efficiency.push_back( effpu );

	return efficiency;
}*/

void PlotMaker::DrawEffSignal(vector<string> vsHN, vector<string> vsN, string HD, vector<string> vsD, vector<string> vsLeg, vector<double> vd, string Cname, int Rebin, bool useRef){

        vector< vector< TH1F* > > effHistos;
	effHistos = GetEffSignal( vsHN, vsN, HD, vsD, Rebin, useRef );
	vector<TCanvas*> canvas =  CreateCanvases( (int)effHistos.size(), Cname );
        vector<string> s;
        s.push_back("pt"); s.push_back("pu"); s.push_back("eta");
	StyleEffHisto( effHistos.at(0), s.at(0), Cname );
	StyleEffHisto( effHistos.at(1), s.at(1), Cname );
	StyleEffHisto( effHistos.at(2), s.at(2), Cname );

        TPaveText* l1 = GetCMSTitle();

	for(uint i=0; i<effHistos.size(); i++){
                canvas[i]->cd();
		for(uint j=0; j<effHistos.at(i).size(); j++){
                        
                        //if(File.find("ZTT") != string::npos && vsHN.at(0).find("MVA3oldDMwLT") != string::npos ) 
			//		effHistos.at(i).at(j)->GetYaxis()->SetRangeUser(0.,0.8);
                        //if(vsHN.at(0).find("Muon") != string::npos ) 
			//		effHistos.at(i).at(j)->GetYaxis()->SetRangeUser(0.6,1.2);
                        //if(vsHN.at(0).find("Electron") != string::npos ) 
		        //			effHistos.at(i).at(j)->GetYaxis()->SetRangeUser(0.,1.2);
                        
			if(j==0) effHistos.at(i).at(j)->Draw("E1P");
			else effHistos.at(i).at(j)->Draw("same E1P");
                        //SetOptimalRange( effHistos.at(i).at(j) );
		}
		        l1->Draw("same");
			SetHistoLegend( effHistos.at(i), vsLeg, vd, typeLeg.at(1) )->Draw("same");
                        if(m_File.find("QCD") != string::npos || m_File.find("WJETS") != string::npos || m_File.find("ZEE") != string::npos || m_File.find("ZMM") != string::npos){
				canvas[i]->SetLogy(1);
				canvas[i]->SaveAs(((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+m_Folder+"_FakeRate.pdf").c_str());
				canvas[i]->SaveAs(((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+m_Folder+"_FakeRate.png").c_str());
				canvas[i]->SaveAs(((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+m_Folder+"_FakeRate.eps").c_str());
                        }else{
				canvas[i]->SaveAs(((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+m_Folder+"_Efficiency.pdf").c_str());
				canvas[i]->SaveAs(((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+m_Folder+"_Efficiency.png").c_str());
				canvas[i]->SaveAs(((string)(m_Folder+"/"+canvas[i]->GetName())+"_"+m_Folder+"_Efficiency.eps").c_str());
                        }

	}
}

void PlotMaker::FinalDrawEffSignal(int Rebin){
	vector<double> posLegEffLeptons;
	posLegEffLeptons.push_back(0.40); posLegEffLeptons.push_back(0.76); posLegEffLeptons.push_back(0.74); posLegEffLeptons.push_back(0.91);
	vector<double> posLegEff3Hits;
	posLegEff3Hits.push_back(0.56); posLegEff3Hits.push_back(0.77); posLegEff3Hits.push_back(0.90); posLegEff3Hits.push_back(0.91);
   	vector<double> posLegEffMVA;
	posLegEffMVA.push_back(0.55); posLegEffMVA.push_back(0.71); posLegEffMVA.push_back(0.89); posLegEffMVA.push_back(0.91);
	vector<double> posLegEff3HitsQCD;
	posLegEff3HitsQCD.push_back(0.58); posLegEff3HitsQCD.push_back(0.70); posLegEff3HitsQCD.push_back(0.92); posLegEff3HitsQCD.push_back(0.91);
	vector<double> posLegEffMVAQCD;
	posLegEffMVAQCD.push_back(0.58); posLegEffMVAQCD.push_back(0.70); posLegEffMVAQCD.push_back(0.92); posLegEffMVAQCD.push_back(0.91);
        if(m_File.find("QCD") != string::npos || m_File.find("WJETS") != string::npos){ 
		DrawEffSignal( iso3Hits, forEff, refCollection,  forEffRef, stringLegEff3Hits, posLegEff3HitsQCD, "Iso",  Rebin, true);
		DrawEffSignal( MVAoldLT, forEff, refCollection,  forEffRef, stringLegEffMVAIso, posLegEffMVAQCD,   "MVA",  Rebin, true);
        }else if(m_File.find("ZEE") != string::npos){
		DrawEffSignal( eleRej, forEff, refCollection,  forEffRef,   stringLegEffEleRej, posLegEffLeptons, "EleCutB",  Rebin, true);
		DrawEffSignal( MVAeleRej, forEff, refCollection, forEffRef, stringLegEffMVAEleRej, posLegEffLeptons, "EleMVA",  Rebin, true);
        }else if(m_File.find("ZMM") != string::npos){
		DrawEffSignal( muonRej,  forEff, refCollection, forEffRef, stringLegEffMuonRej, posLegEffLeptons, "MuonCutB", Rebin, true);
		DrawEffSignal( MVAmuonRej,  forEff, refCollection, forEffRef, stringLegEffMVAMuonRej, posLegEffLeptons, "MuonMVA", Rebin, true);
        }
        else{
		DrawEffSignal( iso3Hits, forEff, refCollection,  forEffRef, stringLegEff3Hits, posLegEff3Hits, "Iso",  Rebin, true);
		DrawEffSignal( MVAoldLT, forEff, refCollection,  forEffRef, stringLegEffMVAIso, posLegEffMVA,   "MVA",  Rebin, true);
		DrawEffSignal( eleRej,   forEff, iso3Hits.at(0),  forEff,   stringLegEffEleRej, posLegEffLeptons, "EleCutB",  Rebin, false);
		DrawEffSignal( muonRej,  forEff, iso3Hits.at(0),  forEff,   stringLegEffMuonRej, posLegEffLeptons, "MuonCutB", Rebin, false);
		DrawEffSignal( MVAeleRej,   forEff, iso3Hits.at(0),  forEff,   stringLegEffMVAEleRej, posLegEffLeptons, "EleMVA",  Rebin, false);
		DrawEffSignal( MVAmuonRej,  forEff, iso3Hits.at(0),  forEff,   stringLegEffMVAMuonRej, posLegEffLeptons, "MuonMVA", Rebin, false);
	}
}

//void PlotMaker::GetEffANDFakeRate(string File, int Rebin){
/*void PlotMaker::GetEffANDFakeRate(string File, vector<string> vsHN, vector<string> vsN, string HD, vector<string> vsD, int Rebin, bool useRef){
        vector< vector< TH1F* > > effHistos;
			cout << "sono qui prima di effHistos" << endl ;
	effHistos = GetEffSignal( vsHN, vsN, HD, vsD, true );
			cout << "dopo avere riempito effHistos" << endl ;
	for(uint i=0; i<effHistos.size(); i++){
		for(uint j=0; j<effHistos.at(i).size(); j++){
			cout << "sono qui" << endl ;
			cout << effHistos.at(i).at(j)->GetBinContent(1) << endl ;
		}
	}
}*/
     
//void PlotMaker::MakeTables(string File){
//	GetEffANDFakeRate( File, MVAoldLT, forEff1bin, refCollection, forEffRef1bin, true );

//}        
	//*********************************************************************************************************
	//Signal EFFICIENCIES
	//*********************************************************************************************************
        //MVA ISO EFFICIENCY
        //cout << stringLegEff3Hits.at(0) << " & " << stringLegEff3Hits.at(1) << " & " << stringLegEff3Hits.at(2) << " \\hline" << endl;
        //cout << effHistos.at(0).at(0)->GetBinContent(1) << " & " << effHistos.at(0).at(1)->GetBinContent(1) << " & " << effHistos.at(0).at(2)->GetBinContent(1) << " \\hline" << endl;
        //cout << effHistos.at(1).at(0)->GetBinContent(1) << " & " << effHistos.at(1).at(1)->GetBinContent(1) << " & " << effHistos.at(1).at(2)->GetBinContent(1) << " \\hline" << endl;
        //MVA ISO EFFICIENCY
        /*effHistos.clear();
	effHistos = GetEffSignal( MVAoldLT, forEff1bin, refCollection, forEffRef1bin, Rebin, true );
        cout << " \\hline " << endl;
        cout << stringLegEffMVAIso.at(0) << " & " << stringLegEffMVAIso.at(1) << " & " << stringLegEffMVAIso.at(2) << " & " << stringLegEffMVAIso.at(3) << " & " << stringLegEffMVAIso.at(4) << " & " << stringLegEffMVAIso.at(5) << " \\hline" << endl;
        cout << effHistos.at(0).at(0)->GetBinContent(1) << " & " << effHistos.at(0).at(1)->GetBinContent(1) << " & " << effHistos.at(0).at(2)->GetBinContent(1) << " & " << effHistos.at(0).at(3)->GetBinContent(1) << " & " << effHistos.at(0).at(4)->GetBinContent(1) << " & " << effHistos.at(0).at(5)->GetBinContent(1) << " \\hline" << endl;
        cout << effHistos.at(1).at(0)->GetBinContent(1) << " & " << effHistos.at(1).at(1)->GetBinContent(1) << " & " << effHistos.at(1).at(2)->GetBinContent(1) << " & " << effHistos.at(1).at(3)->GetBinContent(1) << " & " << effHistos.at(1).at(4)->GetBinContent(1) << " & " << effHistos.at(1).at(5)->GetBinContent(1) << " \\hline" << endl;
        //Anti-ELE EFFICIENCY
        effHistos.clear();
	effHistos = GetEffSignal( eleRej, forEff1bin, iso3Hits.at(0), forEff1bin, Rebin, false );
        cout << stringLegEffEleRej.at(0) << " & " << stringLegEffEleRej.at(1) << " & " << stringLegEffEleRej.at(2) << " \\hline" << endl;
        cout << effHistos.at(0).at(0)->GetBinContent(1) << " & " << effHistos.at(0).at(1)->GetBinContent(1) << " & " << effHistos.at(0).at(2)->GetBinContent(1) << " \\hline" << endl;
        cout << effHistos.at(1).at(0)->GetBinContent(1) << " & " << effHistos.at(1).at(1)->GetBinContent(1) << " & " << effHistos.at(1).at(2)->GetBinContent(1) << " \\hline" << endl;
        //Anti-MUON EFFICIENCY
        effHistos.clear();
	effHistos = GetEffSignal( muonRej, forEff1bin, iso3Hits.at(0), forEff1bin, Rebin, false );
        cout << stringLegEffMuonRej.at(0) << " & " << stringLegEffMuonRej.at(1) << " \\hline" << endl;
        cout << effHistos.at(0).at(0)->GetBinContent(1) << " & " << effHistos.at(0).at(1)->GetBinContent(1) << " \\hline" << endl;
        cout << effHistos.at(1).at(0)->GetBinContent(1) << " & " << effHistos.at(1).at(1)->GetBinContent(1) << " \\hline" << endl;
        //Anti-ELE MVA EFFICIENCY
        effHistos.clear();
	effHistos = GetEffSignal( MVAeleRej, forEff1bin, iso3Hits.at(0), forEff1bin, Rebin, false );
        cout << stringLegEffMVAEleRej.at(0) << " & " << stringLegEffMVAEleRej.at(1) << " & " << stringLegEffMVAEleRej.at(2) << " & " << stringLegEffMVAEleRej.at(3) << " & " << stringLegEffMVAEleRej.at(4) <<" \\hline" << endl;
        cout << effHistos.at(0).at(0)->GetBinContent(1) << " & " << effHistos.at(0).at(1)->GetBinContent(1) << " & " << effHistos.at(0).at(2)->GetBinContent(1) << " & " << effHistos.at(0).at(3)->GetBinContent(1) << " & " << effHistos.at(0).at(4)->GetBinContent(1) << " \\hline" << endl;
        cout << effHistos.at(1).at(0)->GetBinContent(1) << " & " << effHistos.at(1).at(1)->GetBinContent(1) << " & " << effHistos.at(1).at(2)->GetBinContent(1) << " & " << effHistos.at(1).at(3)->GetBinContent(1) << " & " << effHistos.at(1).at(4)->GetBinContent(1) << " \\hline" << endl;
        //Anti-MUON MVA EFFICIENCY
        effHistos.clear();
	effHistos = GetEffSignal( MVAmuonRej, forEff1bin, iso3Hits.at(0), forEff1bin, Rebin, false );
        cout << stringLegEffMVAMuonRej.at(0) << " & " << stringLegEffMVAMuonRej.at(1) << " & " << stringLegEffMVAMuonRej.at(2) << " \\hline" << endl;
        cout << effHistos.at(0).at(0)->GetBinContent(1) << " & " << effHistos.at(0).at(1)->GetBinContent(1) << " & " << effHistos.at(0).at(2)->GetBinContent(1) << " \\hline" << endl;
        cout << effHistos.at(1).at(0)->GetBinContent(1) << " & " << effHistos.at(1).at(1)->GetBinContent(1) << " & " << effHistos.at(1).at(2)->GetBinContent(1) << " \\hline" << endl;
        */


