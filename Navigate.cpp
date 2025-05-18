#pragma once
#include "Lidar.h"
#include "Vertex.h"
#include <algorithm>
#include <vector>
#include <math.h>
#define PI  3.141592653589793 //pai
//#include <cmath>




//0-right, 1-left, dir- direction of wall, inDir- direction of new wall
//דרגת סיבוב שנותרה לאחר שהוא הסתובב כבר 90 או 270
//double roundDegrees(bool dir, bool inDir, double angel) {
//	if (angel > 90)//לא אמור לקרות..ליתר בטחון
//		angel = abs(angel - 180);
//	if (inDir == 0)// right- right or left-right
//		return angel;
//	if (inDir == 1)//righr-left or left-left
//		return -angel;
//	else
//		return 0;
//}






void turnTo(double angel) {
	//
}

void goTo(double x, double y, double z) {
	//	
}
void goTo(double dist) {
	//	
}
//direction: 0-right. 1-left. 2-ahead.
void moveToOtherRoom(double dist, Point to, double angel, int direction) {
	//פנה כבר -90
	goTo(dist);
	turnTo(-90);
	goTo(to.getX(), to.getY(), to.getZ());//לרדת ל-Y המתאים
	turnTo(90);//מכיוון ראש למטה, לכיוון ראש ישר
	goTo(5);//its only an example, i need to know the wall depth   ?????
	turnTo(90);//אין צורך לסכום את מס' הזוויות כי זה על ציר הY
}
