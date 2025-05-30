#include "GraphMatrix.h"
#include <iomanip>

GraphMatrix::GraphMatrix(int vertices) : n(vertices) {
    matrix = new int*[n];
    for (int i = 0; i < n; ++i) {
        matrix[i] = new int[n];
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = 0; // 0 indicates no edge
        }
    }
}

GraphMatrix::~GraphMatrix() {
    for (int i = 0; i < n; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void GraphMatrix::addEdge(int u, int v, int w) {
    if (u >= 0 && u < n && v >= 0 && v < n) {
        matrix[u][v] = w;
        matrix[v][u] = w; // Undirected graph
    }
}

void GraphMatrix::display() const {
    std::cout << "Adjacency Matrix (" << n << " x " << n << "):\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << std::setw(3) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
int GraphMatrix::getEdge(int u, int v) const {
    if (u >= 0 && u < n && v >= 0 && v < n) {
        return matrix[u][v];
    }
    return 0;
}
