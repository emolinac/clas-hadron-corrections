#!/bin/bash

#SBATCH --time=00:30:00
#SBATCH --mem=2G

data_target=${1}
vertex_cut=${2}

# Bin convention : starts from 0
Q2_bin=${3}
Nu_bin=${4}
Zh_bin=${5}

echo "Working on directory " $(pwd)

cd ../bin

./fit-phipq ${data_target} ${vertex_cut} ${Q2_bin} ${Nu_bin} ${Zh_bin}
