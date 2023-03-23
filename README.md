# clas-hadron-corrections
This software intends to have a complete set of corrections to hadronic data. It is optimized in the sense that couples the acceptance correction with SIDIS radiative correction (HAPRAD). HAPRAD performs acceptance correction; this is highly non-efficient because the acceptance-corrected data is not stored. Therefore, acceptance correction mut be performed and stored in a different process.

## Prerequisites
- ROOT

## Setting the software
1. Run the make-software.sh code:
```
bash make-software.sh
```
2. Add the slib path of HAPRAD to the env. variable LD_LIBRARY_PATH located in your *.bashrc* script, like this:
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"other_paths_already_set":"path_to_clas-hadron-corrections/haprad-cpp/slib"
```
3. Source the .bashrc file 

## How the software works
1. Perform acceptance correction and store the corrected data, as phipq distributions, alongside the uncorrected data and the acceptance factors.
2. Calculate the centroids of the non-corrected data and then store these values in a TNtuple.
3. Use the results of step (1) to perform fits in the acceptance-corrected phipq. The histos are stored alongside a TNtuple that contains the results of these fits.
4. Calculate the radiative correction factors with the results of step (2) and (3).

## Dependence between the processes
- *acceptance* and *centroids* are independent between them and with respect to the other processes.
- *fit-phipq* depends on *acceptance* and *centroids*
- *get-rc* depends on *fit-phipq* and *centroids*
