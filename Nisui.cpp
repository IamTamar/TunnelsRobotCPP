////#include "Main.h"
#pragma once
#include "Vertex.h"
#include <stack>
#include <vector>
#include "Matrix.h"
using namespace std;
//int main() {
//	Vertex v1 = Vertex(1, 1, 30, 30, nullptr);
//	Vertex v2 = Vertex(-2, -2, 30, 30, &v1);
//	Vertex v3 = Vertex(2, 10, 30, 0, &v1);
//	Vertex v4 = Vertex(10, 2, 30, 0, &v1);
//	Matrix mat;
//	mat.initial();
//	mat.changeValue(1, 1, &v1);
//	mat.changeValue(-2, -2, &v2);
//	mat.changeValue(2, 10, &v3);
//	mat.changeValue(10, 2, &v4);
//	mat.printMat();
//	mat.deleteMatrix();
//}
//
//
//	//std::cout << "Hello, World!" << std::endl;
//	//Vertex v1 = Vertex(1, 1, 1, Colors::WHITE);
//	//Vertex v2 = Vertex(-2, -2, 1, Colors::WHITE);
//	//Vertex v3 = Vertex(2, 1, 1, Colors::WHITE);
//	//Vertex v4 = Vertex(4, 2, 1, Colors::WHITE);
//	//Vertex v5= Vertex(-1, 1, 1, Colors::WHITE);
//	//Vertex v6 = Vertex(0, 0, 0, Colors::BLACK);
//
//
//	//std::cout << v1.vertexDirection(v2, v3) << std::endl;
//	//std::cout << v1.vertexDirection(v2, v4) << std::endl;
//	//std::cout << v1.vertexDirection(v2, v5) << std::endl;
//	//std::cout << v1.vertexDirection(v2, v6) << std::endl;
//	//std::cout << " " << std::endl;
//	//std::cout << v2.vertexDirection(v1, v3) << std::endl;
//	//std::cout << v2.vertexDirection(v1, v4) << std::endl;
//	//std::cout << v2.vertexDirection(v1, v5) << std::endl;
//	//std::cout << v2.vertexDirection(v1, v6) << std::endl;
//
//	//Vertex v7 = Vertex(-4, 3, 1, Colors::WHITE);
//	//Vertex v8 = Vertex(2, -2, 1, Colors::WHITE);
//	//Vertex v9 = Vertex(1, 1, 1, Colors::WHITE);
//	//Vertex v10 = Vertex(-2, 2, 1, Colors::WHITE);
//	//Vertex v11= Vertex(1, -2, 1, Colors::WHITE);
//	//Vertex v12= Vertex(0, -0.33333,0, Colors::BLACK);
//
//	//std::cout << v7.vertexDirection(v8, v9) << std::endl;
//	//std::cout << v7.vertexDirection(v8, v10) << std::endl;
//	//std::cout << v7.vertexDirection(v8, v11) << std::endl;
//	//std::cout << v7.vertexDirection(v8, v12) << std::endl;
//	//std::cout << " " << std::endl;
//	//std::cout << v8.vertexDirection(v7, v9) << std::endl;
//	//std::cout << v8.vertexDirection(v7, v10) << std::endl;
//	//std::cout << v8.vertexDirection(v7, v11) << std::endl;
//	//std::cout << v8.vertexDirection(v7, v12) << std::endl;
//}
// 