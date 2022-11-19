workload=(1 2 4 16 100 128 256 512 1000 1024 2048 4096 8192 10000 16384 20000 25600 32768 51200 65536 80000 102400 262144 500000 1000000)
benchmark=(int2float)
baknums=(4)

echo "memory,benchmark,workload,latency(ns),energy(nJ),simdlatency(ns),simdenergy(nJ),oplatency(ns),simdoplatency(ns),temporalutil,simdtemporalutil,maxspatialutil,avgspatialutil,simdmaxspatialutil,simdavgspatialutil"
for t in ${baknums[@]}
do
    for j in ${benchmark[@]}
    do
        g++ simulate.cpp ../src/*.cpp ../ILP/ILP.cpp -DBANKNUM=$t -lOsiClp -lCbc -lClp -o simulate
        ./simulate ../benchmark/$j.v ${#workload[@]} ${workload[@]}
    done
done
rm -f simulate