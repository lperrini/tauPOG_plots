#include "PlotMaker.h"

PlotMaker::PlotMaker(){
}

PlotMaker::PlotMaker(string File,
                     string EvT,
                     string Eff,
                     int    Rebin){
	gROOT->ProcessLine(".L tdrstyle.C");
	gROOT->ProcessLine("setTDRStyle()");

    m_File    = File;
    m_EvT     = EvT;
    m_Eff     = Eff;
    m_Rebin   = Rebin;
}

void PlotMaker::ReadFiles(string EvT){

    file = new TFile( (m_File).c_str() );
    discrList = GetDiscriminators(EvT,file);
    //PrintDiscriminators(discrList);
}
    
//*********************************************************************************************************
//Get the list of discriminators
//*********************************************************************************************************
vector<string> PlotMaker::GetDiscriminators(string EvT,
                                            TFile *f){


    string prefix   = "hpsPFTauProducer"+EvT+"_hpsPFTauDiscriminationBy";
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
void PlotMaker::Initialize(string EvT){

	dmLabels.clear();
	dmHistos.clear();
	iso3Hits.clear();
	MVAoldLT.clear();
        eleRej.clear();
        muonRej.clear();
        MVAeleRej.clear();
        MVAmuonRej.clear();
        forEff.clear();
        forEff.clear();
        forEffRef.clear();
        forEffRefFull.clear();
        dir = ""; hpsPFTau = ""; refCollection = "";  
	
	dir       	 	 = "DQMData/RecoTauV/";
	hpsPFTau  	 	 = "hpsPFTauProducer"+EvT+"_";
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
        MVAmuonRej.push_back("hpsPFTauDiscriminationByMVALooseMuonRejection");  //to be changed 
        MVAmuonRej.push_back("hpsPFTauDiscriminationByMVAMediumMuonRejection"); //to be changed
        MVAmuonRej.push_back("hpsPFTauDiscriminationByMVATightMuonRejection");  //to be changed
        MVAeleRej.push_back("hpsPFTauDiscriminationByMVA5VLooseElectronRejection");     //to be changed
        MVAeleRej.push_back("hpsPFTauDiscriminationByMVA5LooseElectronRejection");     //to be changed
        MVAeleRej.push_back("hpsPFTauDiscriminationByMVA5MediumElectronRejection");    //to be changed
        MVAeleRej.push_back("hpsPFTauDiscriminationByMVATightElectronRejection");     //to be changed
        MVAeleRej.push_back("hpsPFTauDiscriminationByMVAVTightElectronRejection");     //to be changed

        forEff.push_back("_vs_etaTauVisible");
        forEff.push_back("_vs_phiTauVisible");
        forEff.push_back("_vs_pileupTauVisible");
        forEff.push_back("_vs_ptTauVisible");
        forEffRef.push_back("nRef_Taus_vs_etaTauVisible");
        forEffRef.push_back("nRef_Taus_vs_phiTauVisible");
        forEffRef.push_back("nRef_Taus_vs_pileupTauVisible");
        forEffRef.push_back("nRef_Taus_vs_ptTauVisible");
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
        stringLegEff3Hits.push_back("HPS #delta#beta 3-hits Loose");
        stringLegEff3Hits.push_back("HPS #delta#beta 3-hits Medium");
        stringLegEff3Hits.push_back("HPS #delta#beta 3-hits Tight");
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
        colors.clear();
        colors.push_back(kRed); colors.push_back(kBlue); colors.push_back(kGreen+1); 
        colors.push_back(kCyan-2); colors.push_back(kMagenta+2); colors.push_back(kOrange);
        markers.clear();
        markers.push_back(20); markers.push_back(21); markers.push_back(22); 
        markers.push_back(24); markers.push_back(23); markers.push_back(25);
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

void PlotMaker::DrawScatterHisto(string Cname, string File){
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
                finalScatter.at(i)->Draw("TEXT");    
		l1->Draw("same");
		canvas[i]->SaveAs(((string)(canvas[i]->GetName())+"_"+forName[i]+"_"+File+"_.pdf").c_str());
		canvas[i]->SaveAs(((string)(canvas[i]->GetName())+"_"+forName[i]+"_"+File+"_.png").c_str());
		canvas[i]->SaveAs(((string)(canvas[i]->GetName())+"_"+forName[i]+"_"+File+"_.eps").c_str());
	}
}

pair<TH1F*,TH1F*> PlotMaker::RetrieveGenHisto(string File){
	pair<TH1F*, TH1F*> gHistos;
	TH1F* histoCheck = (TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(3) ).c_str() );
	double min = histoCheck->GetXaxis()->GetXmin();
	double max = histoCheck->GetXaxis()->GetXmax();

	if( File.find("ZpTT") != string::npos ){
		if( (max - min) > 150. ){
			gHistos = make_pair(  RebinHisto( (TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRefFull.at(0) ).c_str() ), 2), 
				(TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(0) ).c_str() ) );
		}else{
			gHistos = make_pair(  RebinHisto( (TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(3) ).c_str() ), 2), 
				(TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(0) ).c_str() ) );
		}
	}else if( File.find("ZTT") != string::npos ){
                        cout << "je suis ici " << endl;
			gHistos = make_pair(    (TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(3) ).c_str() ), 
				(TH1F*)file->Get( (dir+hpsPFTau+refCollection+forEffRef.at(0) ).c_str() ) );
			}
	return gHistos;
}

void PlotMaker::StyleGenHisto(TH1F* h1, TH1F* h2){
     h1->SetMarkerStyle( markers.at(0) );
     h2->SetMarkerStyle( markers.at(0) );
     h1->SetLineWidth( 2 );
     h2->SetLineWidth( 2 );
     h1->SetMarkerColor( colors.at(0) );
     h2->SetMarkerColor( colors.at(0) );
     h1->SetLineColor( colors.at(0) );
     h2->SetLineColor( colors.at(0) );
     h1->GetYaxis()->SetTitle( "Relative yield" );
     h2->GetYaxis()->SetTitle( "Relative yield" );
     h1->GetYaxis()->SetTitleOffset(1.70);
     h2->GetYaxis()->SetTitleOffset(1.80);
     h1->GetXaxis()->SetTitle( "generated p_{T}^{#tau} [GeV/c]" );
     h2->GetXaxis()->SetTitle( "generated #eta_{#tau}" );
     h1->GetXaxis()->SetTitleOffset(1.30);
     h2->GetXaxis()->SetTitleOffset(1.30);
}

void PlotMaker::DrawGenHisto(string File){
	TH1F *h1 = (TH1F*)RetrieveGenHisto(File).first;
	TH1F *h2 = (TH1F*)RetrieveGenHisto(File).second;
        TPaveText* l1 = GetCMSTitle();
        StyleGenHisto(h1,h2);
	TCanvas *c1 = new TCanvas();
        c1->cd();
        h1->Draw("HE1");    
        l1->Draw("same");
        c1->SaveAs(("genpt_"+File+".pdf").c_str());
        c1->SaveAs(("genpt_"+File+".png").c_str());
        c1->SaveAs(("genpt_"+File+".eps").c_str());
	TCanvas *c2 = new TCanvas();
        c2->cd();
	h2->Draw("HE1");    
        l1->Draw("same");
        c2->SaveAs(("geneta_"+File+".pdf").c_str());
        c2->SaveAs(("geneta_"+File+".png").c_str());
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
	std::cout << "multiplyin HISTOS "<< std::endl;
	TH1F *tmp1 = h1;
	TH1F *tmp2 = h2;
	TH1F* multiply;
	std::cout<< "bin 1 :" << tmp1->GetBinContent(3)<< " : "<< tmp2->GetBinContent(3)<< " : ";
	tmp1->Multiply(tmp2);
        multiply = tmp1;
	std::cout<<multiply->GetBinContent(3)<< std::endl;
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
			cout << finalHisto.at(i).at(j)->Integral() << endl;
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
		v1.at(i)->SetLineColor(colors.at(i));
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


int PlotMaker::FindMaximum(vector<TH1F*> vh){
	
        //vh.at(0)->Scale( 1. / vh.at(0)->Integral() );
	double ymaximum = vh.at(0)->GetMaximum();
	int index;
	//cout << "vector size: " << vh.size() << endl;
	for(uint i=0; i < vh.size(); i++){
		//vh.at(i)->Scale( 1./ vh.at(i)->Integral() );
		  //      cout << "ymaximum: " << ymaximum << " max of the histo " << vh.at(i)->GetMaximum() <<  endl;
		if (ymaximum < vh.at(i)->GetMaximum()){
			ymaximum = vh.at(i)->GetMaximum();
			index = i;
		}
	}
	return index;
}

void PlotMaker::DrawPtRatioHisto(string Cname, string File){
	vector< vector<TH1F*> > histos = RetrievePtRatioHisto();
	vector<TH1F*> histoScaled;
        vector<TCanvas*> canvas = CreateCanvases( (int)histos.size(), Cname );
        TPaveText* l1 = GetCMSTitle();

        int index;
	for(uint i=0;i<histos.size();i++){
                histoScaled.clear();
		canvas[i]->cd();
                TPaveText* pavet = CreatePaveTextDM( 0.21, 0.85, 0.31, 0.92, (dmLabels.at(i)+" decay mode").c_str() );
                StylePtRatioHisto( histos.at(i) );
		histos.at(i).at(0)->Scale( 1./ histos.at(i).at(0)->Integral() );
		if( File.find("ZpTT") != string::npos)
			histos.at(i).at(0)->GetYaxis()->SetRangeUser(0.,0.18);
		else if( File.find("ZTT") != string::npos){
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
                canvas[i]->SaveAs( ((string)(canvas[i]->GetName())+"_"+File+"_.pdf").c_str() );
                canvas[i]->SaveAs( ((string)(canvas[i]->GetName())+"_"+File+"_.png").c_str() );
                canvas[i]->SaveAs( ((string)(canvas[i]->GetName())+"_"+File+"_.eps").c_str() );
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
        }else{
		histosForEff.push_back(  (TH1F*)file->Get( (dir+hpsPFTau+s+"/"+s+vs.at(3)).c_str() ) );
		histosForEff.push_back(  (TH1F*)file->Get( (dir+hpsPFTau+s+"/"+s+vs.at(2)).c_str() ) );
        }
        
        return histosForEff;
}

void PlotMaker::StyleEffHisto( vector<TH1F *> vh, string s ){
	for(uint i=0; i<vh.size(); i++){
                if(s=="pt") vh.at(0)->GetXaxis()->SetTitle("generated P_{T}^{#tau} [GeV/c]");
                else if(s=="pu") vh.at(0)->GetXaxis()->SetTitle("N_{vtx}");
                vh.at(i)->GetXaxis()->SetTitleOffset(1.30);
		vh.at(i)->GetYaxis()->SetTitle("Expected #tau efficiency");
                vh.at(i)->GetYaxis()->SetTitleOffset(1.50);
		vh.at(i)->GetYaxis()->SetRangeUser(0,1.);
		vh.at(i)->SetLineColor( colors.at(i) );
		vh.at(i)->SetMarkerStyle( markers.at(i) );
		vh.at(i)->SetMarkerSize( 1.2 );
		vh.at(i)->SetLineWidth( 2 );
		vh.at(i)->SetMarkerColor( colors.at(i) );
	}
}

vector<vector< TH1F* > > PlotMaker::GetEffSignal(vector<string> vsHN, vector<string> vsN, string HD, vector<string> vsD, int Rebin, bool useRef){
	vector<vector <TH1F*> > efficiency;
	vector <TH1F*> effpt;
	vector <TH1F*> effpu;

	for(uint i=0; i<vsHN.size(); i++){
		effpt.push_back( DivideHisto( RebinHisto( GetHistosForEff( vsHN.at(i), vsN).at(0), Rebin), 
					      RebinHisto( (GetHistosForEff( HD, vsD, useRef )).at(0), Rebin) ));
		effpu.push_back( DivideHisto( RebinHisto( GetHistosForEff( vsHN.at(i), vsN ).at(1), 4),     
				              RebinHisto( (GetHistosForEff( HD, vsD, useRef )).at(1), 4) ));
	}
	efficiency.push_back( effpt );
	efficiency.push_back( effpu );

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

void PlotMaker::DrawEffSignal(string File, vector<string> vsHN, vector<string> vsN, string HD, vector<string> vsD, vector<string> vsLeg, vector<double> vd, string Cname, int Rebin, bool useRef, bool range){

        vector< vector< TH1F* > > effHistos;

	effHistos = GetEffSignal( vsHN, vsN, HD, vsD, Rebin, useRef );
	vector<TCanvas*> canvas =  CreateCanvases( (int)effHistos.size(), Cname );
        vector<string> s;
        s.push_back("pt"); s.push_back("pu");
	StyleEffHisto( effHistos.at(0), s.at(0) );
	StyleEffHisto( effHistos.at(1), s.at(1) );
        TPaveText* l1 = GetCMSTitle();

	for(uint i=0; i<effHistos.size(); i++){
                canvas[i]->cd();
		for(uint j=0; j<effHistos.at(i).size(); j++){
                        
                        //cout << effHistos.at(i).at(j)->GetNbinsX() << endl;
                        if(File.find("ZTT") != string::npos && vsHN.at(0).find("MVA3oldDMwLT") != string::npos ) 
					effHistos.at(i).at(j)->GetYaxis()->SetRangeUser(0.,0.8);
			if(range) 
					effHistos.at(i).at(j)->GetYaxis()->SetRangeUser(0.4,1.2);
                        
			if(j==0) effHistos.at(i).at(j)->Draw("E1P");
			else effHistos.at(i).at(j)->Draw("same E1P");
                        //SetOptimalRange( effHistos.at(i).at(j) );
		}
		        l1->Draw("same");
			canvas[i]->SaveAs(((string)(canvas[i]->GetName())+"_"+File+"_Eff_.pdf").c_str());
			canvas[i]->SaveAs(((string)(canvas[i]->GetName())+"_"+File+"_Eff_.png").c_str());
			canvas[i]->SaveAs(((string)(canvas[i]->GetName())+"_"+File+"_Eff_.eps").c_str());
			//canvas[i]->SaveAs( ((string)("Eff_"+(canvas[i]->GetName())+"_"+File+".eps")).c_str() );
			//canvas[i]->SaveAs( ((string)("Eff_"+(canvas[i]->GetName())+"_"+File+".pdf")).c_str() );
			//canvas[i]->SaveAs( ((string)("Eff_"+(canvas[i]->GetName())+"_"+File+".png")).c_str() );

			SetHistoLegend( effHistos.at(i), vsLeg, vd, typeLeg.at(1) )->Draw("same");
	}
}

void PlotMaker::FinalDrawEffSignal(string File, int Rebin){
	vector<double> posLegEffLeptons;
	posLegEffLeptons.push_back(0.39); posLegEffLeptons.push_back(0.21); posLegEffLeptons.push_back(0.73); posLegEffLeptons.push_back(0.32);
	vector<double> posLegEff3Hits;
	posLegEff3Hits.push_back(0.58); posLegEff3Hits.push_back(0.21); posLegEff3Hits.push_back(0.92); posLegEff3Hits.push_back(0.32);
	vector<double> posLegEffMVA;
	posLegEffMVA.push_back(0.58); posLegEffMVA.push_back(0.18); posLegEffMVA.push_back(0.92); posLegEffMVA.push_back(0.38);
	DrawEffSignal( File, iso3Hits, forEff, refCollection,  forEffRef, stringLegEff3Hits, posLegEff3Hits, "Iso",  Rebin, true);
	DrawEffSignal( File, MVAoldLT, forEff, refCollection,  forEffRef, stringLegEffMVAIso, posLegEffMVA,   "MVA",  Rebin, true);
	DrawEffSignal( File, eleRej,   forEff, iso3Hits.at(0),  forEff,   stringLegEffEleRej, posLegEffLeptons, "EleCutB",  Rebin, false, true);
	DrawEffSignal( File, muonRej,  forEff, iso3Hits.at(0),  forEff,   stringLegEffMuonRej, posLegEffLeptons, "MuonCutB", Rebin, false, true);
}


