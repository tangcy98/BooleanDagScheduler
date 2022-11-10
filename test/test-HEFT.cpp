#include <stdio.h>
#include "../src/list-scheduler.h"

using namespace Priority;

int main()
{
    BooleanDag G;

    int vweight[] = {2, 3, 3, 4, 5, 4, 4, 4, 1};
    uint src[] = {0, 0, 0, 0, 0, 1, 1, 2, 3, 5, 6, 7};
    uint dest[] = {1, 2, 3, 4, 6, 5, 6, 7, 7, 8, 8, 8};
    int eweight[] = {4, 1, 1, 1, 10, 1, 1, 1, 1, 5, 6, 5};
    G.init(9);

    for (int i = 0; i < 9; ++i) {
        G.addVertice(vweight[i]);
    }
    for (int i = 0; i < 12; ++i) {
        G.addEdge(src[i], dest[i], eweight[i]);
    }
    G.linkDAG();
    // G.traversePrint();

    ListScheduler S;
    StageProcessors *PU = S.HEFTU(&G, 3);
    StageProcessors *PD = S.HEFTD(&G, 3);
    StageProcessors *PUD = S.HEFTUD(&G, 3);
    PU->printScheduleByPE();
    PU->printScheduleByTasks();
    printf("The makespan = %d\n", PU->getmakespan());

    PD->printScheduleByPE();
    PD->printScheduleByTasks();
    printf("The makespan = %d\n", PD->getmakespan());

    PUD->printScheduleByPE();
    PUD->printScheduleByTasks();
    printf("The makespan = %d\n", PUD->getmakespan());
    delete PU;
    delete PD;
    delete PUD;

    return 0;
}

