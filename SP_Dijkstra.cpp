#include "SP_Dijkstra.h"
#include <iostream>

SPDijkstra::SPDijkstra(int vertices) : n(vertices) {}
SPDijkstra::~SPDijkstra() {}

void SPDijkstra::printPath(int v, int* prev) {
    if (prev[v] == -1) { std::cout << v; return; }
    printPath(prev[v], prev);
    std::cout << " -> " << v;
}

void SPDijkstra::runMatrix(GraphMatrix& gm, int start) {
    const int INF = std::numeric_limits<int>::max();
    int* dist = new int[n];
    int* prev = new int[n];
    bool* used = new bool[n];
    for (int i = 0; i < n; ++i) {
        dist[i]=INF; prev[i]=-1; used[i]=false;
    }
    dist[start]=0;

    MinHeap mh(n);
    mh.setDist(dist);
    for(int i=0;i<n;++i) mh.insert(i);

    while(!mh.empty()) {
        int u = mh.extractMin();
        if(dist[u]==INF) break;
        used[u]=true;
        for(int v=0; v<n; ++v) {
            int w = gm.getEdge(u,v);
            if(w>0 && !used[v] && dist[u]+w<dist[v]) {
                dist[v]=dist[u]+w;
                prev[v]=u;
                mh.decreaseKey(v);
            }
        }
    }

    std::cout<<"Dijkstra (macierz) od wezla "<<start<<":\n";
    for(int i=0;i<n;++i){
        if(dist[i]==INF) std::cout<<i<<": nieosiagalny\n";
        else {
            std::cout<<i<<": koszt="<<dist[i]<<", sciezka=";
            printPath(i,prev);
            std::cout<<"\n";
        }
    }

    delete[] dist; delete[] prev; delete[] used;
}

void SPDijkstra::runList(GraphList& gl, int start) {
    const int INF = std::numeric_limits<int>::max();
    int* dist = new int[n];
    int* prev = new int[n];
    bool* used = new bool[n];
    for (int i = 0; i < n; ++i) {
        dist[i]=INF; prev[i]=-1; used[i]=false;
    }
    dist[start]=0;

    MinHeap mh(n);
    mh.setDist(dist);
    for(int i=0;i<n;++i) mh.insert(i);

    while(!mh.empty()) {
        int u = mh.extractMin();
        if(dist[u]==INF) break;
        used[u]=true;
        Node* cur = gl.getAdjacencyList(u);
        while(cur) {
            int v=cur->v, w=cur->w;
            if(!used[v] && dist[u]+w<dist[v]) {
                dist[v]=dist[u]+w;
                prev[v]=u;
                mh.decreaseKey(v);
            }
            cur=cur->next;
        }
    }

    std::cout<<"Dijkstra (lista) od wezla "<<start<<":\n";
    for(int i=0;i<n;++i){
        if(dist[i]==INF) std::cout<<i<<": nieosiagalny\n";
        else {
            std::cout<<i<<": koszt="<<dist[i]<<", sciezka=";
            printPath(i,prev);
            std::cout<<"\n";
        }
    }

    delete[] dist; delete[] prev; delete[] used;
}
