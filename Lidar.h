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
	double filterFirstsForSlope(vector<Point> v);
	Point openOrDoor(vector<Point> v, double avg);
	Point maxPoint(vector<Point> v);
	

	pair<vector<Point>, vector<Point>> wallsFilter(vector<Point> v, double rightDist, double leftDist);
	vector<double> avgs(vector<Point*> vects);
	bool wallInFront(vector<Point> v);
	double rightDist();
	double leftDist();
};


