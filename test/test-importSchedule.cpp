#include <stdio.h>
#include "../src/booleandag.h"
#include "../src/importdag.h"
#include "../src/scheduler.h"
#include "../src/priority.h"

using namespace Priority;

int main()
{
    BooleanDag *G = v2booleandag("../benchmark/test.v");

    scheduleDAG(G, 32u);

    // printf("The makespan = %d\n", PU->getmakespan());

    // delete PU;

    return 0;
}

