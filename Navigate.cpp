#pragma once
#include "Lidar.h"
#include "Vertex.h"
#include <algorithm>
#include <vector>
#include <math.h>
#include "AStar.cpp"
#define PI  3.141592653589793 //pai
//#include <cmath>


static void turnTo(float angel) {
	//
}

static void goTo(float x, float y, float z) {
	int grid[ROW][COL]
		        = { { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
		            { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
		            { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
		            { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
		            { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
		            { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
		            { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
		            { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
		            { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 } };
		 	Pair src = make_pair(0, 0);
	Pair dest = make_pair(x, z);
    aStarSearch(grid, src, dest);

}
static void goTo(float dist) {
	//	
}
//direction: 0-right. 1-left. 2-ahead.
static void moveToOtherRoom(float dist, float x, float y,float z , float angel, int direction) {
	//פנה כבר -90
	goTo(dist);
	turnTo(-90);
	goTo(x, y, z);//לרדת ל-Y המתאים
	turnTo(90);//מכיוון ראש למטה, לכיוון ראש ישר
	goTo(5);//its only an example, i need to know the wall depth   ?????
	turnTo(90);//אין צורך לסכום את מס' הזוויות כי זה על ציר הY
}
