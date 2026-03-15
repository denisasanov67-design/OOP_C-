#include "Rational.h"
#include <cstdlib>
#include <cmath>
using namespace std;


Rational::Rational()
{
	numer = 0;
	denom = 1;

}

Rational::Rational(int number)
{
	numer = number;
	denom = 1;
}

int Rational::getNod(int a, int b) const
{
	a = abs(a);
	b = abs(b);
	while (a != b)
	{
		if (a < b)
			b = b - a;
		else
			a = a - b;
	}
	return a;
}

Rational::Rational(int n, int d)
{
	if (d == 0) {
		numer = 0;
		denom = 1;
	}
	else {
		numer = n;
		denom = d;
		simplify();
	}
}

Rational Rational::operator+(const Rational& r) const
{
	Rational res(*this);
	res += r;
	res.simplify(); 
	return res;

}

Rational Rational::operator -() const
{
	Rational r(-numer, denom);
	r.simplify(); 
	return r;
}


Rational Rational :: operator *(const Rational& r) const
{
	return Rational(numer * r.numer, denom * r.denom);
}


Rational Rational::operator/(const Rational& r) const
{
	return Rational(numer * r.denom, denom * r.numer);
}


Rational& Rational::operator*=(const Rational& r)
{
	numer *= r.numer;
	denom *= r.denom;
	simplify(); 
	return *this;
}


Rational& Rational::operator -=(const Rational& r)
{
	return (*this += (-r));
}

Rational& Rational::operator +=(const Rational& r)
{
	numer = (numer * r.denom + denom * r.numer);
	denom *= r.denom;
	return *this;
}

void Rational::simplify()
{
	int nod = getNod(numer, denom);
	numer /= nod;
	denom /= nod;


	if (denom < 0)
	{
		numer *= -1;
		denom *= -1;
	}
}

Rational Rational::operator++(int)
{
	Rational oldValue = *this;
	numer += denom;          
	simplify();        
	return oldValue;
}


Rational& Rational::operator++()
{
	numer += denom; 
	simplify();    
	return *this;  
}


bool Rational::operator ==(const Rational& r) const
{
	return (numer == r.numer) && (denom == r.denom);
}


bool Rational::operator !=(const Rational& r) const
{
	return !(*this == r);
}


Rational::operator int() const
{
	return numer / denom;
}


Rational::operator double() const
{
	return ((double)numer) / denom;
}


istream& operator >>(istream& in, Rational& r)
{
	in >> r.numer >> r.denom;
	return in;
}


ostream& operator <<(ostream& out, const Rational& r)
{
	out << r.numer << "/" << r.denom;
	return out;
}


int Rational::solveQuadratic(int a, int b, int c, Rational& root1, Rational& root2) const
{
	int discriminant = b * b - 4 * a * c;

	if (discriminant < 0) return 0;

	if (discriminant == 0) {
		root1 = Rational(-b, 2 * a);
		return 1;
	}

	int sqrt_discr = sqrt(discriminant);

	if (sqrt_discr * sqrt_discr != discriminant) return 0;

	root1 = Rational(-b - sqrt_discr, 2 * a);
	root2 = Rational(-b + sqrt_discr, 2 * a);
	return 2;
}