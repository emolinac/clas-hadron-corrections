#include <iostream>
#include <string>
#include "TFile.h"
#include "TROOT.h"
#include "TNtuple.h"
#include "TH1F.h"
#include "TF1.h"
#include "names.h"
#include "utils.h"
#include "analysis-constants.h"

int main(int argc, char* argv[])
{
    // Target settings
    int dat_target_index = std::stoi(argv[1]);
    int vertex_cut_value = std::stof(argv[2]);

    // Q2, Nu, Zh bin settings
    int Q2_bin = std::stoi(argv[3]);
    int Nu_bin = std::stoi(argv[4]);
    int Zh_bin = std::stoi(argv[5]);

    // Check number of arguments is correct
    if(argc!=6) {std::cout<<"Incorrect number of argument!"<<std::endl; return 1;}

    // Create file that will contain the tntuple and the fits
    std::string fit_file_name = rad_result_dir+"newphihist"+dat_targets[dat_target_index]+"_VC"+std::to_string(vertex_cut_value)+"_"+
                                std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+".root";
    TFile* foutput = new TFile(fit_file_name.c_str(),"RECREATE");
    gROOT->cd();

    // Open file that contains acceptance corrected phipq
    std::string phi_file_name = acc_result_dir+"acc"+dat_targets[dat_target_index]+"_VC"+std::to_string(vertex_cut_value)+"_"+
                                std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+".root";
    
    TFile* fphi = new TFile(phi_file_name.c_str());
    
    // Open file that contains centroids
    std::string centroids_file_name = rad_result_dir+"centroids"+dat_targets[dat_target_index]+"_VC"+std::to_string(vertex_cut_value)+"_"+
                                      std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+".root";
    TFile* fcentroids = new TFile(centroids_file_name.c_str());

    // Obtain the tuple that contains the centroids
    float Q2_centroid, Xb_centroid, Zh_centroid, Pt_centroid, Phi_centroid, Q2_centroid_bin, Xb_centroid_bin, Zh_centroid_bin, Pt_centroid_bin, Phi_centroid_bin;
    TNtuple* centroids = (TNtuple*) fphi->Get("centroids_data");
    centroids->SetBranchAddress("Q2"     , &Q2_centroid     );
    centroids->SetBranchAddress("Xb"     , &Xb_centroid     );
    centroids->SetBranchAddress("Zh"     , &Zh_centroid     );
    centroids->SetBranchAddress("Pt"     , &Pt_centroid     );
    centroids->SetBranchAddress("Phi"    , &Phi_centroid    );
    centroids->SetBranchAddress("Q2_bin" , &Q2_centroid_bin );
    centroids->SetBranchAddress("Xb_bin" , &Xb_centroid_bin );
    centroids->SetBranchAddress("Zh_bin" , &Zh_centroid_bin );
    centroids->SetBranchAddress("Pt_bin" , &Pt_centroid_bin );
    centroids->SetBranchAddress("Phi_bin", &Phi_centroid_bin);

    // Define the fitting function
    TF1* fit_func = new TF1("fit_func", "[0]+TMath::Cos(x*TMath::Pi()/180)*[1]+TMath::Cos(2*x*TMath::Pi()/180)*[2]");

    // Define the tuple that will contain the info obtained from the fits
    TNtuple* fittuple = new TNtuple("AAcAcc_data", "AAcAcc_data", "Q2:Xb:Zh:Pt:A:AErr:Ac:AcErr:Acc:AccErr:ChiSQ");

    // Start the fits
    TH1F* h;
    for(int Pt2_bin = 0 ; Pt2_bin < N_Pt2 ; Pt2_bin++)
    {
        for(int Phi_bin = 0 ; Phi_bin < N_Phi ; Phi_bin++)
        {
            // Obtain the acceptance corrected Phi histo
            h = (TH1F*) fphi->Get((histo_corr+targets[vertex_cut_value-1][dat_target_index]+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin)).c_str());

            // Check the minimum number of non-empty bins to perform fit
            int not_empty_bins = get_filled_bins(h);

            // Apply condition of minimum number of bins
            if(not_empty_bins<4) continue;

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
                for(int entry = 0 ; entry < centroids->GetEntries() ; entry++)
                {
                    centroids->GetEntry(entry);
                    if(Q2_bin==Q2_centroid_bin&&Nu_bin==Xb_centroid_bin&&Zh_bin==Zh_centroid_bin&&Pt2_bin==Pt_centroid_bin)
                    {
                        // DELETE LATER
                        std::cout<<"A="<<A<<"  Ac="<<Ac<<"   Acc="<<Acc<<std::endl;
                        // Write the histogram with the fit
                        foutput->cd();
                        h->Write((const char*) Form("PhiDist Q2=%.3f Xb=%.3f Zh=%.3f Pt2=%.3f", Q2_centroid, Xb_centroid, Zh_centroid, Pt_centroid));
                        gROOT->cd();
                
                        // Write the entry in the TNtuple
                        fittuple->Fill(Q2_centroid, Xb_centroid, Zh_centroid, Pt_centroid, A, AErr, Ac, AcErr, Acc, AccErr, ChiSQ);
                    }
                }
            }
        }
    }

    // Write the TNtuple
    foutput->cd();
    fittuple->Write();
    gROOT->cd();

    // Close the TFiles!
    foutput->Close();
    fphi->Close();
    fcentroids->Close();

    return 1;
}