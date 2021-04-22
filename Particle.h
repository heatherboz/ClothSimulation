#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include "vector.h" 

class Particle {
public: 
	Particle(); 
	Particle(Vector pos, float mass, bool pinned); 

	void unpin();
	void pin();

	Vector pos;
	Vector prevPos; 
	Vector a; // acceleration 
	Vector normal; // used for rendering 
	float invMass; // 0 for pinned particles 
	float mass;
};

#endif