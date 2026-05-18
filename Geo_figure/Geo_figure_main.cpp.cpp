#include "Header.h"
#include <iostream>

int main() {
    Circle c(Point(0, 0), 5.0);
    c.name();
    cout << " | Area: " << c.calc_area()
        << " | Perimeter: " << c.calc_perimetr() << "\n";

    Ellipse e(Point(1, 2), 4.0, 3.0);
    e.name();
    cout << " | Area: " << e.calc_area()    
        << " | Perimeter: " << e.calc_perimetr() << "\n";

    Triangle t(3.0, 4.0, 5.0);
    t.name();
    cout << " | Area: " << t.calc_area()
        << " | Perimeter: " << t.calc_perimetr() << "\n";

    Rectangle r(10.0, 5.0);
    r.name();
    cout << " | Area: " << r.calc_area()
        << " | Perimeter: " << r.calc_perimetr() << "\n";

    return 0;
}