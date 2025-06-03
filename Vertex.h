#pragma once
#include "Point.h"	
#include <list>
#include <mutex>
#include "Stops.cpp" // ����� ����� ������ ��� �-Stops.h �� �� ����� enum �� class

class Vertex {
public:
    Point p; // Point ����� �� mutex�� ����
    float angel;
    Vertex* prev;
    std::list<Vertex*> nextList;
    Stops s;
    // �������� ���� ����� �� ���� ������� �� Vertex, �� ����� �� ���� ������� ���� Point
    std::mutex mtxList;
    std::mutex mtxP; // ��� �� ����� �������� p ���� (�� ������� �������� ���)
    std::mutex mtxS;
    std::mutex mtxAngle;
    std::mutex mtxPrev;

    // Constructor - ����� Point ���� ����� ������
    Vertex(float x, float y, float z, float angel, Vertex* prev)
        : p(x, y, z), angel(angel), prev(prev), s(Stops::DEFAULT) {
    }

    Vertex(Vertex* v)
        : p(v->getPoint().getX(), v->getPoint().getY(), v->getPoint().getZ()), // Point ����� ���� ����
        angel(v->getAngel()),
        prev(v->getPrev()),
        s(v->getStop()) { }

    Vertex() : p(0, 0, 0), angel(0), prev(nullptr), s(Stops::DEFAULT) {}

    void addNext(Vertex* v) {
        std::lock_guard<std::mutex> lock(mtxList);
        nextList.push_back(v);
    }
    std::list<Vertex*>& getNextList() {
        std::lock_guard<std::mutex> lock(mtxList);
        return nextList;
    }

    const Point& getPoint()  { 
        std::lock_guard<std::mutex> lock(mtxP);
        return p;
    }

    Stops getStop() {
        std::lock_guard<std::mutex> lock(mtxS);
        return s;
    }
    float getAngel() {
        std::lock_guard<std::mutex> lock(mtxAngle);
        return angel;
    }
    Vertex* getPrev() {
        std::lock_guard<std::mutex> lock(mtxPrev);
        return prev;
    }
    void setStop(Stops s) {
        std::lock_guard<std::mutex> lock(mtxS);
        this->s = s;
    }
    void setAngel(float angel) {
        std::lock_guard<std::mutex> lock(mtxAngle);
        this->angel = angel;
    }


    void setPoint(float x_val, float y_val, float z_val) {
    
        std::lock_guard<std::mutex> lock_p_access(mtxP); 

        p.changeX(x_val - p.getX()); 
        p.changeY(y_val - p.getY());
        p.changeZ(z_val - p.getZ());
   
    }

    void setPointByPoint(Point point);
    bool isSimilliar(Vertex* v);
    float distance(Vertex* v);
    Point vertexToPoint(); 
    void printVertex();
    void printAngel();
};