#include "vector.h" 
#include "Particle.h"

using namespace std;

Particle::Particle(Vector pos, float mass, bool pinned) {
	this->pos = pos; 
	this->prevPos = pos;  // prevPos used in verlet integration

	this->mass = mass; 
	this->invMass = 1 / mass; 
	// if the particle is pinned, forces will be 0, invmass=0 will allow for that 
	if (pinned) {
		this->invMass = 0; 
	}
}

void Particle::unpin() {
	this->invMass = 1 / mass; 
}

void Particle::pin() {
	this->invMass = 0; 
}