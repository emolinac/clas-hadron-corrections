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

# Make the sofware
make clean
make

# Copy the MAID data into the bin folder
cp ./src-rad/pi_n_maid.dat ./bin/

# Check if err and output dirs are present
errout_check

# Set target to use!
target_names=("DC" "DFe" "DPb" "C" "Fe" "Pb")
array_pos=0
data_target=1   # 0->C, 1->Fe, 2->Pb 
vertex_cut=1    # 1->liquid, 2->solid

if [ ${vertex_cut} -eq 2 ]
then
    array_pos+=3
fi

echo "Target is "${target_names[${array_pos}+${data_target}]}

# Merge results from fit to phipq
hadd ./output-files-rad/newphihist_${target_names[${array_pos}+${data_target}]}.root ./output-files-rad/newphihist${target_names[${array_pos}+${data_target}]}_*.root

# Return to job directory
cd ./src-jobs

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
             
            sbatch --job-name=rc_${data_target}${vertex_cut} \
                   --error=${main_dir}/err/rc-${data_target}${vertex_cut}_${Q2_bin}${Nu_bin}${Zh_bin} \
                   --output=${main_dir}/out/rc-${data_target}${vertex_cut}_${Q2_bin}${Nu_bin}${Zh_bin} \
                   ./job-getrc.sh ${data_target} ${vertex_cut} ${Q2_bin} ${Nu_bin} ${Zh_bin}
        done
    done
done
