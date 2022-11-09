#include <stdio.h>
#include "booleandag.h"
#include "priority.h"

using namespace Priority;

int main()
{
    BooleanDag G;

    int vweight[] = {2, 3, 3, 4, 5, 4, 4, 4, 1};
    uint src[] = {0, 0, 0, 0, 0, 1, 1, 2, 3, 5, 6, 7};
    uint dest[] = {1, 2, 3, 4, 6, 5, 6, 7, 7, 8, 8, 8};
    int eweight[] = {4, 1, 1, 1, 10, 1, 1, 1, 1, 5, 6, 5};
    int *value = NULL;
    G.init(9);

    for (int i = 0; i < 9; ++i) {
        G.addVertice(vweight[i]);
    }
    for (int i = 0; i < 12; ++i) {
        G.addEdge(src[i], dest[i], eweight[i]);
    }
    G.linkDAG();
    G.traversePrint();

    value = ranku(&G);

    printf("RankU:\n");
    for (int i = 0; i < 9; ++i) {
        printf("%d ", value[i]);
    }
    printf("\n");
    delete[] value;
    return 0;
}

