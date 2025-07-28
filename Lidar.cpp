#pragma once
#include "Lidar.h"
#include "Vertex.h"
#include <algorithm>
#include <vector>
#define PI  3.141592653589793 //pai
#include <cmath>
#include "file.h"
#include "Point.h"

using namespace std;
File f;

 vector<Point> Lidar::GetUpTDownLidar() {
	float angel = 360;

	//lidar performance:
	// up to down on right, and keeps going to buttom and down to up in left
	// writing into file
	//reading and moving into vector<vertex>
	return f.fileToCoordinatesVector("");
 }
 vector<Point> Lidar::GetRightToLeftLidar() {
	//lidar performance:
	//  right near robot, and keeps going in front and left until near robot
	// writing into file
	//reading and moving into vector<vertex>
	return f.fileToCoordinatesVector("C:\\Users\\PC\\Downloads\\TunnelsRobot\\src\\coordinatesRightToLeft.txt");

 }
 vector<Point> Lidar::GetWallInFront() {//����� �� ��������!
	return f.fileToCoordinatesVector("C:\\Users\\PC\\Downloads\\TunnelsRobot\\src\\coordinatesRightToLeftWallInFront.txt");

 }
 vector<Point> Lidar::GetUpLidar() {
	//looks up
	return f.fileToCoordinatesVector("C:\\Users\\PC\\Downloads\\TunnelsRobot\\src\\coordinatesRightToLeft.txt");

 }
float Lidar::rightDist() { //for distances between walls
	//....
	return 0;
}
float Lidar::leftDist() {
	//....
	return 0;
}

float Lidar::filterFirstsForSlope(vector<Point>& v) {
	int i = 0; 
	float sumX = 0, sumZ = 0, sumMulXZ = 0, sumXSquare = 0;
	for (Point& p : v) {
		i++;
		sumX += p.getX();
		sumZ += p.getZ();
		sumMulXZ += (p.getX() * p.getZ());
		sumXSquare += (p.getX() * p.getX());
		if (i > 20)
			break;
	}
	float m = (20 * sumMulXZ - sumX * sumZ) / (20 * sumXSquare - (sumX* sumX));
	return m;
}

//function that filters only walls points and then sorts up to down(y axe),
// and returns differences between up-down on right & left
//�� ���� ���� �� ����� ������� �2 ������ ��� ��!!! ���� ����� ��� ��
pair<vector<Point>, vector<Point>> Lidar::wallsFilter(vector<Point>& v, float rightDist, float leftDist) {
	vector<Point> right = {}, left = {};
	for (Point& p : v) { // p ��� ����� ��-���� �-Point ���� v, ��� ����� ����� 
		if (p.getX() > 0 && p.getX() >= rightDist) {

			right.emplace_back(p.getX(), p.getY(), p.getZ());
		}
		if (p.getX() < 0 && p.getX() <= leftDist) {
			left.emplace_back(p.getX(), p.getY(), p.getZ());
		}
	}
	//��� ���� ����� �� ������ ����� ������ ��� ��� ������!! �� ��� ���� ���� �� ����� ����
	return { right, left };
}

void Lidar::maxPoint(const vector<Point>& v,Point& p) {
	if (v.empty()) {
		throw runtime_error("Vector 'v' is empty in maxPoint; cannot find max point.");
	}
	int maxIndex = 0;
	float maxY = v[0].getY();
	for (size_t i = 1; i < v.size(); ++i) {
		if (v[i].getY() > maxY) {
			maxY = v[i].getY();
			maxIndex = i;
		}
	}
	p.changeX(v[maxIndex].getX());
	p.changeY(v[maxIndex].getY());
	p.changeZ(v[maxIndex].getZ());
}
//z=mx+b
//���� �� ���� ����� ������ �� ��� ���� �� ���� ���� 
//������ �� ������ �� ���� ����
//float Lidar::prevWallFilter(Point curr, float gradient, vector<Point> v) {
//	float b = curr.getZ() - (curr.getX() * gradient);
//	float sumX = 0, sumZ = 0, sumMulXZ = 0, sumXSquare = 0, m;
//	vector<Point> newV;
//	int i = 0;
//	for (Point p : v) {
//		if (curr.getZ() != (curr.getX() * gradient + b))//�� �� ���� �����
//		{
//			newV.push_back(p);
//			i++;
//			sumX += curr.getX();
//			sumZ += curr.getZ();
//			sumMulXZ += (curr.getX() * curr.getZ());
//			sumXSquare += sqrt(curr.getX());
//		}
//		if (i == 9)//9 is only example..
//			break;
//	}
//	// linear regression - B^ :
//
//	m = (20 * sumMulXZ - sumX * sumZ) / (20 * sumXSquare - sqrt(sumX));
//	return m;
//}


// avgs function is in calculates!!
//bool Lidar::wallInFront(vector<Point&> v) {
//	vector<Point> front = {};
//	for (Point p : v)
//	{
//		if (p.getX() >= -20 && p.getX() <= 20)
//			front.push_back(p);
//	}
//
//	sort(front.begin(), front.end(), [](Point a, Point b) {
//		return a.getZ() > b.getZ();
//		});
//	//������� ����� ���� ��� �����! 
//	//�� ���� �� ��� ������ �������� ������ ����� ����� �� ���� ���� ���
//	float sum = 0, avg, median;
//	for (Point& p : front) {
//		sum += p.getZ();
//	}
//	avg = sum / front.size();
//	median = front[(front.size() / 2)].getZ();
//	//���� ����� �� ������ �������?
//	if (avg <= 30)
//		return true;
//	return false;
//}
bool Lidar::wallInFront(const vector<Point>& v)
{

	vector<Point> front;
	front.reserve(v.size()); //  ����� ���� ����
	for (const Point& p : v) { 
		if (p.getX() >= -20 && p.getX() <= 20) {
			front.push_back(p); 
		}
	}
	if (front.empty()) {
		// ��� ������ �����, ��� ���
		return false;
	}
	// ���� ��� Z ���� ����
	std::sort(front.begin(), front.end(), [](const Point& a, const Point& b) {
		return a.getZ() > b.getZ();
		});

	float sum = 0;
	for (const Point& p : front) { // ��� �-const Point&
		sum += p.getZ();
	}
	float avg = sum / front.size();

	float median;
	if (front.size() % 2 == 1) {
		// ���� ��-����, ������ ��� ����� ������
		median = front[front.size() / 2].getZ();
	}
	else {
		// ���� ����, ������ ��� ����� ��� ������� ��������
		median = (front[front.size() / 2 - 1].getZ() + front[front.size() / 2].getZ()) / 2.0f;
	}
	if (avg <= 30) 
		return true;
	return false;
}

void Lidar::openOrDoor(std::vector<Point>& v, float avg, Point& p) { 
	if (v.empty()) {
		throw runtime_error("Vector 'v' is empty in openOrDoor.");
	}
	int i;
	for (i = 1; i < v.size(); i++) {
		if (std::abs(v.at(i).getX() - avg) > 10)
			break;
	}
	int target_index = (i == v.size()) ? v.size() - 1 : i - 1;
   // move ����� ����� ������ ��� ������� ���� ��� ������ �����
	p.changeX(v.at(target_index).getX());
	p.changeY(v.at(target_index).getY());
	p.changeZ(v.at(target_index).getZ());
	//return Point(v.at(target_index).getX(), v.at(target_index).getY(), v.at(target_index).getZ());
}


