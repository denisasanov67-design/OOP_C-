#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// Структура точки
struct Point {
    double x, y;
    Point(double xx = 0, double yy = 0) : x(xx), y(yy) {}
};

// Абстрактный базовый класс фигур
class Shape {
public:
    virtual double calc_area() const = 0; 
    virtual double calc_perimetr() const = 0; 
    virtual void name() const = 0;
    virtual ~Shape() {} 
};

// Класс окружности
class Circle : public Shape {
private:
    Point center;
    double r;

public:
    Circle(Point c, double radius) : center(c), r(radius) {}
    Circle(double cx, double cy, double radius) : center(cx, cy), r(radius) {}

    double calc_area() const override;
    double calc_perimetr() const override;
    void name() const override;
};

// Класс элипса 
class Ellipse : public Shape {
private:
    Point center;
    double a, b;

public:

    Ellipse(Point c, double major, double minor) // передается большая полуось и малая полуось
        : center(c), a(major), b(minor) {
    }

    Ellipse(double cx, double cy, double major, double minor)
        : center(cx, cy), a(major), b(minor) {
    }


    double calc_area() const override;
    double calc_perimetr() const override;
    void name() const override;
};

// Класс многоугольника 
class Polygon : public Shape {
protected:
    vector<Point> points; //вершины многоугольника

public:
    Polygon(const vector<Point>& pts) : points(pts) {}  

    double calc_area() const override;
    double calc_perimetr() const override;
    void name() const override;

    const vector<Point>& get_points() const { return points; } // конструктор для эффективной передачи 
};

// Класс триугольника 
class Triangle : public Polygon {
public:
    Triangle(const vector<Point>& pts) : Polygon(pts) {}
    Triangle(double side);
    Triangle(double a, double b, double c);

    void name() const override;
};

// Класс прямоугольника
class Rectangle : public Polygon {
public:
    Rectangle(const vector<Point>& pts) : Polygon(pts) {}
    Rectangle(double width, double height);
    Rectangle(double width, double height, Point bottom_left);

    void name() const override;
};
    
#endif 