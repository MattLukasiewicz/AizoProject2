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

void GraphList::addEdge(int u, int v, int w) {
    if (u >= 0 && u < n && v >= 0 && v < n) {
        Node* nodeU = new Node{v, w, adj[u]};
        adj[u] = nodeU;
        Node* nodeV = new Node{u, w, adj[v]};
        adj[v] = nodeV; // Undirected graph
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
