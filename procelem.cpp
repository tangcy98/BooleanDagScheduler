
/**  
 * @file    procelem.cpp
 * @brief   Processing Element Implementation
 * @author  Chenu Tang
 * @version 0.1
 * @date    2022-10-18
 * @note    
 */


#include "procelem.h"



Processors::Processors()
{
    pnum = 0;
    PE = NULL;
    makespan = 0;
}


int Processors::init(uint pn, uint tn)
{
    if (PE) {
        delete[] PE;
    }
    pnum = pn;
    schedule.clear();
    Assignment defaultAssignment;
    defaultAssignment.pid = 0u;
    defaultAssignment.tid = 0u;
    defaultAssignment.starttime = 0;
    defaultAssignment.finishtime = 0;
    schedule.resize(tn, defaultAssignment);
    PE = new _PE[pnum];
    makespan = 0;
    return 1;
}

int Processors::clean()
{
    if (PE) {
        delete[] PE;
    }
    pnum = 0;
    schedule.clear();
    PE = NULL;
    makespan = 0;
    return 1;
}

// TODO: Check conflict with existing assignment
int Processors::assignTask(uint taskid, uint PEid, int starttime, int finishtime)
{
    Assignment *a = &schedule.at(taskid);
    a->pid = PEid;
    a->tid = taskid;
    a->starttime = starttime;
    a->finishtime = finishtime;
    makespan = makespan > finishtime ? makespan : finishtime;

    _PE *pe = PE+PEid;
    pe->eft = pe->eft > finishtime ? pe->eft : finishtime;
    for (std::vector<Assignment*>::iterator iter = pe->tasks.begin(); iter != pe->tasks.end(); ++iter) {
        if (starttime < (*iter)->starttime) {
            pe->tasks.insert(iter, a);
            return 1;
        }
    }
    pe->tasks.push_back(a);
    return 1;
}


/* Visitors */

int Processors::getmakespan()
{
    return makespan;
}


_PE* Processors::getPE(uint id)
{
    return PE+id;
}


uint Processors::getPEByTask(uint taskid)
{
    return schedule.at(taskid).pid;
}

uint Processors::getTaskByPE(uint PEid, int n)
{
    if (!PE) {
        exit(-1);
    }
    _PE * pe;
    pe = PE+PEid;
    if (n >= 0 && n < pe->tasks.size()) {
        return pe->tasks[n]->tid;
    }
    return pe->tasks.at(pe->tasks.size()-1)->tid;
}

Assignment* Processors::getAssignmentByTask(uint taskid)
{
    return &schedule.at(taskid);
}

void Processors::printScheduleByTasks()
{
    printf("--------Schedule of each Task BEGIN--------\n");

    uint size = schedule.size();
    Assignment *a;
    for (uint i = 0u; i < size; ++i) {
        a = getAssignmentByTask(i);
        printf("Task %u is assigned to PE No.%u. Start at %d. End at %d.\n", a->tid, a->pid, a->starttime, a->finishtime);
    }
    printf("--------Schedule of each Task END--------\n");
}

void Processors::printScheduleByPE()
{
    _PE *pe;
    std::vector<Assignment*>::iterator ait;
    int tasknum;
    printf("--------Schedule of each PE BEGIN--------\n");
    for (uint i = 0u; i < pnum; ++i) {
        pe = PE+i;
        printf("No.%u PE's schedule:\n", i);
        for (ait = pe->tasks.begin(); ait != pe->tasks.end(); ++ait) {
            printf("\t[Task No.%u] Start at %d. End at %d.\n", (*ait)->tid, (*ait)->starttime, (*ait)->finishtime);
        }
    }
    printf("--------Schedule of each PE END--------\n");
}