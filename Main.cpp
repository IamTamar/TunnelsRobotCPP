#pragma once
#include "Vertex.h"
#include "Lidar.h"
#include <stack>
#include <thread> 
#include <chrono>
#include <vector>
#include "YoloData.h"
#include <iostream>
#include "Calculates.cpp"
#include "Graph.h"
#include "Navigate.cpp"
#include "GasSensors.h"
#include "Matrix.h"
#include <Eigen/Dense>
#include "SoundSensors.h"
#include <utility>
#include <functional>
using namespace std;
void print(const std::string& myString) {
	cout << myString << endl; // ����� �� ������� ����� ����
}
void move() {
	Graph graph;
	Matrix mat;
	Lidar lidar;
	stack <Vertex> turnsStack;
	float mainAngel = 90, currnetAngel = 90;
	bool turnflag = 0;// ��� ����� ���� ����
	bool flag = true;//true- right side, false - left side
	int counter;//num of turns that i changed directions
	GasSensors gasSensors = GasSensors();
	SoundSensors soundSensors = SoundSensors();
	vector<Point> lidarPointRL, lidarPointUD, rightSide, leftSide, UpSide;
	pair<vector<Point>, vector<Point>> walls;
	vector <float> avgsOfWalls;
	float m0 = 0, m1, m2, angel, rightDist, leftDist;
	Vertex* curr, * newCurr, * v, *last;
	Eigen::Vector2d r;
	File f;
    YoloData yd(&graph, &mat);

	graph.initial();
	mat.initial();
	mat.printMat();

	//����� ���� ��������
	thread yoloThread(&YoloData::processDirectoryAndReadFiles, &yd, &graph, &mat);

    curr = new Vertex(0, 30, 0, 0, graph.getFirst());//����� ������� ������
	mat.changeValue( curr);
	graph.addVertex(curr); 
	rightDist = lidar.rightDist();//������ �������
	leftDist = lidar.leftDist();
	last = new Vertex((-leftDist) , 30, 0, 0, nullptr);
	mat.changeValue(last);

	while (curr->isSimilliar(last))
	{
		//����� ����� �� ����
		lidarPointRL = lidar.GetRightToLeftLidar(); //����� ����� �����
		m1 = lidar.filterFirstsForSlope(lidarPointRL); //����� ����� ����� �� ����
		angel = angelCalc(mainAngel, m1);// ����� ����� �� ���� ��� ���� ����� ������
		if (!turnflag && angel < 0) {  //�� ����� ��� ���� �����, ��� ��� ���� ������ �� ������ ����� ������. �� ���� ���� ����� ��
			angel += 180; 	
			mainAngel += angel;
		} 
		turnTo( angel); //������ ��� ����� ����
		rightDist = lidar.rightDist();//������ �������
		leftDist = lidar.leftDist();
		while (lidar.wallInFront(lidarPointRL) == false)
		{
			currnetAngel = flag == true ? -90 : 90; 
			turnflag = 1;
			lidarPointRL = lidar.GetRightToLeftLidar();
			walls = lidar.wallsFilter(lidarPointRL, rightDist, leftDist);//���� �� ������ ����
			rightSide = walls.first;
			leftSide = walls.second;
			avgsOfWalls = avgs(rightSide, leftSide);

			//�� �� ���
			gasSensors.getLeftLevels();
			gasSensors.getRightLevels();
			soundSensors.getRightLevels();
			soundSensors.getLeftLevels();
			if (avgsOfWalls[0] > 30)  //�� ������� ����� ����� �30 �"� ����� �� �����
			{
				Point point(0, 0, 0);
				lidar.openOrDoor(lidarPointUD, avgsOfWalls[0], std::ref(point));
				r = rotateVectorByAngle(point.getZ() ,0,  mainAngel);//����� 2 �"� ��� ����� ������ ���� / ��� ��� ����
				curr = new Vertex(curr->getPoint().getX() + r(0), curr->getPoint().getY(), curr->getPoint().getZ() + r(1) + 2, curr->getAngel(), curr);
				graph.addVertex(curr);
				mat.changeValue(curr);
				goTo( 0, 0 ,point.getZ() + 2 );//����� ����� (�����), ������ ��� �������� ������
				
				lidarPointUD = lidar.GetUpTDownLidar();
				walls = lidar.wallsFilter(lidarPointUD, rightDist, leftDist);//���� �� ������ ����
				rightSide = walls.first;
				leftSide = walls.second;
				avgsOfWalls = avgs(rightSide, leftSide);
				curr->setAngel(currnetAngel);//������
				turnTo(currnetAngel);
				mainAngel += (currnetAngel);

				if (avgsOfWalls[0] > 30)
				{//�� �� ��� ��� �� ���!!
					if (yd.getFlag() == true)//1-���
					{
						Point p(0, 0, 0);
						lidar.openOrDoor(lidarPointUD, avgsOfWalls[0],std::ref(p));
						r = rotateVectorByAngle(0,  rightDist + p.getX(),  mainAngel);//������ �X �� ���� ���� ���� �� ����� ����� ���� ����
						newCurr = new Vertex(curr->getPoint().getX() + r(0), p.getY(), curr->getPoint().getZ() + r(1), -90, curr);

						if (mat.isVisited(newCurr->getPoint().getX(), newCurr->getPoint().getZ()) == -1)//�� ���� �� �����
						{
							graph.addVertex(newCurr);
							moveToOtherRoom(rightDist, p.getX(), p.getY(), p.getZ(), -90, 0);//������ �����
							UpSide = lidar.GetUpLidar();
							Point up(0, 0, 0);
							lidar.maxPoint(UpSide,std::ref(up));//������ ��������� �����
							goTo(up.getX(), up.getY(), up.getZ());
							mat.changeValue(newCurr);
							mainAngel += -90;//������ ������ ����� ������ �����- ��"� ������ ������ ����� ����: -180
							curr = newCurr;
							break;
						}
						else {
							mainAngel -= (currnetAngel);//������ �� ������ �����
							curr->setAngel(-currnetAngel);//������
							turnTo(-currnetAngel);
							goTo(10);//��� ��� 10 �"� ������ ����� �� ����
						}
					}
					else
					{
						//�� �� �� ���/����� ����
						goTo(rightDist); //���� ��� ���� ���� ���� ������� ������� -90 �����
						lidarPointRL = lidar.GetRightToLeftLidar();
						m2 = lidar.filterFirstsForSlope(lidarPointRL);//������ ����
						angel = angelCalc(mainAngel, m2);// ����� ����� �� ���� ��� ���� ����� ������
						if ( angel > 0)// ����� ��� ���� ���� ������
							angel -= 180;
						turnTo(angel -currnetAngel);//����� ��� ������ -90 �� ����� ���� �� ������� ������� �� ������ ���� �� ����� ���� ���� ���� 
						if(mat.isVisited(curr->getPoint().getX(), curr->getPoint().getZ()) ==-1)//�� ����� ��
						{
							curr->setAngel(angel);
							curr->setStop(Stops::RIGHT);
							r = rotateVectorByAngle(0, rightDist,  mainAngel);
							curr->setPoint(curr->getPoint().getX() + r(0), curr->getPoint().getY(), curr->getPoint().getZ() + r(1));
							mainAngel += angel;
							m1 = m2;
							break;
						}
						else {//����� ��� ����� ������
							mainAngel -= (currnetAngel);//������ �� ������ �����
							curr->setAngel(-currnetAngel);//������
							turnTo(-currnetAngel);
							goTo(10);//��� ��� 10 �"� ������ ����� �� ����
						}
					}
				}
			}
			else
			{
				//������� ���
				goTo(20.0);
				r = rotateVectorByAngle(20, 0,  mainAngel);
				newCurr = new Vertex(curr->getPoint().getX() + r(0), curr->getPoint().getY(), curr->getPoint().getZ() + r(1), 0, curr);
				newCurr = graph.addVertex(newCurr);
				mat.changeValue(newCurr);
				curr = newCurr;//���� �������� ����
			}
		}
		if (turnflag == 0)//�� �� ��� ���� ������
		{
			flag = false;//������ ������ �����
			currnetAngel = 90;
			mainAngel += currnetAngel;
			curr->setStop(Stops::LEFT);
			curr->setAngel(90);
			turnTo(curr->getAngel());
			flag = true;// ���� ����� 
		}
		turnflag = 0;
	}
	yoloThread.join();
	//mat.deleteMatrix();
}

int main() {
	move();
}

