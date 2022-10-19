/**  
 * @file    booleandag.cpp
 * @brief   Basic implementation of boolean dag
 * @author  Chenu Tang
 * @version 0.1
 * @date    2022-10-17
 * @note    
 */

#include <cstdio>
#include "booleandag.h"

///< WIP
int destroySingleEdge(Edge *e)
{
    delete e;
    return 1;
}

///< WIP
int cleanSingleVertice(Vertice *v)
{
    if (v->prednum) {

    }
    if (v->succnum) {
        
    }
    v->id = -1;
    v->type = NORMAL;
    v->weight = 0;
    return 1;
}


/* Constructor & Destructor */
BooleanDag::BooleanDag()
{
    V = NULL;
    size = 0u;
}

BooleanDag::BooleanDag(uint n)
{
    size = n;
    if (!n) {
        V = NULL;
    }
    else {
        V = new Vertice[n];
    }
}


/* Construction related */
int BooleanDag::init()
{
    if (size > 0) {
        destroy();
    }
    V = NULL;
    size = 0u;
    return 1;
}

int BooleanDag::init(uint n)
{
    if (size > 0) {
        destroy();
    }
    size = n;
    if (!n) {
        V = NULL;
    }
    else {
        V = new Vertice[n];
    }
    return 1;
}

///< WIP
int BooleanDag::destroy()
{

    return 1;
}

int BooleanDag::addVertice(int weight)
{
    for (uint i = 0; i < size; ++i) {
        if ((V+i)->weight < 0) {
            (V+i)->id = i;
            (V+i)->weight = weight;
            return 1;
        }
    }
    return 0;
}

int BooleanDag::addVertice(uint id, int weight)
{
    Vertice *v = V+id;
    if (v->weight >= 0) {
        cleanSingleVertice(v);
    }
    v->id = id;
    v->weight = weight;
    return 1;
}

int BooleanDag::addEdge(uint src, uint dest, int weight)
{
    Edge e;
    e.weight = weight;
    e.src = V+src;
    e.dest = V+dest;
    E.push_back(e);
    return 1;
}

int BooleanDag::linkDAG()
{
    int edgenum = (int)(E.size());
    std::vector<Edge>::iterator iter;
    for (iter = E.begin(); iter !=E.end(); ++iter) {
        iter->src->succnum++;
        iter->dest->prednum++;
    }

    Vertice *v;
    for (uint i = 0u; i < size; ++i) {
        v = V+i;
        if (v->prednum) {
            v->predecessors = new Edge*[v->prednum];
        }
        if (v->succnum) {
            v->successors = new Edge*[v->succnum];
        }
    }

    for (iter = E.begin(); iter !=E.end(); ++iter) {
        *(iter->src->successors++) = &(*iter);
        *(iter->dest->predecessors++) = &(*iter);
    }

    for (uint i = 0u; i < size; ++i) {
        v = V+i;
        v->predecessors -= v->prednum;
        v->successors -= v->succnum;
    }
    return 1;
}



/* Vistors */
uint BooleanDag::getsize()
{
    return size;
}

Vertice* BooleanDag::getvertice(uint id)
{
    return V+id;
}

void BooleanDag::traversePrint()
{
    Vertice *v;
    Edge *e;
    printf("--------Boolean Dag Traverse BEGIN--------\n\n");
    for (uint i = 0; i < size; ++i) {
        v = V+i;
        printf("[Vertice No.%u] weight:%d\n", i+1, v->weight);

        printf("\tPredecessors:");
        for (uint j = 0; j < v->prednum; ++j) {
            e = *(v->predecessors+j);
            printf(" %u(%d)", e->src->id+1, e->weight);
        }
        printf("\n");

        printf("\tSuccessors:");
        for (uint j = 0; j < v->succnum; ++j) {
            e = *(v->successors+j);
            printf(" %u(%d)", e->dest->id+1, e->weight);
        }
        printf("\n");

        printf("\n");
    }
    printf("--------Boolean Dag Traverse END--------\n");
}