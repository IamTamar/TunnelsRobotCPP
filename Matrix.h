#pragma once
#include <cmath>
#include <vector>
#define SIZE 10
#include <iostream>
#include "Vertex.h"
using namespace std;

class Matrix
{

public:
	vector<vector<Vertex*>> mat;
	int posX = SIZE / 2 - 1;
	int posZ = SIZE / 2 - 1;//0,0- נמצאת במקום 500
	int colNum = SIZE;
	int rowNum = SIZE;
	vector<float> Xdirections = { 0,-1,0,1,-1,1,-1,0,1 };//לכאורה צריך להגדיל את הטווח של הבדיקות מסביב
	vector<float> Zdirections = { 0,-1,-1,-1,0,0,1,1,1 };

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


};