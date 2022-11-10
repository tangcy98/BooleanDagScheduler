/**  
 * @file    priority.h
 * @brief   Compute the priority of each vertice in a DAG
 * @author  Chenu Tang
 * @version 2.0
 * @date    2022-11-09
 * @note    
 */
#ifndef _PRIORITY_
#define _PRIORITY_
#include "booleandag.h"
namespace Priority {
    /* Basic Functions */
    int* ranku(BooleanDag *g, int *value=NULL);     ///< calculate the upward rank of each vertice in a DAG (static usually)
    int* bleavel(BooleanDag *g, int *value=NULL);   ///< calculate the bottom level of each vertice in a DAG - same as ranku (static usually)
    int* rankd(BooleanDag *g, int *value=NULL);     ///< calculate the downward rank of each vertice in a DAG
    int* tleavel(BooleanDag *g, int *value=NULL);   ///< calculate the top level of each vertice in a DAG - same as rankd

    /* Recursive Functions */
    int ranku(BooleanDag *g, uint id, int *value);      ///< calculate the upward rank of a vertice in a DAG in a recursive way
    int bleavel(BooleanDag *g, uint id, int *value);    ///< calculate the bottom level of a vertice in a DAG in a recursive way
    int rankd(BooleanDag *g, uint id, int *value);      ///< calculate the downward rank of a vertice in a DAG in a recursive way
    int tleavel(BooleanDag *g, uint id, int *value);    ///< calculate the top level of a vertice in a DAG in a recursive way
}
#endif