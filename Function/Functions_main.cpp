#include <iostream>
#include "Header.h"

int main()
{
    cout << "Parabola" << endl;
    Parabola p(1, -4, 3); 
    p.print();
    cout << "f(2) = " << p.calculate(2) << endl;
    cout << "Min on [0, 5]: " << p.findMin(0, 5) << endl;
    cout << "Max on [0, 5]: " << p.findMax(0, 5) << endl;
    cout << "f'(2) ≈ " << p.derivation(2) << endl;
    cout << "∫[0,3] f(x)dx ≈ " << p.integral(0, 3) << endl;

    cout << "\nHiperbola" << endl;
    Hiperbola h(10);  
    h.print();
    cout << "f(2) = " << h.calculate(2) << endl;
    cout << "f(5) = " << h.calculate(5) << endl;
    cout << "Min on [1, 10]: " << h.findMin(1, 10) << endl;
    cout << "Max on [1, 10]: " << h.findMax(1, 10) << endl;

    cout << "\nExponenta" << endl;
    Exponenta e(2, 0.3);  
    e.print();
    cout << "f(0) = " << e.calculate(0) << endl;
    cout << "f(5) = " << e.calculate(5) << endl;
    cout << "Min on [0, 5]: " << e.findMin(0, 5) << endl;
    cout << "Max on [0, 5]: " << e.findMax(0, 5) << endl;
    cout << "∫[0,2] f(x)dx ≈ " << e.integral(0, 2) << endl;

    return 0;
}
