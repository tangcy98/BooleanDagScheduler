/**  
 * @file    placement.h
 * @brief   Task placement strategy
 * @author  Chenu Tang
 * @version 2.0
 * @date    2022-11-09
 * @note    
 */
#ifndef _PLACEMENT_
#define _PLACEMENT_

#include "booleandag.h"
#include "procelem.h"
#include <map>

int getCommLevel(uint totalpnum, uint p1, uint p2);

uint ESTPlacement(BooleanDag *G, StageProcessors *P, uint taskid, std::map<std::pair<uint, uint>, bigint>* newWeight=NULL);


#endif