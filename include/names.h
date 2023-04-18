#ifndef NAMES_H
#define NAMES_H

#include <string>

// Directories
std::string dat_dir = "/work/emolina/";
std::string sim_dir = "/work/emolina/";

std::string acc_result_dir = "../output-files-acc/";
std::string rad_result_dir = "../output-files-rad/";

// Extensions
std::string dat_ext = "_data_Npion.root";
std::string sim_ext = "1_simul.root";

// TNtuples
const char* ntuple_thr_name  = "ntuple_thrown";
const char* ntuple_rec_name  = "ntuple_accept";
const char* ntuple_dat_name  = "ntuple_data";
const char* ntuple_rad_name  = "rcfactors_tuple";
const char* ntuple_centroids = "centroids_data";

// Targets
std::string sim_targets[4] = { "C" , "Fe" , "Pb" , "D" };
std::string dat_targets[3] = { "C" , "Fe" , "Pb" };

std::string targets[2][3] = { { "DC" , "DFe" , "DPb" } , { "C" , "Fe" , "Pb" } };

std::string histo_data  = "data_";
std::string histo_accf  = "accf_";
std::string histo_acc   = "acc_data_";
std::string histo_accrc = "accrc_data_";

#endif
