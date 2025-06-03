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

using namespace std;
static float calcSlope(Point p1, Point p2) {
	float deltaX = p2.getX() - p1.getX();
	float deltaZ = p2.getZ() - p1.getZ();

	if (deltaX == 0) {
		// קו אנכי - שיפוע אינסופי
		return std::numeric_limits<float>::infinity();
	}
	cout << "slope: " << deltaZ / deltaX << endl;
	return deltaZ / deltaX;
}

// m2- next m
// מחשבת את הזווית בין שני שיפועים במעלות, עם טיפול במקרים בעייתיים
static float angelCalc(float mainAngel, float m2) {
	float angelInRad = mainAngel * (PI / 180.0); 	// המרת זווית הרובוט הנוכחית לרדיאנים
	//  קביעת זווית היעד (הכיוון שאליו הרובוט צריך לפנות) ברדיאנים
	float newAngel;

	if (isinf(m2))
		newAngel = PI / 2.0; // 90 מעלות (אנכי)
	else
		newAngel = atan(m2); // זווית מה-atan

	float rot_rad = newAngel - angelInRad;	// חישוב ההפרש בין זוויות היעד וההתחלה
	cout << "angel: " << rot_rad * (180.0 / PI) << endl;
	return (rot_rad * (180.0 / PI)); 		// המרה למעלות
}

//0-right, 1-left, dir- direction of wall, inDir- direction of new wall
//דרגת סיבוב שנותרה לאחר שהוא הסתובב כבר 90 או 270
static float roundDegrees(bool dir, bool inDir, float angel) {
	if (angel > 90)//לא אמור לקרות..ליתר בטחון
		angel = abs(angel - 180);
	if (inDir == 0)// right- right or left-right
		return angel;
	if (inDir == 1)//righr-left or left-left
		return -angel;
	else
		return 0;
}

static std::vector<float> avgs(const std::vector<Point>& right, const std::vector<Point>& left){
float avgRightUp, avgLeftUp, sumRightUp = 0, sumLeftUp = 0, avgRightDown, avgLeftDown, sumRightDown = 0, sumLeftDown = 0;
	if (right.size() < 10 || left.size() < 10) {
		throw invalid_argument("Both 'right' and 'left' vectors must contain at least 10 points.");
	}
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
	avgRightDown = sumRightDown / 10;
	avgLeftDown = sumLeftDown / 10;
	//מחזיר הפרש של נקודות עליונות ותחתונות , ממוצע ימין עליון וממוצע שמאלי תחתון
	vector<float> avgs = { abs(avgRightUp - avgRightDown), abs(avgLeftUp - avgLeftDown) , avgRightUp , avgLeftDown };

	return avgs;
}

// ממיר מעלות לרדיאנים
inline float deg2rad(float degrees) {
	return degrees * PI / 180.0;
}

// פונקציה שמבצעת סיבוב של וקטור דו-ממדי לפי זווית כוללת
static Eigen::Vector2d rotateVectorByAngle(float x, float z, float theta_degrees) {//כששולחים את הנקודות לפה- זה אמור להיות הנקודת לידאר
	float theta_rad = deg2rad(theta_degrees);//המרת מעלות הזוית לרדיאנים
	Eigen::Matrix2d R;//מטריצת החישובים
	Eigen::Vector2d v(x, z);//יצירת וקטור הנקודה

	if (theta_degrees >= 0) {
		R << cos(theta_rad), sin(theta_rad),
			-sin(theta_rad), cos(theta_rad);
	}
	/*else
	{
		R << cos(theta_rad), -sin(theta_rad),
			sin(theta_rad), cos(theta_rad);
	}*/

	return R * v;//הכפלת וקטור במטריצה מביאה לנקודות הסופיות
}



