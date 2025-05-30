#include "MaxFlow_FF.h"
#include <algorithm>
#include <iostream>

MaxFlowFF::MaxFlowFF(int vertices) : n(vertices), parent(vertices) {}

bool MaxFlowFF::bfsResidual(const std::vector<std::vector<int>>& residual, int s, int t) {
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && residual[u][v] > 0) {
                parent[v] = u;
                visited[v] = true;
                if (v == t) return true;
                q.push(v);
            }
        }
    }
    return false;
}

int MaxFlowFF::fordFulkerson(GraphMatrix& gm, int s, int t) {
    // 1) zbuduj capacity[][] z gm.getEdge
    std::vector<std::vector<int>> residual(n, std::vector<int>(n));
    for (int u = 0; u < n; ++u)
        for (int v = 0; v < n; ++v)
            residual[u][v] = gm.getEdge(u, v);

    int maxFlow = 0;

    // 2) dopóki istnieje ścieżka powiększająca
    while (bfsResidual(residual, s, t)) {
        // 3) znajdź minimalny przepływ na ścieżce
        int pathFlow = std::numeric_limits<int>::max();
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, residual[u][v]);
        }
        // 4) zaktualizuj residual
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            residual[u][v] -= pathFlow;
            residual[v][u] += pathFlow;
        }
        maxFlow += pathFlow;
    }

    // 5) wypisz wynik
    std::cout << "Maksymalny przeplyw (Ford-Fulkerson): " << maxFlow << "\n";
    return maxFlow;
}
