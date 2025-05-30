// SP_Bellman.h
#ifndef SP_BELLMAN_H
#define SP_BELLMAN_H

#include "GraphMatrix.h"
#include "GraphList.h"
#include <limits>

class SPBellman {
public:
    SPBellman(int vertices);
    ~SPBellman();

    void runMatrix(GraphMatrix& gm, int start);
    void runList(GraphList& gl, int start);

private:
    int n;
    void printPath(int v, int* prev);
};

#endif // SP_BELLMAN_H