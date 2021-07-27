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
    TH1D* pTReco_Truth_R10[totRanges];
<<<<<<< HEAD

    TH1D* pTReco_Truth_IsoR4[totRanges];
    TH1D* pTReco_Truth_R4[totRanges];

    int num_bins = 45;

    Make_Pre_JER_Or_JES(pTReco_Truth_R10, "JER_R10",pTRanges,totRanges,num_bins);
    Make_Pre_JER_Or_JES(pTReco_Truth_IsoR10, "JER_R10_IsoJet",pTRanges,totRanges,num_bins);

    Make_Pre_JER_Or_JES(pTReco_Truth_R4, "JER_R4",pTRanges,totRanges,num_bins);
    Make_Pre_JER_Or_JES(pTReco_Truth_IsoR4, "JER_R4_IsoJet",pTRanges,totRanges,num_bins);

=======
    Make_Pre_JER_Or_JES(pTReco_Truth_R10, "JER_R10",pTRanges,totRanges,45);
    Make_Pre_JER_Or_JES(pTReco_Truth_IsoR10, "JER_R10_IsoJet",pTRanges,totRanges,45);
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf

    //------------------------------------------------------------------------//
    //---------------------------------pp Data--------------------------------//
    //------------------------------------------------------------------------//
    
    TH1D* eta_dis_SumpT10[4];
    MakeTH1D_Eta_Array(eta_dis_SumpT10, "etaDis_pp_data", 25,eta_pT_slices ,ends_of_pTSlices); 

    TH2D* eta_phi_SumpT10[4]; 
    MakeTH2D_EtaPhi_Array(eta_phi_SumpT10, "eta_phi_pp_data", 25, eta_pT_slices,ends_of_pTSlices);
    
    TH1D *jet_pT_dis_run[totRuns][tot_jetRadii];
    TH1D *jet_pTDisSumpT[totRuns][tot_jetRadii];

    TH1D *jetpTDis[tot_jetRadii][sumpTWO_With];    //----Contains the pT distributions for different Rs and with and without the sumpT cut


    for(int iJetR = 0; iJetR < tot_jetRadii; iJetR++){
      for(int iRun = 0; iRun < totRuns; iRun++){
	jet_pT_dis_run[iRun][iJetR] = new TH1D(Form("RunNumber_%d_R_%d",runNumber[iRun],jetRadius[iJetR]),Form("RunNumber_%d_R_%d",runNumber[iRun],jetRadius[iJetR]),25,0,1800);
	jet_pTDisSumpT[iRun][iJetR] = new TH1D(Form("RunNumber_%d_R_%d_sumpTCut",runNumber[iRun],jetRadius[iJetR]),Form("RunNumber_%d_R_%d_sumpTCut",runNumber[iRun],jetRadius[iJetR]),25,0,1800);
      }//---Looping over all radii
    }//---Creating Jet/Lumi Plots For R=0.2,0.3,0.4,1.0 Jets


    SumpT_TF withSumpT = WSumpT;
    SumpT_TF woSumpT = WOSumpT;
 
    for(int iSumpT = 0; iSumpT < sumpTWO_With; iSumpT++){
      for(int iJetR =0; iJetR < tot_jetRadii; iJetR++){
	jetpTDis[iJetR][iSumpT] = new TH1D(Form("pTDis_R%d_%s",jetRadius[iJetR],SumpT[iSumpT].c_str()),Form("pTDis_R%d_%s",jetRadius[iJetR],SumpT[iSumpT].c_str()),25,0,1800);
    
	}//------Looping over Radii
    }//-------Looping over sumpt cut (with and without)

  if(mc_pp){
    //--------------------------------------------------------------------------//
    //---------------------------------pp MC------------------------------------//
    //--------------------------------------------------------------------------//
    


    MCRadii LargeR = R10;
    MCRadii JetR04 = R4;

    
    for(int iJZ_Samples = 0; iJZ_Samples < total_samples; iJZ_Samples++){
      cout << Form("~/usatlasdata/LargeRJet_MC/JZ%d/*.root",iJZ_Samples+2) << endl;
      tree_pp_MC->Add(Form("~/usatlasdata/LargeRJet_MC/JZ%d/*.root",iJZ_Samples+2));
     
      num_JZ_range_entries[iJZ_Samples+1]=tree_pp_MC->GetEntries();
      cout << "inside jz array: " << tree_pp_MC->GetEntries() << endl;
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

    for(int iJetR = 0; iJetR < 2; iJetR++){
        cout << "This is the jet radius that we are looking at: " << JetRadius[iJetR] << endl;
	//---------Setting Branches for Reco Jets-------//
	tree_pp_MC->SetBranchAddress(Form("m_b_akt%d_reco_jet_n",JetRadius[iJetR]), &aktR_jet_n_MCReco);
	tree_pp_MC->SetBranchAddress(Form("akt%d_reco_jet_etaJES_calib_pt",JetRadius[iJetR]), &aktR_etaJES_jet_MCReco_pt);
	tree_pp_MC->SetBranchAddress(Form("akt%d_reco_jet_etaJES_calib_eta",JetRadius[iJetR]), &aktR_etaJES_jet_MCReco_eta);
	tree_pp_MC->SetBranchAddress(Form("akt%d_reco_jet_etaJES_calib_phi",JetRadius[iJetR]), &aktR_etaJES_jet_MCReco_phi);

	//--------Setting Branches for Truth Jets------//
        tree_pp_MC->SetBranchAddress(Form("akt%d_truth_jet_n",JetRadius[iJetR]), &aktR_jet_n_MCTrth);
        tree_pp_MC->SetBranchAddress(Form("akt%d_truth_jet_pt",JetRadius[iJetR]), &aktR_jet_MCTrth_pt);
        tree_pp_MC->SetBranchAddress(Form("akt%d_truth_jet_eta",JetRadius[iJetR]), &aktR_jet_MCTrth_eta);
        tree_pp_MC->SetBranchAddress(Form("akt%d_truth_jet_phi",JetRadius[iJetR]), &aktR_jet_MCTrth_phi);



	int events_tot_pp_MC = tree_pp_MC->GetEntries();
    
	cout << "This is how many entries there are: " << events_tot_pp_MC << endl;

	for (int iEvent_pp_MC = 0; iEvent_pp_MC < events_tot_pp_MC; ++iEvent_pp_MC){

	 
	  if(iEvent_pp_MC % 1000000 == 0) cout << iEvent_pp_MC << endl;
      
      
	  tree_pp_MC->GetEntry(iEvent_pp_MC);
	
	  /**
	  //----------------------------------------------------------------------------//
	  //------------------------Matching a truth jet to a reco jet------------------//
	  //----------------------------------------------------------------------------//
      
	  for (int iJetTruth = 0; iJetTruth < akt10_pp_jet_n_trth; ++iJetTruth){
	
	    if( (akt10_pp_jet_eta_trth->at(iJetTruth) > etaCut) || (akt10_pp_jet_eta_trth->at(iJetTruth)< -etaCut) ) continue;
	
	    bool matched_reco = false;
	    int index_RecoJet = -1;
	    float delR_smallest = 1000;
	
	   
	    for (int iJetReco = 0; iJetReco < akt10_pp_jet_n_reco; ++iJetReco){
	      if( (akt10_reco_jet_etaJES_calib_eta->at(iJetReco) > etaCut) || (akt10_reco_jet_etaJES_calib_eta->at(iJetReco)< -etaCut) ) continue;
	   
	      float delta_R = deltaR_calc(akt10_pp_jet_eta_trth->at(iJetTruth),akt10_reco_jet_etaJES_calib_eta->at(iJetReco),akt10_reco_jet_etaJES_calib_phi->at(iJetReco),akt10_pp_jet_phi_trth->at(iJetTruth));
	   
	      if(delta_R > delR_smallest) continue;
	      matched_reco = true;
	      index_RecoJet = iJetReco;
	  
	    }//-----Reco Jet Loop

	    if(!matched_reco  || (index_RecoJet == -1)) continue;
	    //---Calculating Smallest deltaR

	   
	    for(int ipTRange = 0; ipTRange < totRanges; ipTRange++){
	      if((akt10_pp_jet_pt_trth->at(iJetTruth) < pTRanges[ipTRange]) || (akt10_pp_jet_pt_trth->at(iJetTruth) > pTRanges[ipTRange+1]) ) continue;
	      
	      for(int iJZ = 0; iJZ < total_samples; iJZ++){
		
		if( (iEvent_pp_MC <  num_JZ_range_entries[iJZ]) || (iEvent_pp_MC > num_JZ_range_entries[iJZ+1])) continue;
		
		if(debug) cout << "Reco pT: " << akt10_reco_jet_etaJES_calib_pt->at(index_RecoJet) << endl;
		
		if(debug) cout << "truth pT: " << akt10_pp_jet_pt_trth->at(iJetTruth) << endl;
		
		if(debug) cout << "weight: " << weights_ppMC[ipTRange] << endl;
		
		pTReco_Truth_R10[ipTRange]->Fill(akt10_reco_jet_etaJES_calib_pt->at(index_RecoJet)/akt10_pp_jet_pt_trth->at(iJetTruth),weights_ppMC[ipTRange]);
		
	      }//------Looping over JZ samples
	    }//---Looping over pT ranges
	  }//----Matching Reco To Truth Loop
	  */
	
	  //----------------------------------------------------------------------------//
	  //------------------------Matching a truth jet to a reco jet------------------//
	  //---------------------------But With Jet Isolation Cut-----------------------//
	  //----------------------------------------------------------------------------//	  

<<<<<<< HEAD

	   isolatedRecoJetsIndex.clear();  //-----We will keep the indicies of these isolated jets (Clearing it first)
            

	   if(debug_reco) cout << "We will now look for isolated jets! " << endl;
	   if(debug_reco) cout << "This is how many reco jets we have: " << akt10_pp_jet_n_reco << endl;
	   for(int iRecoR = 0; iRecoR < aktR_jet_n_MCReco; iRecoR++){
=======
      
      tree_pp_MC->GetEntry(iEvent_pp_MC);
      
	//----------------------------------------------------------------------------//
	//------------------------Matching a truth jet to a reco jet------------------//
	//----------------------------------------------------------------------------//
      
      for (int iJetTruth = 0; iJetTruth < akt10_pp_jet_n_trth; ++iJetTruth){
	
	if( (akt10_pp_jet_eta_trth->at(iJetTruth) > etaCut) || (akt10_pp_jet_eta_trth->at(iJetTruth)< -etaCut) ) continue;
	
	bool matched_reco = false;
	int index_RecoJet = -1;
	float delR_smallest = 1000;
	
	if(debug) cout << __LINE__ << endl;
	for (int iJetReco = 0; iJetReco < akt10_pp_jet_n_reco; ++iJetReco){
	  if( (akt10_reco_jet_etaJES_calib_eta->at(iJetReco) > etaCut) || (akt10_reco_jet_etaJES_calib_eta->at(iJetReco)< -etaCut) ) continue;
	  if(debug) cout << __LINE__ << endl;
	  float delta_R = deltaR_calc(akt10_pp_jet_eta_trth->at(iJetTruth),akt10_reco_jet_etaJES_calib_eta->at(iJetReco),akt10_reco_jet_etaJES_calib_phi->at(iJetReco),akt10_pp_jet_phi_trth->at(iJetTruth));
	  if(debug) cout << __LINE__ << endl;
	  if(delta_R > delR_smallest) continue;
	  matched_reco = true;
	  index_RecoJet = iJetReco;
	  
	}//-----Reco Jet Loop
	if(debug) cout << __LINE__ << endl;
        if(!matched_reco  || (index_RecoJet == -1)) continue;
        //---Calculating Smallest deltaR
	if(debug) cout << __LINE__ << endl;
	for(int ipTRange = 0; ipTRange < totRanges; ipTRange++){
	  if(debug) cout << __LINE__ << endl;
	  if((akt10_pp_jet_pt_trth->at(iJetTruth) < pTRanges[ipTRange]) || (akt10_pp_jet_pt_trth->at(iJetTruth) > pTRanges[ipTRange+1]) ) continue;
	  if(debug) cout << __LINE__ << endl;  
	  for(int iJZ = 0; iJZ < total_samples; iJZ++){
	    if(debug) cout << __LINE__ << endl;
	     if( (iEvent_pp_MC <  num_JZ_range_entries[iJZ]) || (iEvent_pp_MC > num_JZ_range_entries[iJZ+1])) continue;
	     if(debug) cout << __LINE__ << endl;
	     if(debug) cout << "Reco pT: " << akt10_reco_jet_etaJES_calib_pt->at(index_RecoJet) << endl;
	     if(debug) cout << __LINE__ << endl;
	     if(debug) cout << "truth pT: " << akt10_pp_jet_pt_trth->at(iJetTruth) << endl;
	     if(debug) cout << __LINE__ << endl;
	     if(debug) cout << "weight: " << weights_ppMC[ipTRange] << endl;
	     if(debug) cout << __LINE__ << endl;
	     pTReco_Truth_R10[ipTRange]->Fill(akt10_reco_jet_etaJES_calib_pt->at(index_RecoJet)/akt10_pp_jet_pt_trth->at(iJetTruth),weights_ppMC[ipTRange]);
	     if(debug) cout << __LINE__ << endl;
	  }//------Looping over JZ samples
	}//---Looping over pT ranges
      }//----Matching Reco To Truth Loop

	
     //----------------------------------------------------------------------------//
     //------------------------Matching a truth jet to a reco jet------------------//
     //---------------------------But With Jet Isolation Cut-----------------------//
     //----------------------------------------------------------------------------//	  


      //-----------R=1.0-----------//
      isolatedRecoJetsIndex.clear();  //-----We will keep the indicies of these isolated jets (Clearing it first)
            
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf

	     if(aktR_etaJES_jet_MCReco_pt->at(iRecoR) < pTRecoCut) continue;
	     if( (aktR_etaJES_jet_MCReco_eta->at(iRecoR) > etaCut) || (aktR_etaJES_jet_MCReco_eta->at(iRecoR)< -etaCut) ) continue;

<<<<<<< HEAD
	     if(debug_reco) cout << "This is how many R=1.0 jets there are: " << aktR_jet_n_MCReco << endl;
=======
      for(int iRecoR10 = 0; iRecoR10 < akt10_pp_jet_n_reco; iRecoR10++){

	if(akt10_reco_jet_etaJES_calib_eta->at(iRecoR10) < pTRecoCut) continue;
	if( (akt10_reco_jet_etaJES_calib_eta->at(iRecoR10) > etaCut) || (akt10_reco_jet_etaJES_calib_eta->at(iRecoR10)< -etaCut) ) continue;
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf

	     for(int iNearJet = 0; iNearJet < aktR_jet_n_MCReco; iNearJet++ ){
	       if(debug_reco) cout << "we will be looking at nearby jets. There are:  " << aktR_jet_n_MCReco -1  << endl;

	       if(iNearJet == iRecoR) continue;

<<<<<<< HEAD
	       if( (aktR_etaJES_jet_MCReco_eta->at(iNearJet) > etaCut) || (aktR_etaJES_jet_MCReco_eta->at(iNearJet)< -etaCut) ) continue;
=======
	  if(iNearJet == iRecoR10) continue;

          if( (akt10_reco_jet_etaJES_calib_eta->at(iNearJet) > etaCut) || (akt10_reco_jet_etaJES_calib_eta->at(iNearJet)< -etaCut) ) continue;
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf

	       if(debug_reco) cout << "This nearby jet passed the eta cut! " << endl;
 
<<<<<<< HEAD
	       float delta_IsoR = deltaR_calc(aktR_etaJES_jet_MCReco_eta->at(iNearJet), aktR_etaJES_jet_MCReco_eta->at(iRecoR),aktR_etaJES_jet_MCReco_phi->at(iNearJet), aktR_etaJES_jet_MCReco_phi->at(iRecoR));
=======
	  float delta_IsoR10 = deltaR_calc(akt10_reco_jet_etaJES_calib_eta->at(iNearJet), akt10_reco_jet_etaJES_calib_eta->at(iRecoR10),akt10_reco_jet_etaJES_calib_phi->at(iNearJet), akt10_reco_jet_etaJES_calib_phi->at(iRecoR10));
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf

	       if(debug_reco) cout << "We calculated the deltaR of nearby jet and the first reco jet: " << delta_IsoR << endl; 
	  
<<<<<<< HEAD
	       if(delta_IsoR <  deltaR_IsoCut) break;
	  
	  
	  
	       if(debug_reco) cout << "**************This reco is islated!************************" << endl;
	  
	       if(iNearJet+1 == aktR_jet_n_MCReco){
		 isolatedRecoJetsIndex.push_back(iRecoR);
	       }//---Collecting the index of the Isolated Reco Jet!
	

	     }//---Looping over all the rest of reco jets 
       
	   }//----Looping over all jets. I am looking for an isolated jet.
   
   
	   if(isolatedRecoJetsIndex.size() == 0) continue; //------If there are no isolated reco jets continue to the next event
   
	   isolatedTruthJetsIndex.clear();  //-----We will keep the indicies of these isolated jets

      
	   for(int iTruthR = 0; iTruthR < aktR_jet_n_MCTrth; iTruthR++){
	 
	     if(debug_trth) cout << "This is how many R=1.0 jets there are: " << aktR_jet_n_MCTrth << endl; 
	     if(aktR_jet_MCTrth_pt->at(iTruthR) < pTTruthCut) continue;
	     if( (aktR_jet_MCTrth_eta->at(iTruthR) > etaCut) || (aktR_jet_MCTrth_eta->at(iTruthR) < -etaCut) ) continue;
=======
	  if(delta_IsoR10 >  deltaR_IsoCut) continue;
	  
	  
	  
	  if(debug_reco) cout << "**************This reco is islated!************************" << endl;
	  
	  if(iNearJet+1 == akt10_pp_jet_n_reco){
	    isolatedRecoJetsIndex.push_back(iRecoR10);
	  }//---Collecting the index of the Isolated Reco Jet!
	

	}//---Looping over all the rest of reco jets 
       
      }//----Looping over all jets. I am looking for an isolated jet.
   
   
      if(isolatedRecoJetsIndex.size() == 0) continue; //------If there are no isolated reco jets continue to the next event
   
      isolatedTruthJetsIndex.clear();  //-----We will keep the indicies of these isolated jets

      
       for(int iTruthR10 = 0; iTruthR10 < akt10_pp_jet_n_trth; iTruthR10++){
	 
	 if(debug_trth) cout << "This is how many R=1.0 jets there are: " << akt10_pp_jet_n_trth << endl; 
	 if(akt10_pp_jet_pt_trth->at(iTruthR10) < pTTruthCut) continue;
	 if( (akt10_pp_jet_eta_trth->at(iTruthR10) > etaCut) || (akt10_pp_jet_eta_trth->at(iTruthR10) < -etaCut) ) continue;
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf
	 
	     for(int iNearTruthJet = 0; iNearTruthJet < aktR_jet_n_MCTrth; iNearTruthJet++){

	       if(debug_trth) cout << "we will be looking at nearby jets. There are:  " << aktR_jet_n_MCTrth-1  << endl;

	       if(iNearTruthJet  == iTruthR) continue;

<<<<<<< HEAD
	       if( (aktR_jet_MCTrth_eta->at(iNearTruthJet) > etaCut) || (aktR_jet_MCTrth_eta->at(iNearTruthJet) < -etaCut) ) continue;
=======
	   if(iNearTruthJet  == iTruthR10){continue;}
	   if( (akt10_pp_jet_eta_trth->at(iNearTruthJet) > etaCut) || (akt10_pp_jet_eta_trth->at(iNearTruthJet) < -etaCut) ) continue;
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf
	   
	       if(debug_trth) cout << "This nearby jet passed the eta cut! " << endl;

<<<<<<< HEAD
	       float deltaR_Truth = deltaR_calc(aktR_jet_MCTrth_eta->at(iTruthR), aktR_jet_MCTrth_eta->at(iNearTruthJet),aktR_jet_MCTrth_phi->at(iTruthR),aktR_jet_MCTrth_eta->at(iNearTruthJet));
=======
	   float deltaR10_Truth = deltaR_calc(akt10_pp_jet_eta_trth->at(iTruthR10), akt10_pp_jet_eta_trth->at(iNearTruthJet),akt10_pp_jet_phi_trth->at(iTruthR10),akt10_pp_jet_phi_trth->at(iNearTruthJet));
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf

	       if(debug_trth) cout << "We calculated the deltaR of nearby jet and the first reco jet: " << deltaR_Truth << endl;

<<<<<<< HEAD
	       if(deltaR_Truth <  deltaR_IsoCut) break;

	       if((iNearTruthJet+1 == aktR_jet_n_MCTrth)){
		 if(debug_trth) cout << "**************This truth is islated!************************" << endl;
		 if(debug) cout << "this is the index of your matched truth jet: " << iTruthR << endl;
		 isolatedTruthJetsIndex.push_back(iTruthR);
	       }
=======
	   if(deltaR10_Truth > deltaR_IsoCut) continue;

	   if((iNearTruthJet+1 == akt10_pp_jet_n_trth)){
	     //if(debug_trth) cout << "**************This reco is islated!************************" << endl;
	    if(debug) cout << "this is the index of your matched truth jet: " << iTruthR10 << endl;
	    isolatedTruthJetsIndex.push_back(iTruthR10);
          }
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf

	     }//----Looping over all the rest of the jets
	
<<<<<<< HEAD
	   }//---Looping over all jets. I am looking for an isolated jet

	   if((isolatedTruthJetsIndex.size() == 0) || (isolatedRecoJetsIndex.size() == 0)) continue; //----making sure I have at least one isolated reco and truth jets
     
      for(int iIsoRecoJets = 0; iIsoRecoJets < (int)isolatedRecoJetsIndex.size(); iIsoRecoJets++ ){
=======
      }//---Looping over all jets. I am looking for an isolated jet

      if((isolatedTruthJetsIndex.size() == 0) || (isolatedRecoJetsIndex.size() == 0)){continue;} //----making sure I have at least one isolated reco and truth jets
     
      for(int iIsoRecoJets = 0; iIsoRecoJets < (int)isolatedRecoJetsIndex.size(); iIsoRecoJets++ ){
        float deltaR_Smallest = 10000;
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf
	int matched_truth_index = -1;

	int reco_index = isolatedRecoJetsIndex.at(iIsoRecoJets); 
 
	for(int iIsoTruthJets = 0; iIsoTruthJets < (int)isolatedTruthJetsIndex.size(); iIsoTruthJets++){

	  int truth_index = isolatedTruthJetsIndex.at(iIsoTruthJets);

<<<<<<< HEAD
	  float deltaRTemp = deltaR_calc(aktR_jet_MCTrth_eta->at(truth_index),aktR_etaJES_jet_MCReco_eta->at(reco_index),aktR_jet_MCTrth_phi->at(truth_index),aktR_etaJES_jet_MCReco_phi->at(reco_index)); 
=======
	  float deltaRTemp = deltaR_calc(akt10_pp_jet_eta_trth->at(truth_index),akt10_reco_jet_etaJES_calib_eta->at(reco_index),akt10_pp_jet_phi_trth->at(truth_index),akt10_reco_jet_etaJES_calib_phi->at(reco_index)); 
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf
	  if(debug) cout << "This is the delta R between reco jet number " << iIsoRecoJets << " and truth jet number " << iIsoTruthJets << endl;
	  if(deltaR_MatchCut  > deltaRTemp){
	    matched_truth_index = truth_index;
	    
	  }

	}//-----Looping truth jets

        if(matched_truth_index == -1) continue;
	if(debug) cout << "**********************************WE FOUND A MATCH************************" << endl;
	
	for(int ipTRange = 0; ipTRange < totRanges; ipTRange++){	
	  
	  if((aktR_jet_MCTrth_pt->at(matched_truth_index) < pTRanges[ipTRange]) ||  (aktR_jet_MCTrth_pt->at(matched_truth_index) >  pTRanges[ipTRange+1]) ) continue;

	  //if((aktR_jet_MCTrth_pt->at(matched_truth_index) > 600) && (aktR_jet_MCTrth_pt->at(matched_truth_index) < 700))  cout << "The pT of this jet is between 600-700 GeV!" <<endl; 
	      

	      for(int iJZ = 0; iJZ < total_samples; iJZ++){
		
		if( (iEvent_pp_MC >  num_JZ_range_entries[iJZ]) && (iEvent_pp_MC <=  num_JZ_range_entries[iJZ+1])) {
		  float rationum = aktR_etaJES_jet_MCReco_pt->at(reco_index)/aktR_jet_MCTrth_pt->at(matched_truth_index);

    		  if((aktR_jet_MCTrth_pt->at(matched_truth_index) > 100) && (aktR_jet_MCTrth_pt->at(matched_truth_index)<200) && (rationum < 0.35)){
		  //if(1){
		    cout << "Mached Truth Jet: pT/eta/phi " << aktR_jet_MCTrth_pt->at(matched_truth_index) << " / " << aktR_jet_MCTrth_eta->at(matched_truth_index) << " / " << aktR_jet_MCTrth_phi->at(matched_truth_index) << endl;
		    cout << "Matched Reco Jet: pT/eta/phi " << aktR_etaJES_jet_MCReco_pt->at(reco_index) << " / " << aktR_etaJES_jet_MCReco_eta->at(reco_index) << " / " << aktR_etaJES_jet_MCReco_phi->at(reco_index) << endl;
		    
		    cout << endl;
		    cout << endl;
		    cout << "Printing all of reco jets " << endl;;
		    for(int iReco = 0; iReco < aktR_jet_n_MCReco; iReco++ ){
		      cout << " Index " << iReco  << " pT/eta/phi " << aktR_etaJES_jet_MCReco_pt->at(iReco) << " / " << aktR_etaJES_jet_MCReco_eta->at(iReco) << " / " << aktR_etaJES_jet_MCReco_phi->at(iReco) << endl;
		      for(int iNear =0; iNear < aktR_jet_n_MCReco; iNear++){
			if(iNear == iReco) continue;
			cout <<"Index: " << iNear << endl;
			cout << "This is the deltaR =  " << deltaR_calc(aktR_etaJES_jet_MCReco_eta->at(iReco),aktR_etaJES_jet_MCReco_eta->at(iNear),aktR_etaJES_jet_MCReco_phi->at(iReco), aktR_etaJES_jet_MCReco_phi->at(iNear)) << endl;
		      }//-----Looping over the rest of jets
		    }//----Looping over all reco jets

		    cout << "printing all of truth jets " << endl;
		    for(int iTruth = 0; iTruth < aktR_jet_n_MCTrth; iTruth++ ){
                      cout << " Index " << iTruth  << " pT/eta/phi " << aktR_jet_MCTrth_pt->at(iTruth) << " / " << aktR_jet_MCTrth_eta->at(iTruth) << " / " << aktR_jet_MCTrth_phi->at(iTruth) << endl;
		      for(int iNearT =0; iNearT < aktR_jet_n_MCTrth; iNearT++){
			if(iNearT == iTruth) continue;
			cout << "Index: " << iNearT << endl;
			cout << "This is the deltaR =  " << deltaR_calc(aktR_jet_MCTrth_eta->at(iTruth),aktR_jet_MCTrth_eta->at(iNearT),aktR_jet_MCTrth_phi->at(iTruth), aktR_jet_MCTrth_phi->at(iNearT)) << endl;
		      }//----Looping over the rest of jets
		    }//----Looping over all truth
		    
		    cout << "This is the list of jets that are isolated " << endl;
		    cout << "Truth Jets " << endl; 
		    for(int iIsoTruthJets = 0; iIsoTruthJets < (int)isolatedTruthJetsIndex.size(); iIsoTruthJets++){

		      int truth_index = isolatedTruthJetsIndex.at(iIsoTruthJets);
		      cout << "pT/eta/phi: " << aktR_jet_MCTrth_pt->at(truth_index) << " / " << aktR_jet_MCTrth_eta->at(truth_index) << " / " << aktR_jet_MCTrth_phi->at(truth_index) << endl;
		    }//-----Looping over isolated truth jets
		    

		    cout << "Reco Jets " << endl;
		    for(int iIsoRecoJets = 0; iIsoRecoJets < (int)isolatedRecoJetsIndex.size(); iIsoRecoJets++ ){
		      
		      int reco_index = isolatedRecoJetsIndex.at(iIsoRecoJets);
		      
		      cout << " pT/eta/phi " << aktR_etaJES_jet_MCReco_pt->at(reco_index) << " / " << aktR_etaJES_jet_MCReco_eta->at(reco_index) << " / " << aktR_etaJES_jet_MCReco_phi->at(reco_index) << endl;
		    }//----Looping over jets that are isolated 
		  }

		  if(iJetR == LargeR){
		    //cout << "we are in the pT range: " << pTRanges[ipTRange] << " - " << pTRanges[ipTRange+1] << endl;
		    /**
		    if((aktR_jet_MCTrth_pt->at(matched_truth_index) > 900) && (aktR_jet_MCTrth_pt->at(matched_truth_index) < 1000)){
		      cout << "trtuh pT "  << aktR_jet_MCTrth_pt->at(matched_truth_index) << endl;
		      cout << "we are in the pT range: " << pTRanges[ipTRange] << " - " << pTRanges[ipTRange+1] << endl;
		      cout << "ratio " << aktR_InsituCalib_jet_MCReco_pt->at(reco_index)/aktR_jet_MCTrth_pt->at(matched_truth_index) << endl;
		      cout << "weight :" << weights_ppMC[iJZ] << endl; 
		      }*/
		      pTReco_Truth_IsoR10[ipTRange]->Fill(aktR_etaJES_jet_MCReco_pt->at(reco_index)/aktR_jet_MCTrth_pt->at(matched_truth_index),weights_ppMC[iJZ]);
		    
		    //if((aktR_jet_MCTrth_pt->at(matched_truth_index) > 600) && (aktR_jet_MCTrth_pt->at(matched_truth_index) < 700)) cout << "Ratio for large r: " << aktR_InsituCalib_jet_MCReco_pt->at(reco_index)/aktR_jet_MCTrth_pt->at(matched_truth_index) << endl;

		  }

		  if(iJetR == JetR04){
		    
		    //cout << "trtuh pT "  << aktR_jet_MCTrth_pt->at(matched_truth_index) << endl;
		    pTReco_Truth_IsoR4[ipTRange]->Fill(aktR_etaJES_jet_MCReco_pt->at(reco_index)/aktR_jet_MCTrth_pt->at(matched_truth_index),weights_ppMC[iJZ]);
		    //cout << "Ratio for jets with r =0.4: " << aktR_InsituCalib_jet_MCReco_pt->at(reco_index)/aktR_jet_MCTrth_pt->at(matched_truth_index) << endl;
		  }
		}
		
	       }//------Looping over JZ samples
	    
	  
	  }//---Looping over pT ranges

       }//-----Matched Reco Jets and Truth Pass Through here    

      }//-----Event Loop      
   
<<<<<<< HEAD
 }//----Looping over different R's
=======
 }//----Event Loop
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf



    
}//----------pp MC






  if(data_pp){
    //----------------------------------------------------------------------------//
    //---------------------------------pp Data------------------------------------//
    //----------------------------------------------------------------------------//


    //----------------Initializing TH1D,TH2D,etc arrays

    
    //------Grabbing All Root Files
    tree_pp_Data->Add("/gpfs/mnt/atlasgpfs01/usatlas/data/bereniceg299/user.berenice.07202021.LargeRJet_pp2017_Data_GRL_Analysis.0000000000_myOutput.root/*.root");

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
    //----Trigger
    tree_pp_Data->SetBranchAddress("HLT_j110_a10_lcw_subjes_L1J30",&HLT_j110_a10_lcw_subjes_L1J30);
    tree_pp_Data->SetBranchAddress("HLT_j85",&HLT_j85);
    /**
    tree_pp_Data->SetBranchAddress("akt4hi_jet_n", &akt04hi_jet_n_ppData);
    tree_pp_Data->SetBranchAddress("akt4hi_jet_Insitu_calib_pt", &akt04_InsituCalib_jet_pp_pt);
    tree_pp_Data->SetBranchAddress("akt4hi_jet_Insitu_calib_eta", &akt04_InsituCalib_jet_pp_eta);
    tree_pp_Data->SetBranchAddress("akt4hi_jet_Insitu_calib_phi", &akt04_InsituCalib_jet_pp_phi);
    */
    /**
    tree_pp_Data->SetBranchAddress("akt3hi_jet_n", &akt03hi_jet_n_ppData);
    tree_pp_Data->SetBranchAddress("akt3hi_jet_Insitu_calib_pt", &akt03_InsituCalib_jet_pp_pt);
    tree_pp_Data->SetBranchAddress("akt3hi_jet_Insitu_calib_eta", &akt03_InsituCalib_jet_pp_eta);
    tree_pp_Data->SetBranchAddress("akt3hi_jet_Insitu_calib_phi", &akt03_InsituCalib_jet_pp_phi);
    */

    for(int iJetR = 0; iJetR < 2; iJetR++){
      

      cout << "This is the jet radius that we are looking at: " << jetRadius[iJetR] << endl;
      cout << "Printing out the names of my branches:" << Form("akt%ihi_jet_n",jetRadius[iJetR]) << " / " << Form("akt%ihi_jet_Insitu_calib_pt",jetRadius[iJetR]) << endl;
      tree_pp_Data->SetBranchAddress(Form("akt%ihi_jet_n",jetRadius[iJetR]), &aktRhi_jet_n_ppData);
      tree_pp_Data->SetBranchAddress(Form("akt%ihi_jet_Insitu_calib_pt",jetRadius[iJetR]), &aktR_InsituCalib_jet_pp_pt);
      tree_pp_Data->SetBranchAddress(Form("akt%ihi_jet_Insitu_calib_eta",jetRadius[iJetR]), &aktR_InsituCalib_jet_pp_eta);
      tree_pp_Data->SetBranchAddress(Form("akt%ihi_jet_Insitu_calib_phi",jetRadius[iJetR]), &aktR_InsituCalib_jet_pp_phi);
      
      int events_tot_pp_Data = tree_pp_Data->GetEntries();

      cout << "number of entries: " << 100   << endl;
 
      for(int iEvent_pp_Data = 0; iEvent_pp_Data < 1000; iEvent_pp_Data++){

	if (iEvent_pp_Data % 1000000 == 0) printf("entry: %lu\n", iEvent_pp_Data);



	tree_pp_Data->GetEntry(iEvent_pp_Data);


	if(!HLT_j110_a10_lcw_subjes_L1J30 || !HLT_j85){continue;}

	if(nvert < 2 ){continue;}

	for(int iRunNumber = 0; iRunNumber < totRuns; iRunNumber++){
	  if(RunNumber != runNumber[iRunNumber]) continue;

	  for(int iJet = 0; iJet < aktRhi_jet_n_ppData; iJet++){

	    if(aktR_InsituCalib_jet_pp_pt->at(iJet) < pTCut){continue;}
	    if(aktR_InsituCalib_jet_pp_eta->at(iJet) > etaCut || aktR_InsituCalib_jet_pp_eta->at(iJet) < -etaCut ){continue;}

	    if(iJetR == 0){
	      cout << "THE FIRST ONE" << endl;
	      cout << "This is the radius we are looking at: " << jetRadius[iJetR] << endl;
	      cout << "pT of jet: " << aktR_InsituCalib_jet_pp_pt->at(iJet) << endl;
	      jet_pT_dis_run[iRunNumber][iJetR]->Fill(aktR_InsituCalib_jet_pp_pt->at(iJet));
	    }else{
	      cout << "THE FIRST SEGUNDO!!" << endl;
	      cout << "This is the radius we are looking at: " << jetRadius[iJetR] << endl;
	      cout << "pT of jet: " << aktR_InsituCalib_jet_pp_pt->at(iJet) << endl;
	      jet_pT_dis_run[iRunNumber][iJetR]->Fill(aktR_InsituCalib_jet_pp_pt->at(iJet));
	    }
	    jetpTDis[iJetR][WOSumpT]->Fill(aktR_InsituCalib_jet_pp_pt->at(iJet));


	    /**
	    float sum_trk_pT_value = 0;

	    for(int iVertex = 0; iVertex < nvert; iVertex++){

	      if(vert_type[iVertex] != 1){continue;}

	      float dR_trk_jet = 0;

	      for(int iTrack = 0; iTrack < ntrk; iTrack++){
		dR_trk_jet = deltaR_calc(trk_eta->at(iTrack), akt03_InsituCalib_jet_pp_eta->at(iJet),trk_phi->at(iTrack),akt03_InsituCalib_jet_pp_phi->at(iJet) );

		if(dR_trk_jet > 1){continue;}
		if(trk_pt->at(iTrack) < pTtrackCut){continue;}

		sum_trk_pT_value = trk_pt->at(iTrack) + sum_trk_pT_value;

	      }//--Looping over all tracks
	    }//--Looping over all verticies
	    
	    if(sum_trk_pT_value > sumpTCut){
	      jet_pTDisSumpT[iRunNumber][iJetR]->Fill(akt03_InsituCalib_jet_pp_pt->at(iJet));
	      
	      jetpTDis[iJetR][WSumpT]->Fill(akt03_InsituCalib_jet_pp_pt->at(iJet));
	    }
	    */
	  }//----Looping over all of jets

	}//------Looping over all run numbers 

      
      //---------------------------------------------------------------------------//
      //-----------------------------------R=1.0-----------------------------------//
      //---------------------------------------------------------------------------//
      /**
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
      */
      }//-----Event Loop

    }//-----Looping over differe R's

    
  }//-----------pp Data

  //---------------------------------------------------------------------//
  //-------------------Filling in my histogram root file-----------------//
  //---------------------------------------------------------------------//

  //TFile *file_root = new TFile("new_histograms.root","RECREATE");
  TFile *file_root = new TFile("test_histograms.root","RECREATE");
  file_root->cd();
  
  if(data_pp){  
    //---Writing my histograms to file
    write_TH1D_1DArray_to_file(eta_dis_SumpT10,4,"eta_phi_pp_data",eta_pT_slices);
  
    for(int iJetR = 0; iJetR < tot_jetRadii; iJetR++){
      for(int iRun = 0; iRun < totRuns; iRun++){
	jet_pT_dis_run[iRun][iJetR]->Write(Form("RunNumber_%d_R_%d",runNumber[iRun],jetRadius[iJetR]),TObject::kOverwrite); 
	jet_pTDisSumpT[iRun][iJetR]->Write(Form("RunNumber_%d_R_%d_sumpTCut",runNumber[iRun],jetRadius[iJetR]),TObject::kOverwrite);
      }
    }//---Creating Jet/Lumi Plots For R=0.2,0.3,0.4,1.0 Jets


    for(int iSumpT = 0; iSumpT < sumpTWO_With; iSumpT++){
      for(int iJetR =0; iJetR < tot_jetRadii; iJetR++){
        jetpTDis[iJetR][iSumpT]->Write(Form("pTDis_R%d_%s",jetRadius[iJetR],SumpT[iSumpT].c_str()),TObject::kOverwrite);

      }//------Looping over Radii
    }//-------Looping over sumpt cut (with and without)

 
  }//-------pp Data

 
   if(mc_pp){
     if(debug_write) cout << "We will now write your histograms!! " << endl;
<<<<<<< HEAD

     write_TH1D_1DArray_to_file(pTReco_Truth_R10,totRanges,"JER_R10",pTRanges);
     write_TH1D_1DArray_to_file(pTReco_Truth_IsoR10,totRanges,"JER_R10_IsoJet", pTRanges);
     
     write_TH1D_1DArray_to_file(pTReco_Truth_R4,totRanges, "JER_R4",pTRanges);
     write_TH1D_1DArray_to_file(pTReco_Truth_IsoR4,totRanges, "JER_R4_IsoJet",pTRanges);


=======
     write_TH1D_1DArray_to_file(pTReco_Truth_R10,totRanges,"JER_R10",pTRanges);
     write_TH1D_1DArray_to_file(pTReco_Truth_IsoR10,totRanges,"JER_R10_IsoJet", pTRanges);
>>>>>>> b318532d1d0a2759e2e785dfcfdefc6f81595baf
   }//-----pp MC
 

  return 0;
}
