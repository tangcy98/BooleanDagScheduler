/**  
 * @file    placement.cpp
 * @brief   Task placement strategy
 * @author  Chenu Tang
 * @version 2.3
 * @date    2022-11-18
 * @note    
 */

#include "placement.h"


uint ESTPlacement(BooleanDag *G, StageProcessors *P, uint taskid)
{
    uint pid;   ///< return value
    bigint est = INT_MAX;
    uint pnum = P->getpnum();
    uint prednum;
    _PE* pe;
    Vertice *v = G->getvertice(taskid);
    prednum = v->prednum;
    if (prednum == 0) {
        return pnum;
    }

    uint *predpeid = new uint[prednum];
    bigint *predfinishtime = new bigint[prednum];
    bigint *predcommcost = new bigint[prednum];


    for (uint i = 0u; i < prednum; ++i) {
        Edge *e = v->predecessors[i];
        uint predid = e->src->id;
        Assignment* predassignment = P->getAssignmentByTask(predid);

        if (predassignment) {
            predpeid[i] = predassignment->pid;
            predfinishtime[i] = predassignment->finishtime;
            predcommcost[i] = e->weight;
        }
        else {
            if (P->prior) {
                predassignment = P->prior->getAssignmentByTask(predid);
            }
            if (predassignment && P->prior->getLine(predid)>=P->getOverwritepos(predassignment->pid)) {
                predpeid[i] = predassignment->pid;
            }
            else {
                predpeid[i] = pnum;
            }
            
            predfinishtime[i] = 0;
            predcommcost[i] = e->weight;
        }
    }

    ///< Try each pe as target
    int placeable = 0;
    for (uint i = 0u; i < pnum; ++i) {
        pe = P->getPE(i);
        if (P->checkPlaceable(G, i, taskid)) {
            placeable = 1;
        }
        else {
            continue;
        }
        bigint avail = pe->eft;
        for (uint j = 0u; j < prednum; ++j) {
            bigint predt = predfinishtime[j] + (CommWeight[getCommLevel(pnum, i, predpeid[j])]);
            avail = avail > predt ? avail : predt;
        }
        if (avail < est) {
            est = avail;
            pid = i;
        }
    }
    if (placeable) {
        P->assignTask(G, taskid, pid, est, est+v->weight);
    }
    else {
        return UINT_MAX;
    }
    return pid;
}
