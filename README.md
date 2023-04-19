# clas-hadron-corrections
Software that corrects hadron data in a five-dimensional phase space. Currently, the software is hard-coded to work with the following variables: Q2, Nu, Zh, Pt2, and PhiPQ.

## Prerequisites
- ROOT

## Setting the environment
1. Add the slib path of HAPRAD to the env. variable LD_LIBRARY_PATH located in your *.bashrc* script, like this:
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"other_paths_already_set":"path_to_clas-hadron-corrections/haprad-cpp/slib"
```
2. Source the .bashrc file 

## Setting the specs for the analysis
### analysis-constants.h
1. Set the binning you want to use in the analysis. Q2, Nu, and Zh can have bins of different sizes because it is usual practice. At the moment, there are no plans to include this feature in Pt2 and PhiPQ.
2. Specify additional cuts you would like to use on the TCut arrays. Cuts by default are applied on Q2, Nu, Zh, and the vertex; the rest of the cuts are considered additional.
    1. Remember to adjust the array size to the number of cuts inside of it.
    2. The cuts by default are used to speed up the operations of the software by using TEventLists.

### names.h
1. Set the directories of data and simulations as absolute paths.
2. Set the extensions of the data and simulation files.
    1. What is the extension? We use files where the name of the target comes first, the rest is the extension. For instance: *C_data_Npion.root*. The extension is *_data_Npion.root*

## Dependence between the processes
```mermaid
graph LR;
    acceptance-->fit-phipq;
    centroids-->fit-phipq;
    centroids-->get-rc;
    fit-phipq-->get-rc;
    get-rc-->apply-rc;
```