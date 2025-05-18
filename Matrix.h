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
	vector<double> Xdirections = { 0,-1,0,1,-1,1,-1,0,1 };//לכאורה צריך להגדיל את הטווח של הבדיקות מסביב, כביכול עשיתי זאת
	vector<double> Zdirections = { 0,-1,-1,-1,0,0,1,1,1 };

public:
	void initial();
	void addRowUP(int num);
	void addRowDown(int num);
	void addCulomnLeft(int num);
	void addColumnRight(int num);
	int findXIndex(int x);
	int findZIndex(int z);
	//int value(double x, double z);
	void changeValue(double x, double z, Vertex* p);
	void printMat();
	Vertex* isVisited(double x, double z);
	void deleteMatrix();

};


//struct cell
//{
//	b
//	int* p = nullptr;
//};
//vector<vector<cell>> mat1;//x+ z+
//	vector<vector<cell>> mat2;//x+ y-
//	vector<vector<cell>> mat3;//x- y-
//	vector<vector<cell>> mat4;//x- y+
//	Matrix() = default;
//public:
//
//	int getValue(double x, double z) {
//		int i = static_cast<int>(round(z));//z axe
//		int j = static_cast<int>(round(x));//x axe
//		if (x >= 0 && z >= 0) {
//			(i>)
//		}
//			return *(mat1[i][j].p);
//		if (x > 0 && z < 0)
//			return *(mat2[-i][j].p);
//		if (x < 0 && z < 0)
//			return *(mat3[-i][-j].p);
//		if (x < 0 && z>0)
//			return *(mat4[i][-j].p);
//	}
//
//	bool changeValue() {};
//	void resizeCols(int newSize, int newCols, vector<vector<cell>> mat) {
//		for (auto& row : mat) {
//			row.resize(newCols); // שינוי מספר העמודות בכל שורה
//		}
//	};
//	void resizeRows(int newRows, int defaultValue = 0, vector<vector<cell>> mat) {
//		mat.resize(newRows); // שינוי מספר השורות
//	}