#include "../src/memory.h"
#include "../src/booleandag.h"
#include "../src/scheduler.h"
#include "../src/importdag.h"
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
    int size = atoi(argv[2]);
    int Bsize = (size + BLOCKCOL - 1) / BLOCKCOL;

    BooleanDag *G = v2booleandag(inputfile);

    

    return 0;
}


