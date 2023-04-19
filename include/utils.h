#include "TH1F.h"
#include "TNtuple.h"
#include "names.h"

// MISC
int check_target(int sim_target_index , int dat_target_index, int vertex_cut_value)
{
    if(vertex_cut_value==2&&sim_target_index!=dat_target_index) return 0;
    if(vertex_cut_value==1&&sim_target_index!=3) return 0;
    if(vertex_cut_value>2||vertex_cut_value<1) return 0;

    // Everything ok!
    return 1;
}

void set_ctr_branches(float* Q2_centroid, float* Xb_centroid, float* Zh_centroid, float* Pt_centroid, float* Phi_centroid, 
                      float* Q2_centroid_bin, float* Xb_centroid_bin, float* Zh_centroid_bin, float* Pt_centroid_bin, float* Phi_centroid_bin,
                      TNtuple* ntuple_ctr)
{
    ntuple_ctr->SetBranchAddress("Q2"     , Q2_centroid     );
    ntuple_ctr->SetBranchAddress("Xb"     , Xb_centroid     );
    ntuple_ctr->SetBranchAddress("Zh"     , Zh_centroid     );
    ntuple_ctr->SetBranchAddress("Pt"     , Pt_centroid     );
    ntuple_ctr->SetBranchAddress("Phi"    , Phi_centroid    );
    ntuple_ctr->SetBranchAddress("Q2_bin" , Q2_centroid_bin );
    ntuple_ctr->SetBranchAddress("Xb_bin" , Xb_centroid_bin );
    ntuple_ctr->SetBranchAddress("Zh_bin" , Zh_centroid_bin );
    ntuple_ctr->SetBranchAddress("Pt2_bin", Pt_centroid_bin );
    ntuple_ctr->SetBranchAddress("Phi_bin", Phi_centroid_bin);

    return;
}

void set_rad_branches(float* Q2_bin, float* Nu_bin, float* Zh_bin, float* Pt2_bin, float* Phi_bin, float* rc1,
                      TNtuple* ntuple_rad)
{
    ntuple_rad->SetBranchAddress("Q2_bin" , Q2_bin );
    ntuple_rad->SetBranchAddress("Nu_bin" , Nu_bin );
    ntuple_rad->SetBranchAddress("Zh_bin" , Zh_bin );
    ntuple_rad->SetBranchAddress("Pt2_bin", Pt2_bin);
    ntuple_rad->SetBranchAddress("Phi_bin", Phi_bin);
    ntuple_rad->SetBranchAddress("rc1"    , rc1    );

    return;
}

// ACCEPTANCE 

// Returns 1 when histo is empty
int empty_histo(TH1F* h)
{
    int empty = 0;
    
    for(int bin = 1 ; bin <= h->GetNbinsX() ; bin++)
    {
        if(h->GetBinContent(bin)==0){empty++;}
    }
    
    if(empty==h->GetNbinsX())
    {
        return 1;
    }
    
    return 0;
}

// Applies N_accept>1
void rec_histo_process(TH1F* h)
{
    for(int bin = 1 ; bin <= h->GetNbinsX() ; bin++)
    {
        if(h->GetBinContent(bin)==1)
        {
            h->SetBinContent(bin,0);
            h->SetBinError(bin,0);
        }
    }

    return;
}

// Applies acc<1
void acc_histo_process(TH1F* h)
{
    for(int bin = 1 ; bin <= h->GetNbinsX() ; bin++)
    {
        if(h->GetBinContent(bin)>=1)
        {
            h->SetBinContent(bin,0);
            h->SetBinError(bin,0);
        }
    }

    return;
}

// RAD

// Return number of non-empty bins
int get_filled_bins(TH1F* h)
{
    if(h==NULL) return 0;
    
    int filled_bins = 0;
    for(int bin = 1 ; bin <= h->GetNbinsX() ; bin++)
    {
        if(h->GetBinContent(bin)!=0)filled_bins++;
    }

    return filled_bins;
}

void assign_n2p_ratio(double& NAZ, std::string target)
{
    if(target == "C") NAZ = 0.5;
    else if(target == "Fe")  NAZ = 0.5;
    else if(target == "Pb")  NAZ = 82./208.;
    else if(target == "DC")  NAZ = 0.5;
    else if(target == "DFe") NAZ = 0.5;
    else if(target == "DPb") NAZ = 0.5;
    else NAZ = 0.5;

    return;
}
