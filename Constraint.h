#ifndef CONSTRAINT_H_INCLUDED
#define CONSTRAINT_H_INCLUDED

#include "Particle.h"

class Constraint {
public: 
	Constraint(Particle * pA, Particle * pB);
	void satisfyConstraint(); 

	float restLength;  // for cloth, defined as initial distance between the two particles 
	Particle * particleA; 
	Particle * particleB; 
};

#endif