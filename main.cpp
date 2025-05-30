// Main.cpp
#include <iostream>
#include <cstdlib> // for rand()
#include <ctime>   // for seeding rand()
#include <fstream>
#include <cmath>
#include <string>

#include "GraphMatrix.h"
#include "GraphList.h"
#include "MST_Kruskal.h"
#include "MST_Prim.h"
#include "SP_Dijkstra.h"
#include "SP_Bellman.h"
#include "MaxFlow_FF.h"


void readGraphFromFile(const std::string& filename, bool directed, GraphMatrix*& graphMatrix, GraphList*& graphList) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Blad otwarcia pliku: " << filename << "\n";
        return;
    }

    int vertices, edges;
    infile >> vertices >> edges;

    // Usuń stare struktury grafu
    delete graphMatrix;
    delete graphList;

    // Inicjalizuj nowe struktury
    graphMatrix = new GraphMatrix(vertices);
    graphList = new GraphList(vertices);

    // Wczytaj krawędzie
    for (int i = 0; i < edges; ++i) {
        int u, v, w;
        infile >> u >> v >> w;
        if (directed) {
            // Dodaj krawędzie skierowane
            graphMatrix->addDirectedEdge(u, v, w);
            graphList->addDirectedEdge(u, v, w);
        } else {
            // Dodaj krawędzie nieskierowane
            graphMatrix->addEdge(u, v, w);
            graphList->addEdge(u, v, w);
        }
    }

    infile.close();
    std::cout << "Graf wczytano z pliku: " << filename << "\n";
}

void generateAndSaveRandomGraph(int vertices, int edges, const std::string& filename, bool directed) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Blad zapisu pliku: " << filename << "\n";
        return;
    }

    // Zapisz liczbę wierzchołków i krawędzi
    outfile << vertices << " " << edges << "\n";

    // Przygotuj struktury pomocnicze
    bool* visited = new bool[vertices]();
    bool** exists = new bool*[vertices];
    for (int i = 0; i < vertices; ++i) {
        exists[i] = new bool[vertices]();
    }

    srand(static_cast<unsigned>(time(nullptr)));

    // Generowanie spójnego grafu
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

    // Dla skierowanego: pętla zapewniająca dojścia
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

    // Generowanie dodatkowych krawędzi
    int extraEdges = edges - (directed ? treeEdges + vertices : treeEdges);
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

    // Czyszczenie pamięci
    for (int i = 0; i < vertices; ++i) delete[] exists[i];
    delete[] exists;
    delete[] visited;

    outfile.close();
    std::cout << "Graf zapisano do pliku: " << filename << "\n";
}


void removeOnePercent(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Blad otwierania pliku: " << filename << "\n";
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

    // Usuwanie 1% krawędzi
    int toRemove = edges / 100;
    srand(static_cast<unsigned>(time(nullptr)));

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
    outfile.close();

    delete[] u;
    delete[] v;
    delete[] w;

    std::cout << "Usunieto 1% krawedzi z grafu: " << filename << "\n";
}




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
    std::cout << "9. Wyjscie\n";
    std::cout << "========================================\n";
    std::cout << "Wybierz opcje: ";
}


int main() {
    int n = 0, m = 0; // Number of vertices and edges
    GraphMatrix* graphMatrix = nullptr;
    GraphList* graphList = nullptr;
    MSTKruskal* kruskal = nullptr;
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

                // Oblicz maksymalną liczbę krawędzi w grafie nieskierowanym
                int maxEdges = n * (n - 1) / 2;

                // Wybór gęstości
                std::cout << "Wybierz gestosc grafu:\n";
                std::cout << "  1) 20%\n";
                std::cout << "  2) 60%\n";
                std::cout << "  3) 99%\n";
                std::cout << "Twoj wybor [1-3]: ";
                int densChoice;
                std::cin >> densChoice;

                double density = 0.2; // Domyślna gęstość to 20%
                if (densChoice == 2) density = 0.6;
                else if (densChoice == 3) density = 0.99;

                // Oblicz liczbę krawędzi na podstawie wybranej gęstości
                int edges = static_cast<int>(std::ceil(maxEdges * density));

                std::cout << "Wybrana gestosc = " << static_cast<int>(density * 100)
                          << "%, liczba krawedzi = " << edges << "\n";

                // Wybór trybu grafu
                std::cout << "Czy graf ma byc skierowany? (1 = tak, 0 = nie): ";
                bool directed;
                std::cin >> directed;

                std::string filename;
                std::cout << "Podaj nazwe pliku do zapisu: ";
                std::cin >> filename;

                // Generuj graf
                generateAndSaveRandomGraph(n, edges, filename, directed);

                // Jeśli wybrano 99% gęstości, usuń 1% krawędzi
                if (densChoice == 3) removeOnePercent(filename);

                // Wczytaj jako nieskierowany (dla MST)
                readGraphFromFile(filename, false, graphMatrixU, graphListU);

                // Wczytaj jako skierowany (dla SP)
                readGraphFromFile(filename, true, graphMatrixD, graphListD);
                break;
            }



            case 2: {
                // Wczytanie z pliku
                std::string filename;
                std::cout << "Podaj nazwe pliku: ";
                std::cin >> filename;

                std::ifstream infile(filename.c_str());
                if (!infile) {
                    std::cerr << "Blad otwierania pliku: " << filename << std::endl;
                    break;
                }

                infile >> n >> m;

                // Usuwamy poprzednie grafy
                delete graphMatrixU; delete graphListU;
                delete graphMatrixD; delete graphListD;
                graphMatrixU = new GraphMatrix(n);
                graphListU   = new GraphList(n);
                graphMatrixD = new GraphMatrix(n);
                graphListD   = new GraphList(n);

                int u, v, w;
                for (int i = 0; i < m; ++i) {
                    infile >> u >> v >> w;

                    // MST: Nieskierowane
                    graphMatrixU->addEdge(u, v, w);
                    graphListU->addEdge(u, v, w);

                    // SP: Skierowane
                    graphMatrixD->addDirectedEdge(u, v, w);
                    graphListD->addDirectedEdge(u, v, w);
                }
                infile.close();

                std::cout << "Graf wczytano z pliku.\n";
                std::cout << "Reprezentacja macierzowa (Nieskierowany - MST):\n";
                graphMatrixU->display();
                std::cout << "Reprezentacja listowa (Nieskierowany - MST):\n";
                graphListU->display();

                std::cout << "Reprezentacja macierzowa (Skierowany - SP):\n";
                graphMatrixD->display();
                std::cout << "Reprezentacja listowa (Skierowany - SP):\n";
                graphListD->display();
                break;
            }

            case 3: {
                if (graphMatrixU && graphListU) {
                    std::cout << "Reprezentacja macierzowa (Nieskierowany - MST):\n";
                    graphMatrixU->display();
                    std::cout << "Reprezentacja listowa (Nieskierowany - MST):\n";
                    graphListU->display();
                }

                if (graphMatrixD && graphListD) {
                    std::cout << "Reprezentacja macierzowa (Skierowany - SP):\n";
                    graphMatrixD->display();
                    std::cout << "Reprezentacja listowa (Skierowany - SP):\n";
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
                // Maksymalny przepływ na grafie skierowanym
                if (graphMatrixD && graphListD) {
                    int s, t;
                    std::cout << "Podaj wierzcholek zrodlowy (s): ";
                    std::cin >> s;
                    std::cout << "Podaj wierzcholek ujscia (t): ";
                    std::cin >> t;
                    MaxFlowFF mf(n);
                    mf.fordFulkerson(*graphMatrixD, s, t);
                } else {
                    std::cout << "Graf skierowany nie zostal zainicjowany.\n";
                }
                break;
            }

            case 9:
                std::cout << "Konczenie programu.\n";
                // usuń wszystkie utworzone wskaźniki...
                delete graphMatrixU; delete graphListU;
                delete graphMatrixD; delete graphListD;
                return 0;

        }
    }
}
