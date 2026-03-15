#ifndef RATIONAL_H
#define RATIONAL_H
#include <iostream>
using namespace std;


class Rational 
{
private:
    int numer;
    int denom;
    void simplify();
    int getNod(int a, int b) const;

public:

    Rational();
    Rational(int nunber);
    Rational(int n, int d);
    Rational(const Rational& other) = default;

    int solveQuadratic(int a, int b, int c, Rational& root1, Rational& root2) const;


    Rational& operator +=(const Rational& r);
    Rational operator +(const Rational& r) const;


    Rational operator *(const Rational& r) const;
    Rational& operator *=(const Rational& r);


    Rational operator -() const;
    Rational& operator -=(const Rational& r);


    Rational operator ++(int);
    Rational& operator++(); 


    Rational operator / (const Rational& r) const;

   
    bool operator ==(const Rational& r) const; 
    bool operator !=(const Rational& r) const; 


    operator int() const;

    operator double() const;


    friend istream& operator >>(istream& in, Rational& r);

    friend ostream& operator <<(ostream& out, const Rational& r);
};

#endif 
