// Main.cpp
#include <iostream>
#include <cstdlib> // for rand()
#include <ctime>   // for seeding rand()
#include <fstream>
#include "GraphMatrix.h"
#include "GraphList.h"
#include "MST_Kruskal.h"
#include "MST_Prim.h"
#include "SP_Dijkstra.h"
#include "SP_Bellman.h"


void generateAndSaveRandomGraph(int vertices, int edges, const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Blad zapisu pliku: " << filename << std::endl;
        return;
    }

    srand(static_cast<unsigned int>(time(nullptr)));

    outfile << vertices << " " << edges << "\n";
    for (int i = 0; i < edges; ++i) {
        int u = rand() % vertices;
        int v = rand() % vertices;
        int w = rand() % 100 + 1; // Random weight between 1 and 100
        if (u != v) {
            outfile << u << " " << v << " " << w << "\n";
        } else {
            --i; // Avoid loops by decrementing i to retry
        }
    }

    outfile.close();
    std::cout << "Graf zapisano do pliku: " << filename << "\n";
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
    std::cout << "8. Wyjscie\n";
    std::cout << "========================================\n";
    std::cout << "Wybierz opcje: ";
}

int main() {
    int n = 0, m = 0; // Number of vertices and edges
    GraphMatrix* graphMatrix = nullptr;
    GraphList* graphList = nullptr;
    MSTKruskal* kruskal = nullptr;

    while (true) {
        displayMenu();
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                // Generowanie losowego grafu i zapis do pliku
                std::cout << "Podaj liczbe wierzcholkow: ";
                std::cin >> n;
                std::cout << "Podaj liczbe krawedzi: ";
                std::cin >> m;

                std::string filename;
                std::cout << "Podaj nazwe pliku do zapisu: ";
                std::cin >> filename;

                generateAndSaveRandomGraph(n, m, filename);
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
                delete graphMatrix;
                delete graphList;
                delete kruskal;
                graphMatrix = new GraphMatrix(n);
                graphList = new GraphList(n);
                kruskal = new MSTKruskal(n);

                int u, v, w;
                for (int i = 0; i < m; ++i) {
                    infile >> u >> v >> w;
                    graphMatrix->addEdge(u, v, w);
                    graphList->addEdge(u, v, w);
                }
                infile.close();

                std::cout << "Graf wczytano z pliku.\n";
                break;
            }
            case 3: {
                // Wyświetlenie grafu
                if (graphMatrix && graphList) {
                    std::cout << "Graf w reprezentacji macierzowej:\n";
                    graphMatrix->display();
                    std::cout << "Graf w reprezentacji listowej:\n";
                    graphList->display();
                } else {
                    std::cout << "Graf nie zostal jeszcze wczytany ani wygenerowany.\n";
                }
                break;
            }
            case 4:
                if (graphMatrix && graphList) {
                    MSTPrim prim(n);
                    std::cout << "MST (Prim) macierzowo:\n";
                    prim.runMatrix(*graphMatrix);
                    std::cout << "\nMST (Prim) listowo:\n";
                    prim.runList(*graphList);
                } else {
                    std::cout << "Graf nie zainicjowany.\n";
                }
                break;

            case 5: {
                // Algorytm Kruskala
                if (kruskal && graphMatrix && graphList) {
                    std::cout << "MST dla reprezentacji macierzowej:\n";
                    kruskal->run(*graphMatrix);
                    std::cout << "\nMST dla reprezentacji listowej:\n";
                    kruskal->run(*graphList);
                } else {
                    std::cout << "Graf nie zostal jeszcze wczytany ani wygenerowany.\n";
                }
                break;
            }case 6: {
                int start;
                std::cout << "Podaj wierzcholek startowy: ";
                std::cin >> start;
                SPDijkstra dij(n);
                std::cout << "Dijkstra macierzowo:\\n";
                dij.runMatrix(*graphMatrix, start);
                std::cout << "Dijkstra listowo:\\n";
                dij.runList(*graphList, start);
                break;
            }
            case 7: {
                int start;
                std::cout << "Podaj wierzcholek startowy: ";
                std::cin >> start;
                SPBellman bf(n);
                std::cout << "Bellman-Ford macierzowo:\\n";
                bf.runMatrix(*graphMatrix, start);
                std::cout << "Bellman-Ford listowo:\\n";
                bf.runList(*graphList, start);
                break;
            }

            case 8:
                std::cout << "Konczenie programu.\n";
                delete graphMatrix;
                delete graphList;
                delete kruskal;
                return 0;
            default:
                std::cout << "Nieprawidlowa opcja, sproboj ponownie.\n";
        }
    }
}
