#pragma once
#include <iostream>
#include <vector>
using namespace std;
//a position point contains 3 coordinates values
class Point
{
public:
	float x;
	float y;
	float z;
public:
	Point() : x(0), y(0), z(0) {} // Default constructor
	Point(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
	float changeX(float x) {
		this->x += x;
		return this->x;
	}
	float changeY(float y) {
		this->y += y;
		return this->y;
	}
	float changeZ(float z) {
		this->z += z;
		return this->z;
	}
	void printPoint();
	float pointDirection(Point p_v, Point o_v);
};


