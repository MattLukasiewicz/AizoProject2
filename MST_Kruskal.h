// MST_Kruskal.h
#ifndef MST_KRUSKAL_H
#define MST_KRUSKAL_H

#include "GraphMatrix.h"
#include "GraphList.h"
#include <vector>
#include <algorithm>

struct Edge {
    int u, v, w;
};

class MSTKruskal {
public:
    MSTKruskal(int vertices);
    ~MSTKruskal();

    int findSet(int v);
    void unionSets(int u, int v);
    void run(GraphMatrix& graphMatrix);
    void run(GraphList& graphList);

private:
    int n;             // Number of vertices
    int* parent;       // Union-Find parent array
    int* rank;         // Union-Find rank array
    std::vector<Edge> mstEdges; // Resulting MST edges
    int totalWeight;   // Total weight of MST

    void addEdge(int u, int v, int w);
    void initializeUnionFind();
    void processMST();
};

#endif // MST_KRUSKAL_H