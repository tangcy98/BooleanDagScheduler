/**  
 * @file    list-scheduler.h
 * @brief   List scheduling implemetation
 * @author  Chenu Tang
 * @version 2.0
 * @date    2022-11-09
 * @note    
 */
#ifndef _LISTSCHEDULER_
#define _LISTSCHEDULER_
#include "booleandag.h"
#include "priority.h"
#include "procelem.h"


class ListScheduler {

public:
    StageProcessors* HEFTU(BooleanDag *g, uint pnum);
    StageProcessors* HEFTD(BooleanDag *g, uint pnum);
    StageProcessors* HEFTUD(BooleanDag *g, uint pnum);
};


#endif
