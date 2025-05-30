#ifndef MAXFLOW_FF_H
#define MAXFLOW_FF_H

#include "GraphMatrix.h"
#include <vector>
#include <queue>
#include <limits>

class MaxFlowFF {
public:
    MaxFlowFF(int vertices);
    // zwraca maksymalny przepływ z s do t na grafie gm (skierowany, capacity w macierzy)
    int fordFulkerson(GraphMatrix& gm, int s, int t);

private:
    int n;
    std::vector<int> parent;

    // BFS po grafie residualnym, wypełnia parent[] i zwraca true jeśli dotarliśmy do t
    bool bfsResidual(const std::vector<std::vector<int>>& residual, int s, int t);
};

#endif // MAXFLOW_FF_H
