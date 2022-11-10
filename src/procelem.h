/**  
 * @file    procelem.h
 * @brief   Processing Element Definition
 * @author  Chenu Tang
 * @version 2.0
 * @date    2022-11-09
 * @note    
 */
#ifndef _PROCELEM_
#define _PROCELEM_
#include <map>
#include <vector>
#include <cstdio>
#include "instruction.h"
#include "booleandag.h"


class StageProcessors;
typedef struct Assignment {
    uint pid;   ///< pe id
    uint tid;   ///< task(vertice) id
    int starttime;      ///< starttime
    int finishtime;     ///< starttime
    Assignment() : pid(0), tid(0), starttime(0), finishtime(0) {};
} Assignment;


int checkPEAvail(); ///< If PE state allow a newly assigned task
int updatePEState();    ///< update PE state


///< @brief record real-time state of a single PE
typedef struct _PE {
    uint id;            ///< unique id
    std::vector<Assignment*>    tasks;  ///< Assigned tasks
    int eft;            ///< earlist finish time
    std::map<uint, uint> cache;     ///< <taskid, index> record cached id of nodes
    uint line[BLOCKROW];            ///< index -> taskid
    uint smallestfreeidx;
    uint overwriteflag;
    _PE() : id(0), eft(0), smallestfreeidx(0), overwriteflag(0) {for(uint i=0;i<BLOCKROW;++i)line[i]=UINT_MAX;};
} _PE;



class StageProcessors {
    uint pnum;      ///< number of processors
    std::map<uint, Assignment> schedule;    ///< <taskid, Assignment>
    _PE *PE;
    int makespan;
    std::vector<InstructionNameSpace::Instruction> inst;

public:
    StageProcessors *next;
    StageProcessors *prior;
    /* Constructor & Destructor */
    StageProcessors();
    // ~StageProcessors();

    /* Construction related */
    int init(uint n);  // unlimited memory for DAG with certain number of tasks
    int clean();

    /* TaskAssignment */
    int checkPlaceable(BooleanDag *G, uint peid, uint taskid);
    int assignTask(BooleanDag* g, uint taskid, uint PEid, int starttime, int finishtime);
    int releaseMem(BooleanDag* g, uint taskid, int *priority);
    int releaseMem(BooleanDag* g, uint taskid, bool *assigned);
    int assignFinish();
    int storeCache();

    /* Setters */
    int removeStoreInst(uint taskid);

    /* Visitors */
    int getmakespan();
    uint getpnum();
    _PE* getPE(uint id);
    StageProcessors* getNext();
    StageProcessors* getPrior();
    uint getPEByTask(uint taskid);
    uint getTaskByPE(uint PEid, int n = -1);
    uint getLine(uint taskid);
    uint getOverwritepos(uint peid);
    Assignment* getAssignmentByTask(uint taskid);

    /* Printers */
    void printScheduleByTasks();
    void printScheduleByPE();
    void printInstructions(int stage=0);
};

#endif