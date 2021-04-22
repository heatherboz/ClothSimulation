#ifdef __MAIN__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glut.h>
#endif

#include "vector.h"
#include "ParticleSystem.h"

using namespace std; 

ParticleSystem::ParticleSystem() {
	// set default parameters for the system 
	m_gravity = Vector(0, -0.5, 0); 
	sphereRad = 1; 
	cubeSideLen = 2;
	numIterations = 1; 
	h = 0.01;
}

void ParticleSystem::createXZCloth(Vector startingPos, float dist) {
	Vector position = startingPos; 
	// add the particles in a grid in the x-z plane 
	// index of the particle relative to its position in the cloth is i*GRID_LENGTH + j, where (i,j) is its location in the cloth grid
	for (int i = 0; i < GRID_LENGTH; i++) {
		for (int j = 0; j < GRID_LENGTH; j++) {
			Particle * p = new Particle(position, 1, false);
			m_particles[i*GRID_LENGTH + j] = p; 

			position.x += dist; 
		}
		position.x = startingPos.x; 
		position.z += dist; 
	}
	computeNormals();
}

void ParticleSystem::createXYCloth(Vector startingPos, float dist) {
	Vector position = startingPos; 
	// add the particles in a grid in the x-y plane 
	// index of the particle relative to its position in the cloth is i*GRID_LENGTH + j, where (i,j) is its location in the cloth grid
	for (int i = 0; i < GRID_LENGTH; i++) {
		for (int j = 0; j < GRID_LENGTH; j++) {
			Particle * p = new Particle(position, 1, false);
			m_particles[i*GRID_LENGTH + j] = p; 

			position.y += dist; 
		}
		position.y = startingPos.y; 
		position.x += dist; 
	}
	computeNormals();
}

// scene 1: cloth in x-z plane, two corners pinned, sphere collision
void ParticleSystem::createScene1() { 
	curScene = 1; 
	// GOOD STARTING POSITIONS: 
	// (-1, 2, -1)
	// (-2, 2, -2) 

	Vector startingPos = Vector(-2, 2, -2); 
	float dist = 0.05; 
	createXZCloth(startingPos, dist);

	m_particles[(GRID_LENGTH-1)*GRID_LENGTH + GRID_LENGTH-1]->pin(); 
	m_particles[GRID_LENGTH-1]->pin();

	addConstraints();
}

// scene 2: cloth in x-z plane, no pins, sphere collision
void ParticleSystem::createScene2() {
	curScene = 2; 
	Vector startingPos = Vector(-2, 2, -2);
	float dist = 0.05; 
	createXZCloth(startingPos, dist); 

	addConstraints(); 
}

// scene 3: cloth in x-z plane, two particles pinned, cube collision
void ParticleSystem::createScene3() {
	curScene = 3; 
	Vector startingPos = Vector(-2, 2, -2);
	float dist = 0.05; 
	createXZCloth(startingPos, dist);

	m_particles[(GRID_LENGTH-1)*GRID_LENGTH + GRID_LENGTH-1]->pin(); 
	m_particles[GRID_LENGTH-1]->pin();

	addConstraints(); 
}

// scene 4: cloth in x-z plane, no pins, cube collision
void ParticleSystem::createScene4() {
	curScene = 4; 
	Vector startingPos = Vector(-2, 2, -2);
	float dist = 0.05; 
	createXZCloth(startingPos, dist);

	addConstraints(); 
}

// scene 2: cloth in x-z plane, two particles pinned 
void ParticleSystem::createScene5() {
	curScene = 5; 
	Vector startingPos = Vector(-2, 2, -2); 
	float dist = 0.05; 
	createXZCloth(startingPos, dist); 

	addConstraints(); 

	m_particles[(GRID_LENGTH-1)*GRID_LENGTH + GRID_LENGTH-1]->pin(); 
	m_particles[GRID_LENGTH-1]->pin();
}

// called right after the cloth is created, to make constraints between particles on the cloth 
void ParticleSystem::addConstraints() {
	// add the constraints -- horizontals (i, i+1)
	int k = 0; 
	for (int i = 0; i < GRID_LENGTH-1; i++) {
		for (int j = 0; j < GRID_LENGTH; j++) { 
			Constraint * c = new Constraint(m_particles[i*GRID_LENGTH + j], m_particles[(i+1)*GRID_LENGTH + j]);

			m_constraints[k] = c; 
			k++; 
		}
	}

	// add the constraints -- verticals (j, j+1)  
	for (int i = 0; i < GRID_LENGTH; i++) {
		for (int j = 0; j < GRID_LENGTH-1; j++) {
			Constraint * c = new Constraint(m_particles[i*GRID_LENGTH + j], m_particles[i*GRID_LENGTH + (j+1)]);

			m_constraints[k] = c; 
			k++; 
		}
	}

	// add the constraints -- diagonals (i,j) -> (i+1,j+1) and (i+1, j) -> (i, j+1)  
	for (int i = 0; i < GRID_LENGTH-1; i++) {
		for (int j = 0; j < GRID_LENGTH-1; j++) {
			Constraint * c1 = new Constraint(m_particles[i*GRID_LENGTH + j], m_particles[(i+1)*GRID_LENGTH + (j+1)]); 
			Constraint * c2 = new Constraint(m_particles[(i+1)*GRID_LENGTH + j], m_particles[i*GRID_LENGTH + (j+1)]);

			m_constraints[k] = c1;
			m_constraints[k+1] = c2;

			k+=2; 
		}
	}
}

// verlet integration to compute each particle's next position 
void ParticleSystem::verlet() {
	for (int i = 0; i < GRID_LENGTH; i++) {
		for (int j = 0; j < GRID_LENGTH; j++) {
			Vector x = m_particles[i*GRID_LENGTH + j]->pos; 

			Vector oldx = m_particles[i*GRID_LENGTH + j]->prevPos;
			Vector a = m_particles[i*GRID_LENGTH + j]->a;

			m_particles[i*GRID_LENGTH + j]->pos += x - oldx + a * (h * h); 
			m_particles[i*GRID_LENGTH + j]->prevPos = x; 
		}
	}
	// particles moved, so need to update their normals so they're displayed properly 
	computeNormals();
}

// computes and sets the normal of all particles 
void ParticleSystem::computeNormals() {
	for (int i = 0; i < GRID_LENGTH; i++) {
		for (int j = 0; j < GRID_LENGTH; j++) {
			Vector vy; 
			Vector vx; 

			if (i == 0) vy = Vector(0, 0, 0); 
			else vy = m_particles[(i-1)*GRID_LENGTH + j]->pos - m_particles[i*GRID_LENGTH+j]->pos;

			if (j == 0) vx = Vector(0, 0, 0);
			else vx = m_particles[i*GRID_LENGTH + (j-1)]->pos - m_particles[i*GRID_LENGTH+j]->pos;

			m_particles[i*GRID_LENGTH+j]->normal = vy.cross(vx);
			m_particles[i*GRID_LENGTH+j]->normal.normalize(); 
		}
	}
	
}

void ParticleSystem::accumulateForces() {
	// the only force present in the system is gravity
	// if invMass = 0 (i.e. particle is pinned), no acceleration 
	for (int i = 0; i < NUM_PARTICLES; i++) {
		m_particles[i]->a = m_gravity * m_particles[i]->invMass; 
	}
}

void ParticleSystem::satisfyConstraints() {
	for (int iter = 0; iter < numIterations; iter++) {
		for (int i = 0; i < NUM_PARTICLES; i++) {
			// only check collisions for the relevant objects to the scene 
			if (curScene == 1 || curScene == 2) {
				sphereCollision(i);
			}

			if (curScene == 3 || curScene == 4) {
				cubeCollision(i);
			}
		}
		
		// constraints between particles of the cloth 
		for (int k = 0; k < NUM_CONSTRAINTS; k++) {
			m_constraints[k]->satisfyConstraint();	
		}
	}
}

void ParticleSystem::cubeCollision(int i) { 
	// assumes cube's center is at the origin 
	float yDist = abs(m_particles[i]->pos.y) - 0.5*cubeSideLen; 
	float xDist = abs(m_particles[i]->pos.x) - 0.5*cubeSideLen; 
	float zDist = abs(m_particles[i]->pos.z) - 0.5*cubeSideLen;

	// if cloth is within cube 
	if (yDist <= 0 && xDist <= 0 && zDist <= 0) {
		// y is least distance to edge distance, so project to y face 
		if (yDist >= xDist && yDist >= zDist) {
			if (m_particles[i]->pos.y <= 0) {
				m_particles[i]->pos.y = - 0.5*cubeSideLen;
			} else {
				m_particles[i]->pos.y = 0.5*cubeSideLen;
			}
		}

		// x is least distance, so project to x face 
		else if (xDist >= yDist && xDist >= zDist) {
			if (m_particles[i]->pos.x <= 0) {
				m_particles[i]->pos.x = - 0.5*cubeSideLen; 
			} else {
				m_particles[i]->pos.x = 0.5*cubeSideLen; 
			}
		}

		// z is least distance, so project to z face 
		else if (zDist >= xDist && zDist >= yDist) {
			if (m_particles[i]->pos.z <= 0) {
				m_particles[i]->pos.z = - 0.5*cubeSideLen; 
			} else {
				m_particles[i]->pos.z = 0.5*cubeSideLen; 
			}
		}
	}
}

void ParticleSystem::sphereCollision(int i) {
	// get vector from center of sphere to cur position 
	Vector spherePos = Vector(0, 0, 0); 
	Vector v = m_particles[i]->pos - spherePos;
		
	// get length of that vector 
	float len = v.magnitude(); 

	// if the vector is within the sphere, need to update the position (project to closest point outside of sphere) 
	if (len < sphereRad) { 
		m_particles[i]->pos += v * ((sphereRad-len)/len); 
 
		// set prevPos to cur pos so that there isn't a large force pushing it out 
		m_particles[i]->prevPos = m_particles[i]->pos; 
	}
}

void ParticleSystem::timeStep() {
	accumulateForces(); 
	verlet(); 
	satisfyConstraints(); 
}

void ParticleSystem::display() {
	GLfloat materialColor[] = {0.8f, 0.0f, 1.0f}; 

	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
	glMaterialfv(GL_BACK, GL_AMBIENT, materialColor);
	glMaterialfv(GL_BACK, GL_DIFFUSE, materialColor);
	glBegin(GL_TRIANGLES);
	glColor3f(0.5f, 0.0f, 1.0f);
			
	// draw all the triangles in the cloth 
	for(int i=0; i < GRID_LENGTH-1; ++i)
	{
		for(int j=0; j < GRID_LENGTH-1; ++j)
		{
			glNormal3f(m_particles[i*GRID_WIDTH+j]->normal.x, m_particles[i*GRID_WIDTH+j]->normal.y, m_particles[i*GRID_WIDTH+j]->normal.z);
			glVertex3f(m_particles[i*GRID_WIDTH+j]->pos.x, m_particles[i*GRID_WIDTH+j]->pos.y, m_particles[i*GRID_WIDTH+j]->pos.z);
			glNormal3f(m_particles[i*GRID_WIDTH+j+1]->normal.x, m_particles[i*GRID_WIDTH+j+1]->normal.y, m_particles[i*GRID_WIDTH+j+1]->normal.z);
			glVertex3f(m_particles[i*GRID_WIDTH+j+1]->pos.x, m_particles[i*GRID_WIDTH+j+1]->pos.y, m_particles[i*GRID_WIDTH+j+1]->pos.z);
			glNormal3f(m_particles[(i+1)*GRID_WIDTH+j]->normal.x, m_particles[(i+1)*GRID_WIDTH+j]->normal.y, m_particles[(i+1)*GRID_WIDTH+j]->normal.z);
			glVertex3f(m_particles[(i+1)*GRID_WIDTH+j]->pos.x, m_particles[(i+1)*GRID_WIDTH+j]->pos.y, m_particles[(i+1)*GRID_WIDTH+j]->pos.z);

			glNormal3f(m_particles[(i+1)*GRID_WIDTH+j]->normal.x, m_particles[(i+1)*GRID_WIDTH+j]->normal.y, m_particles[(i+1)*GRID_WIDTH+j]->normal.z);
			glVertex3f(m_particles[(i+1)*GRID_WIDTH+j]->pos.x, m_particles[(i+1)*GRID_WIDTH+j]->pos.y, m_particles[(i+1)*GRID_WIDTH+j]->pos.z);
			glNormal3f(m_particles[i*GRID_WIDTH+j+1]->normal.x, m_particles[i*GRID_WIDTH+j+1]->normal.y, m_particles[i*GRID_WIDTH+j+1]->normal.z);
			glVertex3f(m_particles[i*GRID_WIDTH+j+1]->pos.x, m_particles[i*GRID_WIDTH+j+1]->pos.y, m_particles[i*GRID_WIDTH+j+1]->pos.z);
			glNormal3f(m_particles[(i+1)*GRID_WIDTH+j+1]->normal.x, m_particles[(i+1)*GRID_WIDTH+j+1]->normal.y, m_particles[(i+1)*GRID_WIDTH+j+1]->normal.z);
			glVertex3f(m_particles[(i+1)*GRID_WIDTH+j+1]->pos.x, m_particles[(i+1)*GRID_WIDTH+j+1]->pos.y, m_particles[(i+1)*GRID_WIDTH+j+1]->pos.z);
		}
	}

	glEnd(); 

	// sphere is in scene for scenes 1 and 2 
	if (curScene == 1 || curScene == 2) {
		displaySphere(); 
	}

	// cube is in scene for scenes 3 and 4 
	if (curScene == 3 || curScene == 4) {
		displayCube();
	}
 
		
	glDisable(GL_LIGHTING);
}

void ParticleSystem::displaySphere() {
	glColor3f(0.0f, 1.0f, 1.0f);
	glutSolidSphere(sphereRad, 25, 25);
}

void ParticleSystem::displayCube() {
	float x = cubeSideLen/2; 
	// draw a cube 
	glBegin(GL_QUADS);
      // Top face (y = 1.0f)
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f( x, x, -x);
      glVertex3f(-x, x, -x);
      glVertex3f(-x, x,  x);
      glVertex3f( x, x,  x);
 
      // Bottom face (y = -1.0f)
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( x, -x,  x);
      glVertex3f(-x, -x,  x);
      glVertex3f(-x, -x, -x);
      glVertex3f( x, -x, -x);
 
      // Front face  (z = 1.0f)
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f( x,  x, x);
      glVertex3f(-x,  x, x);
      glVertex3f(-x, -x, x);
      glVertex3f( x, -x, x);
 
      // Back face (z = -1.0f)
      glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
      glVertex3f( x, -x, -x);
      glVertex3f(-x, -x, -x);
      glVertex3f(-x,  x, -x);
      glVertex3f( x,  x, -x);
 
      // Left face (x = -1.0f)
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-x,  x,  x);
      glVertex3f(-x,  x, -x);
      glVertex3f(-x, -x, -x);
      glVertex3f(-x, -x,  x);
 
      // Right face (x = 1.0f)
      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      glVertex3f(x,  x, -x);
      glVertex3f(x,  x,  x);
      glVertex3f(x, -x,  x);
      glVertex3f(x, -x, -x);
	glEnd(); 
}
