#include <iostream>
#include <string>
#include "TFile.h"
#include "TROOT.h"
#include "TNtuple.h"
#include "TH1F.h"
#include "TF1.h"
#include "names.h"
#include "utils.h"
#include "utils-names.h"
#include "analysis-constants.h"

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
    
    // Create file that will contain the tntuple and the fits
    TFile* fout = new TFile(get_fitphipq_file_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin).c_str(),"RECREATE");
    gROOT->cd();

    // Open file that contains acceptance corrected phipq
    TFile* facc = new TFile(get_acc_file_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin).c_str());
    
    // Open file that contains centroids
    TFile* fctr = new TFile(get_ctr_file_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin).c_str());

    if(!facc->IsOpen()||!fctr->IsOpen()){std::cout<<"ERROR! Files were not opened!"<<std::endl; return 1;}

    // Obtain the tuple that contains the centroids
    TNtuple* ntuple_ctr = (TNtuple*) fctr->Get(ntuple_ctr_name);

    // Set the branches of the centroids TNtuple
    float Q2_centroid, Xb_centroid, Zh_centroid, Pt_centroid, Phi_centroid;
    float Q2_centroid_bin, Xb_centroid_bin, Zh_centroid_bin, Pt_centroid_bin, Phi_centroid_bin;
    set_ctr_branches(&Q2_centroid, &Xb_centroid, &Zh_centroid, &Pt_centroid, &Phi_centroid, &Q2_centroid_bin, &Xb_centroid_bin, &Zh_centroid_bin, &Pt_centroid_bin, &Phi_centroid_bin, ntuple_ctr);
    
    // Define the fitting function
    TF1* fit_func = new TF1("fit_func", "[0]+TMath::Cos(x*TMath::Pi()/180)*[1]+TMath::Cos(2*x*TMath::Pi()/180)*[2]");

    // Define the tuple that will contain the info obtained from the fits
    TNtuple* ntuple_fit = new TNtuple("AAcAcc_data", "AAcAcc_data", "Q2:Xb:Zh:Pt:A:AErr:Ac:AcErr:Acc:AccErr:ChiSQ");

    // Associate the TNuple with the output directory
    ntuple_fit->SetDirectory(fout);

    // Start the fits
    for(int Pt2_bin = 0 ; Pt2_bin < N_Pt2 ; Pt2_bin++)
    {   
        // Obtain the acceptance corrected Phi histo
        TH1F* h = (TH1F*) facc->Get(get_acccorr_histo_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin,Pt2_bin).c_str());
        
        // Apply condition of minimum number of bins
        if(get_filled_bins(h)<4) {delete h; continue;}
        
        // Fit the plot
        h->Fit(fit_func, "q");
        
        // Get and store the variables of the fit
        double ChiSQ  = fit_func->GetChisquare();
        double A      = fit_func->GetParameter(0);
        double AErr   = fit_func->GetParError(0);
        double Ac     = fit_func->GetParameter(1);
        double AcErr  = fit_func->GetParError(1);
        double Acc    = fit_func->GetParameter(2);
        double AccErr = fit_func->GetParError(2);
        double ndf    = fit_func->GetNDF();
        
        if(ndf!=0)
        {
            for(int entry = 0 ; entry < ntuple_ctr->GetEntries() ; entry++)
            {
                ntuple_ctr->GetEntry(entry);
                if(Q2_bin==Q2_centroid_bin&&Nu_bin==Xb_centroid_bin&&Zh_bin==Zh_centroid_bin&&Pt2_bin==Pt_centroid_bin)
                {
                    // Write the histogram with the fit
                    fout->cd();
                    h->Write((const char*) Form("PhiDist Q2=%.3f Xb=%.3f Zh=%.3f Pt=%.3f", Q2_centroid, Xb_centroid, Zh_centroid, Pt_centroid));
                    gROOT->cd();
            
                    // Write the entry in the TNtuple
                    ntuple_fit->Fill(Q2_centroid, Xb_centroid, Zh_centroid, Pt_centroid, A, AErr, Ac, AcErr, Acc, AccErr, ChiSQ);

                    break;
                }
            }
        }

        delete h;
    }

    // Write the TNtuple
    fout->cd();
    ntuple_fit->Write();
    gROOT->cd();

    // Close the TFiles!
    facc->Close();
    fctr->Close();
    fout->Close();

    return 1;
}