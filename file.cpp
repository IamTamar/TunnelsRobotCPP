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
            std::cout << latestFolder << std::endl;

        }
        catch (const std::exception& e)
        {
            std::cerr << "An error occurred: " << e.what() << std::endl;
        }
        return latestFolder;
    }


    std::string File::getLastCreatedFile(fs::path folderPath)
    {
        std::time_t latestTime = 0;
        std::string latestFile;
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
            std::cout << latestFile << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "An error occurred: " << e.what() << std::endl;
        }
        return latestFile;
    }

    //קורא מקובץ לוקטור נקודות
    vector<Point> File::fileToCoordinatesVector(const std::string& predictionFilePath) {
        ifstream file(predictionFilePath);
        vector<Point> v; 
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file: " << predictionFilePath << std::endl;
            return v; // החזרת וקטור ריק במקרה של כשל בפתיחת קובץ
        }
        string line;
        while (getline(file, line)) {
            istringstream iss(line);//יוצר שורה ןמחלץ ממנה את 3 הנתונים
            float x, y, z;

            if (!(iss >> x >> y >> z)) {
                std::cerr << "Warning: Skipping illegal line in " << predictionFilePath << ": " << line << std::endl;
                continue;
            }

            // יצירה יעילה יותר של אובייקט Point והוספתו לוקטור
            v.emplace_back(x, y, z);
        }
        return v;
    }