// SP_Dijkstra.h
#ifndef SP_DIJKSTRA_H
#define SP_DIJKSTRA_H

#include "GraphMatrix.h"
#include "GraphList.h"
#include <limits>

class SPDijkstra {
public:
    SPDijkstra(int vertices);
    ~SPDijkstra();

    void runMatrix(GraphMatrix& gm, int start);
    void runList(GraphList& gl, int start);

private:
    int n;
    void printPath(int v, int* prev);
};

#endif // SP_DIJKSTRA_H