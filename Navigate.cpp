#pragma once
#include "Lidar.h"
#include "Vertex.h"
#include <algorithm>
#include <vector>
#include <math.h>
#define PI  3.141592653589793 //pai
//#include <cmath>


static void turnTo(float angel) {
	//
}

static void goTo(float x, float y, float z) {
	//	
}
static void goTo(float dist) {
	//	
}
//direction: 0-right. 1-left. 2-ahead.
static void moveToOtherRoom(float dist, Point to, float angel, int direction) {
	//פנה כבר -90
	goTo(dist);
	turnTo(-90);
	goTo(to.getX(), to.getY(), to.getZ());//לרדת ל-Y המתאים
	turnTo(90);//מכיוון ראש למטה, לכיוון ראש ישר
	goTo(5);//its only an example, i need to know the wall depth   ?????
	turnTo(90);//אין צורך לסכום את מס' הזוויות כי זה על ציר הY
}
