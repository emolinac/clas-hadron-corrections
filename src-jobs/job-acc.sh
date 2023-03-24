#!/bin/bash

#SBATCH --time=00:30:00
#SBATCH --mem=2G

simu_target=${1}
data_target=${2}
vertex_cut=${3}

# Bin convention : starts from 0
Q2_bin=${4}
Nu_bin=${5}
Zh_bin=${6}

echo "Working on directory " $(pwd)

cd ../bin

./bin/acceptance ${simu_target} ${data_target} ${vertex_cut} ${Q2_bin} ${Nu_bin} ${Zh_bin}
