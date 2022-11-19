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
bigint *Priority::ranku(BooleanDag *g, bigint *value)
{
    int size = g->getsize();
    if (!value) {
        value = new bigint[size];
    }
    for (uint i = 0u; i < size; ++i) {
        *(value+i) = -1;
    }
    for (uint i = 0u; i < size; ++i) {
        ranku(g, i, value);
    }
    return value;
}

bigint Priority::ranku(BooleanDag *g, uint id, bigint *value)
{
    if (value[id] >= 0) {
        return value[id];
    }
    Vertice *v = g->getvertice(id);
    if (v->succnum == 0) {
        return (value[id] = v->weight);
    }
    bigint max = v->weight;
    bigint currank;
    Edge *e;
    for (uint i = 0; i < v->succnum; ++i) {
        e = *(v->successors+i);
        currank = v->weight + e->weight + ranku(g, e->dest->id, value);
        max = max > currank ? max : currank;
    }
    return (value[id] = max);
}

bigint *Priority::rankd(BooleanDag *g, bigint *value)
{
    int size = g->getsize();
    if (!value) {
        value = new bigint[size];
        for (uint i = 0u; i < size; ++i) {
            *(value+i) = -1;
        }
    }

    for (uint i = 0u; i < size; ++i) {
        rankd(g, i, value);
    }
    return value;
}

bigint Priority::rankd(BooleanDag *g, uint id, bigint *value, std::map<std::pair<uint, uint>, bigint> *newWeight)
{
    if (value[id] >= 0 && !newWeight) {
        return value[id];
    }
    Vertice *v = g->getvertice(id);
    if (v->prednum == 0) {
        return (value[id] = 0);
    }
    bigint max = 0;
    bigint currank;
    Edge *e;
    std::map<std::pair<uint, uint>, bigint>::iterator it;
    for (uint i = 0; i < v->prednum; ++i) {
        e = *(v->predecessors+i);
        if (newWeight)
            it = newWeight->find(std::make_pair(e->src->id, e->dest->id));
        if (newWeight && it != newWeight->end()) {
            currank = e->src->weight + it->second + rankd(g, e->src->id, value, newWeight);
        }
        else {
            currank = e->src->weight + e->weight + rankd(g, e->src->id, value, newWeight);
        }
        max = max > currank ? max : currank;
    }
    return (value[id] = max);
}

void clearChildren(const BooleanDag *g, bigint *value, const Vertice *v)
{
    uint succnum = v->succnum;
    value[v->id] = -1;
    for (uint i = 0; i < succnum; ++i) {
        clearChildren(g, value, v->successors[i]->dest);
    }
}

// int main()
// {
//     BooleanDag g;
//     Priority::ranku(&g);
//     printf("%s\n", __FILE__);
//     return 0;
// }