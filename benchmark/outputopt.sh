benchmarks=(adder cavlc dec div int2float log2 max multiplier priority router sin sqrt)
g++ outputopt.cpp -o outputopt
for b in ${benchmarks[@]}
do
    ./outputopt ${b}.v ${b}_opt.v
    mv ${b}.v ${b}_old.v
    mv ${b}_opt.v ${b}.v
done
rm -f outputopt