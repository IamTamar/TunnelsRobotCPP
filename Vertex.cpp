#pragma once
#include "Vertex.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

using namespace std;
void Vertex::setPointByPoint(Point point) {
	this->p.changeX(point.getX());
	this->p.changeY(point.getY());
	this->p.changeZ(point.getZ());
}
void Vertex::printVertex() {//הדפסת הקואורדינטות

	p.printPoint();
	cout << "angel :" << this->getAngel() << " prev_angel: "/*<< "stop: "*/ << this->getPrev()->getAngel() << /*to_string(this->getStop())<<*/  endl;
}
Point Vertex::vertexToPoint() {  
   return Point(this->getPoint().getX(), this->getPoint().getY(), this->getPoint().getZ());  
}

bool Vertex::isSimilliar(Vertex* v) {
	//check if the vertex v is similliar to this vertex
	if (this->distance(v) <= 5) // 5 is only an example!
		return true;
	return false;
}

//a distance between 2 vertice
float Vertex::distance(Vertex* v) {

	float x = this->getPoint().getX() - v->getPoint().getX();
	float y = this->getPoint().getY() - v->getPoint().getY();
	float z = this->getPoint().getZ() - v->getPoint().getZ();
	//std::cout << sqrt(x * x + y * y + z * z);
	return sqrt(x * x + y * y + z * z);
}

void Vertex::printAngel() {
	cout << "angel: " << this->getAngel() << endl;
}


