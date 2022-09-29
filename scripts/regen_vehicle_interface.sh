#!/bin/bash
#Author: Matthew Nice

##this section will remove the lines between the start and end of the messages to be published
start="$(echo "$a" | sed -n '/msgs to be published start/=' ../src/vanilla_vehicle_interface.cpp)"
start=$((start+1))

end="$(echo "$a" | sed -n '/msgs to be published end/=' ../src/vanilla_vehicle_interface.cpp)"
end=$((end-1))

if [[ $end -gt $start ]]
then
  cmd="sed -i '${start}, ${end} d' ../src/vanilla_vehicle_interface.cpp"
  echo $cmd
  eval $cmd
fi


##this section reads in the values from python helper to be published

OUTPUT=($(python3 insert_lines_vanilla_vehicle_interface.py | tr -d '[],'))

##this section writes the lines to the file

## for each value pair from python:
## sed 'lineNum a\text' fileName
#start=$((start-1))
for i in ${OUTPUT[*]};
do
  #echo $i;
  cmdLoop="sed -i '${start} i ${i}' ../src/vanilla_vehicle_interface.cpp"
  echo $cmdLoop
  eval $cmdLoop
  start=$((start+1))
done
