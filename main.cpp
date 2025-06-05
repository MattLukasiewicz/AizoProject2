#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <string>

#include "GraphMatrix.h"
#include "GraphList.h"
#include "MST_Kruskal.h"
#include "MST_Prim.h"
#include "SP_Dijkstra.h"
#include "SP_Bellman.h"
#include "MaxFlow_FF.h"

enum ExperimentType {
    EXP_MST  = 1,
    EXP_SP   = 2,
    EXP_FLOW = 3
};

// ****************************
// Funkcja: generowanie w pamięci (bez pliku)
void generateGraphInMemory(GraphMatrix& gm, GraphList& gl,
                           int V, int E, bool directed) {
    bool* visited = new bool[V]();
    bool** exists = new bool*[V];
    for (int i = 0; i < V; ++i) {
        exists[i] = new bool[V]();
    }
    srand((unsigned)time(nullptr));

    // 1) drzewo (V − 1 krawędzi)
    visited[0] = true;
    int treeE = 0;
    while (treeE < V - 1) {
        int u = rand() % V;
        int v = rand() % V;
        if (u != v && (visited[u] != visited[v])) {
            int w = rand() % 100 + 1;
            gm.addEdge(u, v, w);
            gl.addEdge(u, v, w);
            exists[u][v] = exists[v][u] = true;
            visited[u] = visited[v] = true;
            ++treeE;
        }
    }

    // 2) w grafie skierowanym dorzuć cykl, by każdy miał wejście
    if (directed) {
        for (int i = 0; i < V; ++i) {
            int j = (i + 1) % V;
            if (!exists[i][j]) {
                int w = rand() % 100 + 1;
                gm.addDirectedEdge(i, j, w);
                gl.addDirectedEdge(i, j, w);
                exists[i][j] = true;
            }
        }
    }

    // 3) dorzuć dodatkowe krawędzie, by mieć E
    int baseCount = directed ? (V) : (V - 1);
    int extra = E - baseCount;
    while (extra > 0) {
        int u = rand() % V;
        int v = rand() % V;
        if (u != v && !exists[u][v]) {
            int w = rand() % 100 + 1;
            if (directed) {
                gm.addDirectedEdge(u, v, w);
                gl.addDirectedEdge(u, v, w);
                exists[u][v] = true;
            } else {
                gm.addEdge(u, v, w);
                gl.addEdge(u, v, w);
                exists[u][v] = exists[v][u] = true;
            }
            --extra;
        }
    }

    delete[] visited;
    for (int i = 0; i < V; ++i) delete[] exists[i];
    delete[] exists;
}

// ****************************
// Funkcja: zapisanie losowego grafu do pliku (menu “1.”)
void generateAndSaveRandomGraph(int vertices, int edges,
                                const std::string& filename,
                                bool directed) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Blad zapisu pliku: " << filename << "\n";
        return;
    }
    outfile << vertices << " " << edges << "\n";
    bool* visited = new bool[vertices]();
    bool** exists = new bool*[vertices];
    for (int i = 0; i < vertices; ++i) {
        exists[i] = new bool[vertices]();
    }
    srand((unsigned)time(nullptr));

    int treeEdges = 0;
    visited[0] = true;
    while (treeEdges < vertices - 1) {
        int u = rand() % vertices;
        int v = rand() % vertices;
        if (u != v && (visited[u] != visited[v])) {
            int w = rand() % 100 + 1;
            outfile << u << " " << v << " " << w << "\n";
            exists[u][v] = exists[v][u] = true;
            visited[u] = visited[v] = true;
            ++treeEdges;
        }
    }
    if (directed) {
        for (int i = 0; i < vertices; ++i) {
            int next = (i + 1) % vertices;
            if (!exists[i][next]) {
                int w = rand() % 100 + 1;
                outfile << i << " " << next << " " << w << "\n";
                exists[i][next] = true;
            }
        }
    }
    int extraEdges = edges - (directed ? (treeEdges + vertices) : treeEdges);
    while (extraEdges > 0) {
        int u = rand() % vertices;
        int v = rand() % vertices;
        if (u != v && !exists[u][v]) {
            int w = rand() % 100 + 1;
            outfile << u << " " << v << " " << w << "\n";
            if (directed) exists[u][v] = true;
            else exists[u][v] = exists[v][u] = true;
            --extraEdges;
        }
    }
    for (int i = 0; i < vertices; ++i) delete[] exists[i];
    delete[] exists;
    delete[] visited;
    outfile.close();
    std::cout << "Graf zapisano do pliku: " << filename << "\n";
}

// ****************************
// Funkcja: usuń 1% krawędzi z pliku (menu “1.” dla density=99%)
void removeOnePercent(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Blad otwarcia pliku: " << filename << "\n";
        return;
    }
    int vertices, edges;
    infile >> vertices >> edges;
    int* u = new int[edges];
    int* v = new int[edges];
    int* w = new int[edges];
    for (int i = 0; i < edges; ++i) {
        infile >> u[i] >> v[i] >> w[i];
    }
    infile.close();

    int toRemove = edges / 100;
    srand((unsigned)time(nullptr));
    for (int i = 0; i < toRemove; ++i) {
        int idx = rand() % edges;
        u[idx] = u[edges - 1];
        v[idx] = v[edges - 1];
        w[idx] = w[edges - 1];
        --edges;
    }

    std::ofstream outfile(filename);
    outfile << vertices << " " << edges << "\n";
    for (int i = 0; i < edges; ++i) {
        outfile << u[i] << " " << v[i] << " " << w[i] << "\n";
    }
    delete[] u;
    delete[] v;
    delete[] w;
    outfile.close();
    std::cout << "Usunieto 1% krawedzi z grafu: " << filename << "\n";
}

// ****************************
// Funkcja: eksperymenty (MST / SP / MaxFlow)
void runExperiments(int expType, bool verbose) {
    const int repetitions = 10;
    //const int numVertices[] = {10, 50, 100, 200, 300, 400};
    const int numVertices[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };

    const double densities[] = {0.2, 0.6, 0.99};

    std::ofstream results("experiment_summary.csv");
    if (!results) {
        std::cerr << "Nie mozna otworzyc experiment_summary.csv do zapisu.\n";
        return;
    }
    results << "Vertices,Density,Representation,Algorithm,AverageTime_ms\n";

    for (int V : numVertices) {
        int maxEdges = V * (V - 1) / 2;
        for (double density : densities) {
            int E = static_cast<int>(std::ceil(maxEdges * density));

            bool directed = (expType == EXP_SP || expType == EXP_FLOW);
            // zadbajmy, by E było co najmniej (V-1)+(directed?V:0):
            int minEdges = directed ? (V - 1 + V) : (V - 1);
            if (E < minEdges) E = minEdges;

            double tPmat = 0, tPlist = 0, tKmat = 0, tKlist = 0;
            double tDmat = 0, tDlist = 0, tBmat = 0, tBlist = 0;
            double tFmat = 0, tFlist = 0;

            for (int run = 0; run < repetitions; ++run) {
                GraphMatrix* gm = new GraphMatrix(V);
                GraphList*   gl = new GraphList(V);
                generateGraphInMemory(*gm, *gl, V, E, directed);
                /*
                int countMat = 0;
                for (int i = 0; i < V; ++i)
                    for (int j = 0; j < V; ++j)
                        if (gm->getEdge(i, j) > 0) countMat++;
                std::cout << "W macierzy zapisana jest " << (directed ? countMat : countMat / 2)
                          << " krawedzi. Oczekiwano: " << E << ".\n";
                */

                if (expType == EXP_MST) {
                    if (verbose) {
                        std::cout << "[MST] Instancja " << run+1
                                  << "/" << repetitions
                                  << " (V=" << V << ", d=" << (density*100) << "%)\n";
                    }
                    // Prim (macierz)
                    {
                        MSTPrim pM(V);
                        auto s0 = std::chrono::high_resolution_clock::now();
                        pM.runMatrix(*gm);
                        auto e0 = std::chrono::high_resolution_clock::now();
                        double dt = std::chrono::duration<double,std::milli>(e0 - s0).count();
                        tPmat += dt;
                        if (verbose) std::cout << "   Prim(Matrix) = " << dt << " ms\n";
                    }
                    // Prim (lista)
                    {
                        MSTPrim pL(V);
                        auto s0 = std::chrono::high_resolution_clock::now();
                        pL.runList(*gl);
                        auto e0 = std::chrono::high_resolution_clock::now();
                        double dt = std::chrono::duration<double,std::milli>(e0 - s0).count();
                        tPlist += dt;
                        if (verbose) std::cout << "   Prim(List)   = " << dt << " ms\n";
                    }
                    // Kruskal (macierz)
                    {
                        MSTKruskal kM(V);
                        auto s0 = std::chrono::high_resolution_clock::now();
                        kM.run(*gm);
                        auto e0 = std::chrono::high_resolution_clock::now();
                        double dt = std::chrono::duration<double,std::milli>(e0 - s0).count();
                        tKmat += dt;
                        if (verbose) std::cout << "   Kruskal(Matrix) = " << dt << " ms\n";
                    }
                    // Kruskal (lista)
                    {
                        MSTKruskal kL(V);
                        auto s0 = std::chrono::high_resolution_clock::now();
                        kL.run(*gl);
                        auto e0 = std::chrono::high_resolution_clock::now();
                        double dt = std::chrono::duration<double,std::milli>(e0 - s0).count();
                        tKlist += dt;
                        if (verbose) std::cout << "   Kruskal(List)   = " << dt << " ms\n";
                    }
                }

                if (expType == EXP_SP) {
                    if (verbose) {
                        std::cout << "[SP] Instancja " << run+1
                                  << "/" << repetitions
                                  << " (V=" << V << ", d=" << (density*100) << "%)\n";
                    }
                    int startVertex = 0;
                    {
                        SPDijkstra dM(V);
                        auto s0 = std::chrono::high_resolution_clock::now();
                        dM.runMatrix(*gm, startVertex);
                        auto e0 = std::chrono::high_resolution_clock::now();
                        double dt = std::chrono::duration<double,std::milli>(e0 - s0).count();
                        tDmat += dt;
                        if (verbose) std::cout << "   Dijkstra(Matrix) = " << dt << " ms\n";
                    }
                    {
                        SPDijkstra dL(V);
                        auto s0 = std::chrono::high_resolution_clock::now();
                        dL.runList(*gl, startVertex);
                        auto e0 = std::chrono::high_resolution_clock::now();
                        double dt = std::chrono::duration<double,std::milli>(e0 - s0).count();
                        tDlist += dt;
                        if (verbose) std::cout << "   Dijkstra(List)   = " << dt << " ms\n";
                    }
                    {
                        SPBellman bM(V);
                        auto s0 = std::chrono::high_resolution_clock::now();
                        bM.runMatrix(*gm, startVertex);
                        auto e0 = std::chrono::high_resolution_clock::now();
                        double dt = std::chrono::duration<double,std::milli>(e0 - s0).count();
                        tBmat += dt;
                        if (verbose) std::cout << "   Bellman(Matrix)  = " << dt << " ms\n";
                    }
                    {
                        SPBellman bL(V);
                        auto s0 = std::chrono::high_resolution_clock::now();
                        bL.runList(*gl, startVertex);
                        auto e0 = std::chrono::high_resolution_clock::now();
                        double dt = std::chrono::duration<double,std::milli>(e0 - s0).count();
                        tBlist += dt;
                        if (verbose) std::cout << "   Bellman(List)    = " << dt << " ms\n";
                    }
                }

                if (expType == EXP_FLOW) {
                    if (verbose) {
                        std::cout << "[MaxFlow] Instancja " << run+1
                                  << "/" << repetitions
                                  << " (V=" << V << ", d=" << (density*100) << "%)\n";
                    }
                    int s = 0, t = V - 1;
                    {
                        MaxFlowFF fM(V);
                        auto s0 = std::chrono::high_resolution_clock::now();
                        fM.fordFulkerson(*gm, s, t);
                        auto e0 = std::chrono::high_resolution_clock::now();
                        double dt = std::chrono::duration<double,std::milli>(e0 - s0).count();
                        tFmat += dt;
                        if (verbose) std::cout << "   FF(Matrix) = " << dt << " ms\n";
                    }
                    {
                        MaxFlowFF fL(V);
                        auto s0 = std::chrono::high_resolution_clock::now();
                        fL.fordFulkerson(*gm, s, t);
                        auto e0 = std::chrono::high_resolution_clock::now();
                        double dt = std::chrono::duration<double,std::milli>(e0 - s0).count();
                        tFlist += dt;
                        if (verbose) std::cout << "   FF(List)   = " << dt << " ms\n";
                    }
                }

                delete gm;
                delete gl;
            } // koniec powtórzeń

            if (expType == EXP_MST) {
                double aPmat = tPmat / repetitions;
                double aPlist = tPlist / repetitions;
                double aKmat = tKmat / repetitions;
                double aKlist = tKlist / repetitions;
                results << V << "," << density << ",Matrix,Prim,"   << aPmat  << "\n";
                results << V << "," << density << ",List,Prim,"     << aPlist << "\n";
                results << V << "," << density << ",Matrix,Kruskal,"<< aKmat  << "\n";
                results << V << "," << density << ",List,Kruskal,"  << aKlist << "\n";
                std::cout << "\n[MST] V=" << V << ", d=" << (density*100) << "%  => "
                          << "PrimM="   << aPmat  << " ms, "
                          << "PrimL="   << aPlist << " ms, "
                          << "KrM="     << aKmat  << " ms, "
                          << "KrL="     << aKlist << " ms\n\n";
            }
            if (expType == EXP_SP) {
                double aDmat = tDmat / repetitions;
                double aDlist = tDlist / repetitions;
                double aBmat = tBmat / repetitions;
                double aBlist = tBlist / repetitions;
                results << V << "," << density << ",Matrix,Dijkstra,"   << aDmat  << "\n";
                results << V << "," << density << ",List,Dijkstra,"     << aDlist << "\n";
                results << V << "," << density << ",Matrix,BellmanFord,"<< aBmat  << "\n";
                results << V << "," << density << ",List,BellmanFord,"  << aBlist << "\n";
                std::cout << "\n[SP] V=" << V << ", d=" << (density*100) << "%  => "
                          << "DijM=" << aDmat  << " ms, "
                          << "DijL=" << aDlist << " ms, "
                          << "BF_M=" << aBmat  << " ms, "
                          << "BF_L=" << aBlist << " ms\n\n";
            }
            if (expType == EXP_FLOW) {
                double aFmat = tFmat / repetitions;
                double aFlist = tFlist / repetitions;
                results << V << "," << density << ",Matrix,FordFulkerson," << aFmat  << "\n";
                results << V << "," << density << ",List,FordFulkerson,"   << aFlist << "\n";
                std::cout << "\n[MaxFlow] V=" << V << ", d=" << (density*100) << "%  => "
                          << "FF_M=" << aFmat << " ms, "
                          << "FF_L=" << aFlist << " ms\n\n";
            }
        }
    }

    results.close();
    std::cout << "Eksperyment zakonczony. Wyniki zapisano w experiment_summary.csv\n";
}

// ****************************
// Funkcja: wczytywanie z pliku (menu “2.”)
void readGraphFromFile(const std::string& filename,
                       bool directed,
                       GraphMatrix*& graphMatrix,
                       GraphList*& graphList) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Blad otwarcia pliku: " << filename << "\n";
        return;
    }
    int vertices, edges;
    infile >> vertices >> edges;
    delete graphMatrix;
    delete graphList;
    graphMatrix = new GraphMatrix(vertices);
    graphList   = new GraphList(vertices);
    for (int i = 0; i < edges; ++i) {
        int u, v, w;
        infile >> u >> v >> w;
        if (directed) {
            graphMatrix->addDirectedEdge(u, v, w);
            graphList->addDirectedEdge(u, v, w);
        } else {
            graphMatrix->addEdge(u, v, w);
            graphList->addEdge(u, v, w);
        }
    }
    infile.close();
    std::cout << "Graf wczytano z pliku: " << filename << "\n";
}

// ****************************
// Funkcja: wyświetlenie menu
void displayMenu() {
    std::cout << "================= Menu =================\n";
    std::cout << "1. Generuj losowy graf i zapisz do pliku\n";
    std::cout << "2. Wczytaj graf z pliku\n";
    std::cout << "3. Wyswietl graf (listowo i macierzowo)\n";
    std::cout << "4. Algorytm Prima\n";
    std::cout << "5. Algorytm Kruskala\n";
    std::cout << "6. Dijkstra\n";
    std::cout << "7. Bellman-Ford\n";
    std::cout << "8. Max flow (Ford-Fulkerson)\n";
    std::cout << "9. Tryb eksperymentu (MST / SP / MaxFlow)\n";
    std::cout << "10. Wyjscie\n";
    std::cout << "========================================\n";
    std::cout << "Wybierz opcje: ";
}

// ****************************
// JEDYNE main()
int main() {
    int n = 0, m = 0;
    GraphMatrix *graphMatrixU = nullptr, *graphMatrixD = nullptr;
    GraphList   *graphListU   = nullptr, *graphListD   = nullptr;

    while (true) {
        displayMenu();
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::cout << "Podaj liczbe wierzcholkow: ";
                std::cin >> n;
                int maxEdges = n * (n - 1) / 2;

                std::cout << "Wybierz gestosc grafu:\n";
                std::cout << "  1) 20%\n";
                std::cout << "  2) 60%\n";
                std::cout << "  3) 99%\n";
                std::cout << "Twoj wybor [1-3]: ";
                int densChoice;
                std::cin >> densChoice;

                double density = 0.2;
                if (densChoice == 2) density = 0.6;
                else if (densChoice == 3) density = 0.99;

                int edges;
                if (densChoice == 3) {
                    edges = maxEdges;                       // najpierw 100%
                } else {
                    edges = static_cast<int>(std::ceil(maxEdges * density));
                }

                std::cout << "Wybrana gestosc = " << static_cast<int>(density * 100)
                          << "%, liczba krawedzi = " << edges << "\n";

                std::cout << "Czy graf ma byc skierowany? (1 = tak, 0 = nie): ";
                bool directed;
                std::cin >> directed;

                std::string filename;
                std::cout << "Podaj nazwe pliku do zapisu: ";
                std::cin >> filename;

                generateAndSaveRandomGraph(n, edges, filename, directed);

                if (densChoice == 3) {
                    removeOnePercent(filename);
                }

                readGraphFromFile(filename, false, graphMatrixU, graphListU);
                readGraphFromFile(filename, true,  graphMatrixD, graphListD);
                break;
            }

            case 2: {
                std::string filename;
                std::cout << "Podaj nazwe pliku: ";
                std::cin >> filename;

                std::ifstream infile(filename);
                if (!infile) {
                    std::cerr << "Blad otwierania pliku: " << filename << "\n";
                    break;
                }
                infile >> n >> m;
                delete graphMatrixU; delete graphListU;
                delete graphMatrixD; delete graphListD;
                graphMatrixU = new GraphMatrix(n);
                graphListU   = new GraphList(n);
                graphMatrixD = new GraphMatrix(n);
                graphListD   = new GraphList(n);

                for (int i = 0; i < m; ++i) {
                    int u, v, w;
                    infile >> u >> v >> w;
                    graphMatrixU->addEdge(u, v, w);
                    graphListU->addEdge(u, v, w);
                    graphMatrixD->addDirectedEdge(u, v, w);
                    graphListD->addDirectedEdge(u, v, w);
                }
                infile.close();

                std::cout << "Graf wczytano z pliku.\n";
                std::cout << "Repr. macierzowa (Nieskierowany - MST):\n";
                graphMatrixU->display();
                std::cout << "Repr. listowa    (Nieskierowany - MST):\n";
                graphListU->display();
                std::cout << "Repr. macierzowa (Skierowany - SP):\n";
                graphMatrixD->display();
                std::cout << "Repr. listowa    (Skierowany - SP):\n";
                graphListD->display();
                break;
            }

            case 3: {
                if (graphMatrixU && graphListU) {
                    std::cout << "Repr. macierzowa (Nieskierowany - MST):\n";
                    graphMatrixU->display();
                    std::cout << "Repr. listowa    (Nieskierowany - MST):\n";
                    graphListU->display();
                }
                if (graphMatrixD && graphListD) {
                    std::cout << "Repr. macierzowa (Skierowany - SP):\n";
                    graphMatrixD->display();
                    std::cout << "Repr. listowa    (Skierowany - SP):\n";
                    graphListD->display();
                }
                break;
            }

            case 4: {
                if (graphMatrixU && graphListU) {
                    MSTPrim prim(n);
                    std::cout << "MST (Prim) macierzowo:\n";
                    prim.runMatrix(*graphMatrixU);
                    std::cout << "\nMST (Prim) listowo:\n";
                    prim.runList(*graphListU);
                } else {
                    std::cout << "Graf nieskierowany nie zostal zainicjowany.\n";
                }
                break;
            }

            case 5: {
                if (graphMatrixU && graphListU) {
                    MSTKruskal kruskal(n);
                    std::cout << "MST (Kruskal) macierzowo:\n";
                    kruskal.run(*graphMatrixU);
                    std::cout << "\nMST (Kruskal) listowo:\n";
                    kruskal.run(*graphListU);
                } else {
                    std::cout << "Graf nieskierowany nie zostal zainicjowany.\n";
                }
                break;
            }

            case 6: {
                if (graphMatrixD && graphListD) {
                    int start;
                    std::cout << "Podaj wierzcholek startowy: ";
                    std::cin >> start;
                    SPDijkstra dij(n);
                    std::cout << "Dijkstra macierzowo:\n";
                    dij.runMatrix(*graphMatrixD, start);
                    std::cout << "Dijkstra listowo:\n";
                    dij.runList(*graphListD, start);
                } else {
                    std::cout << "Graf skierowany nie zostal zainicjowany.\n";
                }
                break;
            }

            case 7: {
                if (graphMatrixD && graphListD) {
                    int start;
                    std::cout << "Podaj wierzcholek startowy: ";
                    std::cin >> start;
                    SPBellman bf(n);
                    std::cout << "Bellman-Ford macierzowo:\n";
                    bf.runMatrix(*graphMatrixD, start);
                    std::cout << "Bellman-Ford listowo:\n";
                    bf.runList(*graphListD, start);
                } else {
                    std::cout << "Graf skierowany nie zostal zainicjowany.\n";
                }
                break;
            }

            case 8: {
                if (graphMatrixD && graphListD) {
                    int s, t;
                    std::cout << "Podaj wierzcholek zrodlowy (s): ";
                    std::cin >> s;
                    std::cout << "Podaj wierzcholek ujscia   (t): ";
                    std::cin >> t;
                    MaxFlowFF mf(n);
                    mf.fordFulkerson(*graphMatrixD, s, t);
                } else {
                    std::cout << "Graf skierowany nie zostal zainicjowany.\n";
                }
                break;
            }

            case 9: {
                std::cout << "Wybierz typ eksperymentu:\n";
                std::cout << "  1) MST  (Prim + Kruskal)\n";
                std::cout << "  2) SP   (Dijkstra + Bellman-Ford)\n";
                std::cout << "  3) MaxFlow (Ford-Fulkerson)\n";
                std::cout << "Twoj wybor [1-3]: ";
                int expChoice;
                std::cin >> expChoice;
                if (expChoice < 1 || expChoice > 3) {
                    std::cout << "Nieprawidlowy wybor eksperymentu.\n";
                    break;
                }
                std::cout << "Czy wyswietlac kazda instancje (1 = tak, 0 = nie)? ";
                bool verbose;
                std::cin >> verbose;
                runExperiments(expChoice, verbose);
                break;
            }

            case 10:
                std::cout << "Konczenie programu.\n";
                delete graphMatrixU; delete graphListU;
                delete graphMatrixD; delete graphListD;
                return 0;

            default:
                std::cout << "Nieprawidlowa opcja, sproboj ponownie.\n";
        }
    }

    return 0;
}
