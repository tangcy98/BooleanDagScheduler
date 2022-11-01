/**  
 * @file    list-scheduler.h
 * @brief   List scheduling implemetation
 * @author  Chenu Tang
 * @version 0.1
 * @date    2022-10-17
 * @note    
 */
#ifndef _LISTSCHEDULER_
#define _LISTSCHEDULER_
#include "booleandag.h"
#include "priority.h"
#include "procelem.h"


class ListScheduler {

public:
    Processors* HEFTU(BooleanDag *g, uint pnum);
    Processors* HEFTD(BooleanDag *g, uint pnum);
    Processors* HEFTUD(BooleanDag *g, uint pnum);
};


#endif
