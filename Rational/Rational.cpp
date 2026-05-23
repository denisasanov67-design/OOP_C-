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

Rational Rational::operator-(const Rational& r) const {
	Rational res(*this); res -= r; res.simplify(); return res;
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

bool Rational::operator >(const Rational& r)const
{
	return(numer * r.denom) > (r.numer * denom);
}

bool Rational::operator <(const Rational& r)const
{
	return(numer * r.denom) < (r.numer * denom);
}

bool Rational::operator >=(const Rational& r)const
{
	return(*this > r) || (*this == r);
}

bool Rational::operator <=(const Rational& r)const
{
	return(*this < r) || (*this == r);
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


int Rational::solveQuadratic(const Rational& a, const Rational& b, const Rational& c, Rational& root1, Rational& root2) const {

	Rational b_squared(b.numer * b.numer, b.denom * b.denom);
	b_squared.simplify();

	Rational ac(a.numer * c.numer, a.denom * c.denom);
	ac.simplify();
	Rational four_ac(4 * ac.numer, ac.denom);
	four_ac.simplify();

	Rational D = b_squared - four_ac;
	D.simplify();

	if (D.numer < 0) {
		return 0; 
	}


	int gcd_d = getNod(abs(D.numer), D.denom);
	int p = abs(D.numer) / gcd_d;
	int q = D.denom / gcd_d;

	int sqrt_p = (int)sqrt(p);
	int sqrt_q = (int)sqrt(q);

	if (sqrt_p * sqrt_p != p || sqrt_q * sqrt_q != q) {
		return 0; 
	}

	Rational sqrt_D(sqrt_p, sqrt_q);
	sqrt_D.simplify();

	Rational two_a(2 * a.numer, a.denom);
	two_a.simplify();

	Rational neg_b(-b.numer, b.denom);
	neg_b.simplify();

	if (D.numer == 0) {
		root1 = neg_b / two_a;
		root1.simplify();
		return 1;
	}

	root1 = (neg_b + sqrt_D) / two_a;
	root2 = (neg_b - sqrt_D) / two_a;
	root1.simplify();
	root2.simplify();

	return 2;
}