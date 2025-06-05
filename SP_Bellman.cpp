// SP_Bellman.cpp
#include "SP_Bellman.h"
#include <iostream>

SPBellman::SPBellman(int vertices) : n(vertices) {}
SPBellman::~SPBellman() {}

void SPBellman::printPath(int v, int* prev) {
    if (prev[v] == -1) {
        std::cout << v;
        return;
    }
    printPath(prev[v], prev);
    std::cout << " -> " << v;
}

void SPBellman::runMatrix(GraphMatrix& gm, int start) {
    const int INF = std::numeric_limits<int>::max();
    int* dist = new int[n];
    int* prev = new int[n];
    for (int i = 0; i < n; ++i) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0;
    for (int k = 0; k < n - 1; ++k) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                int w = gm.getEdge(u, v);
                if (w > 0 && dist[u] != INF && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                }
            }
        }
    }

    std::cout << "Bellman-Ford (macierz) od wezla " << start << ":\n";
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
}

void SPBellman::runList(GraphList& gl, int start) {
    const int INF = std::numeric_limits<int>::max();
    int* dist = new int[n];
    int* prev = new int[n];
    for (int i = 0; i < n; ++i) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0;
    for (int k = 0; k < n - 1; ++k) {
        for (int u = 0; u < n; ++u) {
            Node* curr = gl.getAdjacencyList(u);
            while (curr) {
                int v = curr->v;
                int w = curr->w;
                if (dist[u] != INF && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                }
                curr = curr->next;
            }
        }
    }

    std::cout << "Bellman-Ford (lista) od wezla " << start << ":\n";
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
}