#!/bin/bash

#SBATCH --time=02:00:00
#SBATCH --mem=2G

echo "Working on directory " $(pwd)

cd ../bin

./apply-rad 0 1
./apply-rad 1 1
./apply-rad 2 1
./apply-rad 0 2
./apply-rad 1 2
./apply-rad 2 2

echo "Done!"
