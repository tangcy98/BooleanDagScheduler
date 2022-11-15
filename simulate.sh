workload=(1 2 100 128 256 512 1000 1024 2048 4096 8192 10000 16384 20000 25600 32768 51200 65536 80000 102400)
for i in ${workload[@]}
do
    # echo "workload[$i]"
    ./main benchmark/$1.v $i
done
