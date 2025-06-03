#pragma once
#include <cmath>
#include <vector>
#define SIZE 10
#include <iostream>
#include <mutex>
#include "Vertex.h"
using namespace std;

class Matrix
{

public:
	vector<vector<Vertex*>> mat;
	int posX = SIZE / 2 - 1;
	int posZ = SIZE / 2 - 1;//0,0- ����� ����� 500
	int colNum = SIZE;
	int rowNum = SIZE;
	vector<float> Xdirections = { 0,-1,0,1,-1,1,-1,0,1 };//������ ���� ������ �� ����� �� ������� �����
	vector<float> Zdirections = { 0,-1,-1,-1,0,0,1,1,1 };
	mutex mtxMat;
public:
	void initial();
	void addRowUP(int num);
	void addRowDown(int num);
	void addCulomnLeft(int num);
	void addColumnRight(int num);
	int findXIndex(int x);
	int findZIndex(int z);
	//int value(float x, float z);
	void changeValue(Vertex* p);
	void printMat();
	float isVisited(float x, float z);
	void deleteMatrix();
	void addPeopleVertex(Vertex* p);
	void addWeaponVertex(Vertex* p);

	void setMat(int i, int j, Vertex* p) {
		// ����� �-mutex ���� ����� ������
		std::lock_guard<std::mutex> lock(mtxMat);
		mat[findZIndex(i)][findXIndex(j)] = p;  // ����� ���� �������
	}

	// ������� ����� ��� ������ I/J �������
	Vertex* getMat(int i, int j)
	{
		// ����� �-mutex ���� ����� ����
		std::lock_guard<std::mutex> lock(mtxMat);
		return mat[findZIndex(i)][findXIndex(j)];
	}

};