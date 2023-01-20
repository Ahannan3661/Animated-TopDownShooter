#pragma once
#include <iostream>
#include <cmath>
using namespace std;

class Vector2D
{
public:

	float x;
	float y;

	Vector2D();
	Vector2D(float, float);

	Vector2D& Add(const Vector2D&);
	Vector2D& Subtract(const Vector2D&);
	Vector2D& Multiply(const Vector2D&);
	Vector2D& Divide(const Vector2D&);

	friend Vector2D& operator+(Vector2D&, const Vector2D&);
	friend Vector2D& operator-(Vector2D&, const Vector2D&);
	friend Vector2D& operator/(Vector2D&, const Vector2D&);
	friend Vector2D& operator*(Vector2D&, const Vector2D&);

	Vector2D& operator+=(const Vector2D&);
	Vector2D& operator-=(const Vector2D&);
	Vector2D& operator/=(const Vector2D&);
	Vector2D& operator*=(const Vector2D&);

	float dot(const Vector2D&);
	float magnitude();
	Vector2D& unit(const Vector2D&);
	float getAngle(Vector2D&);

	Vector2D& Zero();
	Vector2D& operator+(const int a);
	Vector2D& operator-(const int a);
	Vector2D& operator/(const int a);
	Vector2D& operator*(const int a);

	Vector2D& operator+(const float a);
	Vector2D& operator-(const float a);
	Vector2D& operator/(const float a);
	Vector2D& operator*(const float a);


	friend ostream& operator<<(ostream&, const Vector2D);
};