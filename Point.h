#pragma once
#include <iostream>
#include <vector>
using namespace std;
//a position point contains 3 coordinates values
class Point
{
public:
	double x;
	double y;
	double z;
public:
	Point() : x(0), y(0), z(0) {} // Default constructor
	Point(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }
	double changeX(double x) {
		this->x += x;
		return this->x;
	}
	double changeY(double y) {
		this->y += y;
		return this->y;
	}
	double changeZ(double z) {
		this->z += z;
		return this->z;
	}
	void printPoint();
	double pointDirection(Point p_v, Point o_v);
};


