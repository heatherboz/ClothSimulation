#include <stdlib.h>

#ifdef __MAIN__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glui.h>
#endif

#include "vector.h"
#include "ParticleSystem.h" 

using namespace std;

int angle = 75; 
ParticleSystem * _particleSystem; 
int paused = 1; 
int main_window;
int spin = 0; 
float spin_angle = 0;

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}
 
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -20.0f);
	glRotatef(-angle + spin_angle, 0.0f, 1.0f, 0.0f);
	
	GLfloat ambientColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat lightPos[] = {10.0f, 10.0f, 10.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	_particleSystem->display();
 
	glutSwapBuffers();
}

void createScene(int sceneNum) {
	switch (sceneNum) {
		case (1):
			_particleSystem->createScene1();
			break;
		case (2):
			_particleSystem->createScene2(); 
			break;
		case (3): 
			_particleSystem->createScene3(); 
			break; 
		case (4): 
			_particleSystem->createScene4(); 
			break;
		case (5):
			_particleSystem->createScene5(); 
			break;
	}
}

// control window 
void glui() {
    GLUI * glui_window;

	glui_window = GLUI_Master.create_glui("Controls"); 
	glui_window->set_main_gfx_window( main_window );

	glui_window->add_checkbox("PAUSED", &paused); 

	glui_window->add_button("CREATE SCENE 1", 1, GLUI_Update_CB(createScene)); 
	glui_window->add_button("CREATE SCENE 2", 2, GLUI_Update_CB(createScene)); 
	glui_window->add_button("CREATE SCENE 3", 3, GLUI_Update_CB(createScene)); 
	glui_window->add_button("CREATE SCENE 4", 4, GLUI_Update_CB(createScene)); 
	glui_window->add_button("CREATE SCENE 5", 5, GLUI_Update_CB(createScene));

	glui_window->add_edittext("ANGLE", GLUI_EDITTEXT_INT, &angle);

	glui_window->add_checkbox("SPIN", &spin);

	glui_window->add_edittext("RADIUS OF SPHERE", GLUI_EDITTEXT_FLOAT, &_particleSystem->sphereRad); 
	glui_window->add_edittext("SIDE LENGTH OF CUBE", GLUI_EDITTEXT_FLOAT, &_particleSystem->cubeSideLen);

	glui_window->add_edittext("NUMBER OF ITERATIONS", GLUI_EDITTEXT_INT, &_particleSystem->numIterations);

	glui_window->add_edittext("TIMESTEP", GLUI_EDITTEXT_FLOAT, &_particleSystem->h);
}

void update(int value) {
	if (paused == 0) {
		_particleSystem->timeStep();
	
		// make the whole scene spin  
		if (spin == 1) {	
			spin_angle += 0.25; 
			if (spin_angle > 360) {
				spin_angle -= 360;
			}
		} 
	}

	glutTimerFunc(_particleSystem->h * 1000, update, 0); // waits for h*1000 seconds, then calls update again
}

void myGlutIdle( void )
{
  if ( glutGetWindow() != main_window )
    glutSetWindow(main_window);

  glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	
	main_window = glutCreateWindow("Cloth Simulation");
	initRendering();

	_particleSystem = new ParticleSystem(); 
	_particleSystem->createScene1();  // load the default scene 

	glui(); // make control window 

	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);
	glutTimerFunc(_particleSystem->h * 1000, update, 0);
	GLUI_Master.set_glutIdleFunc( myGlutIdle );
	
	glutMainLoop();
	return 0;
}









