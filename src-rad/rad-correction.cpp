#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <map>
#include "TString.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TChain.h"
#include "TCut.h"
#include "TH1.h"
#include "TF1.h"
#include "TRadCor.h"
#include "TMath.h"
#include "haprad_constants.h"
#include "names.h"
#include "utils.h"
#include "utils-names.h"
#include "analysis-constants.h"

std::string newphihist_targ;

int main(int argc, char* argv[])
{
    // Check number of arguments is correct
    if(argc!=6) {std::cout<<"Incorrect number of argument!"<<std::endl; return 1;}

    // Target settings
    int dat_target_index = std::stoi(argv[1]);
    int vertex_cut_value = std::stof(argv[2]);

    // Q2, Nu, Zh bin settings
    int Q2_bin = std::stoi(argv[3]);
    int Nu_bin = std::stoi(argv[4]);
    int Zh_bin = std::stoi(argv[5]);

    // Create file where the fully-corrected data will be stored
    TFile* fout = new TFile(get_fullcorr_file_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin).c_str(),"RECREATE");
    gROOT->cd();

    // Open the file with the acceptance-corrected data
    TFile* facc = new TFile(get_acc_file_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin).c_str());

    // Open the file with the centroids
    TFile* fctr = new TFile(get_ctr_file_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin).c_str());

    // Obtain the tuple that contains the centroids
    TNtuple* ntuple_ctr = (TNtuple*) fctr->Get(ntuple_ctr_name);

    // Set the branches of the centroids TNtuple
    float Q2_centroid, Xb_centroid, Zh_centroid, Pt_centroid, Phi_centroid;
    float Q2_centroid_bin, Xb_centroid_bin, Zh_centroid_bin, Pt_centroid_bin, Phi_centroid_bin;
    set_ctr_branches(&Q2_centroid, &Xb_centroid, &Zh_centroid, &Pt_centroid, &Phi_centroid, &Q2_centroid_bin, &Xb_centroid_bin, &Zh_centroid_bin, &Pt_centroid_bin, &Phi_centroid_bin, ntuple_ctr);
    
    // Create the TNtuple that will contain the rc factors
    TNtuple *ntuple_rad = new TNtuple(ntuple_rad_name,ntuple_rad_name,"rc1:rc3:Q2_bin:Nu_bin:Zh_bin:Pt2_bin:Phi_bin"); 

    // Target proton to neutron ratio
    double NAZ = 0;
    assign_n2p_ratio(NAZ, targets[vertex_cut_value-1][dat_target_index]);

    double f1, f2, f3;
    double m = TMath::Power((kMassNeutron+kMassPion), 2);
    double a1 = 1;
    double a2 = 0;
    double a3 = a1/a2;
    
    int sysReturn;

    TRadCor rc;

    // This variable is necessary to set the names of the fit phi pq files to be opened in the TSemiInclusiveModel.cxx file
    newphihist_targ = targets[vertex_cut_value-1][dat_target_index];

    for(int entry = 0 ; entry < ntuple_ctr->GetEntries() ; entry++)
    {
        //
        ntuple_ctr->GetEntry(entry);

        if(Q2_centroid_bin==Q2_bin&&Xb_centroid_bin==Nu_bin&&Zh_centroid_bin==Zh_bin)
        {
            // Define variables to have Mx2 ready.
            double S_x    = Q2_centroid / Xb_centroid;//HH: check if it is correct
	        double Nu2    = TMath::Power(S_x/2./kMassProton,2);
	        double PlQ2NU = TMath::Sqrt((Nu2 + S_x*Xb_centroid)*(TMath::Power(Zh_centroid,2)*Nu2 - TMath::Power(kMassPion,2) - TMath::Power(Pt_centroid,2)));
	        double t_temp = TMath::Power(kMassPion,2) - S_x*Xb_centroid + 2*(PlQ2NU - Nu2*Zh_centroid);
	        double Mx2    = TMath::Power(kMassProton,2) + S_x * (1 - Zh_centroid) + t_temp;//HH: check if T is the correct one, if the evaluate is called correctly
            
            // If missing mass condition is not ful
	        if(Mx2<m)
            {
                std::cout<<"Missing mass condition not fulfilled! Setting rc factor to -1"<<std::endl;
	            ntuple_rad->Fill(-1,-1,Q2_centroid_bin,Xb_centroid_bin,Zh_centroid_bin,Pt_centroid_bin,Phi_centroid_bin-1);
	            continue;
	        }

	        rc.CalculateRCFactor(5.015, Xb_centroid, Q2_centroid, Zh_centroid, Pt_centroid, Phi_centroid, m, NAZ);
	        f1 = rc.GetFactor1();
	        f2 = rc.GetFactor2();
	        f3 = rc.GetFactor3();
	        if(TMath::IsNaN(f1) || f1 == a3) f1 = 0;
	        if(TMath::IsNaN(f2) || f2 == a3) f2 = 0;
	        if(TMath::IsNaN(f3) || f3 == a3) f3 = 0;
	        ntuple_rad->Fill(f1,f3,Q2_centroid_bin,Xb_centroid_bin,Zh_centroid_bin,Pt_centroid_bin,Phi_centroid_bin);            
        }
    }
    
    //Store the TNtuple
    fout->cd();
    ntuple_rad->Write();
    gROOT->cd();

    // Close the TFiles
    fout->Close();
    fctr->Close();

    return 1;
}
