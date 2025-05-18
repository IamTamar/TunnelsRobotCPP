#pragma once
#include "Matrix.h"
#include "Vertex.h"



void Matrix::initial() {
	mat.resize(rowNum); // הגדרת מספר השורות
	for (int i = 0; i < rowNum; ++i) {
		mat[i].resize(colNum); // הגדרת מספר העמודות בכל שורה
		for (int j = 0; j < colNum; ++j) {
			mat[i][j] = nullptr;
		}
	}
	Vertex* v0 = new Vertex(0, 30, -1, 0, nullptr);
	this->changeValue(0, 0, v0);
}

void Matrix::addRowUP(int num) {
	vector<Vertex*> newV(colNum, nullptr);
	for (int i = 1; i <= num; i++)
		mat.insert(mat.begin(), newV);
	posZ += num;
	rowNum += num;
	cout << "posZ: " << posZ << "\n";
}
void Matrix::addRowDown(int num) {
	vector<Vertex*> newV(colNum, nullptr);
	for (int i = 1; i <= num; i++)
		mat.push_back(newV);
	rowNum += num;
}
void Matrix::addCulomnLeft(int num) {
	vector<Vertex*> v(num, nullptr);
	for (auto& row : mat)
		row.insert(row.begin(), v.begin(), v.end());
	colNum += num;
	posX += num;
	cout << "posX: " << posX << "\n";

}
void Matrix::addColumnRight(int num) {
	vector<Vertex*> v(num, nullptr);
	for (auto& row : mat)
		row.insert(row.end(), v.begin(), v.end());
	colNum += num;
	cout << "colNum: " << colNum << "\n";

}
int Matrix::findXIndex(int x) {
	if (x >= 0 && x + posX >= colNum)
		addColumnRight(1 + x + colNum);
	if (x<0 && abs(x)>posX)
		addCulomnLeft(abs(x) + colNum);
	x += posX;
	cout << "x: " << colNum << "posX: " << posX << "mat.size(): " << mat.size() << "\n";
	return x;
}
int Matrix::findZIndex(int z) {
	if (z >= 0 && z + posZ >= mat.size())
		addRowDown(1 + z + rowNum);
	if (z<0 && abs(z)>posZ)
		addRowUP(abs(posZ) + rowNum);
	z += posZ;
	cout << "z: " << colNum << "posZ: " << posZ << "mat.size(): " << mat.size() << "\n";
	return z;
}
//
void Matrix::changeValue(double x, double z, Vertex* p) {
	// אם אין עדיין מצביע באותו מקום, ניצור חדש
	int indZ = int(round(z)), indX = int(round(x));
	if (mat[findZIndex(indZ)][findXIndex(indX)] == nullptr) {
		mat[findZIndex(indZ)][findXIndex(indX)] = new Vertex(*p); // משכפלת את הערך של p
	}
	else {
		*(mat[findZIndex(indZ)][findXIndex(indX)]) = *p; // משנה את הערך הקיים
	}
}
Vertex* Matrix::isVisited(double x, double z) {//פונקציה המשתמשת בוקטור כיוונים
	for (int i = 0; i < 9; i++) {
		int cx = findXIndex(int(round(x)) + Xdirections[i]);
		int cz = findZIndex(int(round(z)) + Zdirections[i]);

		Vertex* p = mat[cz][cx];
		if (p != nullptr)
			return p;
	}
	return nullptr;
}
void Matrix::printMat() {//לבדוק שהתנאים טובים ולא סיבוכיות גדולה מידי!
	for (int i = rowNum - 1; i >= 0; i--) {
		cout << "\n";
		for (int j = colNum - 1; j >= 0; j--) {
			Vertex* v = mat[i][j];
			if (v != nullptr) {
				if (j == posX && i == posZ) {
					cout << "\033[1;33m**\033[0m"; // צהוב
				}
				else
				{
					cout << "\033[1;31m[]\033[0m";
				}
			}
			else
			{
				cout << "__";
			}


		}
	}
}

void Matrix::deleteMatrix() {
	for (int i = 0; i < mat.size(); i++) {
		for (int j = 0; j < colNum; j++) {
			if (mat[i][j] != nullptr) {
				delete mat[i][j];
				mat[i][j] = nullptr;
			}
		}
	}
	mat.clear();
}


//int Matrix::value(double x, double z) {
	//	int* ptr = mat[findZIndex(int(round(z)))][findXIndex(int(round(x)))];
	//	return ptr ? *ptr : -1; 
	//}