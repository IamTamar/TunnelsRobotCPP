#pragma once
#include "Matrix.h"
#include "Graph.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <mutex>
using namespace std;
class YoloData {
public:
	vector<function<void()>> hashFunctions;
	vector<int> classes;// מערך הקלאסים המתקבלים מהמודל על כל חיזוי
	bool flag = false;
	int onyolo = 0;
	std::mutex mtxOnYolo;      // המוטקס שמגן על הגישה למשתנה
	mutex mtxFlag;



public:
	YoloData(Graph* graph, Matrix* mat);
	vector<int> runYoloPrediction(const string& FilePath);
	//bool hasClassInPrediction(const string& predictionFilePath, int targetClassId);
	void processDirectoryAndReadFiles(Graph* graph, Matrix* mat);
	int Getonyolo();
	void setonyolo(int newValue);
	void addPeople(Graph* graph, Matrix* mat);
	void addWeapon(Graph* graph, Matrix* mat);	
	void isOpening();
	void playHash(int index);
	bool getFlag() { std::lock_guard<std::mutex> lock(mtxFlag); return flag; }
	void setFlag(bool newValue) { std::lock_guard<std::mutex> lock(mtxFlag); flag = newValue; }
	void runYolo();
	};