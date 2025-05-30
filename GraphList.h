// GraphList.h
#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H

#include <iostream>
#include <fstream>

struct Node {
    int v, w;
    Node* next;
};

class GraphList {
public:
    GraphList(int vertices);
    ~GraphList();

    void addEdge(int u, int v, int w);        // Nieskierowane
    void addDirectedEdge(int u, int v, int w); // Skierowane
    void display() const;
    Node* getAdjacencyList(int u) const;

private:
    int n;
    Node** adj;
};

#endif // GRAPH_LIST_H