#include "names.h"

std::string get_sim_name(int sim_target_index)
{
    return sim_dir+sim_targets[sim_target_index]+sim_ext;
}

std::string get_dat_name(int dat_target_index)
{
    return dat_dir+dat_targets[dat_target_index]+dat_ext;
}

std::string get_acc_file_name(int vertex_cut_value, int dat_target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return acc_result_dir+"acc"+targets[vertex_cut_value-1][dat_target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+".root";
}

std::string get_acc_file_name(int vertex_cut_value, int dat_target_index)
{
    return acc_result_dir+"acc"+targets[vertex_cut_value-1][dat_target_index]+".root";
}

std::string get_clt_file_name(int sim_target_index)
{
    return acc_result_dir+"ct_"+sim_targets[sim_target_index]+"_ntuple.root";
}

std::string get_cltacc_file_name(int sim_target_index)
{
    return acc_result_dir+"ctacc_"+sim_targets[sim_target_index]+".root";
}

std::string get_ctr_file_name(int vertex_cut_value, int dat_target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return rad_result_dir+"centroids"+targets[vertex_cut_value-1][dat_target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+".root";
}

std::string get_fitphipq_file_name(int vertex_cut_value, int dat_target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return rad_result_dir+"newphihist"+targets[vertex_cut_value-1][dat_target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+".root";
}

std::string get_rad_file_name(int vertex_cut_value, int dat_target_index, int Q2_bin, int Nu_bin, int Zh_bin)
{
    return rad_result_dir+"rcfactors"+targets[vertex_cut_value-1][dat_target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+".root";
}

std::string get_rad_file_name(int vertex_cut_value, int dat_target_index)
{
    return rad_result_dir+"rcfactors"+targets[vertex_cut_value-1][dat_target_index]+".root";
}

std::string get_fullcorr_file_name(int vertex_cut_value, int dat_target_index, int rad_case)
{
    return rad_result_dir+"fullcorr"+std::to_string(rad_case)+targets[vertex_cut_value-1][dat_target_index]+".root";
}

std::string get_accf_histo_name(int vertex_cut_value, int dat_target_index, int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    return histo_accf+targets[vertex_cut_value-1][dat_target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin);
}

std::string get_accf_histo_name(int sim_target_index, int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    return histo_accf+sim_targets[sim_target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin);
}

std::string get_data_histo_name(int vertex_cut_value, int dat_target_index, int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    return histo_data+targets[vertex_cut_value-1][dat_target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin);
}

std::string get_data_histo_name(int sim_target_index, int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    return histo_data+sim_targets[sim_target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin);
}

std::string get_acccorr_histo_name(int vertex_cut_value, int dat_target_index, int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    return histo_acc+targets[vertex_cut_value-1][dat_target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin);
}

std::string get_acccorr_histo_name(int sim_target_index, int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    return histo_acc+sim_targets[sim_target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin);
}

std::string get_accrccorr_histo_name(int vertex_cut_value, int dat_target_index, int Q2_bin, int Nu_bin, int Zh_bin, int Pt2_bin)
{
    return histo_accrc+targets[vertex_cut_value-1][dat_target_index]+"_"+std::to_string(Q2_bin)+std::to_string(Nu_bin)+std::to_string(Zh_bin)+std::to_string(Pt2_bin);
}

