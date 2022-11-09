/**  
 * @file    list-scheduler.cpp
 * @brief   List scheduling implemetation
 * @author  Chenu Tang
 * @version 2.0
 * @date    2022-11-09
 * @note    
 */
#include "list-scheduler.h"

using namespace Priority;


StageProcessors* ListScheduler::HEFTU(BooleanDag *g, uint pnum)
{
    int *rank;
    uint size = g->getsize();
    StageProcessors *P = new StageProcessors;
    P->init(pnum);

    std::multimap<int, uint, std::greater<int>> ranklist;   ///< <rank, taskid>
    rank = ranku(g);

    for (uint i = 0; i < size; ++i) {
        ranklist.insert(std::make_pair(rank[i], i));    ///< ordered list(map)
    }

    while (ranklist.size()) {
        std::multimap<int, uint, std::greater<int>>::iterator iter;
        int est = INT_MAX;
        uint taskid;
        uint prednum;
        uint pid = 0u;

        iter = ranklist.begin();
        taskid = iter->second;
        Vertice *v = g->getvertice(taskid);
        prednum = v->prednum;
        uint *predpeid = new uint[prednum];
        int *predfinishtime = new int[prednum];
        int *predcommcost = new int[prednum];

        // calculate each predecessors aft(eft) with consideration of communication
        for (uint i = 0u; i < prednum; ++i) {
            Edge *e = v->predecessors[i];
            uint predid = e->src->id;
            Assignment* predassignment = P->getAssignmentByTask(predid);
            predpeid[i] = predassignment->pid;
            predfinishtime[i] = predassignment->finishtime;
            predcommcost[i] = e->weight;
        }
        for (uint i = 0; i < pnum; ++i) {
            int avail = P->getPE(i)->eft;
            for (uint j = 0u; j < prednum; ++j) {
                int predt = predfinishtime[j] + (predpeid[j] == i ? 0 : predcommcost[j]);
                avail = avail > predt ? avail : predt;
            }
            if (avail < est) {
                est = avail;
                pid = i;
            }
        }

        P->assignTask(g, taskid, pid, est, est+v->weight);
        ranklist.erase(iter);
        delete[] predpeid;
        delete[] predfinishtime;
        delete[] predcommcost;
    }

    delete[] rank;
    return P;
}

StageProcessors* ListScheduler::HEFTD(BooleanDag *g, uint pnum)
{
    int *rank;
    uint size = g->getsize();
    StageProcessors *P = new StageProcessors;
    P->init(pnum);

    std::multimap<int, uint, std::less<int>> ranklist;   ///< <rank, taskid>
    rank = rankd(g);

    for (uint i = 0; i < size; ++i) {
        ranklist.insert(std::make_pair(rank[i], i));    ///< ordered list(map)
    }

    while (ranklist.size()) {
        std::multimap<int, uint, std::less<int>>::iterator iter;
        int est = INT_MAX;
        uint taskid;
        uint prednum;
        uint pid = 0u;

        iter = ranklist.begin();
        taskid = iter->second;
        Vertice *v = g->getvertice(taskid);
        prednum = v->prednum;
        uint *predpeid = new uint[prednum];
        int *predfinishtime = new int[prednum];
        int *predcommcost = new int[prednum];

        // calculate each predecessors aft(eft) with consideration of communication
        for (uint i = 0u; i < prednum; ++i) {
            Edge *e = v->predecessors[i];
            uint predid = e->src->id;
            Assignment* predassignment = P->getAssignmentByTask(predid);
            predpeid[i] = predassignment->pid;
            predfinishtime[i] = predassignment->finishtime;
            predcommcost[i] = e->weight;
        }
        for (uint i = 0; i < pnum; ++i) {
            int avail = P->getPE(i)->eft;
            for (uint j = 0u; j < prednum; ++j) {
                int predt = predfinishtime[j] + (predpeid[j] == i ? 0 : predcommcost[j]);
                avail = avail > predt ? avail : predt;
            }
            if (avail < est) {
                est = avail;
                pid = i;
            }
        }

        P->assignTask(g, taskid, pid, est, est+v->weight);
        ranklist.erase(iter);
        delete[] predpeid;
        delete[] predfinishtime;
        delete[] predcommcost;
    }

    delete[] rank;
    return P;
}

StageProcessors* ListScheduler::HEFTUD(BooleanDag *g, uint pnum)
{
    int *urank, *drank;
    uint size = g->getsize();
    StageProcessors *P = new StageProcessors;
    P->init(pnum);

    std::multimap<int, uint, std::less<int>> ranklist;   ///< <rank, taskid>
    drank = rankd(g);
    urank = ranku(g);

    for (uint i = 0; i < size; ++i) {
        ranklist.insert(std::make_pair(drank[i] - urank[i], i));    ///< ordered list(map)
    }

    while (ranklist.size()) {
        std::multimap<int, uint, std::less<int>>::iterator iter;
        int est = INT_MAX;
        uint taskid;
        uint prednum;
        uint pid = 0u;

        iter = ranklist.begin();
        taskid = iter->second;
        Vertice *v = g->getvertice(taskid);
        prednum = v->prednum;
        uint *predpeid = new uint[prednum];
        int *predfinishtime = new int[prednum];
        int *predcommcost = new int[prednum];

        // calculate each predecessors aft(eft) with consideration of communication
        for (uint i = 0u; i < prednum; ++i) {
            Edge *e = v->predecessors[i];
            uint predid = e->src->id;
            Assignment* predassignment = P->getAssignmentByTask(predid);
            predpeid[i] = predassignment->pid;
            predfinishtime[i] = predassignment->finishtime;
            predcommcost[i] = e->weight;
        }
        for (uint i = 0; i < pnum; ++i) {
            int avail = P->getPE(i)->eft;
            for (uint j = 0u; j < prednum; ++j) {
                int predt = predfinishtime[j] + (predpeid[j] == i ? 0 : predcommcost[j]);
                avail = avail > predt ? avail : predt;
            }
            if (avail < est) {
                est = avail;
                pid = i;
            }
        }

        P->assignTask(g, taskid, pid, est, est+v->weight);
        ranklist.erase(iter);
        delete[] predpeid;
        delete[] predfinishtime;
        delete[] predcommcost;
    }

    delete[] drank;
    delete[] urank;
    return P;
}