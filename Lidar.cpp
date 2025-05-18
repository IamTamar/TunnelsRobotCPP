#pragma once
#include "Lidar.h"
#include "Vertex.h"
#include <algorithm>
#include <vector>
#define PI  3.141592653589793 //pai
#include <cmath>
#include "file.h"

using namespace std;

vector<Point> GetUpTDownLidar() {
	double angel = 180;

	//lidar performance:
	// up to down on right, and keeps going to buttom and down to up in left
	// writing into file
	//reading and moving into vector<vertex>
	vector <Point> v;
	return v;
}
vector<Point> GetRightToLeftLidar() {
	//lidar performance:
	//  right near robot, and keeps going in front and left until near robot
	// writing into file
	//reading and moving into vector<vertex>
	vector <Point> v;
	return v;
}
vector <Point> GetWallInFront() {

}
vector <Point> GetUpLidar() {
	//looks up
	vector <Point> v;
	return v;
}
double rightDist() { //for distances between walls
	//....
	return 0;
}
double leftDist() {
	//....
	return 0;
}

double filterFirstsForSlope(vector<Point> v) {
	int i = 0; 
	double sumX = 0, sumZ = 0, sumMulXZ = 0, sumXSquare = 0;
	for (Point p : v) {
		i++;
		sumX += p.getX();
		sumZ += p.getZ();
		sumMulXZ += (p.getX() * p.getZ());
		sumXSquare += sqrt(p.getX());
		if (i > 4)
			break;
	}
	double m = (20 * sumMulXZ - sumX * sumZ) / (20 * sumXSquare - sqrt(sumX));
	return m;
}

//function that filters only walls points and then sorts up to down(y axe),
// and returns differences between up-down on right & left
//יש צורך לדעת מה המרחק מהקירות ב2 הצדדים לשם כך!!! חשוב לוודא שיש לי
pair<vector<Point>, vector<Point>> wallsFilter(vector<Point> v, double rightDist, double leftDist) {
	vector<Point> right = {}, left = {};
	for (Point p : v)
	{
		if (p.getX() > 0 && p.getX() >= rightDist - 5)
			right.push_back(p);
		if (p.getX() < 0 && p.getX() >= (leftDist - 5) * (-1))
			left.push_back(p);
	}
	//אין צורך במיון כי הלידאר מחזיר נתונים לפי סדר הסריקה!! אז בצד שמאל צריך רק לבדוק הפוך
	return { right, left };
	//כשנזמן את הפונקציה הזו נבדוק בכל צד האם ההפרשים מידי גדולים, אם כן - הפעלת מצלמה
}

Point maxPoint(vector<Point> v) {
	Point p ;
	p.x = 0;
	p.y = 0;
	p.z = 0;
	for (Point point : v) 
	{
		if (point.getY() > p.getY()) {
			p.changeX(point.getY());
			p.changeY(point.getY());
			p.changeZ(point.getZ());
		}
	}
	return p;
}

//z=mx+b
//מסנן את הקיר הקודם שהייתי בו כדי שאדע מה הקיר החדש 
//ומחזיר את השיפוע של הקיר החדש
double prevWallFilter(Point curr, double gradient, vector<Point> v) {
	double b = curr.getZ() - (curr.getX() * gradient);
	double sumX = 0, sumZ = 0, sumMulXZ = 0, sumXSquare = 0, m;
	vector<Point> newV;
	int i = 0;
	for (Point p : v) {
		if (curr.getZ() != (curr.getX() * gradient + b))//לא על הישר הקודם
		{
			newV.push_back(p);
			i++;
			sumX += curr.getX();
			sumZ += curr.getZ();
			sumMulXZ += (curr.getX() * curr.getZ());
			sumXSquare += sqrt(curr.getX());
		}
		if (i == 9)//9 is only example..
			break;
	}
	// linear regression - B^ :
	// https://he.wikipedia.org/wiki/%D7%A8%D7%92%D7%A8%D7%A1%D7%99%D7%94_%D7%9C%D7%99%D7%A0%D7%99%D7%90%D7%A8%D7%99%D7%AA_%D7%A4%D7%A9%D7%95%D7%98%D7%94
	m = (20 * sumMulXZ - sumX * sumZ) / (20 * sumXSquare - sqrt(sumX));
	return m;
}
// avgs function is in calculates!!
bool wallInFront(vector<Point> v) {
	vector<Point> front = {};
	for (Point p : v)
	{
		if (p.getX() >= -20 && p.getX() <= 20)
			front.push_back(p);
	}

	sort(front.begin(), front.end(), [](Point a, Point b) {
		return a.getZ() > b.getZ();
		});
	//בינתיים ממוצע פשוט אבל לבדוק! 
	//לא צריך כי אני מתבססת שהנקודות נכונות ועושה ממוצע אם הקיר עקום מעט
	double sum = 0, avg, median;
	for (Point p : front) {
		sum += p.getZ();
	}
	avg = sum / front.size();
	median = front[(front.size() / 2)].getZ();
	//אולי ממוצע של החציון והממוצע?
	if (avg <= 40)
		return true;
	return false;
}
Point openOrDoor(vector<Point> v, double avg) {
	double prevX = v.at(0).getX();
	int i = 1;

	while (1) {
		if (abs(v.at(i).getX() - avg) > 20)
			break;
		prevX = v.at(i).getX();
	}
	return v.at(i - 1);//צריך לחשב את המרחק מהנקודה + המרחק שאני רוצה מקיר בפונקציה הראשית
}

