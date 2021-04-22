#ifndef PARTICLESYSTEM_H_INCLUDED
#define PARTICLESYSTEM_H_INCLUDED

#include "vector.h"
#include "Constraint.h"
#include "Particle.h"

const int GRID_LENGTH = 100; 
const int GRID_WIDTH = 100; 
const int NUM_CONSTRAINTS = 2*(GRID_LENGTH)*(GRID_LENGTH-1) + 2*(GRID_LENGTH-1)*(GRID_LENGTH-1);
const int NUM_PARTICLES = GRID_LENGTH*GRID_LENGTH; 

class ParticleSystem {
public: 
	ParticleSystem(); 
	void createXZCloth(Vector startingPos, float dist); 
	void createXYCloth(Vector startingPos, float dist); 
	void createScene1(); 
	void createScene2(); 
	void createScene3();
	void createScene4(); 
	void createScene5();
	void addConstraints(); 
	void sphereCollision(int i);
	void cubeCollision(int i);
	void computeNormals();

	Vector m_gravity;
	Particle * m_particles[NUM_PARTICLES];
	Constraint * m_constraints[NUM_CONSTRAINTS];  

	float sphereRad;
	float cubeSideLen; 
	int curScene; 
	int numIterations;
	float h;

public: 
	void timeStep(); 
	void display(); 

private: 
	void verlet(); 
	void satisfyConstraints(); 
	void accumulateForces(); 
	void displaySphere();
	void displayCube();
};

#endif



