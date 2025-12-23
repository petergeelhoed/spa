:
a=$(date +%s --date 2025-07-02)
b=$(date +%s --date 2026-07-02)
for i in $(seq $a 86400 $b )
do
    echo $i 
done | build/noon

