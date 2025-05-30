// SP_Dijkstra.cpp
#include "SP_Dijkstra.h"
#include <iostream>

SPDijkstra::SPDijkstra(int vertices) : n(vertices) {}
SPDijkstra::~SPDijkstra() {}

void SPDijkstra::printPath(int v, int* prev) {
    if (prev[v] == -1) {
        std::cout << v;
        return;
    }
    printPath(prev[v], prev);
    std::cout << " -> " << v;
}

void SPDijkstra::runMatrix(GraphMatrix& gm, int start) {
    const int INF = std::numeric_limits<int>::max();
    int* dist = new int[n];
    int* prev = new int[n];
    bool* used = new bool[n];
    for (int i = 0; i < n; ++i) {
        dist[i] = INF;
        prev[i] = -1;
        used[i] = false;
    }
    dist[start] = 0;
    for (int i = 0; i < n; ++i) {
        int u = -1;
        for (int j = 0; j < n; ++j) {
            if (!used[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }
        if (u == -1 || dist[u] == INF) break;
        used[u] = true;
        for (int v = 0; v < n; ++v) {
            int w = gm.getEdge(u, v);
            if (w > 0 && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
            }
        }
    }
    std::cout << "Dijkstra (macierz) od wezla " << start << ":\n";
    for (int i = 0; i < n; ++i) {
        if (dist[i] == INF) {
            std::cout << i << ": nieosiagalny\n";
        } else {
            std::cout << i << ": koszt = " << dist[i] << ", sciezka = ";
            printPath(i, prev);
            std::cout << "\n";
        }
    }
    delete[] dist;
    delete[] prev;
    delete[] used;
}

void SPDijkstra::runList(GraphList& gl, int start) {
    const int INF = std::numeric_limits<int>::max();
    int* dist = new int[n];
    int* prev = new int[n];
    bool* used = new bool[n];
    for (int i = 0; i < n; ++i) {
        dist[i] = INF;
        prev[i] = -1;
        used[i] = false;
    }
    dist[start] = 0;
    for (int i = 0; i < n; ++i) {
        int u = -1;
        for (int j = 0; j < n; ++j) {
            if (!used[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }
        if (u == -1 || dist[u] == INF) break;
        used[u] = true;
        Node* curr = gl.getAdjacencyList(u);
        while (curr) {
            int v = curr->v;
            int w = curr->w;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
            }
            curr = curr->next;
        }
    }
    std::cout << "Dijkstra (lista) od wezla " << start << ":\n";
    for (int i = 0; i < n; ++i) {
        if (dist[i] == INF) {
            std::cout << i << ": nieosiagalny\n";
        } else {
            std::cout << i << ": koszt = " << dist[i] << ", sciezka = ";
            printPath(i, prev);
            std::cout << "\n";
        }
    }
    delete[] dist;
    delete[] prev;
    delete[] used;
}