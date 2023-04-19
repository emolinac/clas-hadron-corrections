# clas-hadron-corrections
## Prerequisites
- ROOT

## Setting the software
1. Add the slib path of HAPRAD to the env. variable LD_LIBRARY_PATH located in your *.bashrc* script, like this:
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"other_paths_already_set":"path_to_clas-hadron-corrections/haprad-cpp/slib"
```
2. Source the .bashrc file 

## Setting the specs for the analysis
bla

## How the software works
1. Perform acceptance correction and store the corrected data, as phipq distributions, alongside the uncorrected data and the acceptance factors.
2. Calculate the centroids of the non-corrected data and then store these values in a TNtuple.
3. Use the results of step (1) to perform fits in the acceptance-corrected phipq. The histos are stored alongside a TNtuple that contains the results of these fits.
4. Calculate the radiative correction factors with the results of step (2) and (3).

## Dependence between the processes
```mermaid
graph LR;
    A(data)-->acceptance;
    A(data)-->centroids;
    B(simul)-->acceptance;
    acceptance-->fit-phipq;
    centroids-->fit-phipq;
    centroids-->get-rc;
    fit-phipq-->get-rc;
    get-rc-->apply-rc;
```