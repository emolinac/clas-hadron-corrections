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

# Be sure to set your ifarm/cluster username!
username=$(echo ${USER})

# Store the mother directory
cd ..
main_dir=$(pwd)

# Make the sofware if there are no running jobs
n_jobs=$(squeue -u ${username} | grep -i " R " | wc -l)
if [ ${n_jobs} = 0 ]
then
    echo "No jobs running! Safe to do make."
    make clean
    make
fi

# Return to job directory
cd ./src-jobs

# Check if err and output dirs are present
errout_check

# Set target to use!
simu_target=1   # 0->C, 1->Fe, 2->Pb, 3->D2
data_target=1   # 0->C, 1->Fe, 2->Pb 
vertex_cut=2    # 1->liquid, 2->solid

# Send jobs
N_Q2=3
N_Nu=3
N_Zh=8

for(( i = 0 ; i < ${N_Q2} ; i++ ))
do
    for(( j = 0 ; j < ${N_Nu} ; j++ ))
    do
        for(( k = 0 ; k < ${N_Zh} ; k++ ))
        do
            Q2_bin=${i}
            Nu_bin=${j}
            Zh_bin=${k}

            echo "Sending job for bin ${Q2_bin} ${Nu_bin} ${Zh_bin}"
             
            sbatch --job-name=acc_${simu_target}${data_target}${vertex_cut} \
                   --error=${main_dir}/err/acc-${simu_target}${data_target}${vertex_cut}_${Q2_bin}${Nu_bin}${Zh_bin} \
                   --output=${main_dir}/out/acc-${simu_target}${data_target}${vertex_cut}_${Q2_bin}${Nu_bin}${Zh_bin} \
                   ./job-acc.sh ${simu_target} ${data_target} ${vertex_cut} ${Q2_bin} ${Nu_bin} ${Zh_bin}
        done
    done
done
