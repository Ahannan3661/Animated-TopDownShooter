#include "Vector2D.h"

const double pi = 3.14159265358979323846;
Vector2D::Vector2D() { x = y = 0.0f; }
Vector2D::Vector2D(float x, float y) { this->x = x; this->y = y; }

Vector2D& Vector2D::Add(const Vector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2D& Vector2D::Divide(const Vector2D& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

Vector2D& Vector2D::Multiply(const Vector2D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

Vector2D& operator+(Vector2D& v1, const Vector2D& v2)
{
	return v1.Add(v2);
}

Vector2D& operator-(Vector2D& v1, const Vector2D& v2)
{
	return v1.Subtract(v2);
}

Vector2D& operator/(Vector2D& v1, const Vector2D& v2)
{
	return v1.Divide(v2);
}

Vector2D& operator*(Vector2D& v1, const Vector2D& v2)
{
	return v1.Multiply(v2);
}

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	return this->Add(vec);
}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	return this->Subtract(vec);
}

Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
	return this->Divide(vec);
}

Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
	return this->Multiply(vec);
}

ostream& operator<<(ostream& stream, const Vector2D vec)
{
	stream << "(" << vec.x << ", " << vec.y << ")";
	return stream;
}

Vector2D& Vector2D::Zero()
{
	this->x = this->y = 0.0f;
	return *this;
}

Vector2D& Vector2D::operator+(const int a)
{
	this->x += a;
	this->y += a;
	return *this;
}
Vector2D& Vector2D::operator-(const int a)
{
	this->x -= a;
	this->y -= a;
	return *this;
}
Vector2D& Vector2D::operator/(const int a)
{
	this->x /= a;
	this->y /= a;
	return *this;
}
Vector2D& Vector2D::operator*(const int a)
{
	this->x *= a;
	this->y *= a;
	return *this;
}

Vector2D& Vector2D::operator+(const float a)
{
	this->x += a;
	this->y += a;
	return *this;
}
Vector2D& Vector2D::operator-(const float a)
{
	this->x -= a;
	this->y -= a;
	return *this;
}
Vector2D& Vector2D::operator/(const float a)
{
	this->x /= a;
	this->y /= a;
	return *this;
}
Vector2D& Vector2D::operator*(const float a)
{
	this->x *= a;
	this->y *= a;
	return *this;
}

float Vector2D::dot(const Vector2D& v2)
{
	return ((this->x * v2.x) + (this->y * v2.y));
}

float Vector2D::getAngle(Vector2D&v)
{
	return 180.0f + atan2(y - v.y, x - v.x) * 180.0f / pi;
}

float Vector2D::magnitude()
{
	return sqrt((this->x * this->x) + (this->y * this->y));
}

Vector2D& Vector2D::unit(const Vector2D& v2)
{
	Vector2D vector;
	vector = v2;
	vector -= *this;
	return vector/vector.magnitude();
}