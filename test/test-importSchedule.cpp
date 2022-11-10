#include <stdio.h>
#include "../booleandag.h"
#include "../importdag.h"
#include "../scheduler.h"
#include "../priority.h"

using namespace Priority;

int main()
{
    BooleanDag *G = v2booleandag("test.v");

    scheduleDAG(G, 32u);

    // printf("The makespan = %d\n", PU->getmakespan());

    // delete PU;

    return 0;
}

