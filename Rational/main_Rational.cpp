#include "Rational.h"
#include <iostream>
using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    Rational a(1, 2), b(-1, 6);
    cout << "a = " << a << ", b = " << b << endl;
    cout << "a + b = " << a + b << endl;

    cout << "--------------" << endl;

    Rational c = a;
    c += b;
    cout << "a += b = " << c << endl;

    cout << "--------------" << endl;

    cout << "-b = " << -b << endl;
    cout << "a - b = " << a + b << endl;

    cout << "--------------" << endl;

    Rational d(2, 3);
    cout << "d = " << d << endl;
    cout << "++d = " << ++d << endl;
    cout << "d++ = " << d++ << ", затем d = " << d << endl;

    cout << "--------------" << endl;

    Rational e(1, 2), f(2, 4);
    cout << "e == f: " << (e == f) << endl;
    cout << "e != f: " << (e != f) << endl;

    cout << "--------------" << endl;

    Rational g(5, 2);
    cout << "g = " << g << ", int(g) = " << int(g) << ", double(g) = " << double(g) << endl;

    cout << "--------------" << endl;

    //Решение квадратного уравнения 
    Rational root1, root2;
    int r = 1, q = -5, p = 6;
    int rootCount = root1.solveQuadratic(r, q, p, root1, root2);

    if (rootCount == 2) cout << "Корни: " << root1 << "и" << root2 << endl;

    else if (rootCount == 1) cout << "Корень: " << root1 << endl;
    else cout << "Нет корней" << endl;
    return 0;

    cout << "--------------" << endl;

    Rational h, i(1, 3);
    cout << "Введите рациональное число (числитель и знаменатель): ";
    cin >> h;
    cout << "Вы ввели: " << h << endl;
    cout << "h == i? " << (h == i) << endl;
    return 0;
}