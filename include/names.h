#include <string>

// ACCEPTANCE 

// Directories
std::string data_dir = "/work/emolina/";
std::string simu_dir = "/work/emolina/";

std::string acc_result_dir = "../output-files-acc/";

// Extensions
std::string data_ext = "_data_Npion.root";
std::string simu_ext = "1_simul.root";

// TNtuples
const char* ntuple_thr_name = "ntuple_thr";
const char* ntuple_rec_name    = "ntuple_accept";
const char* ntuple_dat_name   = "ntuple_dat";

// Targets
std::string simu_targets[4] = { "C" , "Fe" , "Pb" , "D2" };
std::string data_targets[3] = { "C" , "Fe" , "Pb" };

std::string histo_target[2][3] = { { "DC" , "DFe" , "DPb" } , { "C" , "Fe" , "Pb" } };

std::string histo_data = "data_";
std::string histo_accf = "accfactor_";
std::string histo_corr = "corrdata_";

