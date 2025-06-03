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
using namespace std;
#include <iostream>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
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


    // הצגת הפקודה
    cout << "הפקודה שהורכבה: " << command << endl;
    int result = system(command.c_str());
    this_thread::sleep_for(chrono::milliseconds(50)); // כדי שיספיק לכתוב לקובץ

    vector<int> classIDs;
    if (result != 0) {//קוד 0- הצלחה
        cerr << "Error running Python script!" << endl;
        return classIDs;
    }
    // יוצר את שם הקובץ מהתמונה
    string baseName = getBaseFilename(FilePath);
    string outputFile = "yolo_classes_" + baseName + ".txt";
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
    // מעתיק את הקלאסים הייחודיים לתוך וקטור
    classIDs.assign(uniqueClasses.begin(), uniqueClasses.end());
    return classIDs;
}
//bool YoloData::hasClassInPrediction(const string& predictionFilePath, int targetClassId) {
//    File f;
//    ifstream file(predictionFilePath);
//    if (!file.is_open()) {
//        cerr << "error: " << predictionFilePath << endl;
//        return false;
//    }
//    string line;
//    while (getline(file, line)) {
//        istringstream iss(line);
//        int classId;
//        float  x, y, w, h;
//
//        if (!(iss >> classId  >> x >> y >> w >> h)) {
//            cerr << "ilegal line: " << line << endl;
//            continue;
//        }
//
//        // הדפסה לבדיקה
//        std::cout << "class: " << classId << 
//             ", center: (" << x << ", " << y << "), size: (" << w << ", " << h << ")\n";
//
//        if (classId == targetClassId /* && conf >= 0.5 */) {
//            return true; // זיהוי נמצא
//        }
//    }
//
//    return false; // לא נמצא
//}

void YoloData::runYolo()
{
    std::wstring batchFilePath = L"C:\\Users\\PC\\Downloads\\yolo\\my_yolov8_model2\\content\\runs\\detect\\my_yolov8_model2\\CMD.bat.txt";

    // Build the command to run the batch file
    std::wstring fullCommand = L"\"";
    fullCommand += batchFilePath;
    fullCommand += L"\"";

    // Execute the command to run the batch file
    _wsystem(fullCommand.c_str());
}

void YoloData::processDirectoryAndReadFiles(Graph* graph, Matrix* mat) {
    File f;
    
    /*fs::path latest_run_folder_path(f.getLastCreatedFolder());
    fs::path target_labels_path = latest_run_folder_path / "labels";*/
    string latest_file_str;
    setonyolo(1);
    while (Getonyolo()==1) 
    {

        latest_file_str = f.getLastCreatedFile("C:\\Users\\PC\\Downloads\\TunnelsRobot\\yoloRuns");
        if (latest_file_str.empty()) {
            // אם getLastCreatedFile מחזירה מחרוזת ריקה, זה אומר שאין יותר קבצים לטיפול
            std::cout << "לא נמצאו קבצים נוספים לטיפול. סיום טיפול." << std::endl;
            break; // יציאה מהלולאה
        }
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
        setFlag(false);
       
        this->classes = this->runYoloPrediction(latest_file_str);
        string baseName = getBaseFilename(latest_file_str);
        string output_text_file = "yolo_classes_" + baseName + ".txt";
        for (int index : this->classes) {
            this->playHash(index);
        }
        //  מחיקת הקובץ לאחר הטיפול בו 
        try {
            fs::remove(latest_file_path);
            fs::remove(output_text_file);

        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "fatal error - file erasing " << latest_file_path << ": " << e.what() << std::endl;
            break;
        }
        // השהייה קצרה כאן 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
int YoloData::Getonyolo()
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

void YoloData::playHash(int index) {
    if (index >= 0 && hashFunctions.size())
        hashFunctions[index]();
    else
        cerr << "Error: Index out of range for hushFunctions" << endl;
}

void YoloData::isOpening() {
    setFlag(true);
}

