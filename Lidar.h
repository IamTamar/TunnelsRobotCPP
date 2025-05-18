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
	double filterFirstsForSlope(vector<Point> v);
	Point openOrDoor(vector<Point> v, double avg);
	Point maxPoint(vector<Point> v);
	

	pair<vector<Point>, vector<Point>> wallsFilter(vector<Point> v, double rightDist, double leftDist);
	vector<double> avgs(vector<Point*> vects);
	bool wallInFront(vector<Point> v);
	double rightDist();
	double leftDist();
};


