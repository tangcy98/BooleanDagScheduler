#include <stdio.h>
#include "importdag.h"
#include "list-scheduler.h"

using namespace Priority;

int addV(Vertice *v, bool *mark)
{
    int addn = 1;
    mark[v->id] = true;
    if (v->prednum != 0) {
        Vertice *pred;
        for (uint i = 0; i < v->prednum; ++i) {
            pred = v->predecessors[i]->src;
            printf("%u wow!!\n", pred->id);
            if (mark[pred->id] == false) {
                addn += addV(pred, mark);
            }
        }
    }
    if (addn > 1) {
        printf("a\n");
    }
    return addn;
}

int addV2G(BooleanDag *G, Vertice *v, bool *mark)
{
    int addn = 1;
    mark[v->id] = true;
    if (v->prednum != 0) {
        Vertice *pred;
        for (uint i = 0; i < v->prednum; ++i) {
            pred = v->predecessors[i]->src;
            addn += addV2G(G, pred, mark);
        }
    }

    return addn;
}

int main()
{
    BooleanDag *G = v2booleandag("test.v");
    BooleanDag G1;
    uint Gsize = G->getsize() / 2;
    uint size = Gsize / 2;
    uint s = 0;
    bool *mark = new bool[Gsize];

    for (uint i = 0u; i < Gsize; ++i) {
        mark[i] = false;
    }

    uint i = 0;
    while (s < size) {
        if (mark[i]) continue;  // already in G1
        Vertice *v = G->getvertice(i++);
        s += addV(v, mark);
    }
    printf("%u %u", Gsize, s);
    return 0;

    G1.init(s);

    s = 0;
    for (uint i = 0u; i < Gsize; ++i) {
        mark[i] = false;
    }
    while (s < size) {
        if (mark[i]) continue;  // already in G1
        Vertice *v = G->getvertice(i++);
        s += addV2G(&G1, v, mark);
    }


    ListScheduler S;
    StageProcessors *PU = S.HEFTU(&G1, 8);
    StageProcessors *PD = S.HEFTD(&G1, 8);
    StageProcessors *PUD = S.HEFTUD(&G1, 8);
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

