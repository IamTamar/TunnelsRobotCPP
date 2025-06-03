#pragma once
#include <mutex>
#include "Vertex.h"
#include "iostream"
#include "list"
using namespace std;
class Graph {
private:
    list<Vertex*> vertice;
    Vertex* first;
    Vertex* last;
    mutex mtxfirst;
    mutex mtxlast;

public:
    ~Graph() {
        // שחרור זיכרון
        for (Vertex* v : vertice) {
            delete v;
        }
    }
    void initial() {
        Vertex fir = Vertex(0, 30, -1, 0, nullptr);
        vertice.push_back(&fir);
        first = last = &fir;
    }
    Vertex* getFirst() {
        std::lock_guard<std::mutex> lock(mtxfirst);
        return first;
    }
    Vertex* getLast() {
        std::lock_guard<std::mutex> lock(mtxlast);
        return last;
    }
    void SetLast(Vertex* v)
    {
        std::lock_guard<std::mutex> lock(mtxlast);
        last = v;

    };
    Vertex* addVertex(Vertex* newV);
    void printGraph() const;
    Vertex* addPeopleVertex();
    Vertex* addWeaponVertex();

};

