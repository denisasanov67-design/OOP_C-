#pragma once
#include <cmath>

using namespace std;

class Function {

public:

	virtual double calculate(double x) = 0;
	virtual void print() = 0;

	double findMin(double left, double right, double step = 0.01);
	double findMax(double left, double right, double step = 0.01);
	double derivation(double x);
	double integral(double a, double b, double step = 0.01);

	virtual ~Function() {}
};

class Parabola:public Function {
	double a, b, c;
public:
	Parabola(double aa, double bb, double cc) : a(aa), b(bb), c(cc) {}

	double calculate(double x) override;
	void print() override;
};


class Hiperbola : public Function
{
	double a;

public:
	Hiperbola(double aa) : a(aa){}

	double calculate(double x) override;
	void print() override;
};

class Exponenta : public Function
{
private:
	double a, b;

public:
	Exponenta(double a, double b) : a(a), b(b) {}

	double calculate(double x) override;
	void print() override;
};