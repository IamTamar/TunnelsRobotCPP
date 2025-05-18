#pragma once
#include "Graph.h"
#include "Vertex.h"


Vertex* Graph::addVertex(Vertex* newV) {
    Vertex* v = new Vertex(newV->getPoint().getX(), newV->getPoint().getY(),
        newV->getPoint().getZ(), newV->getAngel(), newV->getPrev());
    vertice.push_back(v);
    if (v->getPrev() != nullptr) {
        v->getPrev()->addNext(v); // הוספה לרשימת ה-next של הקודם
    }
    last = v;
    return v;
}

void Graph::addEdge(Vertex* from, Vertex* to) {
    if (from && to) {
        from->addNext(to);
        to->prev = from;
    }
}

void Graph::printGraph() const {
    for (Vertex* v : vertice) {
        cout << "Vertex: ";
        v->printVertex();
        cout << " -> next: ";
        for (Vertex* next : v->getNextList()) {
            next->printVertex();
        }
        cout << endl;
    }
}
