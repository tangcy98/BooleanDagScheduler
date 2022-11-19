/**  
 * @file    scheduler.cpp
 * @brief   Schedule the whole DAG and map it to HW
 * @author  Chenu Tang
 * @version 2.3
 * @date    2022-11-18
 * @note    
 */

#include "scheduler.h"
#include "priority.h"
#include "placement.h"
#include "procelem.h"


Schedule scheduleDAG(BooleanDag *G, uint workload)
{
    Schedule sche;
    sche.chunksize = workload;
    bigint *priority;
    int pnum = MESHSIZE / workload;
    bigint totalms = 0;
    if (pnum <= 0) {
        exit(-1);
    }
    uint size = G->getsize();
    priority = Priority::ranku(G);
    G->setPriority(priority);
    std::multimap<bigint, uint, std::greater<bigint>> ranklist;   ///< <rank, taskid>
    bool *assigned = new bool[size];;
    for (uint i = 0; i < G->getsize(); ++i) {
        ranklist.insert(std::make_pair(priority[i], i));    ///< ordered list(map)
        assigned[i] = false;
    }

    StageProcessors *stages = NULL;
    StageProcessors *prior = NULL;
    StageProcessors **p = &stages;
    uint stagecnt = 0;

    while (ranklist.size()) {
        ++stagecnt;
        std::multimap<bigint, uint, std::greater<bigint>>::iterator iter = ranklist.begin();
        uint taskid;
        uint pid = 0;
        *p = new StageProcessors;
        (*p)->init(pnum);
        (*p)->prior = prior;

        uint taskcnt = 0;
        while (ranklist.size() && pid != UINT_MAX) {
            iter = ranklist.begin();
            taskid = iter->second;
            pid = ESTPlacement(G, *p, taskid);
            if (pid != UINT_MAX) {
                ranklist.erase(iter);
                ++taskcnt;
                assigned[taskid] = true;
                (*p)->releaseMem(G, taskid, assigned);
            }
        }
        (*p)->assignFinish();
        // (*p)->printScheduleByPE();
        // (*p)->printInstructions(stagecnt-1);
        totalms += (*p)->getmakespan();
        // printf("makespan: %lld\n", (*p)->getmakespan());
        prior = *p;
        p = &((*p)->next);
    }

    sche.latency = 0;
    sche.oplatency = 0;
    sche.energy = 0;
    p = &stages;
    while ((*p)) {
        (*p)->calcLatency();
        (*p)->calcEnergy();
        sche.latency += (*p)->getLatency();
        sche.oplatency += (*p)->getOPLatency();
        sche.energy += (*p)->getEnergy();

        // printf("%lld %lf\n", sche.latency, sche.energy);
        p = &((*p)->next);

    }

    // printf("total makespan: %lld\n", totalms);

    delete[] assigned;
    delete[] priority;

    sche.p = stages;
    return sche;
}

inline uint findMaxPrioTask(uint size, bigint* priority, bool* assigned)
{
    bigint max;
    uint i = 0;
    uint res = 0;
    while (assigned[i]) i++;
    res = i;
    max = priority[i];
    for (++i; i < size; ++i) {
        if (!assigned[i] && priority[i] > max) {
            res = i;
        }
    }
    return res;
}

inline uint findMinPrioTask(uint size, bigint* priority, bool* assigned)
{
    bigint min;
    uint i = 0;
    uint res = 0;
    while (assigned[i]) i++;
    res = i;
    min = priority[i];
    for (++i; i < size; ++i) {
        if (!assigned[i] && priority[i] < min) {
            res = i;
        }
    }
    return res;
}

Schedule scheduleDAGDynamic(BooleanDag *G, uint workload)
{
    Schedule sche;
    sche.chunksize = workload;
    bigint *priority;
    int pnum = MESHSIZE / workload;
    bigint totalms = 0;
    std::map<std::pair<uint, uint>, bigint> newWeight;
    if (pnum <= 0) {
        exit(-1);
    }
    uint size = G->getsize();
    priority = Priority::rankd(G);
    G->setPriority(priority);
    bool *assigned = new bool[size];

    for (uint i = 0; i < G->getsize(); ++i) {
        assigned[i] = false;
    }

    StageProcessors *stages = NULL;
    StageProcessors *prior = NULL;
    StageProcessors **p = &stages;
    uint stagecnt = 0;
    uint tasksnum = size;

    while (tasksnum) {
        ++stagecnt;
        uint taskid;
        uint pid = 0;
        *p = new StageProcessors;
        (*p)->init(pnum);
        (*p)->prior = prior;

        uint taskcnt = 0;
        while (tasksnum && pid != UINT_MAX) {
            taskid = findMinPrioTask(size, priority, assigned);
            pid = ESTPlacement(G, *p, taskid, &newWeight);
            if (pid != UINT_MAX) {
                ++taskcnt;
                assigned[taskid] = true;
                --tasksnum;
                (*p)->releaseMem(G, taskid, assigned);
                Priority::rankd(G, taskid, priority, &newWeight);
            }
        }
        (*p)->assignFinish();
        // (*p)->printScheduleByPE();
        // (*p)->printInstructions(stagecnt-1);
        totalms += (*p)->getmakespan();
        // printf("makespan: %lld\n", (*p)->getmakespan());
        prior = *p;
        p = &((*p)->next);
    }

    sche.latency = 0;
    sche.oplatency = 0;
    sche.energy = 0;
    p = &stages;
    while ((*p)) {
        (*p)->calcLatency();
        (*p)->calcEnergy();
        sche.latency += (*p)->getLatency();
        sche.oplatency += (*p)->getOPLatency();
        sche.energy += (*p)->getEnergy();

        // printf("%lld %lf\n", sche.latency, sche.energy);
        p = &((*p)->next);

    }

    // printf("total makespan: %lld\n", totalms);

    delete[] assigned;
    delete[] priority;

    sche.p = stages;
    return sche;
}



void printInst(Schedule *s, uint offset, uint chunksize)
{
    StageProcessors **p = &(s->p);
    int stage = 0;
    printf("Offset: %d, Chunksize: %d\n", offset, chunksize);
    while ((*p)) {
        (*p)->printInstructions(stage++);
        p = &(*p)->next;
    }
}
