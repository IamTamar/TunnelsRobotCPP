#pragma once
#include <iostream>
#include <mutex> // השתמש ב-std::mutex

class Point
{
public:
    float x;
    float y;
    float z;
    mutable std::mutex mtxX; // השתמש ב-mutable אם ה-getters הם const
    mutable std::mutex mtxY;
    mutable std::mutex mtxZ;

    // למנוע העתקה של Point בגלל ה-mutexes
    //Point(const Point&) = delete;
    //Point& operator=(const Point&) = delete;

    Point(Point&&) noexcept = default; 
    Point& operator=(Point&&) noexcept = default;

    Point() : x(0), y(0), z(0) {} // Default constructor
    /*Point(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }*/
    Point(float x_val, float y_val, float z_val)
        : x(x_val), y(y_val), z(z_val), 
        mtxX(), mtxY(), mtxZ() 
    { }

    // Getters עם נעילה
    float getX() const { std::lock_guard<std::mutex> lock(mtxX); return x; }
    float getY() const { std::lock_guard<std::mutex> lock(mtxY); return y; }
    float getZ() const { std::lock_guard<std::mutex> lock(mtxZ); return z; }

    // Setters / Change functions עם נעילה
    float changeX(float val) {
        std::lock_guard<std::mutex> lock(mtxX);
        this->x = val;
        return this->x;
    }
    float changeY(float val) {
        std::lock_guard<std::mutex> lock(mtxY);
        this->y = val;
        return this->y;
    }
    float changeZ(float val) {
        std::lock_guard<std::mutex> lock(mtxZ);
        this->z = val;
        return this->z;
    }


    void printPoint(); 
   /* float pointDirection(const Point& p_v, const Point& o_v); */
};