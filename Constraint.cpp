#include "vector.h"
#include "Constraint.h"
#include "Particle.h" 

using namespace std;

Constraint::Constraint(Particle * pA, Particle * pB) {
	particleA = pA; 
	particleB = pB; 

	restLength = (particleB->pos - particleA->pos).magnitude(); 
};


void Constraint::satisfyConstraint() {
	// case where both particles are pinned, neither should move, avoid division by 0 
	if (particleA->invMass + particleB->invMass == 0) {
		return; 
	}

	Vector delta = particleA->pos - particleB->pos; 
	float deltaLength = sqrt(delta.dot(delta)); 
	float diff = (deltaLength - restLength) / (deltaLength * (particleA->invMass + particleB->invMass));

	particleA->pos -= delta * (particleA->invMass * diff); 
	particleB->pos += delta * (particleB->invMass * diff); 
}