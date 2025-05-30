// GraphMatrix.h
#ifndef GRAPH_MATRIX_H
#define GRAPH_MATRIX_H

#include <iostream>
#include <fstream>

class GraphMatrix {
public:
    GraphMatrix(int vertices);
    ~GraphMatrix();

    void addEdge(int u, int v, int w);         // Nieskierowane
    void addDirectedEdge(int u, int v, int w);  // Skierowane
    void display() const;
    int getEdge(int u, int v) const;

private:
    int n;
    int **matrix;
};

#endif // GRAPH_MATRIX_H