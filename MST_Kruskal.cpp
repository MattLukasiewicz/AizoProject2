#include "MST_Kruskal.h"
#include <iostream>
#include <algorithm>

MSTKruskal::MSTKruskal(int vertices) : n(vertices), totalWeight(0) {
    parent = new int[n];
    rank = new int[n];
}

MSTKruskal::~MSTKruskal() {
    delete[] parent;
    delete[] rank;
}

int MSTKruskal::findSet(int v) {
    if (parent[v] != v) {
        parent[v] = findSet(parent[v]); // Path compression
    }
    return parent[v];
}

void MSTKruskal::unionSets(int u, int v) {
    int ru = findSet(u);
    int rv = findSet(v);

    if (ru != rv) {
        if (rank[ru] > rank[rv]) parent[rv] = ru;
        else if (rank[ru] < rank[rv]) parent[ru] = rv;
        else { parent[rv] = ru; rank[ru]++; }
    }
}

void MSTKruskal::addEdge(int u, int v, int w) {
    mstEdges.push_back({u, v, w});
}

void MSTKruskal::initializeUnionFind() {
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
        rank[i] = 0;
    }
}

void MSTKruskal::processMST() {
    std::sort(mstEdges.begin(), mstEdges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });

    totalWeight = 0;
    std::vector<Edge> mstResult;

    for (const Edge& edge : mstEdges) {
        if (findSet(edge.u) != findSet(edge.v)) {
            unionSets(edge.u, edge.v);
            mstResult.push_back(edge);
            totalWeight += edge.w;
        }
    }

    std::cout << "Krawedzie MST (Kruskal):\n";
    for (const Edge& edge : mstResult) {
        std::cout << edge.u << " - " << edge.v << " (" << edge.w << ")\n";
    }
    std::cout << "Suma wag MST: " << totalWeight << "\n";
}

void MSTKruskal::run(GraphMatrix& gm) {
    mstEdges.clear();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int w = gm.getEdge(i, j);  // use public accessor
            if (w != 0) addEdge(i, j, w);
        }
    }
    initializeUnionFind();
    processMST();
}

void MSTKruskal::run(GraphList& gl) {
    mstEdges.clear();
    for (int i = 0; i < n; ++i) {
        Node* curr = gl.getAdjacencyList(i);  // use public method
        while (curr) {
            if (i < curr->v) { // Avoid duplicate edges
                addEdge(i, curr->v, curr->w);
            }
            curr = curr->next;
        }
    }
    initializeUnionFind();
    processMST();
}
