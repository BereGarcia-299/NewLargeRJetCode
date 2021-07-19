#include <assert.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <tgmath.h>
#include <iostream>
#include <TChain.h>
#include <TProfile.h>
#include <TLine.h>
#include <TLegend.h>
#include <TF1.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TRandom3.h"
#include <TStyle.h>
#include <TAttMarker.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TGaxis.h>
#include <vector>
#include "NewTreeVariables.h"
#include "bFunctions.h"

int NewLargeJets_Histo(){

  //----Uploading Histograms
  TFile *file = new TFile("new_histograms.root","READ");

  if(data_pp){
    TH1D *jet_pT_dis_run[totRuns][tot_jetRadii];

    for(int iJetR = 0; iJetR < tot_jetRadii; iJetR++){
      for(int iRun = 0; iRun < totRuns; iRun++){
        jet_pT_dis_run[iRun][iJetR] = (TH1D*) file->Get(Form("run_%d_jetpT_R%d",runNumber[iRun],jetRadius[iJetR]));

      }//-----Looping over runs
    }//---Looping Over Jet Radii (1.0,0.4,0.3,0.2)

    cout << __LINE__ << endl;

    float y_valuesR2[totRuns] = {};
    float y_valuesR4[totRuns] = {};
    float y_valuesR1[totRuns] = {};
    float y_valuesR3[totRuns] ={};
    cout << __LINE__ << endl;

    float error_y_valR2[totRuns] = {};
    float error_y_valR3[totRuns] = {};
    float error_y_valR4[totRuns] = {};
    float error_y_valR1[totRuns] = {};
    cout << __LINE__ << endl;

    float error_x[totRuns] = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    float run_number[totRuns] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

  cout << __LINE__ << endl;

  Radii LargeR = R10;
  Radii JetR4 = R4;
  Radii JetR3 = R3;
  Radii JetR2 = R2;
    

  for(int iJetRadii = 0; iJetRadii < tot_jetRadii; iJetRadii++){
    cout << __LINE__ << endl;
    if(jetRadius[iJetRadii]==10){
      cout << __LINE__ << endl;
	    for(int iRun = 0; iRun < totRuns; iRun++){
        cout << __LINE__ << endl;
	     y_valuesR1[iRun] = jet_pT_dis_run[iRun][LargeR]->Integral();
	     error_y_valR1[iRun] = sqrt(y_valuesR1[iRun]);
       cout << __LINE__ << endl;
	    }//--Runs Loop
    }else if(jetRadius[iJetRadii]==4){
      cout << __LINE__ << endl;
	   for(int iRun = 0; iRun < totRuns; iRun++){
      cout << __LINE__ << endl;
	     y_valuesR4[iRun] = jet_pT_dis_run[iRun][JetR4]->Integral();
	     error_y_valR4[iRun] = sqrt(y_valuesR4[iRun]);
	   }//--Runs Loop
    }else if(jetRadius[iJetRadii]==3){
	   for(int iRun = 0; iRun < totRuns; iRun++){
      cout << __LINE__ << endl;
	     y_valuesR3[iRun] = jet_pT_dis_run[iRun][JetR3]->Integral();
	     error_y_valR3[iRun] = sqrt(y_valuesR3[iRun]);
       cout << __LINE__ << endl;
	   }//--Runs Loop
    }else{
      cout << __LINE__ << endl;
	   for(int iRun = 0; iRun < totRuns; iRun++){
	     y_valuesR2[iRun] = jet_pT_dis_run[iRun][JetR2]->Integral();
	     error_y_valR2[iRun] = sqrt(y_valuesR2[iRun]);
	    }//--Runs Loop
   }


}//------Jet Radii Loop

  std::vector<float*> y_valuesRX = std::vector<float*>();
  y_valuesRX.push_back(y_valuesR1);
  y_valuesRX.push_back(y_valuesR4);
  y_valuesRX.push_back(y_valuesR3);
  y_valuesRX.push_back(y_valuesR2);

  std::vector<float*> error_y_valRX = std::vector<float*>();    
  error_y_valRX.push_back(error_y_valR4);
  error_y_valRX.push_back(error_y_valR3);
  error_y_valRX.push_back(error_y_valR2);
  error_y_valRX.push_back(error_y_valR1);


  TGraphErrors *tGraphError[tot_jetRadii];

  for(int iJetRadius = 0; iJetRadius < tot_jetRadii; iJetRadius++){
    tGraphError[iJetRadius] = new TGraphErrors(totRuns,run_number,y_valuesRX.at(iJetRadius),error_x,error_y_valRX.at(iJetRadius));
    tGraphError[iJetRadius]->SetName(Form("jetR%i_Lumi_block",jetRadius[iJetRadius]));
  }//----Looping over jet Radii



  for(int iTGraph = 0; iTGraph < tot_jetRadii; iTGraph++){

    tGraphError[iTGraph]->GetYaxis()->SetTitle("Number of Jets");
    tGraphError[iTGraph]->GetXaxis()->SetTitle("Runs");


    if(iTGraph == 0){
      tGraphError[iTGraph]->SetMarkerColor(kRed);
      tGraphError[iTGraph]->SetMarkerStyle(21);
    }else if(iTGraph == 1){
      tGraphError[iTGraph]->SetMarkerColor(kBlack);
      tGraphError[iTGraph]->SetMarkerStyle(20);
    }else if(iTGraph == 2){
      tGraphError[iTGraph]->SetMarkerColor(kBlue);
      tGraphError[iTGraph]->SetMarkerStyle(20);
    }else{
      tGraphError[iTGraph]->SetMarkerColor(kMagenta);
      tGraphError[iTGraph]->SetMarkerStyle(20);
    }
  }
  cout << __LINE__ << endl;
  TCanvas *new_jet_perLumi_canv = new TCanvas("new_jet_perLumi_canv","new_jet_perLumi_canv",600,500);
  TLegend *legend_jet_perLumi = new TLegend(0.6,0.5,0.85,0.6);
  cout << __LINE__ << endl;
  tGraphError[0]->Draw("AP ");
  tGraphError[1]->Draw("P same");
  tGraphError[2]->Draw("P same");
  tGraphError[3]->Draw("P same");
  cout << __LINE__ << endl;
  for(int iJetRadius =0; iJetRadius < tot_jetRadii; iJetRadius++){
    cout << __LINE__ << endl;
    legend_jet_perLumi->AddEntry(Form("jetR%i_Lumi_block",jetRadius[iJetRadius]),Form("R=%f",jetRadius[iJetRadius]/10.0),"pe");
    legend_jet_perLumi->Draw("same");
    cout << __LINE__ << endl;
  }
}//------Only for pp Data plots









if(mc_pp){
  cout << __LINE__ << endl;
  //-------Starting to make Gaussian distributions to eventually make the JER and JES plots
  TH1D* pTReco_Truth_IsoR10[totRanges];
  cout << __LINE__ << endl;
  for(int ipTRange = 0; ipTRange < totRanges; ipTRange++){
    cout << __LINE__ << endl;.q
    pTReco_Truth_IsoR10[ipTRange] = (TH1D*) file->Get(Form("JER_R10_%i_%iGeV",pTRanges[ipTRange],pTRanges[ipTRange+1]));
    cout << __LINE__ << endl;
  }//---Looping over pT Ranges
  cout << __LINE__ << endl;
  
  TF1 * fits_JetR10[totRanges];
  for(int ipTRange = 0; ipTRange < totRanges; ipTRange++){
    fits_JetR10[ipTRange] = new TF1(Form("fit_%i_%i",pTRanges[ipTRange],pTRanges[ipTRange+1]),"gaus",0,2);
  }
  cout << __LINE__ << endl;
  int num_rows = 3;
  int num_coloumns = 4;
  cout << __LINE__ << endl;

  //MakeGaussianDis("R10_MatchRecoTruth",num_rows, num_coloumns, pTReco_Truth_IsoR10, pTRanges, fits_JetR10);
  cout << __LINE__ << endl;


}//-----Only for pp MC plots

  return 0;
}
