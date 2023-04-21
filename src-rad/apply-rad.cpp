#include <iostream>
#include "TFile.h"
#include "TROOT.h"
#include "TNtuple.h"
#include "TH1F.h"
#include "names.h"
#include "utils.h"
#include "utils-names.h"
#include "analysis-constants.h"

int main(int argc, char* argv[])
{
    // Check number of arguments is correct
    if(argc!=3) {std::cout<<"Incorrect number of argument!"<<std::endl; return 1;}

    // Target settings
    int dat_target_index = std::stoi(argv[1]);
    int vertex_cut_value = std::stof(argv[2]);

    // Open radiative corrections file and acceptance corrected data file
    TFile* frad = new TFile(get_rad_file_name(vertex_cut_value,dat_target_index).c_str());
    TFile* facc = new TFile(get_acc_file_name(vertex_cut_value,dat_target_index).c_str());

    // Open the output files
    TFile* fout1 = new TFile(get_fullcorr_file_name(vertex_cut_value,dat_target_index,1).c_str(),"RECREATE");
    gROOT->cd();
    TFile* fout2 = new TFile(get_fullcorr_file_name(vertex_cut_value,dat_target_index,2).c_str(),"RECREATE");
    gROOT->cd();

    //  Obtain the rcfactors TNtuple
    TNtuple* ntuple_rad = (TNtuple*) frad->Get(ntuple_rad_name);

    // Set the branches of the rc factors TNtuple
    float Q2_bin_rad, Nu_bin_rad, Zh_bin_rad, Pt2_bin_rad, Phi_bin_rad, rc1;
    set_rad_branches(&Q2_bin_rad, &Nu_bin_rad, &Zh_bin_rad, &Pt2_bin_rad, &Phi_bin_rad, &rc1, ntuple_rad);
    
    // Declare instrumental histos (correction and data)
    TH1F* h;
    TH1F* hdat;

    // Start the correction process
    for(int Q2_bin = 0 ; Q2_bin < N_Q2 ; Q2_bin++)
    {
        for(int Nu_bin = 0 ; Nu_bin < N_Nu ; Nu_bin++)
        {
            for(int Zh_bin = 0 ; Zh_bin < N_Zh ; Zh_bin++)
            {
                for(int Pt2_bin = 0 ; Pt2_bin < N_Pt2 ; Pt2_bin++)
                {
                    // Get acceptance-corrected PhiPQ . Then, write them.
                    h    = (TH1F*) facc->Get(get_acccorr_histo_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin,Pt2_bin).c_str());
                    hdat = (TH1F*) facc->Get(get_data_histo_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin,Pt2_bin).c_str());
                    if(h==NULL) continue;
                    fout1->cd();
                    h->Write(get_acccorr_histo_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin,Pt2_bin).c_str());
                    hdat->Write(get_data_histo_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin,Pt2_bin).c_str());
                    gROOT->cd();

                    // Apply the RC factor
                    // Case 1 : There is a positive RC factor
                    for(int Phi_bin = 0 ; Phi_bin < N_Phi ; Phi_bin++)
                    {
                        for(int entry = 0 ; entry < ntuple_rad->GetEntries() ; entry++)
                        {
                            ntuple_rad->GetEntry(entry);
                            if(Q2_bin==Q2_bin_rad&&Nu_bin==Nu_bin_rad&&Zh_bin==Zh_bin_rad&&Pt2_bin==Pt2_bin_rad&&Phi_bin==Phi_bin_rad&&rc1>0)
                            {
                                h->SetBinContent(Phi_bin+1, h->GetBinContent(Phi_bin+1)/rc1);
                                h->SetBinError(Phi_bin+1, h->GetBinError(Phi_bin+1)/rc1);
                                break;                             
                            }
                        }
                    }

                    // Write the rad corrected histo
                    fout1->cd();
                    h->Write(get_accrccorr_histo_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin,Pt2_bin).c_str());
                    gROOT->cd();

                    // Reset and start the process again
                    h->Reset();
                    h = (TH1F*) facc->Get(get_acccorr_histo_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin,Pt2_bin).c_str());
                    fout2->cd();
                    h->Write(get_acccorr_histo_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin,Pt2_bin).c_str());
                    hdat->Write(get_data_histo_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin,Pt2_bin).c_str());
                    gROOT->cd();
                    
                    // Case 2 : There is a positive RC factor. The rest goes to zero
                    int corr_aplied = 0;
                    for(int Phi_bin = 0 ; Phi_bin < N_Phi ; Phi_bin++)
                    {
                        for(int entry = 0 ; entry < ntuple_rad->GetEntries() ; entry++)
                        {
                            ntuple_rad->GetEntry(entry);
                            if(Q2_bin==Q2_bin_rad&&Nu_bin==Nu_bin_rad&&Zh_bin==Zh_bin_rad&&Pt2_bin==Pt2_bin_rad&&Phi_bin==Phi_bin_rad&&rc1>0)
                            {
                                corr_aplied++;
                                h->SetBinContent(Phi_bin+1, h->GetBinContent(Phi_bin+1)/rc1);
                                h->SetBinError(Phi_bin+1, h->GetBinError(Phi_bin+1)/rc1);
                                break;                             
                            }
                        }
                    }

                    if(corr_aplied==0)
                    {
                        for(int Phi_bin = 0 ; Phi_bin < N_Phi ; Phi_bin++)
                        {
                            h->SetBinContent(Phi_bin+1, 0);
                            h->SetBinError(Phi_bin+1, 0);
                        }
                    }

                    // Write the rad corrected histo
                    fout2->cd();
                    h->Write(get_accrccorr_histo_name(vertex_cut_value,dat_target_index,Q2_bin,Nu_bin,Zh_bin,Pt2_bin).c_str());
                    gROOT->cd();
                }
            }
        }
    }

    // Close the files
    facc->Close();
    frad->Close();
    fout1->Close();
    fout2->Close();

    return 1;
}