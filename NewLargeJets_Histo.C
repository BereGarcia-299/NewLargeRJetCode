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

int NewLargeJets_Histo(){

  //----Uploading Histograms
  TFile *file = new TFile("new_histograms.root","READ");


  TH1D *jet_pT_dis_run[totRuns][tot_jetRadii];

  for(int iJetR = 0; iJetR < tot_jetRadii; iJetR++){
    for(int iRun = 0; iRun < totRuns; iRun++){
      jet_pT_dis_run[iRun][iJetR] = (TH1D*) file->Get(Form("run_%d_jetpT_R%d",runNumber[iRun],jetRadius[iJetR]));


    }//---Looping Over Jet Radii (1.0,0.4,0.3,0.2)



    float y_valuesR2[totRuns] = {};
    float y_valuesR4[totRuns] = {};
    float y_valuesR1[totRuns] = {};
    float y_valuesR3[totRuns] ={};

    float error_y_valR2[totRuns] = {};
    float error_y_valR3[totRuns] = {};
    float error_y_valR4[totRuns] = {};
    float error_y_valR1[totRuns] = {};

    float error_x[totRuns] = {0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
    float run_number[totRuns] = {1,2,3,4,5,6,7,8,9,10,11,12,13};

    
    for(int iJetRadii = 0; iJetRadii < tot_jetRadii; iJetRadii++){
      if(jetRadius[iJetRadii]==10){
	for(int iRun = 0; iRun < totRuns; iRun++){
	  y_valuesR1[iRun] = jet_pT_dis_run[iRun][0]->Integral();
	  error_y_valR1[iRun] = sqrt(y_valuesR1[iRun]);
	}//--Runs Loop
      }else if(jetRadius[iJetRadii]==4){
	for(int iRun = 0; iRun < totRuns; iRun++){
	  y_valuesR4[iRun] = jet_pT_dis_run[iRun][1]->Integral();
	  error_y_valR4[iRun] = sqrt(y_valuesR4[iRun]);
	}//--Runs Loop
      }else if(jetRadius[iJetRadii]==3){
	for(int iRun = 0; iRun < totRuns; iRun++){
	  y_valuesR3[iRun] = jet_pT_dis_run[iRun][3]->Integral();
	  error_y_valR3[iRun] = sqrt(y_valuesR3[iRun]);
	}//--Runs Loop
      }else{
	for(int iRun = 0; iRun < totRuns; iRun++){
	  y_valuesR2[iRun] = jet_pT_dis_run[iRun][2]->Integral();
	  error_y_valR2[iRun] = sqrt(y_valuesR2[iRun]);
	}//--Runs Loop
      }


    }//------Jet Radii Loop


  TGraphErrors *tGraphError[tot_jetRadii];
  for(int iJetRadius = 0; iJetRadis < tot_jetRadii; iJetRadius++){
    tGraphError[iJetRadius] = new TGraphErrors(totRuns,values_x_runnumber,y_valuesR1,error_x,error_y_valR1);
    tGraphError[iJetRadius]->SetName("jetR10_Lumi_block");
  }//----Looping over jet Radii
  tGraphError[0] = new TGraphErrors(totRuns,run_number,y_valuesR1,error_x,error_y_valR1);
  tGraphError[0]->SetName("jetR10_Lumi_block");
  tGraphError[1] = new TGraphErrors(totRuns,run_number,y_valuesR4,error_x,error_y_valR4);
  tGraphError[1]->SetName("jetR04_Lumi_block");
  tGraphError[2] = new TGraphErrors(totRuns,run_number,y_valuesR2,error_x,error_y_valR2);
  tGraphError[2]->SetName("jetR02_Lumi_block");
  tGraphError[3] = new TGraphErrors(totRuns,run_number,y_valuesR3,error_x,error_y_valR3);
  tGraphError[3]->SetName("jetR03_Lumi_block");

  for(int iTGraph = 0; iTGraph < 4; iTGraph++){

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

  TCanvas *new_jet_perLumi_canv = new TCanvas("new_jet_perLumi_canv","new_jet_perLumi_canv",600,500);
  TLegend *legend_jet_perLumi = new TLegend(0.6,0.5,0.85,0.6);
  tGraphError[0]->Draw("AP ");
  tGraphError[1]->Draw("P same");
  tGraphError[2]->Draw("P same");
  tGraphError[3]->Draw("P same");
  

  legend_jet_perLumi->AddEntry("jetR10_Lumi_block","R=1.0","pe");
  legend_jet_perLumi->AddEntry("jetR04_Lumi_block","R=0.4","pe");
  legend_jet_perLumi->AddEntry("jetR03_Lumi_block","R=0.3","pe");
  legend_jet_perLumi->AddEntry("jetR02_Lumi_block","R=0.2","pe");
  legend_jet_perLumi->Draw("same");

  TFile *file_root_new = new TFile("histograms_new.root","RECREATE");
  file_root_new->cd();

  for(int iTgraph = 0; iTgraph < 4; iTgraph++){
    tGraphError[iTgraph]->Write(Form("Tgraph_%i",iTgraph),TObject::kOverwrite);
  }







  return 0;
}
