#pragma once
#include "Lidar.h"
#include "Vertex.h"
#include <algorithm>
#include <vector>
#include <math.h>
#include <Eigen/Dense>
#include <cmath>
#define PI  3.141592653589793 //pai
//#include <cmath>


double calcSlope(Point p1, Point p2) {
	double deltaX = p2.getX() - p1.getX();
	double deltaZ = p2.getZ() - p1.getZ();

	if (deltaX == 0) {
		// קו אנכי - שיפוע אינסופי
		return std::numeric_limits<double>::infinity();
	}

	return deltaZ / deltaX;
}

//m1- current m, m2- next m
// מחשבת את הזווית בין שני שיפועים במעלות, עם טיפול במקרים בעייתיים
static double angelCalc(float m1, float m2) {
	// בדיקה: אם אחד השיפועים אינסופי (קווים אנכיים)
	if (isinf(m1) && isinf(m2)) {
		return 0; // שני הקווים אנכיים → זווית ביניהם היא 0
	}
	if (isinf(m1) || isinf(m2) || 1 + m1 * m2 == 0) {
		return 90; // קו אחד אנכי והשני לא → זווית של 90°
		// בדיקה: אם המכנה מתאפס בדיוק (מ1*m2 = -1) → או זווית של 90 מעלות 
	}
	// חישוב רגיל
	double tanTheta = abs((m2 - m1) / (1 + m1 * m2));
	double radians = atan(tanTheta);
	double degrees = radians * (180.0 / PI);
	return degrees;
}



//0-right, 1-left, dir- direction of wall, inDir- direction of new wall
//דרגת סיבוב שנותרה לאחר שהוא הסתובב כבר 90 או 270
double roundDegrees(bool dir, bool inDir, double angel) {
	if (angel > 90)//לא אמור לקרות..ליתר בטחון
		angel = abs(angel - 180);
	if (inDir == 0)// right- right or left-right
		return angel;
	if (inDir == 1)//righr-left or left-left
		return -angel;
	else
		return 0;
}

static vector<double> avgs(vector<Point> right, vector <Point> left) {
	double avgRightUp, avgLeftUp, sumRightUp = 0, sumLeftUp = 0, avgRightDown, avgLeftDown, sumRightDown = 0, sumLeftDown = 0;

	for (int i = 0; i < 10; i++)
	{
		sumRightUp += right[i].getX();
		sumLeftDown += left[i].getX();//בכוונה זה הפוך כי סדר הסריקה כאן היה מלמטה למעלה
	}
	for (int i = 0; i < 10; i++)
	{
		sumRightDown += right[right.size() - 1 - i].getX();
		sumLeftUp += left[left.size() - 1 - i].getX();
	}
	avgRightUp = sumRightUp / 10;
	avgLeftUp = sumLeftUp / 10;
	avgRightDown = sumLeftUp / 10;
	avgLeftDown = sumLeftDown / 10;
	//מחזיר הפרש של נקודות עליונות ותחתונות , ממוצע ימין עליון וממוצע שמאלי תחתון
	vector<double> avgs = { abs(avgRightUp - avgRightDown), abs(avgLeftUp - avgLeftDown) , avgRightUp , avgLeftDown };

	return avgs;
}




// ממיר מעלות לרדיאנים
inline double deg2rad(double degrees) {
	return degrees * PI / 180.0;
}

// פונקציה שמבצעת סיבוב של וקטור דו-ממדי לפי זווית כוללת
Eigen::Vector2d rotateVectorByAngle(double x, double z, double theta_degrees) {//כששולחים את הנקודות לפה- זה אמור להיות הנקודה הנוכחית + לידאר
	double theta_rad = deg2rad(theta_degrees);//המרת מעלות הזוית לרדיאנים

	Eigen::Matrix2d R;//מטריצת החישובים
	R << cos(theta_rad), -sin(theta_rad),
		sin(theta_rad), cos(theta_rad);
	Eigen::Vector2d v(x, z);//יצירת וקטור הנקודה
	return R * v;//הכפלת וקטור במטריצה מביאה לנקודות הסופיות
}



