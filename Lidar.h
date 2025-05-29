#pragma once
#include "Vertex.h"
class Lidar
{
public:
	vector<Point> UpTDownLidar;//לידאר מלמעלה בצד ימין לכיוון מטה, ממשיך בסיבוב לצד שמאל מלמטה למעלה
	vector<Point> RightToLeftLidar;//בצד ימין לידו, ממשיך הלאה - מולו ולשמאלו עד שמשלים חצי סיבוב סביבו מקדימה
	vector <Point> ceilingLidar;//לידאר לתקרה מלפניו, בדיקת מכשולים

public:
	vector<Point> GetUpTDownLidar();
	vector<Point> GetRightToLeftLidar();
	vector <Point> GetUpLidar();
	float filterFirstsForSlope(vector<Point> v);
	Point openOrDoor(vector<Point> v, float avg);
	Point maxPoint(vector<Point> v);
	vector <Point> GetWallInFront();//לשנות את הפונקציה!
	pair<vector<Point>, vector<Point>> wallsFilter(vector<Point> v, float rightDist, float leftDist);
	bool wallInFront(vector<Point> v);
	float rightDist();
	float leftDist();
	float prevWallFilter(Point curr, float gradient, vector<Point> v);

};


