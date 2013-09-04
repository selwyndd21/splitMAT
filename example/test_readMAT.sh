#!/bin/sh

########################################
# check the input parameters
########################################
inpfile=$*
if [ -z "$inpfile" ]; then
  echo "ERROR: no input disignated"
  exit 2
fi

########################################
# main
########################################
#deal with input names
inpdata=(${inpfile// / })
length=${#inpdata[@]}

for (( j=0; j<$length; j++ )) ; do
  # the trigger loop
  inputfile=${inpdata[$j]}
  if [ ! -f $inputfile ]; then # check the file exist or not
    echo "No file name: $inputfile !"
    break
  fi
  
  # set output filename from last fiel after "_"
  outfile=mat_$( echo $inputfile | awk -F "_" '{print $(NF-1)}' ) 

  #include material name and matherial endf number by material list
  echo "Scratch material: $inputfile -> $outfile " # information
  sed '/^c/d' $inputfile > temp01 # delete comment line
  if []
  fi
#  sed -n '/^m[0-9t]/,/^[^m\ ]/p' temp01 > temp02 
    # extract lines between '/word1/,/word2/'
    # print "p" accompany with 'print only valid "-n"'
    # "^" alone = line begin with; [0-9t] = character 0,1,2...9,t
    # [^m\ ] = neithor "m" nor space "\ "
#  grep -e ^m[0-9t] -e ^\  temp02 > temp03 
    # remove entry other than m and space
done

exit

