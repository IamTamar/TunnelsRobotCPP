#pragma once
#include <vector>
#include "Vertex.h"
using namespace std;
class Lidar
{
public:
	//vector <Point> UpTDownLidar;//לידאר מלמעלה בצד ימין לכיוון מטה, ממשיך בסיבוב לצד שמאל מלמטה למעלה
	//vector <Point> RightToLeftLidar;//בצד ימין לידו, ממשיך הלאה - מולו ולשמאלו עד שמשלים חצי סיבוב סביבו מקדימה
	//vector <Point> ceilingLidar;//לידאר לתקרה מלפניו, בדיקת מכשולים
public:
	vector<Point> GetWallInFront();//לשנות את הפונקציה!
	vector<Point> GetUpTDownLidar();
	vector<Point> GetRightToLeftLidar();
	vector<Point> GetUpLidar();
	float filterFirstsForSlope(vector<Point>& v);
	Point openOrDoor(vector<Point>& v, float avg);
	Point maxPoint(const vector<Point>& v);

	pair<vector<Point>, vector<Point>> wallsFilter(vector<Point>& v, float rightDist, float leftDist);
	bool wallInFront(const vector<Point>& v);
	float rightDist();
	float leftDist();
};


