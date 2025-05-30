// MST_Prim.cpp
#include "MST_Prim.h"
#include <iostream>
#include <limits>

MSTPrim::MSTPrim(int vertices) : n(vertices) {
    used = new bool[n];
    minEdge = new int[n];
    selEdge = new int[n];
}

MSTPrim::~MSTPrim() {
    delete[] used;
    delete[] minEdge;
    delete[] selEdge;
}

void MSTPrim::init() {
    for (int i = 0; i < n; ++i) {
        used[i] = false;
        minEdge[i] = std::numeric_limits<int>::max();
        selEdge[i] = -1;
    }
    totalWeight = 0;
}

void MSTPrim::runMatrix(GraphMatrix& gm) {
    init();
    minEdge[0] = 0;
    for (int i = 0; i < n; ++i) {
        int v = -1;
        for (int j = 0; j < n; ++j)
            if (!used[j] && (v == -1 || minEdge[j] < minEdge[v]))
                v = j;
        used[v] = true;
        if (selEdge[v] != -1) {
            std::cout << selEdge[v] << " - " << v << " (" << minEdge[v] << ")\n";
            totalWeight += minEdge[v];
        }
        for (int to = 0; to < n; ++to) {
            int w = gm.getEdge(v, to);
            if (!used[to] && w > 0 && w < minEdge[to]) {
                minEdge[to] = w;
                selEdge[to] = v;
            }
        }
    }
    std::cout << "Suma wag MST: " << totalWeight << "\n";
}

void MSTPrim::runList(GraphList& gl) {
    // Convert list to adjacency matrix-like behavior: getEdge via scanning
    // For simplicity use matrix run: build temporary matrix
    GraphMatrix gm(n);
    for (int i = 0; i < n; ++i) {
        Node* curr = gl.getAdjacencyList(i);
        while (curr) {
            gm.addEdge(i, curr->v, curr->w);
            curr = curr->next;
        }
    }
    runMatrix(gm);
}