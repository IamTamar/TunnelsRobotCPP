#pragma once
#include "Vertex.h"
class Lidar
{
public:
	vector<Point> UpTDownLidar;//����� ������ ��� ���� ������ ���, ����� ������ ��� ���� ����� �����
	vector<Point> RightToLeftLidar;//��� ���� ����, ����� ���� - ���� ������� �� ������ ��� ����� ����� ������
	vector <Point> ceilingLidar;//����� ����� ������, ����� �������

public:
	vector<Point> GetUpTDownLidar();
	vector<Point> GetRightToLeftLidar();
	vector <Point> GetUpLidar();
	float filterFirstsForSlope(vector<Point> v);
	Point openOrDoor(vector<Point> v, float avg);
	Point maxPoint(vector<Point> v);
	vector <Point> GetWallInFront();//����� �� ��������!
	pair<vector<Point>, vector<Point>> wallsFilter(vector<Point> v, float rightDist, float leftDist);
	bool wallInFront(vector<Point> v);
	float rightDist();
	float leftDist();
	float prevWallFilter(Point curr, float gradient, vector<Point> v);

};


