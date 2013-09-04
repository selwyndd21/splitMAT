#!/bin/sh

########################################
# check the input parameters
########################################
while getopts  "abc:" flag
do
  echo "$flag" $OPTIND $OPTARG
  done


inpfile=$*
if [ -z "$inpfile" ]; then
  echo "ERROR: no input disignated"
  exit 2
fi
inpdata=(${inpfile// / })

echo "file No: ${#inpdata[@]}  file name: ${inpdata[@]}"
exit

