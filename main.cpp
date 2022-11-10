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
    const char *inputfile = argv[1];
    uint size = atoi(argv[2]);
    uint Bsize = (size + BLOCKCOL - 1) / BLOCKCOL;

    BooleanDag *G = v2booleandag(inputfile);

    int searchbound = LOG2(Bsize < MESHSIZE ? Bsize : MESHSIZE);
    if ((1<<searchbound) < Bsize) ++searchbound;

    

    return 0;
}


