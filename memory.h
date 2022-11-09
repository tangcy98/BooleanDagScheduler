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



#define MESHSIZE 64     ///< number of mesh
#define BLOCKROW 256    ///< number of rows in a block - NWL
#define BLOCKCOL 256    ///< number of lines in a block - NBL

#define MESHADDR(K,ROW) K*BLOCKROW+ROW


typedef unsigned int uint;


const int memlevel = 4;
const double commfactor[memlevel+1] = {0, 0.01, 0.1, 1, 5};
const uint levelsize[memlevel] = {1, 4, 4, 4};


#endif