:
a=$(date +%s --date 2025-12-31)
b=$(date +%s --date 2026-12-31)
for i in $(seq $a 86400 $b )
do
    echo $i 
done | build/noon

