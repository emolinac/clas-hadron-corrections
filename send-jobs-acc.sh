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
main_dir=$(pwd)

# Check if err and output dirs are present
errout_check

# Set target to use!
simu_target=1   # 0->C, 1->Fe, 2->Pb, 3->D2
data_target=1   # 0->C, 1->Fe, 2->Pb 
vertex_cut=2    # 1->liquid, 2->solid

# Send jobs
sbatch --job-name=acc_${simu_target}${data_target}${vertex_cut} \
       --error=./err/${simu_target}${data_target}${vertex_cut}_${SLURM_JOB_ID} \
       --output=./out/${simu_target}${data_target}${vertex_cut}_${SLURM_JOB_ID} \
       ./src-jobs/job-acc.sh ${simu_target} ${data_target} ${vertex_cut} 0 1 2 
