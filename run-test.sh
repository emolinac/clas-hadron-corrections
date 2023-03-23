#!/bin/bash

# simu_target: 0->C, 1->Fe, 2->Pb, 3->D2
# data_target: 0->C, 1->Fe, 2->Pb
# vertex_cut : 1->liquid, 2->solid

simu_target=1
data_target=1
vertex_cut=2

# Bin convention : starts from 0
Q2_bin=0
Nu_bin=1
Zh_bin=2

mother_dir=$(pwd)

# Clean and compile HAPRAD
#cd ${mother_dir}/haprad-cpp
#make clean
#make

# Clean and compile corrections software
#cd ${mother_dir}
#make clean
#make

echo "RUNNING ACCEPTANCE"
cd ${mother_dir}/bin
echo ${mother_dir}/bin

./acceptance ${simu_target} ${data_target} ${vertex_cut} ${Q2_bin} ${Nu_bin} ${Zh_bin}

echo "RUNNING CENTROIDS"

./centroids ${data_target} ${vertex_cut} ${Q2_bin} ${Nu_bin} ${Zh_bin}

echo "RUNNING FIT PHIPQ"

./fit-phipq ${data_target} ${vertex_cut} ${Q2_bin} ${Nu_bin} ${Zh_bin}

echo "finalizing"
cp ${mother_dir}/src-rad/pi_n_maid.dat ${mother_dir}/bin/
