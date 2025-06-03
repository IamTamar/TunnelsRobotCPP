#pragma once
#include "Vertex.h"
#include "Lidar.h"
#include <stack>
#include <thread> 
#include <chrono>
#include <vector>
#include "YoloData.h"
#include <iostream>
#include "Calculates.cpp"
#include "Graph.h"
#include "Navigate.cpp"
#include "GasSensors.h"
#include "Matrix.h"
#include <Eigen/Dense>
#include "SoundSensors.h"

using namespace std;
void print(const std::string& myString) {
	std::cout << myString << std::endl; // מדפיס את המחרוזת ושורה חדשה
}
void move() {
	Graph graph;
	Matrix mat;
	Lidar lidar;
	stack <Vertex> turnsStack;
	float mainAngel = 90, currnetAngel = 90;
	bool turnflag = 0;// דגל בשביל הקיר ממול
	bool flag = true;//true- right side, false - left side
	int counter;//num of turns that i changed directions
	GasSensors gasSensors = GasSensors();
	SoundSensors soundSensors = SoundSensors();
	vector<Point> lidarPointRL, lidarPointUD, rightSide, leftSide, UpSide;
	pair<vector<Point>, vector<Point>> walls;
	vector <float> avgsOfWalls;
	Point p, door;
	float m0 = 0, m1, m2, angel, rightDist, leftDist;
	Vertex* curr, * newCurr, * v, *last;
	Eigen::Vector2d r;
	File f;
    YoloData yd(&graph, &mat);

	graph.initial();
	mat.initial();
	mat.printMat();

	//הפעלת יולו כתהליכון
	thread yoloThread(&YoloData::processDirectoryAndReadFiles, &yd, &graph, &mat);
	//יצירת קודקוד חדש ואיתחול הגרף והמטריצה- רק בתחילת התוכנית
    curr = new Vertex(0, 30, 0, 0, graph.getFirst());//יצירת הקודקוד הראשון
	mat.changeValue( curr);//מוסיף למטריצה את הקודקטד הראשון
	graph.addVertex(curr); //הוספה לגרף
	rightDist = lidar.rightDist();//מרחקים מהקירות
	leftDist = lidar.leftDist();
	last = new Vertex((-leftDist) , 30, 0, 0, nullptr);
	mat.changeValue(last);

	//curr.printVertex();

	while (curr->isSimilliar(last))
	{
		//חישוב שיפוע של הקיר
		lidarPointRL = lidar.GetRightToLeftLidar(); //סריקה מימין לשמאל
		m1 = lidar.filterFirstsForSlope(lidarPointRL); //חישוב שיפוע עכשוי של הקיר
		angel = angelCalc(mainAngel, m1);// חישוב זווית של הקיר בין הקיר הקודם לעכשיו
		if (!turnflag && angel < 0) {  //רק במקרה הזה פונה לשמאל, ורק כעת צריך להוסיף את השיפוע לזוית הראשית. כי בפתח רגיל מוסיף שם
			angel += 180; 	
			mainAngel += angel;
		} 
		//m0 = m1;
		turnTo( angel); //מסתובב לפי שיפוע הקיר
		rightDist = lidar.rightDist();//מרחקים מהקירות
		leftDist = lidar.leftDist();
		while (lidar.wallInFront(lidarPointRL) == false)
		{
			currnetAngel = flag == true ? -90 : 90; 
			turnflag = 1;
			lidarPointRL = lidar.GetRightToLeftLidar();
			walls = lidar.wallsFilter(lidarPointRL, rightDist, leftDist);//מסנן את הקירות בלבד
			rightSide = walls.first;
			leftSide = walls.second;
			avgsOfWalls = avgs(rightSide, leftSide);

			//אם זה פתח
			gasSensors.getLeftLevels();
			gasSensors.getRightLevels();
			soundSensors.getRightLevels();
			soundSensors.getLeftLevels();
			if (avgsOfWalls[0] > 30)  //אם העומקים שונים ביותר מ30 ס"מ כנראה יש חריגה
			{
				Point point  = lidar.openOrDoor(lidarPointUD, avgsOfWalls[0]);//הגעה לנקודה שממנה מתחיל כיוון חדש
				r = rotateVectorByAngle(0, point.getZ(), mainAngel);//הוספת 2 ס"מ כדי שיהיה בתחילת הפתח / דלת ולא לפני
				curr = new Vertex(curr->getPoint().getX() + r(0), curr->getPoint().getY(), curr->getPoint().getZ() + r(1) + 2, curr->getAngel(), curr);
				curr = graph.addVertex(curr);
				mat.changeValue(curr);
				goTo(point.getZ() + 2);//הליכה בפועל, לנקודה שבה הממוצעים משתנים
				
				lidarPointUD = lidar.GetUpTDownLidar();
				walls = lidar.wallsFilter(lidarPointUD, rightDist, leftDist);//מסנן את הקירות בלבד
				rightSide = walls.first;
				leftSide = walls.second;
				avgsOfWalls = avgs(rightSide, leftSide);
				curr->setAngel(currnetAngel);//מסתובב
				turnTo(currnetAngel);
				mainAngel += (currnetAngel);

				if (avgsOfWalls[0] > 30)
				{//אם זו דלת ולא רק פתח!!
					if (yd.getFlag() == true)//1-דלת
					{
						Point p= lidar.openOrDoor(lidarPointUD, avgsOfWalls[0]);
						r = rotateVectorByAngle(rightDist + p.getX(), 0, mainAngel);//להוסיף לX את רוחב הקיר שהלך בו באמצע המעבר לחדר השני
						newCurr = new Vertex(curr->getPoint().getX() + r(0), p.getY(), curr->getPoint().getZ() + r(1), -90, curr);

						if (mat.isVisited(newCurr->getPoint().getX(), newCurr->getPoint().getZ()) == -1)//לא ביקר שם עדיין
						{
							newCurr = graph.addVertex(newCurr);
							moveToOtherRoom(rightDist, p.getX(), p.getY(), p.getZ(), -90, 0);//ההליכה בפועל
							UpSide = lidar.GetUpLidar();
							//האם להשאיר את הפונקציה הזו? או לעשות משהו יותר חכם
							Point up = lidar.maxPoint(UpSide);//הנקודה המקסימלית בתקרה
							goTo(up.getX(), up.getY(), up.getZ());
							mat.changeValue(newCurr);
							mainAngel += -90;//הרובוט מסתובב בדיוק לכיוון ההפוך- סה"כ הסתובב מהתחלת זיהוי הדלת: -180
							curr = newCurr;
							break;
						}
						else {
							mainAngel -= (currnetAngel);//להחזיר את הסיבוב שנעשה
							curr->setAngel(-currnetAngel);//מסתובב
							turnTo(-currnetAngel);
							goTo(10);//ילך עוד 10 ס"מ ויעקוף כנראה את הדלת
						}
					}
					else
					{
					
						//אם זה רק פתח/סיבוב רגיל
						goTo(rightDist); //הולך ממש לפתח עצמו לאחר שהסתובב לכיוונו -90 וסורק
						lidarPointRL = lidar.GetRightToLeftLidar();
						m2 = lidar.filterFirstsForSlope(lidarPointRL);//השיפוע החדש
						angel = angelCalc(mainAngel, m2);// חישוב זווית של הקיר בין הקיר הקודם לעכשיו
						if ( angel > 0)// בדיקה שלא יצאה זוית חיובית
							angel -= 180;
						turnTo(angel -currnetAngel);//מקודם כבר הסתובב -90 אז עכשיו נותר לו להסתובב ולהחזיר את הסיבוב ההוא כך שתהיה זוית חדשה שלמה 
						if(mat.isVisited(curr->getPoint().getX(), curr->getPoint().getZ()) ==-1)//לא ביקרו שם
						{
							curr->setAngel(angel);
							curr->setStop(Stops::RIGHT);
							r = rotateVectorByAngle(rightDist, 0, mainAngel);
							curr->setPoint(curr->getPoint().getX() + r(0), curr->getPoint().getY(), curr->getPoint().getZ() + r(1));
							mainAngel += angel;
							m1 = m2;
							//curr = newCurr; אין  newCurr במקרה הזה
							break;
						}
						else {//ימשיך ולא ייכנס ללולאה
							mainAngel -= (currnetAngel);//להחזיר את הסיבוב שנעשה
							curr->setAngel(-currnetAngel);//מסתובב
							turnTo(-currnetAngel);
							goTo(10);//ילך עוד 10 ס"מ ויעקוף כנראה את הפתח
						}
					}
				}
			}
			else
			{
				//התקדמות צעד
				goTo(20.0);
				r = rotateVectorByAngle(0, 20, mainAngel);
				newCurr = new Vertex(curr->getPoint().getX() + r(0), curr->getPoint().getY(), p.getZ() + r(1), 0, curr);
				newCurr = graph.addVertex(newCurr);
				mat.changeValue(newCurr);
				curr = newCurr;//הכנה לאיטרציה הבאה

			}
			//בדיקת מכשולים עד לקודקוד הבא- obstacles
			//lidarPointRL = lidar.GetRightToLeftLidar();
			//m1 = lidar.filterFirsts(lidarPointRL);
		}
		if (turnflag == 0)//אם יש קיר ממול הרובוט
		{
			flag = false;//כביכול מסתובב לשמאל
			currnetAngel = 90;
			mainAngel += currnetAngel;
			curr->setStop(Stops::LEFT);
			curr->setAngel(90);
			turnTo(curr->getAngel());
			flag = true;// חזרה לימין 
		}
		turnflag = 0;
	}
	yoloThread.join();
	//mat.deleteMatrix();
}