:
a=$(date +%s --date 2025-12-15)
b=$(date +%s --date 2025-12-31)
for i in $(seq $a 86400 $b )
do
    findnoon.sh $i;
done   | awk '{printf "%s %.3f\n", $1,$2-q; q=$2}' | sed 1d

