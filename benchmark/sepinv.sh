benchmarks=(adder cavlc dec div int2float log2 max multiplier priority router sin sqrt)
g++ sepinv.cpp -o sepinv
for b in ${benchmarks[@]}
do
    ./sepinv ${b}_pre_syn_out_opt_1.v ${b}.v
done
rm -f sepinv