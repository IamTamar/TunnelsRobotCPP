#pragma once
#include "Point.h"	
#include <list>
#include "Stops.cpp"

class Vertex {
public:
	Point p;
	double angel;
	Vertex* prev;
	list<Vertex*> nextList;
	Stops s;

	Vertex(double x, double y, double z, double angel, Vertex* prev)
		: p(x, y, z), angel(angel), prev(prev), s(Stops::DEFAULT) {
	}
	Vertex(Vertex* v)
		: p(v->getPoint().getX(),
			v->getPoint().getY(),
			v->getPoint().getZ()),
			angel(v->getAngel()),
			prev(v->getPrev()),
			s(v->getStop()) {}

	Vertex() : p(0, 0, 0), angel(0), prev(nullptr), s(Stops::DEFAULT) {}
	void addNext(Vertex* v) {
		nextList.push_back(v);
	}
	list<Vertex*>& getNextList() {
		return nextList;
	}
	Point getPoint() {
		return p;
	}
	Stops getStop() {
		return s;
	}
	double getAngel() {
		return angel;
	}
	Vertex* getPrev() {
		return prev;
	}
	void setStop(Stops s) {
		this->s = s;
	}
	void setAngel(double angel) {
		this->angel = angel;
	}
	void setPoint(double x, double y, double z) {
		this->p.changeX(x);
		this->p.changeY(y);
		this->p.changeZ(z);
	}
	void setPointByPoint(Point point);
	bool isSimilliar(Vertex v);
	Point vertexToPoint();
	void changeVertex(double x, double y, double z, double angel, Vertex* v);
	double vertexDirection(Vertex p_v, Vertex o_v);	
	void printVertex();
};


