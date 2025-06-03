#pragma once
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <string>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <cstdlib>
#include <stdbool.h>
#include <locale>
#include <codecvt>
#include "File.h"
#include "Graph.h"
#include "Vertex.h"
#include "Stops.cpp"
#include "Matrix.h"
#include "YoloData.h"

using namespace std;
 namespace fs = filesystem;


File::File() {}
    string File::getLastCreatedFolder()
    {
        fs::path folderPath("C:\\Users\\PC\\Desktop\\python\\runs\\detect");
        time_t latestTime = 0;
        string latestFolder;
        try
        {
            for (const auto& entry : fs::directory_iterator(folderPath))//path= "C:\Users\PC\Desktop\python\runs\detect"+latestFolder+"labels"
            {
                if (fs::is_directory(entry)) {
                    time_t lastWriteTime = fs::last_write_time(entry).time_since_epoch().count();
                    if (lastWriteTime > latestTime) {
                        latestTime = lastWriteTime;
                        latestFolder = entry.path().string();
                    }
                }
            }
             cout << latestFolder <<  endl;

        }
        catch (const  exception& e)
        {
             cerr << "An error occurred: " << e.what() <<  endl;
        }
        return latestFolder;
    }


     string File::getLastCreatedFile(fs::path folderPath)
    {
         time_t latestTime = 0;
         string latestFile;
        try
        {
            for (const auto& entry : fs::directory_iterator(folderPath))
            {
                if (fs::is_regular_file(entry)) // רק קבצים, לא תיקיות
                {
                    time_t lastWriteTime = fs::last_write_time(entry).time_since_epoch().count();
                    if (lastWriteTime > latestTime) {
                        latestTime = lastWriteTime;
                        latestFile = entry.path().string();
                    }
                }
            }
             cout << latestFile <<  endl;
        }
        catch (const  exception& e)
        {
             cerr << "An error occurred: " << e.what() <<  endl;
        }
        return latestFile;
    }

    //קורא מקובץ לוקטור נקודות
    vector<Point> File::fileToCoordinatesVector(const string& predictionFilePath) {
        ifstream file(predictionFilePath);
        vector<Point> v; 
        if (!file.is_open()) {
            cerr << "Error: Unable to open file: " << predictionFilePath <<  endl;
            return v; // החזרת וקטור ריק במקרה של כשל בפתיחת קובץ
        }
        string line;
        while (getline(file, line)) {
            istringstream iss(line);//יוצר שורה ןמחלץ ממנה את 3 הנתונים
            float x, y, z;

            if (!(iss >> x >> y >> z)) {
                 cerr << "Warning: Skipping illegal line in " << predictionFilePath << ": " << line <<  endl;
                continue;
            }

            // יצירה יעילה יותר של אובייקט Point והוספתו לוקטור
            v.emplace_back(x, y, z);
        }
        return v;
    }