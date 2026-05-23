#include "Header.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {

    // Указатель на вектор фигур
    vector<Shape*>* shapes = new vector<Shape*>();

    // Добавление фигур через указатель на вектор
    shapes->push_back(new Circle(Point(0, 0), 5.0));
    shapes->push_back(new Ellipse(Point(1, 2), 4.0, 3.0));
    shapes->push_back(new Triangle(3.0, 4.0, 5.0));
    shapes->push_back(new Rectangle(10.0, 5.0));

    // Вывод информации о фигурах
    for (size_t i = 0; i < shapes->size(); ++i) {
        (*shapes)[i]->name();
        cout << " | Area: " << (*shapes)[i]->calc_area()
            << " | Perimeter: " << (*shapes)[i]->calc_perimetr() << "\n";
    }

    for (size_t i = 0; i < shapes->size(); ++i) {
        delete (*shapes)[i];
    }
    delete shapes;

    return 0;
}
