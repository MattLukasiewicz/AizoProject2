#ifndef SP_DIJSTRA_H
#define SP_DIJSTRA_H

#include "GraphMatrix.h"
#include "GraphList.h"
#include <limits>
#include <algorithm> // dla std::swap

// Prosta min-kopiec na tablicy
struct MinHeap {
    int *heap, *pos, *distArr;
    int size;
    MinHeap(int n): heap(new int[n]), pos(new int[n]), distArr(nullptr), size(0){}
    ~MinHeap(){ delete[] heap; delete[] pos; }
    void setDist(int* d){ distArr=d; }
    void swapN(int i,int j){
        std::swap(pos[heap[i]],pos[heap[j]]);
        std::swap(heap[i],heap[j]);
    }
    void up(int i){
        while(i>0){
            int p=(i-1)/2;
            if(distArr[heap[i]]<distArr[heap[p]]){ swapN(i,p); i=p; }
            else break;
        }
    }
    void down(int i){
        int smallest=i;
        int l=2*i+1, r=2*i+2;
        if(l<size && distArr[heap[l]]<distArr[heap[smallest]]) smallest=l;
        if(r<size && distArr[heap[r]]<distArr[heap[smallest]]) smallest=r;
        if(smallest!=i){ swapN(i,smallest); down(smallest); }
    }
    void insert(int v){
        heap[size]=v; pos[v]=size; up(size); ++size;
    }
    bool empty()const{ return size==0; }
    int extractMin(){
        int root=heap[0];
        heap[0]=heap[--size]; pos[heap[0]]=0;
        down(0);
        return root;
    }
    void decreaseKey(int v){ up(pos[v]); }
};

class SPDijkstra {
public:
    SPDijkstra(int vertices);
    ~SPDijkstra();
    void runMatrix(GraphMatrix& gm, int start);
    void runList(GraphList& gl, int start);
private:
    int n;
    void printPath(int v, int* prev);
};

#endif
