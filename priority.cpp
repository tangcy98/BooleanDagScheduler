/**  
 * @file    priority.cpp
 * @brief   Compute the priority of each vertice in a DAG
 * @author  Chenu Tang
 * @version 0.1
 * @date    2022-10-17
 * @note    
 */

#include "priority.h"
#include "booleandag.h"
#include <cstdio>
int *Priority::ranku(BooleanDag *g, int *value)
{
    int size = g->getsize();
    if (!value) {
        value = new int[size];
    }
    for (uint i = 0u; i < size; ++i) {
        *(value+i) = -1;
    }
    for (uint i = 0u; i < size; ++i) {
        ranku(g, i, value);
    }
    return value;
}

int Priority::ranku(BooleanDag *g, uint id, int *value)
{
    if (value[id] >= 0) {
        return value[id];
    }
    Vertice *v = g->getvertice(id);
    if (v->succnum == 0) {
        return (value[id] = v->weight);
    }
    int max = v->weight;
    int currank;
    Edge *e;
    for (uint i = 0; i < v->succnum; ++i) {
        e = *(v->successors+i);
        currank = v->weight + e->weight + ranku(g, e->dest->id, value);
        max = max > currank ? max : currank;
    }
    return (value[id] = max);
}

// int main()
// {
//     BooleanDag g;
//     Priority::ranku(&g);
//     printf("%s\n", __FILE__);
//     return 0;
// }