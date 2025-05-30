// MST_Prim.h
#ifndef MST_PRIM_H
#define MST_PRIM_H

#include "GraphMatrix.h"
#include "GraphList.h"

class MSTPrim {
public:
    MSTPrim(int vertices);
    ~MSTPrim();

    void runMatrix(GraphMatrix& gm);
    void runList(GraphList& gl);

private:
    int n;
    bool* used;
    int* minEdge;
    int* selEdge;
    int* weightResult;
    int totalWeight;

    void init();
};

#endif // MST_PRIM_H