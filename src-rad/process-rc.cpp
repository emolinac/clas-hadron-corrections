#include <iostream>
#include "TFile.h"
#include "TROOT.h"
#include "TNtuple.h"
#include "TCut.h"
#include "names.h"
#include "analysis-constants.h"

int main(int argc, char* argv[])
{
    // Check number of arguments is correct
    if(argc!=3) {std::cout<<"Incorrect number of argument!"<<std::endl; return 1;}

    // Target settings
    int dat_target_index = std::stoi(argv[1]);
    int vertex_cut_value = std::stof(argv[2]);

    // Open the file with the RC factors TNtuples
    TFile* fin = new TFile((rad_result_dir+"rcfactors"+targets[vertex_cut_value-1][dat_target_index]+".root").c_str());

    // Create a TFile to store the final TNtuple
    TFile* fout = new TFile((rad_result_dir+"rcfactors"+targets[vertex_cut_value-1][dat_target_index]+"_processed.root").c_str(),"RECREATE")
    gROOT->cd();

    // Obtain the TNtuple
    float Q2_bin, Nu_bin, Zh_bin, Pt2_bin, Phi_bin, rc1, rc3;
    TNtuple* rcfactors_tuple_original = (TNtuple*) fin->Get(ntuple_rc_name);
    rcfactors_tuple_original->SetBranchAddress("Q2_bin"  ,&Q2_bin  );
    rcfactors_tuple_original->SetBranchAddress("Nu_bin"  ,&Nu_bin  );
    rcfactors_tuple_original->SetBranchAddress("Zh_bin"  ,&Zh_bin  );
    rcfactors_tuple_original->SetBranchAddress("Pt2_bin" ,&Pt2_bin );
    rcfactors_tuple_original->SetBranchAddress("Phi_bin" ,&Phi_bin );
    rcfactors_tuple_original->SetBranchAddress("rc1"     ,&rc1     );
    rcfactors_tuple_original->SetBranchAddress("rc3"     ,&rc3     );

    // Create TNtuple of processed RC factors
    TNtuple* rcfactors_tuple = new TNtuple(ntuple_rc_name,ntuple_rc_name,"rc1:rc3:rc1_corr:rc3_corr:rc1_int:rc3_int:Q2_bin:Nu_bin:Zh_bin:Pt2_bin:Phi_bin");

    // Declare instrumental histogram
    TH1F* h = new TH1F("h","",200,0.5,1.5);

    // Start the correction process
    for(int Q2_bin_loop = 0 ; Q2_bin_loop < N_Q2 ; Q2_bin_loop++)
    {
        for(int Nu_bin_loop = 0 ; Nu_bin_loop < N_Nu ; Nu_bin_loop++)
        {
            for(int Zh_bin_loop = 0 ; Zh_bin_loop < N_Zh ; Zh_bin_loop++)
            {
                for(int Phi_bin_loop = 0 ; Phi_bin_loop < N_Phi ; Phi_bin_loop++)
                {
                    // Obtain histo with RC factors in (Q2,Nu,Zh,Phi) bin
                    TCut cut_loop = Form("Q2_bin==%i&&Nu_bin==%i&&Zh_bin==%i&&Phi_bin==%i",Q2_bin_loop,Nu_bin_loop,Zh_bin_loop,Phi_bin_loop);
                    rcfactors_tuple_original->Project("h","rc_factor_1",cut_loop);

                    // Obtain statistical properties
                    double mean   = h->GetMean();
                    double stddev = h->GetStdDev();

                    
                }
            }
        }
    }
}