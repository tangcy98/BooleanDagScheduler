/**  
 * @file    scheduler.h
 * @brief   Schedule the whole DAG and map it to HW
 * @author  Chenu Tang
 * @version 2.0
 * @date    2022-11-09
 * @note    
 */
#include "booleandag.h"
#include "procelem.h"


///< MESHSIZE is the max length
///< Explore 2^i's cost and find the best i


typedef struct Schedule {
    int makespan;
    int chunksize;
    StageProcessors *p;
} Schedule;

Schedule scheduleDAG(BooleanDag *G, uint workload);

void printInst(Schedule *s, uint offset, uint chunksize);