/**  
 * @file    booleandag.h
 * @brief   Basic implementation of boolean dag
 * @author  Chenu Tang
 * @version 2.0
 * @date    2022-11-09
 * @note    
 */
#ifndef _BOOLEANDAG_
#define _BOOLEANDAG_
#include <cstdio>
#include <vector>
#include <string>
#include <set>
#include "memory.h"

enum VerticeType {
    NORMAL, CONSTANT, VINV, VAND, VOR, VXOR, VXOR3, VMAJ,
};
struct Edge;

typedef struct SubGraph {
    uint size;      ///< number of vertices
    std::set<uint> idset;   ///< vertices set
    uint length;    ///< workload size
} SubGraph;


/**
 * @brief Vertice class
 * Reperesent a single vertice with its unique id 
 * and other basic information
 */
typedef struct Vertice {
    uint id;                ///< unique id, start from 0
    std::string name;       ///< optional name
    VerticeType type;
    int weight;             ///< computation cost
    Edge **predecessors;    ///< parents lists (must before this vertice)
    Edge **successors;      ///< children lists (must after this vertice)
    uint prednum;           ///< number of predecessors
    uint succnum;           ///< number of successors
    int invflags;
    Vertice() : id(-1u), name(""), type{NORMAL}, weight(-1), \
        predecessors(NULL), successors(NULL), prednum(0u), succnum(0u), invflags(0) {};
} Vertice;

typedef struct Edge {
    Vertice *src;           ///< source vertice
    Vertice *dest;          ///< destination vertice
    int weight;             ///< communication cost
    Edge() : src(NULL), dest(NULL), weight(-1) {};
} Edge;

int destroySingleEdge(Edge *e);     ///< WIP
int cleanSingleVertice(Vertice *v); ///< WIP

class BooleanDag {
private:
    Vertice *V;             ///< vertice list
    std::vector<Edge> E;    ///< edge list
    uint size;              ///< length of the vertice list / number of the vertices
    int *prio;              ///< priority list

public:
    /* Constructor & Destructor */
    BooleanDag();
    BooleanDag(uint n);
    // ~BooleanDag();

    /* Construction related */
    int init();
    int init(uint n);
    int destroy();  ///< WIP
    int addVertice(int weight, const std::string &s="");
    int addVertice(uint id, int weight, const std::string &s="");
    int addVertice(uint id, int weight, VerticeType type, int inv, const std::string &s="");
    int addEdge(uint src, uint dest, int weight);
    int linkDAG();


    /* Setters */
    int setPriority(int *arr);

    /* Vistors */
    uint getsize();
    Vertice* getvertice(uint id);
    int getPriority(uint id);


    void traversePrint();
};




#endif