#pragma once
#include <vector>
#include "Point.h"
#include "algorithm"
#include "Vertex.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>   
#include "Graph.h"
#include "YoloData.h"
#include "file.h"
#include "Matrix.h"
#include <thread>
#include <iostream>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
using namespace std;

namespace fs = std::filesystem;

YoloData::YoloData(Graph* graph, Matrix* mat)
{

    hashFunctions.push_back([this, graph, mat]() { this->addWeapon(graph, mat); });
    hashFunctions.push_back([this]() { this->isOpening(); });
    hashFunctions.push_back([this, graph, mat]() { this->addPeople(graph, mat); });
}

string getBaseFilename(const string& path) {
    filesystem::path p(path);
    return p.stem().string();  // לדוגמה: "dog" מתוך "C:\images\dog.jpg"
}

vector<int> YoloData::runYoloPrediction(const string& FilePath) {
    string pythonExe = "C:\\Users\\PC\\Desktop\\python\\env\\Scripts\\python.exe";
    string scriptPath = "C:\\Users\\PC\\Desktop\\python\\givenFromYolo.py";
    string command = pythonExe + " " + scriptPath + " " + FilePath;
    int result = system(command.c_str());

    vector<int> classIDs;
    if (result != 0) { // קוד 0 = הצלחה
        cerr << "Error running Python script!" << endl;
        return classIDs;
    }
    // יוצר את שם הקובץ מהתמונה
    string baseName = getBaseFilename(FilePath);
    string outputFile = string("C:\\Users\\PC\\Downloads\\TunnelsRobot\\src\\yoloLabels\\yolo_classes_" + baseName + ".txt");

    ifstream infile(outputFile);
    if (!infile) {
        cerr << "Could not open result file: " << outputFile << endl;
        return classIDs;
    }
    set<int> uniqueClasses;
    int cls;
    while (infile >> cls) {
        uniqueClasses.insert(cls);
    }
    classIDs.assign(uniqueClasses.begin(), uniqueClasses.end());
    return classIDs;
}
void YoloData::processDirectoryAndReadFiles(Graph* graph, Matrix* mat) {
    File f;
    string latest_file_str;
    latest_file_str = f.getLastCreatedFile("C:\\Users\\PC\\Downloads\\TunnelsRobot\\src\\yoloRuns");
    if (latest_file_str.empty()) {
        // אם getLastCreatedFile מחזירה מחרוזת ריקה, זה אומר שאין יותר קבצים לטיפול
        cout << "files not found." << std::endl;
        setonyolo(false);
    }
    while (Getonyolo()==true) {
        setonyolo(true);
        fs::path latest_file_path(latest_file_str);
        // וידוא שהקובץ עדיין קיים לפני שמנסים לקרוא אותו 
        if (!fs::exists(latest_file_path)) {
            std::cerr << "file " << latest_file_path.filename() << "unexists." << std::endl;
            continue; // מדלג לאיטרציה הבאה
        }
        if (!fs::is_regular_file(latest_file_path)) {
            std::cerr << "file: " << latest_file_path.filename() << "unregular file" << std::endl;
            continue;
        }
        // קוראים את תוכן הקובץ האחרון שנוצר
        this->classes = this->runYoloPrediction(latest_file_str);

        for (int index : this->classes) {
            this->playHash(index);
        }

        // השהייה קצרה כאן 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        latest_file_str = f.getLastCreatedFile("C:\\Users\\PC\\Downloads\\TunnelsRobot\\src\\yoloRuns");
        if (latest_file_str.empty()) {
            // אם getLastCreatedFile מחזירה מחרוזת ריקה, זה אומר שאין יותר קבצים לטיפול
            std::cout << "files not found." << std::endl;
            setonyolo(false);
        }
    }
}

bool YoloData::Getonyolo()
{
    std::lock_guard<std::mutex> lock(mtxOnYolo);  // סנכרון של גישה עם lock_guard
    return onyolo;
}
void YoloData::setonyolo( int newValue)
{
    std::lock_guard<std::mutex> lock(mtxOnYolo);  // סנכרון של גישה עם lock_guard
    onyolo = newValue;
}
void YoloData::addPeople(Graph* graph, Matrix* mat) {
    Vertex *v=  graph->addPeopleVertex();
    mat->changeValue(v);
}

void YoloData::addWeapon(Graph* graph, Matrix* mat) {
    Vertex *v=  graph->addWeaponVertex();
    mat->changeValue(v);
}

#include <stdexcept>
void YoloData::playHash(int index) {
    if (index >= 0 && index < hashFunctions.size()) {
        if (hashFunctions[index]) {
            hashFunctions[index]();
        }
        else {
            throw std::runtime_error("Error: Attempted to call an empty std::function at the specified index.");
        }
    }
    else {
        throw std::out_of_range("Error: Index out of range for hashFunctions in playHash.");
    }
}

void YoloData::isOpening() {
    setFlag(true);
}

