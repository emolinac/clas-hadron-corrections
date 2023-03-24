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
