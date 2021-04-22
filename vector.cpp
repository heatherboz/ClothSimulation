#include "vector.h";
#include <math.h>;

using namespace std;

Vector::Vector(): x(0), y(0), z(0) 
{
}

Vector::Vector(float x, float y, float z): x(x), y(y), z(z)
{
}

Vector::Vector(const Vector &vector)
{
	this->x = vector.x;
	this->y = vector.y; 
	this->z = vector.z; 
}

Vector Vector::operator+(const Vector &v)
{
	return Vector(v.x + this->x, v.y + this->y, v.z + this->z); 
}

const Vector Vector::operator+=(const Vector &v) {
	this->x += v.x;
	this->y += v.y; 
	this->z += v.z; 

	return * this; 
}

const Vector Vector::operator-=(const Vector &v) {
	this->x -= v.x;
	this->y -= v.y; 
	this->z -= v.z; 

	return * this; 
}

Vector Vector::operator-(const Vector &v)
{
	return Vector(x - v.x, y - v.y, z - v.z); 
}

Vector Vector::operator*(float a)
{
	return Vector(a * this->x, a * this->y, a * this->z); 
}

Vector Vector::operator/(float a)
{
	return Vector(this->x / a, this->y / a, this->z / a); 
}

float Vector::dot(const Vector &v)
{
	return this->x * v.x + this->y * v.y + this->z * v.z; 
}

Vector Vector::cross(const Vector &v)
{
	float x = this->y * v.z + this->z * v.y; 
	float y = this->z * v.x + this->x * v.z; 
	float z = this->x * v.y + this->y * v.x; 

	return Vector(x, y, z); 
}

float Vector::magnitude() const {
	return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

const Vector Vector::normalize() {
	float magnitude = this->magnitude();

	this->x /= magnitude; 
	this->y /= magnitude; 
	this->z /= magnitude; 

	return * this; 
}






