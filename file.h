#pragma once
#include <iostream>
#include <string.h>
#include <vector>
#include "Point.h"
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
namespace fs = filesystem;

class File
{
public:
	File();
	string getLastCreatedFolder();
	std::string getLastCreatedFile(fs::path folderPath);
	vector<Point> fileToCoordinatesVector(const std::string& predictionFilePath);
};

