#include "GraphList.h"

GraphList::GraphList(int vertices) : n(vertices) {
    adj = new Node*[n];
    for (int i = 0; i < n; ++i) {
        adj[i] = nullptr;
    }
}

GraphList::~GraphList() {
    for (int i = 0; i < n; ++i) {
        Node* curr = adj[i];
        while (curr) {
            Node* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
    }
    delete[] adj;
}

// ZAMIENIAMY całą implementację addEdge(...)
void GraphList::addEdge(int u, int v, int w) {
    if (u >= 0 && u < n && v >= 0 && v < n) {
        // dodaj krawędź u->v
        Node* nodeU = new Node;
        nodeU->v = v;
        nodeU->w = w;
        nodeU->next = adj[u];
        adj[u] = nodeU;
        // dodaj krawędź v->u (graf nieskierowany)
        Node* nodeV = new Node;
        nodeV->v = u;
        nodeV->w = w;
        nodeV->next = adj[v];
        adj[v] = nodeV;
    }
}

// ZAMIENIAMY całą implementację addDirectedEdge(...)
void GraphList::addDirectedEdge(int u, int v, int w) {
    if (u >= 0 && u < n && v >= 0 && v < n) {
        Node* newNode = new Node;
        newNode->v = v;
        newNode->w = w;
        newNode->next = adj[u];
        adj[u] = newNode;
    }
}

void GraphList::display() const {
    std::cout << "Adjacency List:\n";
    for (int i = 0; i < n; ++i) {
        std::cout << i << ": ";
        Node* curr = adj[i];
        while (curr) {
            std::cout << "(" << curr->v << "," << curr->w << ") ";
            curr = curr->next;
        }
        std::cout << std::endl;
    }
}
Node* GraphList::getAdjacencyList(int u) const {
    if (u >= 0 && u < n) {
        return adj[u];
    }
    return nullptr;
}
