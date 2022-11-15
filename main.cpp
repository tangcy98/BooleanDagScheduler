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

    uint top = (Bsize < MESHSIZE ? Bsize : MESHSIZE);
    uint searchbound = LOG2(top);
    if (NPOWEROF2(searchbound) < Bsize && NPOWEROF2(searchbound) < MESHSIZE) ++searchbound;
    printf("Bsize:%d, searchbound:%d\n", Bsize, searchbound);
    
    Schedule *sche = new Schedule[searchbound+1];
    double *cost = new double[searchbound+1];

    for (uint i = 0u; i <= searchbound; ++i) {
        sche[i] = scheduleDAG(G, NPOWEROF2(i));
        cost[i] = sche[i].latency;
    }

    CutSolver solver(Bsize, searchbound, cost);

    bool optimal = solver.isOptimal();
    const double *val = solver.getSolution();
    double res = solver.getObjValue();
    printf("cost: %g %g %g %g %g %g %g\n", cost[0], cost[1], cost[2], cost[3], cost[4], cost[5], cost[6]);
    printf("Solution res: %g, %g %g %g %g %g %g %g %d\n", res, val[0], val[1], val[2], val[3], val[4], val[5], val[6], optimal ? 1 : 0);

    uint offset = 0;
    uint chunksize;
    bigint latency = 0;
    double energy = 0.0;
    for (uint i = 0u; i <= searchbound; ++i) {
        chunksize = 1 << i;
        for (uint j = 0; j < uint(val[i]+0.000001); ++j) {
            // printInst(sche+i, offset, chunksize);
            // offset += chunksize;
            latency += sche[i].latency;
            energy += sche[i].energy;
            printf("latency: %lld, energy: %lf\n", sche[i].latency, sche[i].energy);
        }
    }

    printf("final latency: %lldμs, energy: %lfnJ\n", latency / 1000000ll, energy / 1000.0);

    delete[] cost;
    delete[] sche;
    return 0;
}


