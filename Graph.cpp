#pragma once
#include "Graph.h"
#include "Vertex.h"
#include "Stops.cpp"
#include "Point.h"
Vertex* Graph::addVertex(Vertex* newV) {
    Vertex* v = new Vertex(newV->getPoint().getX(), newV->getPoint().getY(),
        newV->getPoint().getZ(), newV->getAngel(), newV->getPrev());
    vertice.push_back(v);
    //if (v->getPrev() != nullptr) 
        v->getPrev()->addNext(v); // ����� ������ �-next �� �����    
        SetLast(v);
    return v;
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

Vertex* Graph::addPeopleVertex() {
    Vertex *v = new Vertex(this->getLast()->getPoint().getX() +1,
        this->getLast()->getPoint().getY(),
        this->getLast()->getPoint().getZ() +1,
        this->getLast()->getAngel(), this->getLast());
    v->setStop(Stops::TERRORIST);
    this->addVertex(v);
    return v;
}

Vertex* Graph::addWeaponVertex() {
    Vertex* v = new Vertex(this->getLast()->getPoint().getX() + 1,
        this->getLast()->getPoint().getY(),
        this->getLast()->getPoint().getZ() + 1,
        this->getLast()->getAngel(), this->getLast());
        v->setStop(Stops::EXPLOSIVES);
        this->addVertex(v);
        return v;
}