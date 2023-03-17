#include "TCut.h"

// BINNING
const int N_Q2  = 3;
const int N_Nu  = 3;
const int N_Zh  = 8;
const int N_Pt2 = 90;
const int N_Phi = 12;

const float Q2_limits[N_Q2+1]   = { 1.0 , 1.3 , 1.8 , 4.0 };
const float Nu_limits[N_Nu+1]   = { 2.2 , 3.2 , 3.7 , 4.26};
const float Zh_limits[N_Zh+1]   = { 0.0 , 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.8 , 1.0 };

const float Pt2_min   = 0.;
const float Pt2_max   = 3.;
const float delta_Pt2 = (Pt2_max-Pt2_min)/N_Pt2;

const float Phi_min   = -180.;
const float Phi_max   =  180.;
const float delta_Phi = (Phi_max-Phi_min)/N_Phi;

// Additional cuts
TCut dat_add_cut[2] = { "TMath::Abs(YC)<1.4" , "TMath::Abs(deltaZ)<3." };
TCut rec_add_cut[2] = { "TMath::Abs(YC)<1.4" , "TMath::Abs(deltaZ)<3." };
TCut thr_add_cut[1] = { "TMath::Abs(deltaZ)<3." };