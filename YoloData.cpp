#pragma once
#include <vector>
#include "Point.h"
#include "algorithm"
#include "Vertex.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>   
//#include <cstdlib>
using namespace std;

//int main() {
//    string scriptPath = "C:\\Users\\PC\\Desktop\\python\\givenFromYolo.py";
//    string imagePath = "C:\\Users\\PC\\Desktop\\python\\datasets\\images\\train\\8.png";
//    string command = "python \"" + scriptPath + "\" \"" + imagePath + "\"";
//    system(command.c_str());
//
//
//    ifstream in("result.txt");
//    int cls;
//    float conf, x, y, w, h;
//    while (in >> cls >> conf >> x >> y >> w >> h) {
//        cout << "category: " << cls << ", confidence: " << conf
//            << ", center: (" << x << "," << y << "), size: (" << w << "," << h << ")\n";
//    }
//
//    return 0;
//}


bool hasClassInPrediction(const string& predictionFilePath, int targetClassId) {
    ifstream file(predictionFilePath);
    if (!file.is_open()) {
        cerr << "error: " << predictionFilePath << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        int classId;
        float  x, y, w, h;

        if (!(iss >> classId  >> x >> y >> w >> h)) {
            cerr << "ilegal line: " << line << endl;
            continue;
        }

        // הדפסה לבדיקה
        std::cout << "class: " << classId << 
             ", center: (" << x << ", " << y << "), size: (" << w << ", " << h << ")\n";

        if (classId == targetClassId /* && conf >= 0.5 */) {
            return true; // זיהוי נמצא
        }
    }

    return false; // לא נמצא
}


void runYoloPrediction() {
    string imagePath = "aa.mp4";//לשנות את הפונקציה שתעבור כל פעם על תמונה בתקיה מסויימת
    string command = "python C:\\Users\\PC\\Desktop\\python\\givenFromYolo.py \"" + imagePath + "\"";
    int result = system(command.c_str());

    if (result != 0) {
        cerr << "Error running Python script!" << endl;
    }
    else {
        cout << "Python script ran successfully!" << endl;
    }
}

string getPredictionFilePath() {
    ifstream pathFile("result_path.txt");
    string path;
    if (getline(pathFile, path)) {
        return path;
    }
    cerr << "could not read result_path.txt" << endl;
    return "";
}


//int main() {
//    string imagePath = "C:\\Users\\PC\\Desktop\\python\\datasets\\images\\test\\91.jpeg";
//    runYoloPrediction(imagePath);
//
//    string txtPath = getPredictionFilePath();
//    if (txtPath.empty()) return 1;
//
//    int targetClass = 0;
//    if (hasClassInPrediction(txtPath, targetClass)) {
//        cout << "detected!!!" << endl;
//    }
//    else {
//        cout << "not detected" << endl;
//    }
//
//    return 0;
//}


//int main() {
//    string labelFilePath = "runs/detect/predict/labels/8.txt";  // או נתיב אחר בהתאם לתמונה שלך
//    int targetClass = 1;
//    bool found = false;
//
//    ifstream file(labelFilePath);
//    if (!file.is_open()) {
//        cerr << "it can't open: " << labelFilePath << endl;
//        return 1;
//    }
//
//    string line;
//    while (getline(file, line)) {
//        if (!line.empty()) {
//            int classId;
//            istringstream iss(line);
//            iss >> classId;
//
//            if (classId == targetClass) {
//                found = true;
//                break;
//            }
//        }
//    }
//
//    file.close();
//
//    if (found)
//        std::cout << "הקטגוריה נמצאה בתמונה." << std::endl;
//    else
//        std::cout << "לא נמצאה קטגוריה מתאימה." << std::endl;
//
//    return 0;
//}

//int main() {
//    string path = "path/to/runs/detect/predict/labels/35.txt";
//    auto detections = loadDetections(path);
//
//    for (const auto& d : detections) {
//        std::cout << "Class: " << d.cls << ", Confidence: " << d.confidence << std::endl;
//        std::cout << "BBox center: (" << d.x_center << ", " << d.y_center << "), size: (" << d.width << ", " << d.height << ")\n";
//    }
//
//    return 0;
//}

//מציאת הנקודה שממנה מתחיל הפתח או הדלת


Point peopleOrWeapon(Vertex prev, Vertex next) {
    double x = prev.getPoint().getX() - next.getPoint().getX();
    double y = prev.getPoint().getY() - next.getPoint().getY();
    double z = prev.getPoint().getZ() - next.getPoint().getZ();
    return Point(x, y, z);
}

//{ 0: 'weapon', 1 : 'openings', 2 : 'people' }