#pragma once
#include "Vertex.h"
#include "Lidar.h"
#include <stack>
#include <thread> 
#include <chrono>
#include <vector>
#include "YoloData.cpp"
#include <iostream>
#include "Calculates.cpp"
#include "Graph.h"
#include "YoloData.cpp"
#include "Navigate.cpp"
#include "GasSensors.h"
#include "Matrix.h"
#include <Eigen/Dense>


using namespace std;

void move() {
	Graph graph;
	Matrix mat;
	Lidar lidar;
	stack <Vertex> turnsStack;
	double mainAngel = 0;
	bool turnflag = 0;
	GasSensors gasSensor = GasSensors();
	
	graph.initial();
	mat.initial();
	mat.printMat();
	//mat.deleteMatrix();

	bool flag = true;//true- right side, false - left side
	int counter;//num of turns that i changed directions
	double rightDist, leftDist;

	//����� ���� ��������
	thread yoloThread(&runYoloPrediction, ref(graph));
	//����� ������ ��� ������� ���� ��������- �� ������ �������

	Vertex* curr = new Vertex(0, 30, 0, 0, graph.getFirst());//����� ������� ������
	mat.changeValue(0, 0, curr);//����� ������� �� ������� ������
	graph.addVertex(curr); //����� ����
	
	//curr.printVertex();
	vector<Point> lidarPointRL;
	vector<Point> lidarPointUD;
	pair<vector<Point>, vector<Point>> walls;
	vector<Point> rightSide;
	vector<Point> leftSide;
	vector<Point> UpSide;
	vector <double> avgsOfWalls;
	Point door;
	Point p;
	double m0 = 0, m1, m2, angel;
	Vertex* newCurr;
	Eigen::Vector2d r;
	Vertex* v;
	//Point lastP;

	while (1)
	{
		//����� ����� �� ����
		lidarPointRL =lidar.GetRightToLeftLidar(); //����� ����� �����
		m1 = lidar.filterFirstsForSlope(lidarPointRL); //����� ����� ����� �� ����
		angel= angelCalc(m0, m1);//����� ������� ����� ���� ��� ������ ����� ����� �� ���� ��� ���� ����� ������
		m0 = m1;
		turnTo(curr->getAngel()- angel);
		rightDist = lidar.rightDist();//������ �������
		leftDist = lidar.leftDist();
		while (lidar.wallInFront(lidarPointRL) == false)
		{
			turnflag = 1;
			lidarPointRL = lidar.GetRightToLeftLidar();
			walls = lidar.wallsFilter(lidarPointRL, rightDist, leftDist);//���� �� ������ ����
			rightSide = walls.first;
			leftSide = walls.second;
			avgsOfWalls = avgs(rightSide, leftSide);
			
			//�� �� ���
			gasSensor.getLeftLevels();
			gasSensor.getRightLevels();
			if (avgsOfWalls[0] > 30)  //�� ������� ����� ����� �30 �"� ����� �� �����
			{
				p = lidar.openOrDoor(lidarPointUD, avgsOfWalls[0]);//���� ������ ����� ����� ����� ���
				r = rotateVectorByAngle(0, p.getZ(), mainAngel);//����� 2 �"� ��� ����� ������ ���� / ��� ��� ����
				curr= new Vertex(curr->getPoint().getX()+r(0), curr->getPoint().getY(),curr->getPoint().getZ()+ r(1)+2, curr->getAngel(), curr);
				curr= graph.addVertex(curr);
				mat.changeValue(curr->getPoint().getX(), curr->getPoint().getZ(), curr);
				goTo(p.getZ() + 2);//����� �����, ������ ��� �������� ������

				lidarPointUD = lidar.GetUpTDownLidar();
				walls = lidar.wallsFilter(lidarPointUD, rightDist, leftDist);//���� �� ������ ����
				rightSide = walls.first;
				leftSide = walls.second;
				avgsOfWalls = avgs(rightSide, leftSide);
				curr->setAngel(-90);//������ 
				turnTo(-90);
				mainAngel += (-90);

				if (avgsOfWalls[0] > 30)
				{//�� �� ��� ��� �� ���!!
					if (hasClassInPrediction("//", 1))//1-���
						//
					{
						p = lidar.openOrDoor(lidarPointUD, avgsOfWalls[0]);
						moveToOtherRoom(rightDist, p, -90, 0);
						UpSide = lidar.GetUpLidar();
						//��� ������ �� �������� ���? �� ����� ���� ���� ���
						p = lidar.maxPoint(UpSide);//������ ��������� �����
						r = rotateVectorByAngle(  rightDist + p.getX(), 0, mainAngel);//������ �X �� ���� ���� ���� �� ����� ����� ���� ����

						//newCurr.changeVertex(p.getX()+?, p.getY(), p.getZ(), -90, &curr);
						newCurr=new Vertex(curr->getPoint().getX()+r(0), p.getY(), curr->getPoint().getZ()+r(1), -90, curr);
						newCurr=graph.addVertex(newCurr);
						mat.changeValue(newCurr->getPoint().getX(), newCurr->getPoint().getZ(), curr);
						mainAngel += -90;//������ ������ ����� ������ �����- ��"� ������ ������ ����� ����: -180
						m1 = m2;
						curr = newCurr;
						break;
					}
					else
					{
						//�� �� �� ���/����� ����
						goTo(rightDist); //���� ��� ���� ���� �����
						lidarPointRL = lidar.GetRightToLeftLidar();
						m2 = lidar.filterFirstsForSlope(lidarPointRL);//������ ����
						angel = angelCalc(m1, m2);//����� ������� ����� ���� ��� ������
						turnTo(angel + 90);//����� 
						curr->setAngel(angel);
						curr->setStop(Stops::RIGHT);
						r = rotateVectorByAngle(rightDist , 0, mainAngel);//
						curr->setPoint(curr->getPoint().getX() + r(0), curr->getPoint().getY(), curr->getPoint().getZ()+r(1));
						mainAngel += angel;
						m1 = m2;
						//curr = newCurr; ���  newCurr ����� ���
						break;
					}
				}		
			}
			else
			{
				//������� ���
				goTo(5.0);
				r = rotateVectorByAngle(0 ,5, mainAngel);
				newCurr= new Vertex(curr->getPoint().getX()+r(0), curr->getPoint().getY(), p.getZ() + r(1), 0, curr);
				newCurr=graph.addVertex(newCurr);
				mat.changeValue(newCurr->getPoint().getX(), newCurr->getPoint().getZ(), curr);
			}
			//����� ������� �� ������� ���- obstacles
			//lidarPointRL = lidar.GetRightToLeftLidar();
			//m1 = lidar.filterFirsts(lidarPointRL);
			curr = newCurr;//���� �������� ����
			if (turnflag == 0)
			{ 	
				mainAngel += 90;
				curr->setStop(Stops::LEFT);
				curr->setAngel(90);
				turnTo(curr->getAngel());
			}
			turnflag = 0;
		}
	}
	yoloThread.join();
}
