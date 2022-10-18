/**  
 * @file    booleandag.h
 * @brief   Basic implementation of boolean dag
 * @author  Chenu Tang
 * @version 0.1
 * @date    2022-10-17
 * @note    
 */
#ifndef _BOOLEANDAG_
#define _BOOLEANDAG_
#include <cstdio>
#include <vector>
typedef unsigned int uint;

enum VerticeType {
    NORMAL, MIG, AOIG, XMG,
};

/**
 * @brief Vertice class
 * Reperesent a single vertice with its unique id 
 * and other basic information
 */

struct Edge;

struct Vertice {
    uint id;                ///< unique id, start from 0
    VerticeType type;
    int weight;             ///< computation cost
    Edge **predecessors;    ///< parents lists (must before this vertice)
    Edge **successors;      ///< children lists (must after this vertice)
    uint prednum;           ///< number of predecessors
    uint succnum;           ///< number of successors
    Vertice() : id(-1u), type{NORMAL}, weight(-1), \
        predecessors(NULL), successors(NULL), prednum(0u), succnum(0u) {};
};

struct Edge {
    Vertice *src;           ///< source vertice
    Vertice *dest;          ///< destination vertice
    int weight;             ///< communication cost
    Edge() : src(NULL), dest(NULL), weight(-1) {};
};

int destroySingleEdge(Edge *e);     ///< WIP
int cleanSingleVertice(Vertice *v); ///< WIP

class BooleanDag {
private:
    Vertice *V;             ///< vertice list
    std::vector<Edge> E;    ///< edge list
    uint size;              ///< length of the vertice list / number of the vertices

public:
    /* Constructor & Destructor */
    BooleanDag();
    BooleanDag(uint n);
    // ~BooleanDag();

    /* Construction related */
    int init();
    int init(uint n);
    int destroy();  ///< WIP
    int addVertice(int weight);
    int addVertice(uint id, int weight);
    int addEdge(uint src, uint dest, int weight);
    int linkDAG();

    /* Vistors */
    uint getsize();
    Vertice* getvertice(uint id);


    void traversePrint();


};

#endif