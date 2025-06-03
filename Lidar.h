#pragma once
#include <vector>
#include "Vertex.h"
using namespace std;
class Lidar
{
public:
	//vector <Point> UpTDownLidar;//����� ������ ��� ���� ������ ���, ����� ������ ��� ���� ����� �����
	//vector <Point> RightToLeftLidar;//��� ���� ����, ����� ���� - ���� ������� �� ������ ��� ����� ����� ������
	//vector <Point> ceilingLidar;//����� ����� ������, ����� �������
public:
	vector<Point> GetWallInFront();//����� �� ��������!
	vector<Point> GetUpTDownLidar();
	vector<Point> GetRightToLeftLidar();
	vector<Point> GetUpLidar();
	float filterFirstsForSlope(vector<Point>& v);
	Point openOrDoor(vector<Point>& v, float avg);
	Point maxPoint(const vector<Point>& v);

	pair<vector<Point>, vector<Point>> wallsFilter(vector<Point>& v, float rightDist, float leftDist);
	bool wallInFront(const vector<Point>& v);
	float rightDist();
	float leftDist();
};


