#include <iostream>
#include <string>
#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TDatime.h"
#include "TRandom.h"
#include "names.h"
#include "utils.h"
#include "utils-names.h"
#include "analysis-constants.h"


int main(int argc, char* argv[])
{
    // Target settings
    int sim_target_index = std::stoi(argv[1]);
    
    // Open the simulations
    TFile* fsim = new TFile(get_sim_name(sim_target_index).c_str(),"READ");
    
    // Create results folder
    TFile* fresult = new TFile(get_clt_file_name(sim_target_index).c_str(),"RECREATE");
    gROOT->cd();

    // Obtain the tuples from simulations
    TNtuple* ntuple_thr = (TNtuple*) fsim->Get(ntuple_thr_name);
    TNtuple* ntuple_rec = (TNtuple*) fsim->Get(ntuple_rec_name);
    
    if(ntuple_thr==NULL||ntuple_rec==NULL){std::cout<<"ERROR! One of the tuples could not be loaded!"<<std::endl; return 1;}

    // Set the branches to read the ntuples from simuls
    float Q2_thr, Nu_thr, Zh_thr, Pt2_thr, PhiPQ_thr, Xf_thr, deltaZ_thr;
    ntuple_thr->SetBranchAddress("Q2"    ,&Q2_thr    );
    ntuple_thr->SetBranchAddress("Nu"    ,&Nu_thr    );
    ntuple_thr->SetBranchAddress("Zh"    ,&Zh_thr    );
    ntuple_thr->SetBranchAddress("Pt2"   ,&Pt2_thr   );
    ntuple_thr->SetBranchAddress("PhiPQ" ,&PhiPQ_thr );
    ntuple_thr->SetBranchAddress("Xf"    ,&Xf_thr    );
    ntuple_thr->SetBranchAddress("deltaZ",&deltaZ_thr);
    
    float Q2_rec, Nu_rec, Zh_rec, Pt2_rec, PhiPQ_rec, Xf_rec, deltaZ_rec, YC_rec, VC_rec;
    ntuple_rec->SetBranchAddress("Q2"    ,&Q2_rec    );
    ntuple_rec->SetBranchAddress("Nu"    ,&Nu_rec    );
    ntuple_rec->SetBranchAddress("Zh"    ,&Zh_rec    );
    ntuple_rec->SetBranchAddress("Pt2"   ,&Pt2_rec   );
    ntuple_rec->SetBranchAddress("PhiPQ" ,&PhiPQ_rec );
    ntuple_rec->SetBranchAddress("Xf"    ,&Xf_rec    );
    ntuple_rec->SetBranchAddress("deltaZ",&deltaZ_rec);
    ntuple_rec->SetBranchAddress("YC"    ,&YC_rec    );
    ntuple_rec->SetBranchAddress("VC_TM" ,&VC_rec    );

    std::cout<<"Branches set."<<std::endl;

    // Create TNtuples for the closure test
    TNtuple* ntuple_thr_ct = new TNtuple(ntuple_thr_name,ntuple_thr_name,"Q2:Nu:Zh:Pt2:PhiPQ:Xf:deltaZ");
    TNtuple* ntuple_rec_ct = new TNtuple(ntuple_rec_name,ntuple_rec_name,"Q2:Nu:Zh:Pt2:PhiPQ:Xf:deltaZ:YC");
    TNtuple* ntuple_dat_ct = new TNtuple(ntuple_dat_name,ntuple_dat_name,"Q2:Nu:Zh:Pt2:PhiPQ:Xf:deltaZ:YC");
    TNtuple* ntuple_ref_ct = new TNtuple(ntuple_ref_name,ntuple_ref_name,"Q2:Nu:Zh:Pt2:PhiPQ:Xf:deltaZ");

    std::cout<<"Ntuples declared."<<std::endl;

    // Set the random number generator and its seed
    TRandom* r = new TRandom();
    TDatime* t = new TDatime();
    int seed = t->GetTime()*t->GetTime();
    r->SetSeed(seed);

    // Fill the closure test's ntuples
    for(int entry = 0 ; entry < ntuple_thr->GetEntries() ; entry++)
    {
        ntuple_thr->GetEntry(entry);

        r->Uniform(0,1)<0.5 ? ntuple_thr_ct->Fill(Q2_thr, Nu_thr, Zh_thr, Pt2_thr, PhiPQ_thr, Xf_thr, deltaZ_thr):
                              ntuple_ref_ct->Fill(Q2_thr, Nu_thr, Zh_thr, Pt2_thr, PhiPQ_thr, Xf_thr, deltaZ_thr);
    }

    for(int entry = 0 ; entry < ntuple_rec->GetEntries() ; entry++)
    {
        ntuple_rec->GetEntry(entry);

        r->Uniform(0,1)<0.5 ? ntuple_rec_ct->Fill(Q2_rec, Nu_rec, Zh_rec, Pt2_rec, PhiPQ_rec, Xf_rec, deltaZ_rec, YC_rec):
                              ntuple_dat_ct->Fill(Q2_rec, Nu_rec, Zh_rec, Pt2_rec, PhiPQ_rec, Xf_rec, deltaZ_rec, YC_rec);
    }

    std::cout<<"# CT Rec entries = "<<ntuple_rec_ct->GetEntries()<<std::endl;
    std::cout<<"# CT Dat entries = "<<ntuple_dat_ct->GetEntries()<<std::endl;
    std::cout<<"# CT Thr entries = "<<ntuple_thr_ct->GetEntries()<<std::endl;
    std::cout<<"# CT Ref entries = "<<ntuple_ref_ct->GetEntries()<<std::endl;

    // Write the TNtuples in the output file
    fresult->cd();
    ntuple_thr_ct->Write();
    ntuple_rec_ct->Write();
    ntuple_dat_ct->Write();
    ntuple_ref_ct->Write();
    gROOT->cd();

    // Close files
    fresult->Close();
    fsim->Close();
    
    return 1;
}
