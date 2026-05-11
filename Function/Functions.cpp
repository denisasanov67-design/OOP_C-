#include <iostream>
#include "Header.h"

double Function::findMin(double left, double right, double step = 0.01) 
{
	double minVal = calculate(left);

	for (double x = left; x < right; x += step) {
		double y = calculate(left);

		if (y < minVal) minVal = y;
	}

	return minVal;
}


double Function::findMax(double left, double right, double step = 0.01) 
{
	double maxVal = calculate(left);

	for (double x = left; x < right; x += step) {
		double y = calculate(left);

		if (y > maxVal) maxVal = y;
	}
	
	return maxVal;
}

double Function::derivation(double x) 
{
	double h = 0.0001;

	return (calculate(x + h) - calculate(x)) / h;
}

double Function::integral(double a, double b, double step = 0.01)
{
	double sum = 0.0;

	for (double x = a; x < b; x += step) {
		sum += (calculate(x) + calculate(x + step) * step) / 2;
	}
	return sum;
}

// Парабола
double Parabola::calculate(double x)
{
	return a * x * x + b * x + c;
}

void Parabola::print() 
{
	cout << "Parabola: " << a << "x^2 + " << b << "x + " << c << endl;
}

// Гипербола 
double Hiperbola::calculate(double x)
{
	return a / x;
}

void Hiperbola::print()
{
	cout<<"Hiberbola: " << a << "/x" << endl;
}


// Экспонента
double Exponenta::calculate(double x)
{
	return a * exp(b * x);
}

void Exponenta::print()
{
	cout << "Exponenta: " << a << " * e^(" << b << "x)" << endl;
}