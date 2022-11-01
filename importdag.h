/**  
 * @file    importdag.h
 * @brief   Import a DAG in other forms
 * @author  Chenu Tang
 * @version 0.1
 * @date    2022-10-25
 * @note    
 */
#ifndef _IMPORTDAG_
#define _IMPORTDAG_
#include <string>
#include "booleandag.h"

BooleanDag *v2booleandag(const std::string &filename);


#endif