#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <iostream>

class Vector {
public:  
	Vector(); // default constructor
	Vector(float x, float y, float z);  
	Vector(const Vector &vector); // copy constructor

	Vector operator+(const Vector &vector); 
	const Vector operator+=(const Vector &vector); 
	Vector operator-(const Vector &vector); 
	const Vector operator-=(const Vector &vector);
	Vector operator*(float a); 
	Vector operator/(float a); 
	
	float dot(const Vector &vector);
	Vector cross(const Vector &vector);
	float magnitude() const;
	const Vector normalize();

	float x;
	float y; 
	float z; 
};

#endif
