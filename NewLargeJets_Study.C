#include <assert.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <tgmath.h>
#include <iostream>
#include <TChain.h>
#include <TLegend.h>
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TRandom3.h"
#include <TAttMarker.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <dirent.h>
#include <sys/types.h>
#include "TLorentzVector.h"
#include "bFunctions.h"
#include "NewTreeVariables.h"


using namespace std;

int NewLargeJets_Study(){
 
      
    //----------------Initializing TH1D,TH2D,etc arrays


    //------------------------------------------------------------------------//
    //---------------------------------pp MC----------------------------------//
    //------------------------------------------------------------------------//
    TH1D* pTReco_Truth_IsoR10[totRanges];
    Make_Pre_JER_Or_JES(pTReco_Truth_IsoR10, "JER_R10",pTRanges,totRanges,45);
    
    //------------------------------------------------------------------------//
    //---------------------------------pp Data--------------------------------//
    //------------------------------------------------------------------------//
    
    TH1D* eta_dis_SumpT10[4];
    MakeTH1D_Eta_Array(eta_dis_SumpT10, "etaDis_pp_data", 25,eta_pT_slices ,ends_of_pTSlices); 

    TH2D* eta_phi_SumpT10[4]; 
    MakeTH2D_EtaPhi_Array(eta_phi_SumpT10, "eta_phi_pp_data", 25, eta_pT_slices,ends_of_pTSlices);
    TH1D *jet_pT_dis_run[totRuns][tot_jetRadii];

    for(int iJetR = 0; iJetR < tot_jetRadii; iJetR++){
      for(int iRun = 0; iRun < totRuns; iRun++){
	jet_pT_dis_run[iRun][iJetR] = new TH1D(Form("RunNumber_%d_R_%d",runNumber[iRun],jetRadius[iJetR]),Form("RunNumber_%d_R_%d",runNumber[iRun],jetRadius[iJetR]),25,0,1800);
      }
    }//---Creating Jet/Lumi Plots For R=0.2,0.3,0.4,1.0 Jets

 


  if(mc_pp){
    //--------------------------------------------------------------------------//
    //---------------------------------pp MC------------------------------------//
    //--------------------------------------------------------------------------//
    


    for(int iJZ_Samples = 0; iJZ_Samples < total_samples; iJZ_Samples++){
    tree_pp_MC->Add(Form("~/usatlasdata/LargeRJet_MC/JZ%i/*.root",iJZ_Samples+1));
    num_JZ_range_entries[iJZ_Samples+1]=tree_pp_MC->GetEntries();

    }//---Picking up all JZ samples


    //----Tracks
    tree_pp_MC->SetBranchAddress("ntrk",&ntrk_reco);
    tree_pp_MC->SetBranchAddress("trk_pt", &trk_pt_reco);
    tree_pp_MC->SetBranchAddress("trk_eta", &trk_eta_reco);
    tree_pp_MC->SetBranchAddress("trk_phi", &trk_phi_reco);


    //---Verticies
    tree_pp_MC->SetBranchAddress("nvert", &nvert_ppMC);
    tree_pp_MC->SetBranchAddress("vert_type", vert_type_reco);
    tree_pp_MC->SetBranchAddress("vert_sumpt", vert_sumpt_reco);
    tree_pp_MC->SetBranchAddress("vert_ntrk", vert_ntrk_reco);

    //-------------R=1.0-----------------//
    tree_pp_MC->SetBranchAddress("m_b_akt10_reco_jet_n", &akt10_pp_jet_n_reco);
    tree_pp_MC->SetBranchAddress("akt10_reco_jet_etaJES_calib_phi", &akt10_reco_jet_etaJES_calib_phi);
    tree_pp_MC->SetBranchAddress("akt10_reco_jet_etaJES_calib_pt", &akt10_reco_jet_etaJES_calib_pt);
    tree_pp_MC->SetBranchAddress("akt10_reco_jet_etaJES_calib_eta", &akt10_reco_jet_etaJES_calib_eta);

    //-----Truth
    tree_pp_MC->SetBranchAddress("akt10_truth_jet_n", &akt10_pp_jet_n_trth);
    tree_pp_MC->SetBranchAddress("akt10_truth_jet_phi", &akt10_pp_jet_phi_trth);
    tree_pp_MC->SetBranchAddress("akt10_truth_jet_pt", &akt10_pp_jet_pt_trth);
    tree_pp_MC->SetBranchAddress("akt10_truth_jet_eta", &akt10_pp_jet_eta_trth);

    //-------------R=0.4----------------//
    //-----Truth
    tree_pp_MC->SetBranchAddress("akt4_truth_jet_n", &akt4_pp_jet_n_trth);
    tree_pp_MC->SetBranchAddress("akt4_truth_jet_e", &akt4_pp_jet_e_trth);
    tree_pp_MC->SetBranchAddress("akt4_truth_jet_phi", &akt4_pp_jet_phi_trth);
    tree_pp_MC->SetBranchAddress("akt4_truth_jet_pt", &akt4_pp_jet_pt_trth);
    tree_pp_MC->SetBranchAddress("akt4_truth_jet_eta", &akt4_pp_jet_eta_trth);


    //---Reco
    tree_pp_MC->SetBranchAddress("m_b_akt4_reco_jet_n", &akt4_pp_jet_n_reco);
    tree_pp_MC->SetBranchAddress("akt4_reco_jet_etaJES_calib_pt", &akt4_reco_jet_etaJES_calib_pt);
    tree_pp_MC->SetBranchAddress("akt4_reco_jet_etaJES_calib_eta", &akt4_reco_jet_etaJES_calib_eta);
    tree_pp_MC->SetBranchAddress("akt4_reco_jet_etaJES_calib_phi", &akt4_reco_jet_etaJES_calib_phi);






    int events_tot_pp_MC = tree_pp_MC->GetEntries();

    
    for (int iEvent_pp_MC = 0; iEvent_pp_MC < events_tot_pp_MC; ++iEvent_pp_MC){


      tree_pp_MC->GetEntry(iEvent_pp_MC);

      
      //-----------R=1.0-----------//
      int totIsolatedRecoJets = 0;    //-----Total number of Isolated Jets 
      isolatedRecoJetsIndex.clear();   //-----We will keep the indicies of these isolated jets
      bool NoRecoJetNearby = false; 

      if(debug_reco) cout << "We will not look for isolated jets! " << endl;

      for(int iIsoRecoR10 = 0; iIsoRecoR10 < akt10_pp_jet_n_reco; iIsoRecoR10++){
	if( (akt10_reco_jet_etaJES_calib_eta->at(iIsoRecoR10) > etaCut) || (akt10_reco_jet_etaJES_calib_eta->at(iIsoRecoR10)< -etaCut) ) continue;

	if(debug_reco) cout << "This is how many R=1.0 jets there are: " << akt10_pp_jet_n_reco << endl;

	for(int iNearJet = 0; iNearJet < akt10_pp_jet_n_reco; iNearJet++ ){
	  if(debug_reco) cout << "we will be looking at nearby jets. There are:  " << akt10_pp_jet_n_reco -1  << endl;

	  if(iNearJet == iIsoRecoR10) continue;
          if( (akt10_reco_jet_etaJES_calib_eta->at(iNearJet) > etaCut) || (akt10_reco_jet_etaJES_calib_eta->at(iNearJet)< -etaCut) ) continue;

	  if(debug_reco) cout << "This nearby jet passed the eta cut! " << endl;
 
	  float delta_IsoR10 = deltaR_calc(akt10_reco_jet_etaJES_calib_eta->at(iNearJet), akt10_reco_jet_etaJES_calib_eta->at(iIsoRecoR10),akt10_reco_jet_etaJES_calib_phi->at(iNearJet), akt10_reco_jet_etaJES_calib_phi->at(iIsoRecoR10));

	  if(debug_reco) cout << "We calculated the deltaR of nearby jet and the first reco jet: " << delta_IsoR10 << endl; 
	  
	  if((delta_IsoR10 < deltaR_IsoCut) && (iNearJet+1 == akt10_pp_jet_n_reco)){
	    if(debug_reco) cout << "**************This reco is islated!************************" << endl;
	   NoRecoJetNearby = true;
	   isolatedRecoJetsIndex.push_back(iIsoRecoR10);
	   totIsolatedRecoJets++;
	   break;
	  }
	  
	}//---Looping over all the rest of reco jets 
       
	
      }//----Looping over all jets. I am looking for an isolated jet.
   
  
       if(!NoRecoJetNearby) continue; //------If there are no isolated reco jets continue to the next event

       int totIsolatedTruthJets = 0;    //-----Total number of Isolated Jets
       isolatedTruthJetsIndex.clear();   //-----We will keep the indicies of these isolated jets
       bool NoTruthJetNearby = false;

       for(int iIsoTruth = 0; iIsoTruth < akt10_pp_jet_n_trth; iIsoTruth++){
	 
	 if(debug_trth) cout << "This is how many R=1.0 jets there are: " << akt10_pp_jet_n_trth << endl; 
	 
	 if( (akt10_pp_jet_eta_trth->at(iIsoTruth) > etaCut) || (akt10_pp_jet_eta_trth->at(iIsoTruth) < -etaCut) ) continue;
	 
	 for(int iNearTruthJet = 0; iNearTruthJet < akt10_pp_jet_n_trth; iNearTruthJet++){

	   if(debug_trth) cout << "we will be looking at nearby jets. There are:  " << akt10_pp_jet_n_trth -1  << endl;

	   if(iNearTruthJet  == iIsoTruth){continue;}
	   if( (akt10_pp_jet_eta_trth->at(iNearTruthJet) > etaCut) || (akt10_pp_jet_eta_trth->at(iNearTruthJet) < -etaCut) ) continue;
	   
	   if(debug_trth) cout << "This nearby jet passed the eta cut! " << endl;

	   float deltaR10_Truth = deltaR_calc(akt10_pp_jet_eta_trth->at(iIsoTruth), akt10_pp_jet_eta_trth->at(iNearTruthJet),akt10_pp_jet_phi_trth->at(iIsoTruth),akt10_pp_jet_phi_trth->at(iNearTruthJet));

	   if(debug_trth) cout << "We calculated the deltaR of nearby jet and the first reco jet: " << deltaR10_Truth << endl;

	   if((deltaR10_Truth < deltaR_IsoCut) && (iNearTruthJet+1 == akt10_pp_jet_n_trth)){
	     //if(debug_trth) cout << "**************This reco is islated!************************" << endl;
	    NoTruthJetNearby = true;
	    if(debug) cout << "this is the index of your matched truth jet: " << iIsoTruth << endl;
	    isolatedTruthJetsIndex.push_back(iIsoTruth);
	    if(debug) cout << "I have now put it in the vector! " << endl;
            totIsolatedTruthJets++;
	    break;
          }

	}//----Looping over all the rest of the jets
	
      }//---Looping over all jets. I am looking for an isolated jet
       if(debug) cout << "We found isolated truth jets? " << NoTruthJetNearby << endl;
       if(debug) cout << "We found isolated Reco jets? " << NoRecoJetNearby << endl;
      
      if(!NoRecoJetNearby || !NoTruthJetNearby) continue; //----making sure I have at least one isolated reco and truth jets
      if(debug){
	cout << "orinting out the indecies of your truth jets" << endl;
        for(int index = 0; index < totIsolatedTruthJets; index++){
	  cout << isolatedTruthJetsIndex.at(index) << endl;
	}//----looping over indecies
      }
      if(debug) cout << "We found isolated truth and reco jets!!" << endl; 
      if(debug) cout << "We will now loop over the number of isolated reco jets. There are: " << totIsolatedRecoJets << endl;
      if(debug) cout << "There are " << totIsolatedTruthJets << " isolated truth jets" << endl;

       for(int iIsoRecoJets = 0; iIsoRecoJets < totIsolatedRecoJets; iIsoRecoJets++ ){
        float deltaR_Smallest = 10000;
	int matched_truth_index = -1;
	if(debug) cout << __LINE__ << endl;
	int reco_index = isolatedRecoJetsIndex.at(iIsoRecoJets); 
	if(debug) cout << __LINE__ << endl; 
	for(int iIsoTruthJets = 0; iIsoTruthJets < totIsolatedTruthJets; iIsoTruthJets++){
	  if(debug) cout << __LINE__ << endl;
	  int truth_index = isolatedTruthJetsIndex.at(iIsoTruthJets);
	  if(debug) cout << __LINE__ << endl;
	  float deltaRTemp = deltaR_calc(akt10_pp_jet_eta_trth->at(truth_index),akt10_reco_jet_etaJES_calib_eta->at(reco_index),akt10_pp_jet_phi_trth->at(truth_index),akt10_reco_jet_etaJES_calib_phi->at(reco_index)); 
	  if(debug) cout << "This is the delta R between reco jet number " << iIsoRecoJets << " and truth jet number " << iIsoTruthJets << endl;
	  if(deltaR_Smallest > deltaRTemp) matched_truth_index = truth_index;
	  if(debug) cout << __LINE__ << endl;

	}//-----Looping truth jets

        if(matched_truth_index == -1) continue;
	if(debug) cout << "**********************************WE FOUND A MATCH************************" << endl;
	if(debug) cout << __LINE__ << endl;
	
	for(int ipTRange = 0; ipTRange < totRanges; ipTRange++){	
	    
	    if((akt10_pp_jet_pt_trth->at(matched_truth_index) > pTRanges[ipTRange]) && (akt10_pp_jet_pt_trth->at(matched_truth_index) <= pTRanges[ipTRange+1]) ){
	      
	      for(int iJZ = 0; iJZ < total_samples; iJZ++){
		
		if( (iEvent_pp_MC >  num_JZ_range_entries[iJZ]) && (iEvent_pp_MC < num_JZ_range_entries[iJZ+1])){
		  if(debug_filled_histo) cout << "*********************FILLED HISTO***************************" << endl; 
		  if(debug_filled_histo) cout << "ratio " << akt10_reco_jet_etaJES_calib_pt->at(reco_index)/akt10_pp_jet_pt_trth->at(matched_truth_index) << endl;
		  pTReco_Truth_IsoR10[ipTRange]->Fill(akt10_reco_jet_etaJES_calib_pt->at(reco_index)/akt10_pp_jet_pt_trth->at(matched_truth_index),weights_ppMC[ipTRange]);
		
		}
	      }//------Looping over JZ samples
	    }
	  
	  }//---Looping over pT ranges

       }//-----Matched Reco Jets and Truth Pass Through here    
      
   
}//----Event Loop



    
 }//----------pp MC






  if(data_pp){
    //----------------------------------------------------------------------------//
    //---------------------------------pp Data------------------------------------//
    //----------------------------------------------------------------------------//


    //----------------Initializing TH1D,TH2D,etc arrays

    
    //------Grabbing All Root Files
    tree_pp_Data->Add("/gpfs/mnt/atlasgpfs01/usatlas/workarea/bereniceg299/user.berenice.06152021.LargeRJet_pp2017_Data_Analysis_GRL.00000000000_myOutput.root/*.root");

    //---------------------Setting Up Branches of My Tree---------------------------------//
    
    //---Run Number
    tree_pp_Data->SetBranchAddress("RunNumber",&RunNumber);
    //---Verticies
    tree_pp_Data->SetBranchAddress("nvert", &nvert);
    tree_pp_Data->SetBranchAddress("vert_type", vert_type);
    tree_pp_Data->SetBranchAddress("vert_sumpt", vert_sumpt);
    tree_pp_Data->SetBranchAddress("vert_ntrk", vert_ntrk);
    //---Tracks
    tree_pp_Data->SetBranchAddress("ntrk",&ntrk);
    tree_pp_Data->SetBranchAddress("trk_eta", &trk_eta);
    tree_pp_Data->SetBranchAddress("trk_phi", &trk_phi);
    tree_pp_Data->SetBranchAddress("trk_pt", &trk_pt);
    //----R=1.0
    tree_pp_Data->SetBranchAddress("akt10hi_jet_n", &akt10hi_jet_n_ppData);
    tree_pp_Data->SetBranchAddress("akt10hi_jet_Insitu_calib_pt", &akt10_InsituCalib_jet_pp_pt);
    tree_pp_Data->SetBranchAddress("akt10hi_jet_Insitu_calib_phi", &akt10_InsituCalib_jet_pp_phi);
    tree_pp_Data->SetBranchAddress("akt10hi_jet_Insitu_calib_eta", &akt10_InsituCalib_jet_pp_eta);
    tree_pp_Data->SetBranchAddress("akt10hi_jet_Insitu_calib_e", &akt10_InsituCalib_jet_pp_e);
    tree_pp_Data->SetBranchAddress("HLT_j110_a10_lcw_subjes_L1J30",&HLT_j110_a10_lcw_subjes_L1J30);

    int events_tot_pp_Data = tree_pp_Data->GetEntries();



    for(int iEvent_pp_Data = 0; iEvent_pp_Data < events_tot_pp_Data ; iEvent_pp_Data++){

      tree_pp_Data->GetEntry(iEvent_pp_Data);
      if(!HLT_j110_a10_lcw_subjes_L1J30){continue;}
      if(nvert < 2 ){continue;}
      
      for(int iJetRadius = 0; iJetRadius < tot_jetRadii; iJetRadius++){
	tree_pp_Data->SetBranchAddress(Form("akt%ihi_jet_n",jetRadius[iJetRadius]), &aktRhi_jet_n_ppData);
	tree_pp_Data->SetBranchAddress(Form("akt%ihi_jet_Insitu_calib_pt",jetRadius[iJetRadius]), &aktR_InsituCalib_jet_pp_pt);
	tree_pp_Data->SetBranchAddress(Form("akt%ihi_jet_Insitu_calib_phi",jetRadius[iJetRadius]), &aktR_InsituCalib_jet_pp_phi);
	tree_pp_Data->SetBranchAddress(Form("akt%ihi_jet_Insitu_calib_eta",jetRadius[iJetRadius]), &aktR_InsituCalib_jet_pp_eta);
	tree_pp_Data->SetBranchAddress(Form("akt%ihi_jet_Insitu_calib_e",jetRadius[iJetRadius]), &aktR_InsituCalib_jet_pp_e);
	
	for(int iJet = 0; iJet < aktRhi_jet_n_ppData; iJet++){

	  if(aktR_InsituCalib_jet_pp_pt->at(iJet) < pTCut){continue;}
	  if(aktR_InsituCalib_jet_pp_eta->at(iJet) > etaCut || aktR_InsituCalib_jet_pp_eta->at(iJet) < -etaCut ){continue;}

	  for(int iRunNumber = 0; iRunNumber < totRuns; iRunNumber++){
	    if(RunNumber == runNumber[iRunNumber]){
	      jet_pT_dis_run[iRunNumber][iJetRadius]->Fill(aktR_InsituCalib_jet_pp_pt->at(iJet));
	    }
	  }//---Run Number Loop
	}//----Looping over the number of jets
      }//-------Looping over all jets 


      //---------------------------------------------------------------------------//
      //-----------------------------------R=1.0-----------------------------------//
      //---------------------------------------------------------------------------//
      for(int iJet_ppData= 0; iJet_ppData < akt10hi_jet_n_ppData; iJet_ppData++){
	float sum_trk_pT_value = 0;

        for(int iVertex = 0; iVertex < nvert; iVertex++){

          if(vert_type[iVertex] != 1){continue;}
	  const int tot_ptslices = 3;
          float sump_track_pT[tot_ptslices]={};              //---sum pt of tracks for different pT slices (600-800,800-1000, and 1000-1300 GeV)
          float dR_trk_jet = 0;
	  for(int iTrack = 0; iTrack < ntrk; iTrack++){
            dR_trk_jet = deltaR_calc(trk_eta->at(iTrack), akt10_InsituCalib_jet_pp_eta->at(iJet_ppData),trk_phi->at(iTrack),akt10_InsituCalib_jet_pp_phi->at(iJet_ppData) ); 
            
            if(dR_trk_jet > 1){continue;}
            if(trk_pt->at(iTrack) < pTtrackCut){continue;}

            sum_trk_pT_value = trk_pt->at(iTrack) + sum_trk_pT_value;
	    

	    for(int ipTSlice =0; ipTSlice < tot_ptslices; ipTSlice++){
	
	      if((akt10_InsituCalib_jet_pp_pt->at(iJet_ppData) > sumpT_Trk_pTSlices[ipTSlice]) && (akt10_InsituCalib_jet_pp_pt->at(iJet_ppData) <= sumpT_Trk_pTSlices[ipTSlice+1])){
	
		sump_track_pT[ipTSlice] = trk_pt->at(iTrack) + sump_track_pT[ipTSlice];
	       }
	
	      
	    }//---Looping Over Different pT Slices
	
	  }//------Looping over tracks
	
	}//-----Vertex Loop
	

	if(sum_trk_pT_value > 10){ //----Jets that pass the sumpT > 10 GeV Requirement
	
          for(int ipTSlice=0; ipTSlice < pTSlices_EtaPlots; ipTSlice++){
            if( (akt10_InsituCalib_jet_pp_pt->at(iJet_ppData) > eta_pT_slices[ipTSlice]) && (akt10_InsituCalib_jet_pp_pt->at(iJet_ppData) <= eta_pT_slices[ipTSlice+1])){
	
	      eta_dis_SumpT10[ipTSlice]->Fill(akt10_InsituCalib_jet_pp_eta->at(iJet_ppData));
	      eta_phi_SumpT10[ipTSlice]->Fill(akt10_InsituCalib_jet_pp_phi->at(iJet_ppData),akt10_InsituCalib_jet_pp_eta->at(iJet_ppData));
	      
            }
          }//----Looping Over pT Slices

        }//----SumpT > 10 GeV
	

      }//---R=1.0 Loop
      
    }//-----Event Loop



    
  }//-----------pp Data

  //---------------------------------------------------------------------//
  //-------------------Filling in my histogram root file-----------------//
  //---------------------------------------------------------------------//

  TFile *file_root = new TFile("new_histograms.root","RECREATE");
  file_root->cd();
  
  if(data_pp){  
    //---Writing my histograms to file
    write_TH1D_1DArray_to_file(eta_dis_SumpT10,4,"eta_phi_pp_data",eta_pT_slices);
  
    for(int iJetR = 0; iJetR < tot_jetRadii; iJetR++){
      for(int iRun = 0; iRun < totRuns; iRun++){
	jet_pT_dis_run[iRun][iJetR]->Write(Form("RunNumber_%d_R_%d",runNumber[iRun],jetRadius[iJetR]),TObject::kOverwrite); 
      }
    }//---Creating Jet/Lumi Plots For R=0.2,0.3,0.4,1.0 Jets 
  }//-------pp Data

 
   if(mc_pp){
     if(debug_write) cout << "We will now write your histograms!! " << endl;
     write_TH1D_1DArray_to_file(pTReco_Truth_IsoR10,totRanges,"JER_R10",pTRanges);
   }//-----pp MC
 

  return 0;
}
