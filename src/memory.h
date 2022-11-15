/**  
 * @file    memory.h
 * @brief   #define basic information of memory limitation
 * @author  Chenu Tang
 * @version 2.0
 * @date    2022-11-09
 * @note    
 */

#ifndef __MEMORY__
#define __MEMORY__
#include <climits>

// ./nvsim 65 0 512 256 2 2 1 1 2 2 1 1 0
#define MESHSIZE 64     ///< number of mesh
#define BLOCKROW 256    ///< number of rows in a block - NWL
#define BLOCKCOL 1024   ///< number of lines in a block - NBL

#define COMPUTELATENCY  1373ll // ps
#define COMPUTEENERGY   74.0249 // pJ


#define BANKREADLATENCY 1231ll       // ps
#define MATREADLATENCY  1197ll
#define SUBARRAYREADLATENCY 987ll

#define BANKWRITELATENCY    20674ll
#define MATWRITELATENCY     20658ll
#define SUBARRAYWRITELATENCY    20448ll

#define BANKREADENERGY 499.379
#define MATREADENERGY  46.3709
#define SUBARRAYREADENERGY 11.4598

#define BANKWRITEENERGY 1134.42
#define MATWRITEENERGY  205.132
#define SUBARRAYWRITEENERGY 51.1502

#define LEACKAGEENERGY  0.0900445   // W


#define LOADLATENCY     BANKWRITELATENCY
#define STORELATENCY    BANKREADLATENCY

#define LOADENERGY      BANKWRITEENERGY
#define STOREENERGY     BANKREADENERGY

#define OPLATENCY   (COMPUTELATENCY+SUBARRAYWRITELATENCY)
#define OPENERGY    (COMPUTEENERGY+SUBARRAYWRITEENERGY)
#define OPWEIGHT    (COMPUTELATENCY+SUBARRAYWRITELATENCY)

#define COMMWEIGHT      BANKREADLATENCY+BANKWRITELATENCY
#define MESHADDR(K,ROW) K*BLOCKROW+ROW

typedef unsigned int uint;
typedef long long bigint;


const int memlevel = 4;
const bigint commweight[memlevel+1] = {0, SUBARRAYREADLATENCY+SUBARRAYWRITELATENCY, MATREADLATENCY+MATWRITELATENCY, BANKREADLATENCY+BANKWRITELATENCY, COMMWEIGHT};
const double readenergy[memlevel+1] = {0.0, SUBARRAYREADENERGY, MATREADENERGY, BANKREADENERGY, LOADENERGY};
const double writeenergy[memlevel+1] = {0.0, SUBARRAYWRITEENERGY, MATWRITEENERGY, BANKWRITEENERGY, STOREENERGY};
const uint levelsize[memlevel] = {1, 4, 4, 4};
const uint maxcopythread[memlevel] = {MESHSIZE/levelsize[0], MESHSIZE/levelsize[0]/levelsize[1], MESHSIZE/levelsize[0]/levelsize[1]/levelsize[2], MESHSIZE/levelsize[0]/levelsize[1]/levelsize[2]/levelsize[3]};

#define INCREMENTALLATENCY(x,y) x+y
#define INCREMENTALENERGY(x,y)  x+y

inline int getCommLevel(uint totalpnum, uint p1, uint p2)
{
    // TODO: Only support situation for 2^n yet
    if (totalpnum > MESHSIZE || (totalpnum > 1 && totalpnum % 2)) {
        return -1;
    }
    if (p1 >= totalpnum || p2 >= totalpnum) {
        return memlevel;
    }

    int level = 0;
    int size = levelsize[level];
    while (level < memlevel) {
        if (p1 / size == p2 / size) {
            return level;
        }
        size *= levelsize[++level];
    }
    return -1;
}

#endif