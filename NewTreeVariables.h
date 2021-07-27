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



 /**
  * Returns a linearly spaced array. The 0th element is lo, and the num-th element is hi.
  */
double* linspace (double lo, double hi, int num) {
  double* arr = new double[num+1];
  double delta = ((double)(hi)-(double)(lo))/(double)(num);
  for (int i = 0; i <= num; i++) {
    arr[i] = lo + i * delta;
  }
  return arr;
}


/**
 181  * Returns a logarithmically spaced array, where the 0th element is lo and the num-th element is hi.
 182  */
double* logspace (double lo, double hi, int num) {
  double loghi = TMath::Log2(hi);
  if (lo == 0) {
    double* arr = linspace(TMath::Log2(hi/(100*num)), loghi, num);
    for (int i = 0; i <= num; i++) {
      arr[i] = TMath::Power(2, arr[i]);
    }
    return arr;
  } else {
    double loglo = TMath::Log2(lo);
    double* arr = linspace(loglo, loghi, num);
    for (int i = 0; i <= num; i++) {
      arr[i] = TMath::Power(2, arr[i]);
    }
    return arr;
  }
}


//-----What Are You Going To Run Over? 
bool mc_pp = false; ///true if you are running over pp mc
bool data_pp = true; //true if you are running over pp data
bool data_PbPb = false; //true if you are running over Pb+Pb Data
bool mc_PbPb = false; //true if you are running over Pb+Pb MC

//-----When debugging--------// 
bool debug_reco = false;
bool debug_trth = false;
bool debug = false;
bool debug_filled_histo = true;
bool debug_write = true;

//----Tree
TChain *tree = new TChain("tree");
TChain *tree_pp_Data = new TChain("tree");
TChain *tree_pp_03Data = new TChain("tree");
TChain *tree_pp_MC = new TChain("tree");
TChain *tree_pythia = new TChain("tree");


//------Vectors for branches

//------------------------------------------------------------------------//
//-------------------------------pp Data----------------------------------//
//------------------------------------------------------------------------//

//--------Tracks
const float pTtrackCut = 4; //GeV 

const int v_size = 100;
float vert_sumpt[v_size];
int vert_ntrk[v_size];
int vert_type[v_size];
int ntrk =0;

std::vector<float> *trk_phi = NULL;
std::vector<float> *trk_eta = NULL;
std::vector<float> *trk_pt = NULL;



//------Vertix, Triggers, Run Number 
int nvert = 0;
int nvert3 = 0;
bool HLT_j110_a10_lcw_subjes_L1J30_3 = 0;
bool HLT_j110_a10_lcw_subjes_L1J30 = 0;
bool HLT_j85 = false;
int RunNumber = 0;


//-----Jet pT Cut
const float pTCut = 160; //GeV 
//-----Jet eta Cut
const float etaCut = 2.2; 

//----Total number of jets for each different radii
int akt10hi_jet_n_ppData = 0;
int akt04hi_jet_n_ppData = 0;
int akt03hi_jet_n_ppData = 0;
int akt02hi_jet_n_ppData = 0;

//-----Jets 
int aktRhi_jet_n_ppData = 0;
std::vector<float> *aktR_InsituCalib_jet_pp_pt = NULL;
std::vector<float> *aktR_InsituCalib_jet_pp_phi = NULL;
std::vector<float> *aktR_InsituCalib_jet_pp_eta = NULL;
std::vector<float> *aktR_InsituCalib_jet_pp_e = NULL;


//----Jets w/ R=0.2
std::vector<float> *akt02_pp_jet_pt = NULL;
std::vector<float> *akt02_pp_jet_eta = NULL;
std::vector<float> *akt02_pp_jet_phi = NULL;
std::vector<float> *akt02_pp_jet_e = NULL;

//----Calibrated Jets
std::vector<float> *akt02_InsituCalib_jet_pp_pt = NULL;
std::vector<float> *akt02_InsituCalib_jet_pp_phi = NULL;
std::vector<float> *akt02_InsituCalib_jet_pp_eta = NULL;
std::vector<float> *akt02_InsituCalib_jet_pp_e = NULL;

//----Jets w/ R=0.3

//----Calibrated Jets
std::vector<float> *akt03_InsituCalib_jet_pp_pt = NULL;
std::vector<float> *akt03_InsituCalib_jet_pp_phi = NULL;
std::vector<float> *akt03_InsituCalib_jet_pp_eta = NULL;
std::vector<float> *akt03_InsituCalib_jet_pp_e = NULL;


//----Jets w/ R=0.4
std::vector<float> *LooseBad_04 = NULL;
std::vector<float> *TightBad_04 = NULL;

std::vector<float> *akt04_pp_jet_pt = NULL;
std::vector<float> *akt04_pp_jet_eta = NULL;
std::vector<float> *akt04_pp_jet_phi = NULL;
std::vector<float> *akt04_pp_jet_e = NULL;

//----Calibrated Jets
std::vector<float> *akt04_InsituCalib_jet_pp_pt = NULL;
std::vector<float> *akt04_InsituCalib_jet_pp_phi = NULL;
std::vector<float> *akt04_InsituCalib_jet_pp_eta = NULL;
std::vector<float> *akt04_InsituCalib_jet_pp_e = NULL;



//----Jets w/ R = 1.0
std::vector<bool> *LooseBad_10 = NULL;
std::vector<bool> *TightBad_10 = NULL;

std::vector<float> *akt10_pp_jet_pt = NULL;
std::vector<float> *akt10_pp_jet_eta = NULL;
std::vector<float> *akt10_pp_jet_phi = NULL;
std::vector<float> *akt10_pp_jet_e = NULL;

//----Calibrated Jets
std::vector<float> *akt10_InsituCalib_jet_pp_pt = NULL;
std::vector<float> *akt10_InsituCalib_jet_pp_phi = NULL;
std::vector<float> *akt10_InsituCalib_jet_pp_eta = NULL;
std::vector<float> *akt10_InsituCalib_jet_pp_e = NULL; 

//-----Enum 
enum Radii {Rad10,Rad4,Rad3,Rad2};


//----Jets per Lumi Block plot
={4,3};const int totRuns = 13;
const int tot_jetRadii = 2; 
int jetRadius[tot_jetRadii] ={4,3};
int runNumber[] = {341184,341123,341027,340973,340925,340910,340850,340849,340814,340718,340697,340683,340644};

//----JetpT Distributions
int sumpTWO_With =2; 
int sumpTCut = 10; //GeV
enum SumpT_TF{WOSumpT, WSumpT}; //---with sumpT Cut and without it 
string SumpT[]={"WOSumpT","WSumpT"};

//----Eta Distributions & Eta-Phi Plots For Jets w/ SumpT > 10 GeV
const int ends_of_pTSlices = 5;
const int pTSlices_EtaPlots = 4;
int eta_pT_slices[ends_of_pTSlices] =  {400,600,800,1000,1200}; //-----4 different pT slices 
int sumpT_Trk_pTSlices[pTSlices_EtaPlots]={600,800,1000,1300};




//------------------------------------------------------------------------//
//---------------------------------pp MC----------------------------------//
//------------------------------------------------------------------------//

//----Jets R's
enum MCRadii {R10,R4};
const int Tot_Radii = 2;
int JetRadius[] = {10,4};

//-----Vectors
//-----Jets
int aktR_jet_n_MCReco = 0;
std::vector<float> *aktR_etaJES_jet_MCReco_pt = NULL;
std::vector<float> *aktR_etaJES_jet_MCReco_phi = NULL;
std::vector<float> *aktR_etaJES_jet_MCReco_eta = NULL;
std::vector<float> *aktR_etaJES_jet_MCReco_e = NULL;

int aktR_jet_n_MCTrth = 0;
std::vector<float> *aktR_jet_MCTrth_pt = NULL;
std::vector<float> *aktR_jet_MCTrth_phi = NULL;
std::vector<float> *aktR_jet_MCTrth_eta = NULL;
std::vector<float> *aktR_MCTrth_e = NULL;

//------Jet Cuts
const int pTTruthCut = 40; //GeV
const int pTRecoCut = 20; //GeV

//------deltaR
const float deltaR_IsoCut = 2.3; //-----this is the delta R use to make sure my jet is isolated
std::vector<int> isolatedRecoJetsIndex;
std::vector<int> isolatedTruthJetsIndex;

const float deltaR_MatchCut = 0.8; //-----this is the deltaR used to match my truth jet to a reco jet  

//------Save the number of event for all the JZ samples
const int total_samples = 4;
const int jz_entry_ranges = total_samples+1;
int num_JZ_range_entries[jz_entry_ranges] = {};

//-----pp MC Weights
float sigma_a_pp = (6.399*pow(10,5));
float eps_a_pp = (4.2785*pow(10,-3));
float num_events_a_pp = 7998000;

float sigma_d_pp = (4.7195*pow(10,3));
float eps_d_pp = (5.2994*pow(10,-3));
float num_events_d_pp = 7999000;

float sigma_e_pp = (2.6602*pow(10,1));
float eps_e_pp = (4.5901*pow(10,-3));
float num_events_e_pp = 8000000;

float sigma_f_pp = (2.2476*pow(10,-1));
float eps_f_pp = (2.1846*pow(10,-3));
float num_events_f_pp = 7999000;

//--Calculating Weights
const int tot_weights_ppMC = 4;
float weight_0_pp = (eps_a_pp*sigma_a_pp)/(num_events_a_pp);
float weight_1_pp = (eps_d_pp*sigma_d_pp)/(num_events_d_pp);
float weight_2_pp = (eps_e_pp*sigma_e_pp)/(num_events_e_pp);
float weight_3_pp = (eps_f_pp*sigma_f_pp)/(num_events_f_pp);

float weights_ppMC[tot_weights_ppMC] = {weight_0_pp,weight_1_pp,weight_2_pp,weight_3_pp};

//------Verticies and Tracks
int nvert_ppMC = 0;
float vert_sumpt_reco[v_size];
int vert_ntrk_reco[v_size];
int vert_type_reco[v_size];
int ntrk_reco = 0;
//----MC Reco
std::vector<float> *trk_phi_reco = NULL;
std::vector<float> *trk_eta_reco = NULL;
std::vector<float> *trk_pt_reco = NULL;


//------R=1.0
//-------Total Number of Reco Jets
int akt10_pp_jet_n_reco = 0;
int akt10_pp_jet_n_trth = 0;
//-------Reco Vectors
std::vector<float> * akt10_reco_jet_etaJES_calib_pt = NULL;
std::vector<float> * akt10_reco_jet_etaJES_calib_phi = NULL;
std::vector<float> * akt10_reco_jet_etaJES_calib_eta = NULL;

std::vector<float> * akt10_reco_jet_Insitu_calib_pt = NULL;
std::vector<float> * akt10_reco_jet_Insitu_calib_eta = NULL;
std::vector<float> * akt10_reco_jet_Insitu_calib_e = NULL;
std::vector<float> * akt10_reco_jet_Insitu_calib_phi = NULL;

//------Truth Vectors
std::vector<float> *akt10_pp_jet_pt_trth = NULL;
std::vector<float> *akt10_pp_jet_eta_trth = NULL;
std::vector<float> *akt10_pp_jet_e_trth = NULL;
std::vector<float> *akt10_pp_jet_phi_trth = NULL;

//-----R=0.4
//--------Total Number of Truth Jets
int akt4_pp_jet_n_reco = 0;
int akt4_pp_jet_n_trth = 0;
//--------Reco Vectors
std::vector<float> * akt4_reco_jet_etaJES_calib_pt = NULL;
std::vector<float> * akt4_reco_jet_etaJES_calib_phi = NULL;
std::vector<float> * akt4_reco_jet_etaJES_calib_eta = NULL;

//-----Truth Vectors
std::vector<float> *akt4_pp_jet_pt_trth = NULL;
std::vector<float> *akt4_pp_jet_eta_trth = NULL;
std::vector<float> *akt4_pp_jet_e_trth = NULL;
std::vector<float> *akt4_pp_jet_phi_trth = NULL;

//----Jet Energy Resolution and Jet Energy Scale 
//-----R=1.0
const int totRanges = 12;
TH1D* pTReco_pTTruth_Dis[totRanges];
const int end_of_Ranges = 13;
int pTRanges[end_of_Ranges] = {100,200,300,400,500,600,700,800,900,1000,1100,1200,1300};
