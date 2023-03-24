#!/bin/bash

errout_check(){
    # checking logs directories
    if [[ ! -d ${main_dir}/err || ! -d ${main_dir}/out ]]
    then
	echo "Making logs directories!"
	mkdir ${main_dir}/err
	mkdir ${main_dir}/out
    fi
}

# Store the mother directory
cd ..
main_dir=$(pwd)

# Return to job directory
cd ./src-jobs

# Check if err and output dirs are present
errout_check

# Set target to use!
simu_target=1   # 0->C, 1->Fe, 2->Pb, 3->D2
data_target=1   # 0->C, 1->Fe, 2->Pb 
vertex_cut=2    # 1->liquid, 2->solid

# Send jobs
Q2_bin=0
Nu_bin=1
Zh_bin=2

sbatch --job-name=acc_${simu_target}${data_target}${vertex_cut} \
       --error=${main_dir}/err/acc-${simu_target}${data_target}${vertex_cut}_${Q2_bin}${Nu_bin}${Zh_bin} \
       --output=${main_dir}/out/acc-${simu_target}${data_target}${vertex_cut}_${Q2_bin}${Nu_bin}${Zh_bin} \
       ./job-acc.sh ${simu_target} ${data_target} ${vertex_cut} ${Q2_bin} ${Nu_bin} ${Zh_bin}
