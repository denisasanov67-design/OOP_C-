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
    Rational coeff_a(1, 1), coeff_b(-5, 1), coeff_c(6, 1);
    int rootCount = coeff_a.solveQuadratic(coeff_a, coeff_b, coeff_c, root1, root2);

    if (rootCount == 2) cout << "Корни: " << root1 << " и " << root2 << endl;
    else if (rootCount == 1) cout << "Корень: " << root1 << endl;
    else cout << "Нет действительных корней" << endl;

    cout << "--------------" << endl;

    Rational h, i(1, 3);
    cout << "Введите рациональное число (числитель и знаменатель): ";
    cin >> h;
    cout << "Вы ввели: " << h << endl;
    cout << "h == i? " << (h == i) << endl;

    return 0;
}