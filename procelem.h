/**  
 * @file    procelem.h
 * @brief   Processing Element Definition
 * @author  Chenu Tang
 * @version 0.1
 * @date    2022-10-18
 * @note    
 */
#ifndef _PROCELEM_
#define _PROCELEM_
#include <map>
#include <vector>
#include <cstdio>
#include "booleandag.h"


class Processors;
typedef struct Assignment {
    uint pid;   ///< pe id
    uint tid;   ///< task(vertice) id
    int starttime;      ///< starttime
    int finishtime;     ///< starttime
    Assignment() : pid(0), tid(0), starttime(0), finishtime(0) {};
} Assignment;

///< @brief record real-time state of a single PE
typedef struct _PE {
    uint id;            ///< unique id
    std::vector<Assignment*>    tasks;  ///< Assigned tasks
    int eft;            ///< earlist finish time
    _PE() : id(0), eft(0) {};
} _PE;

class Processors {
    uint pnum;      ///< number of processors
    std::vector<Assignment> schedule;
    _PE *PE;
    int makespan;

public:
    /* Constructor & Destructor */
    Processors();
    // ~Processors();

    /* Construction related */
    int init(uint n, uint tn);
    int clean();

    /* TaskAssignment */
    int assignTask(uint taskid, uint PEid, int starttime, int finishtime);

    /* Visitors */
    int getmakespan();
    _PE* getPE(uint id);
    uint getPEByTask(uint taskid);
    uint getTaskByPE(uint PEid, int n = -1);
    Assignment* getAssignmentByTask(uint taskid);
    Assignment* getAssignmentByPE(uint PEid, int n = -1);

    /* PrintSchedule */
    void printScheduleByTasks();
    void printScheduleByPE();
};

#endif