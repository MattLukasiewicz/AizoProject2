cmake_minimum_required(VERSION 3.26)
project(Aizo_Projekt_2_grafy)

set(CMAKE_CXX_STANDARD 17)

add_executable(Aizo_Projekt_2_grafy main.cpp
        GraphMatrix.cpp
        GraphMatrix.h
        GraphList.cpp
        GraphList.h
        MST_Kruskal.cpp
        MST_Kruskal.h
        MST_Prim.cpp
        MST_Prim.h
        SP_Dijkstra.cpp
        SP_Dijkstra.h
        SP_Bellman.cpp
        SP_Bellman.h)
