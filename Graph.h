#pragma once
#include "Vertex.h"
#include "iostream"
#include "list"

class Graph {
private:
    list<Vertex*> vertice;
    Vertex* first;
    Vertex* last;
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
        return first;
    }
    Vertex* getLast() {
        return last;
    }

    Vertex* addVertex(Vertex* newV);
    void addEdge(Vertex* from, Vertex* to);
    void printGraph() const;
    Vertex* addPeopleVertex();
    Vertex* addWeaponVertex();

};


//{
//public:
//	list<Vertex> mainList;
//	Vertex* first;
//	Vertex* last;
//public:
//	void initial() {
//		Vertex fir = Vertex(0, 0, -1, 0, nullptr);
//		mainList.push_back(fir);
//		first = last = &fir;
//	}
//	Vertex* getFirst() {
//		return first;
//	}
//	Vertex* getLast() {
//		return last;
//	}
//	bool addVertex(Vertex v) {
//		mainList.push_back(v);
//		last = ;
//	}
//	void printGraph() {
//		for (Vertex v : mainList) {
//			v.printVertex();
//		}
//	}
//};