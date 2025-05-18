#pragma once
#include "Point.h"

//determinant between 2 vectors causes to 3 results: 0- on path, <0- right, >0- left
double Point::pointDirection(Point p_v, Point o_v) {
	//this - robot, p_v- previous, o_v - other vertex from lidar
	double v1, v2, u1, u2, res;
	v1 = this->getX() - p_v.getX();
	v2 = this->getY() - p_v.getY();
	u1 = o_v.getX() - this->getX();
	u2 = o_v.getY() - this->getY();
	res = (v1 * u2) - (v2 * u1);
	return res;

}
void Point::printPoint() {
	cout << "x: " << this->x;
	cout << " y: " << this->y;
	cout << " z: " << this->z;
}


