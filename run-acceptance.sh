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


./bin/acceptance ${simu_target} ${data_target} ${vertex_cut} ${Q2_bin} ${Nu_bin} ${Zh_bin}