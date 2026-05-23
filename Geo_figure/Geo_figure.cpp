#include "Header.h"

// Circle
double Circle::calc_area() const {
    return M_PI * r * r;
}

double Circle::calc_perimetr() const {
    return 2.0 * M_PI * r;
}

void Circle::name() const {
    cout << "Circle" << endl;
}



// Ellipse
double Ellipse::calc_area() const {
    return M_PI * a * b;
}

double Ellipse::calc_perimetr() const {
    return M_PI * (3 * (a + b) - sqrt((3 * a + b) * (a + 3 * b)));
}

void Ellipse::name() const {
    cout << "Ellipse" << endl;
}



// Polygon
double Polygon::calc_area() const {
    if (points.size() < 3) return 0.0;

    double area = 0.0;
    size_t n = points.size(); // переменная для представления размера 

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n; // следующая вершина 
        area += points[i].x * points[j].y; // суммирование векторного произведения соседних ребер
        area -= points[j].x * points[i].y;
    }

    return abs(area) / 2.0;
}

double Polygon::calc_perimetr() const {
    if (points.size() < 2) return 0.0;

    double perimeter = 0.0;
    int n = static_cast<int>(points.size());

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        double dx = points[j].x - points[i].x;
        double dy = points[j].y - points[i].y;
        perimeter += sqrt(dx * dx + dy * dy);
    }

    return perimeter;
}

void Polygon::name() const {
    cout << "Polygon" << endl;
}



// Triangle
Triangle::Triangle(double side) {
    // Вычисляем вершины равностороннего треугольника
    double h = side * sqrt(3) / 2;
    // Для расчётов можно хранить стороны, но здесь просто используем формулы
    (void)h; // заглушка, чтобы не было предупреждения о неиспользуемой переменной
}



void Triangle::name() const {
    cout << "Triangle" << endl;
}


double Triangle::calc_area() const {
    double s = (a + b + c) / 2.0;
    return sqrt(max(0.0, s * (s - a) * (s - b) * (s - c)));
}

// Периметр треугольника
double Triangle::calc_perimetr() const {
    return a + b + c;
}


//Rectangle

Rectangle::Rectangle(double width, double height)
    : width(width), height(height), origin(0, 0) {
}

Rectangle::Rectangle(double width, double height, Point bottom_left)
    : width(width), height(height), origin(bottom_left) {
}

void Rectangle::name() const {
    cout << "Rectangle" << endl;
}

double Rectangle::calc_area() const {
    return width * height;
}

double Rectangle::calc_perimetr() const {
    return 2 * (width + height);
}
