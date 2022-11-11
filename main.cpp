#include "./src/memory.h"
#include "./src/booleandag.h"
#include "./src/scheduler.h"
#include "./src/importdag.h"
#include "./ILP/ILP.h"
#include <cstdio>

using namespace std;

/**
 * @brief main
 * 
 * @param argc arg count
 * @param argv 0:executable file name, 1:inputfile(*.v) 2:workloadsize
 * @return int 
 */
int main(int argc, char *argv[])
{
    if (argc < 3) {
        return 0;
    }
    const char *inputfile = argv[1];
    uint size = atoi(argv[2]);
    uint Bsize = (size + BLOCKCOL - 1) / BLOCKCOL;

    BooleanDag *G = v2booleandag(inputfile);

    uint searchbound = LOG2(Bsize < MESHSIZE ? Bsize : MESHSIZE);
    printf("Bsize:%d, searchbound:%d\n", Bsize, searchbound);
    if (NPOWEROF2(searchbound) < Bsize && NPOWEROF2(searchbound) < MESHSIZE) ++searchbound;
    
    Schedule *sche = new Schedule[searchbound+1];
    double *cost = new double[searchbound+1];

    for (uint i = 0u; i <= searchbound; ++i) {
        sche[i] = scheduleDAG(G, NPOWEROF2(i));
        cost[i] = sche[i].makespan;
    }

    CutSolver solver(Bsize, searchbound, cost);

    bool optimal = solver.isOptimal();
    const double *val = solver.getSolution();
    double res = solver.getObjValue();
    printf("Solution res: %g, %g %g %g %g %g %g %g %d\n", res, val[0], val[1], val[2], val[3], val[4], val[5], val[6], optimal ? 1 : 0);

    uint offset = 0;
    uint chunksize;
    for (uint i = 0u; i <= searchbound; ++i) {
        chunksize = 1 << i;
        for (uint j = 0; j < uint(val[i]+0.000001); ++j) {
            printInst(sche+i, offset, chunksize);
            offset += chunksize;
        }
    }

    delete[] cost;
    return 0;
}


