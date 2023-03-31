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

    // Create file the rad factors will be stored
    std::string output_file_name = rad_result_dir+"rcfactors"+targets[vertex_cut_value-1][dat_target_index]+"_"+
                                   std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+".root";
    TFile* foutput = new TFile(output_file_name.c_str(),"RECREATE");
    gROOT->cd();

    // Open the file with the centroids
    std::string centroids_file_name = rad_result_dir+"centroids"+targets[vertex_cut_value-1][dat_target_index]+"_"+
                                      std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+".root";
    TFile* fcentroids = new TFile(centroids_file_name.c_str());

    // Obtain the tuple that contains the centroids
    float Q2_centroid, Xb_centroid, Zh_centroid, Pt_centroid, Phi_centroid, Q2_centroid_bin, Xb_centroid_bin, Zh_centroid_bin, Pt_centroid_bin, Phi_centroid_bin;
    TNtuple* centroids = (TNtuple*) fcentroids->Get("centroids_data");
    centroids->SetBranchAddress("Q2"     , &Q2_centroid     );
    centroids->SetBranchAddress("Xb"     , &Xb_centroid     );
    centroids->SetBranchAddress("Zh"     , &Zh_centroid     );
    centroids->SetBranchAddress("Pt"     , &Pt_centroid     );
    centroids->SetBranchAddress("Phi"    , &Phi_centroid    );
    centroids->SetBranchAddress("Q2_bin" , &Q2_centroid_bin );
    centroids->SetBranchAddress("Xb_bin" , &Xb_centroid_bin );
    centroids->SetBranchAddress("Zh_bin" , &Zh_centroid_bin );
    centroids->SetBranchAddress("Pt2_bin", &Pt_centroid_bin );
    centroids->SetBranchAddress("Phi_bin", &Phi_centroid_bin);

    // Create the TNtuple that will contain the rc factors
    TNtuple *rcfactors_tuple = new TNtuple(ntuple_rad_name,ntuple_rad_name,"rc1:rc3:Q2_bin:Nu_bin:Zh_bin:Pt2_bin:Phi_bin"); 

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

    newphihist_targ = targets[vertex_cut_value-1][dat_target_index];

    for(int entry = 0 ; entry < centroids->GetEntries() ; entry++)
    {
        centroids->GetEntry(entry);

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
	            rcfactors_tuple->Fill(-1,-1,Q2_centroid_bin,Xb_centroid_bin,Zh_centroid_bin,Pt_centroid_bin,Phi_centroid_bin-1);
	            continue;
	        }

	        rc.CalculateRCFactor(5.015, Xb_centroid, Q2_centroid, Zh_centroid, Pt_centroid, Phi_centroid, m, NAZ);
	        f1 = rc.GetFactor1();
	        f2 = rc.GetFactor2();
	        f3 = rc.GetFactor3();
	        if(TMath::IsNaN(f1) || f1 == a3) f1 = 0;
	        if(TMath::IsNaN(f2) || f2 == a3) f2 = 0;
	        if(TMath::IsNaN(f3) || f3 == a3) f3 = 0;
	        rcfactors_tuple->Fill(f1,f3,Q2_centroid_bin,Xb_centroid_bin,Zh_centroid_bin,Pt_centroid_bin,Phi_centroid_bin);
	        std::cout<<"Q2="<<Q2_centroid<<" || Xb="<<Xb_centroid<<" || Zh="<<Zh_centroid<<" || Pt="<<Pt_centroid<<std::endl;
	        std::cout<<"RC without exc contribution      = "<<f1<<std::endl;
	        std::cout<<"RC with exc contribution         = "<<f2<<std::endl;
	        std::cout<<"RC with exc contribution div NAZ = "<<f3<<std::endl;
	        std::cout<<"________________________________________________________________________________________________"<<std::endl;
            
        }
    }
    
    //Store the TNtuple
    foutput->cd();
    rcfactors_tuple->Write();
    gROOT->cd();

    // Close the TFiles
    foutput->Close();
    fcentroids->Close();

    return 1;
}
