/**  
 * @file    scheduler.cpp
 * @brief   Schedule the whole DAG and map it to HW
 * @author  Chenu Tang
 * @version 2.0
 * @date    2022-11-09
 * @note    
 */

#include "scheduler.h"
#include "priority.h"
#include "placement.h"
#include "procelem.h"

/**
 * @brief Get next stage
 * 
 * @param G BooleanDag
 * @param offset priority beginning offset
 * @return uint next offset
 */

int scheduleDAG(BooleanDag *G, uint workload)
{
    int *priority;
    int pnum = MESHSIZE / workload;
    int totalms = 0;
    if (pnum <= 0) {
        return -1;
    }
    uint size = G->getsize();
    priority = Priority::ranku(G);
    G->setPriority(priority);
    std::multimap<int, uint, std::greater<int>> ranklist;   ///< <rank, taskid>
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
        std::multimap<int, uint, std::greater<int>>::iterator iter = ranklist.begin();
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
            ranklist.erase(iter);
            if (pid != UINT_MAX) {
                ++taskcnt;
                assigned[taskid] = true;
                (*p)->releaseMem(G, taskid, assigned);
            }
        }
        (*p)->assignFinish();
        // (*p)->printScheduleByPE();
        // (*p)->printInstructions(stagecnt-1);
        totalms += (*p)->getmakespan();
        printf("makespan: %d\n", (*p)->getmakespan());
        prior = *p;
        p = &((*p)->next);
    }

    p = &stages;
    for (uint i =0u; i < stagecnt; ++i) {
        (*p)->printInstructions(i);
        p = &((*p)->next);
    }

    printf("total makespan: %d\n", totalms);

    delete[] assigned;
    delete[] priority;
    return 1;
}
