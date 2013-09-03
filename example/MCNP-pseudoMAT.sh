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
  sed -n '/^m[0-9t]/,/^[^m\ ]/p' temp01 > temp02 
    # extract lines between '/word1/,/word2/'
    # print "p" accompany with 'print only valid "-n"'
    # "^" alone = line begin with; [0-9t] = character 0,1,2...9,t
    # [^m\ ] = neithor "m" nor space "\ "
  grep -e ^m[0-9t] -e ^\  temp02 > temp03 
    # remove entry other than m and space
  
  matno=0 # material count
  declare -a matname
  # read file line by line
  while read -a array # read every line in array[0,1,2....]
  do
    i=0 # reset iterator
    # Seperate each m-card and mt-card
    if   [[ ${array[0]} == m[0-9t]* ]] ; then
#      echo "  material ${array[0]}"
      matname[${matno}]=${array[0]}
      matno=$(($matno+1))
      i=1
    fi
    
    # Creat isotopes item
    arraylth=${#array[@]} # data amount of array
    # echo $arraylth $i
    if (( $arraylth <= 1 )) ; then
      echo "    no isotopes in the line"
    else
      while [[ $i < $arraylth ]]
      do
#        echo "${array[$i]}    ${array[$(($i+1))]}"
        echo "${array[$i]}    ${array[$(($i+1))]}" >> tmp_${matname[$(($matno-1))]}
        i=$(($i+2))
      done
    fi
  done < temp03 #input file to while loop
  
  # interpolate material through python script
#  for name in "${matname[@]}" 
  for name in mt23 
  do
    echo "  python interpolate tmp_$name" # informatin
#    python pseudoMAT.py -i $name -o temp04
  done
  
  #clean up temp
#  echo ${matname[*]}
#  for (( i=0; i < $matno; i++ ))
#  do
#    rm tmp_${matname[$i]}
#  done
done

exit

