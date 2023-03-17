#include <iostream>
#include <string>
#include "TROOT.h"
#include "TFile.h"
#include "TH1F.h"
#include "TNtuple.h"
#include "TCut.h"
#include "names.h"
#include "utils.h"
#include "analysis-constants.h"

int main(int argc, char* argv[])
{
    // Target settings
    int dat_target_index = std::stoi(argv[1]);
    int vertex_cut_value  = std::stof(argv[2]);

    // Q2, Nu, Zh bin settings
    int Q2_bin = std::stoi(argv[3]);
    int Nu_bin = std::stoi(argv[4]);
    int Zh_bin = std::stoi(argv[5]);

    double Q2_min = Q2_limits[Q2_bin  ];
    double Q2_max = Q2_limits[Q2_bin+1];
    double Nu_min = Nu_limits[Nu_bin  ];
    double Nu_max = Nu_limits[Nu_bin+1];
    double Zh_min = Zh_limits[Zh_bin  ];
    double Zh_max = Zh_limits[Zh_bin+1];
    
    // Open the simulations and data
    TFile* fdat = new TFile((dat_dir+dat_targets[dat_target_index]+dat_ext).c_str(),"READ");

    // Open create results folder
    std::string output_file_name = rad_result_dir+"centroids"+dat_targets[dat_target_index]+"_VC"+std::to_string(vertex_cut_value)+"_"+
                                   std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+".root";
    TFile* fresult = new TFile(output_file_name.c_str(),"RECREATE");
    gROOT->cd();

    // Obtain the tuples
    TNtuple* ntuple_dat = (TNtuple*) fdat->Get(ntuple_dat_name);
    
    if(ntuple_dat==NULL){std::cout<<"ERROR! Data tuple could not be loaded!"<<std::endl; return 1;}

    // Create centroid tuples
    TNtuple* centroids_data = new TNtuple("centroids_data","centroids_data","Q2:Xb:Zh:Pt:Phi:Q2_bin:Xb_bin:Zh_bin:Pt2_bin:Phi_bin");

    // Setting the base cuts
    std::cout<<"The bin selected was:"<<std::endl;
    std::cout<<Q2_min<<" < Q2 < "<<Q2_max<<std::endl;
    std::cout<<Nu_min<<" < Nu < "<<Nu_max<<std::endl;
    std::cout<<Zh_min<<" < Zh < "<<Zh_max<<std::endl;

    TCut Q2_cut = Form("Q2>%f&&Q2<%f",Q2_min,Q2_max);
    TCut Nu_cut = Form("Nu>%f&&Nu<%f",Nu_min,Nu_max);
    TCut Zh_cut = Form("Zh>%f&&Zh<%f",Zh_min,Zh_max);
    TCut VZ_cut = Form("VC_TM==%i",vertex_cut_value);
    
    TCut cuts_dat = Q2_cut&&Nu_cut&&Zh_cut&&VZ_cut;
    
    // Setting additional data cuts
    for(int i = 0 ; i < sizeof(dat_add_cut)/sizeof(dat_add_cut) ; i++) cuts_dat += dat_add_cut[i];
    
    // Set TEventLists to make everything faster
    ntuple_dat->Draw(">>list_dat",cuts_dat);

    TEventList* evnt_dat = (TEventList*) gDirectory->Get("list_dat");
    
    ntuple_dat->SetEventList(evnt_dat);

    // Declare the necessary histograms
    TH1F* hq2  = new TH1F("hq2" ,"",150,Q2_min       ,Phi_max      );
    TH1F* hxb  = new TH1F("hxb" ,"",150,0.1          ,0.6          );
    TH1F* hzh  = new TH1F("hzh" ,"",150,Zh_min       ,Zh_max       );
    TH1F* hpt  = new TH1F("hpt" ,"",150,sqrt(Pt2_min),sqrt(Pt2_max));
    TH1F* hphi = new TH1F("hphi","",150,Phi_min      ,Phi_max      );

    // Start the centroids process
    for(int Pt2_bin = 0 ; Pt2_bin < N_Pt2 ; Pt2_bin++)
    {
        for(int Phi_bin = 0 ; Phi_bin < N_Phi ; Phi_bin++)
        {
            // Stablish the Pt2 cut
            double Pt2_bin_min = delta_Pt2 *  Pt2_bin;
            double Pt2_bin_max = delta_Pt2 * (Pt2_bin+1);
            TCut loop_cut = Form("Pt2>%f&&Pt2<%f",Pt2_bin_min,Pt2_bin_max);

            // Fill the histos and skip the loop if the bin is empty
            ntuple_dat->Project("hq2" ,"Q2"               ,loop_cut);
            if(empty_histo(hq2)==1){hq2->Reset();continue;}
            ntuple_dat->Project("hxb" ,"Q2/2./Nu/0.938272",loop_cut);
            ntuple_dat->Project("hzh" ,"Zh"               ,loop_cut);
            ntuple_dat->Project("hpt" ,"TMath::Sqrt(Pt2)" ,loop_cut);
            ntuple_dat->Project("hphi","PhiPQ"            ,loop_cut);
            
            // Obtain the centroids
            double q2_centroid  = hq2->GetMean();            
            double xb_centroid  = hxb->GetMean();
            double zh_centroid  = hzh->GetMean();
            double pt_centroid  = hpt->GetMean();
            double phi_centroid = hphi->GetMean();
            
            // Fill the tuples
            centroids_data->Fill(q2_centroid, xb_centroid, zh_centroid, pt_centroid, phi_centroid, Q2_bin, Nu_bin, Zh_bin, Pt2_bin, Phi_bin);

            // Reset histos
            hq2->Reset();
            hxb->Reset();
            hzh->Reset();
            hpt->Reset();
            hphi->Reset();
        }
    }

    // Close TFiles
    fresult->Close();
    fdat->Close();
    
    return 1;
}
