#pragma once
#include "Matrix.h"
#include "Vertex.h"
#include "Stops.cpp"



void Matrix::initial() {
	mat.resize(rowNum); // הגדרת מספר השורות
	for (int i = 0; i < rowNum; ++i) {
		mat[i].resize(colNum); // הגדרת מספר העמודות בכל שורה
		for (int j = 0; j < colNum; ++j) {
			mat[i][j] = nullptr;
		}
	}
	Vertex* v0 = new Vertex(0, 30, -1, 0, nullptr);
	this->changeValue(v0);
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

void Matrix::changeValue( Vertex* p) 
{
	// אם אין עדיין מצביע באותו מקום, ניצור חדש
	int indZ = int(round(p->getPoint().getZ())), indX = int(round(p->getPoint().getX()));

	if (getMat(indZ, indX))
	{
		setMat(indZ,indX,new Vertex(p)); // משכפלת את הערך של p
	}
	else 
	{
		setMat(indZ, indX, p); // משנה את הערך הקיים
	}
}
float Matrix::isVisited(float x, float z) {//פונקציה המשתמשת בוקטור כיוונים
	for (int i = 0; i < 9; i++) {
		int cx = findXIndex(int(round(x)) + Xdirections[i]);
		int cz = findZIndex(int(round(z)) + Zdirections[i]);

		Vertex* p = mat[cz][cx];
		if (p != nullptr && p->getStop() != Stops::KIDNAPPED && p->getStop() != Stops::TERRORIST && p->getStop() != Stops::EXPLOSIVES)
			return p->getAngel();
	}
	return -1;
}

void Matrix::printMat() {
	for (int i = rowNum - 1; i >= 0; i--) {
		cout << "\n";
		for (int j = colNum - 1; j >= 0; j--) {
			Vertex* v = mat[i][j];
			if (v != nullptr) {
				if (j == posX && i == posZ) {
					cout << "\033[1;31m**\033[0m"; // RED
				}
				if(v->getStop()== Stops::KIDNAPPED || v->getStop() == Stops::TERRORIST)
				{
					cout << "\033[1;32m[]\033[0m";// GREEN
				}
				if(v->getStop()== Stops::EXPLOSIVES)
				{
					cout << "\033[1;34m[]\033[0m";// BLUE
				}
				if(v->getStop()== Stops::LEFT || v->getStop() == Stops::RIGHT)
				{
					cout << "\033[1;33m[]\033[0m";// YELLOW	
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

void Matrix::addPeopleVertex(Vertex *p) {
	this->changeValue(p);
}

void Matrix::addWeaponVertex(Vertex *p) {
	this->changeValue(p);
}

