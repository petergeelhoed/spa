#!/bin/bash 

a=$1
z=0
b=1
#b=$(echo $a | azid | awk '{printf "%.3f\n", -($3-180)/15*3600}')
while [[  $(echo $b | awk '{print ($1^2>0.000001)}' ) -ne 0 ]]
do 
z=$((z+1))
#echo $b
a=$(echo $a $b | awk '{printf "%.4f\n", $1+$2}')
b=$(echo $a | azid | awk '{printf "%.4f\n", -($3-180)/15*3600*cos($2/180*3.1415926)}')

done

c=$(echo "@${a%%.*}" | date -f- --iso-8601=seconds )
echo $c $a $b $z

