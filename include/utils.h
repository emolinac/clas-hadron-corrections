#include "TH1F.h"

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
}

