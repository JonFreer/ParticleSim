// FLIP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <MacGrid.h>
#include <Particle.h>
#include <Eigen/Sparse>
#include <chrono>
#include <solver.h>
#include "fluidrender.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>
#include <Timer.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


float angleX = 1.8f;
float angleY = 1.3f;
float lx = -0.2f, lz = 1.0f, ly = 0.2f;

float x = 0;
float y = 0;
float z = 0;

// the key stat

//es. These variables will be zero
//when no key is being presses

float deltaX = 0.0f;
float deltaY = 0.0f;

timer t;
int xOrigin = -1;
int yOrigin = -1;
float radius = 20;
bool boundingBox = true;
bool solidCells = true;
bool airCells = true;
bool fluidCells = true;
bool displayParticles = true;
bool displayVelocities = false;
bool play = false;
int frame = 0;
const int FRAMES = 300;
int particleCount = 5000; //10000
const int CELL_COUNT = 40000;
FluidRenderer fr;


void changeSize(int w, int h) {

	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;


	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void setTitle() {
	std::string s1 = "Frame: ";
	std::string title = s1 + std::to_string(frame%FRAMES);
	const char* c = title.c_str();
	glutSetWindowTitle(c);
}


void renderScene(void) {



	if (play) {
		if (t.getTimePassed() > 10) {
			t.start();
			frame++;
			setTitle();
		}
	}



	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x+radius * lx, y+radius * ly, z+radius * lz,
		x, y, z,
		0.0f, 1.0f, 0.0f);

	// Draw ground
	//fr.drawGridBoundingBox();
	//fr.drawGrid();
	//fr.drawSolidCells();
	if (boundingBox) {
		fr.drawBoundingBox();
	}
	if (fluidCells) {
		fr.drawCellsByType(frame % FRAMES, FLUID , 0.1, 0.1 , 1, CELL_COUNT);
	}
	if (solidCells) {
		fr.drawCellsByType(frame % FRAMES, SOLID , 0.2 , 0.2 ,0.2, CELL_COUNT);
	}
	if (airCells) {
		fr.drawCellsByType(frame % FRAMES, AIR , 0.2 ,0.5, 0.2, CELL_COUNT);
	}
	if (displayParticles) {
		fr.drawParticles(frame % FRAMES);
	}
	if (displayVelocities) {
		fr.drawVelocitites(frame % FRAMES, CELL_COUNT);
	}

	glutSolidSphere(0.1, 5, 5);
	

	


	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) {

	if (key == 27)
		exit(0);
	if (key == 'b') {
		boundingBox = !boundingBox;
	}
	if (key == 'a') {
		airCells = !airCells;
	}
	if (key == 's') {
		solidCells = !solidCells;
	}
	if (key == 'f') {
		fluidCells = !fluidCells;
	}
	if (key == 'p') {
		displayParticles = !displayParticles;
	}
	if (key == 'v') {
		displayVelocities = !displayVelocities;
	}
	if (key == ' ') {
		play = !play;
		if (play) {
			t.start();
		}
	}
	
	
}


void pressKey(int key, int xx, int yy) {

	//switch (key) {
	//case GLUT_KEY_UP: deltaMove = 0.5f; break;
	//case GLUT_KEY_DOWN: deltaMove = -0.5f; break;
	//}
	if (key == GLUT_KEY_LEFT) {
		frame--;
	}
	if (key == GLUT_KEY_RIGHT) {
		frame++;
	}

	

	//char *buf; 
	//sprintf(buf, "%d", frame);

	setTitle();
	std::cout << "frame: " << frame << "\n";
}





void releaseKey(int key, int x, int y) {

	//switch (key) {
	//case GLUT_KEY_UP:
	//case GLUT_KEY_DOWN: deltaMove = 0; break;
	//}
}

void mouseMove(int x, int y) {
	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaX = (x - xOrigin) * 0.001f;
		deltaY = (y - yOrigin) * 0.001f;
		// update camera's direction
		lx = cos(angleX + deltaX) * sin(angleY + deltaY);
		ly = cos(deltaY + angleY);
		lz = sin(angleX + deltaX) * sin(angleY + deltaY);
	}
}

void mouseButton(int button, int state, int x, int y) {
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angleX += deltaX;
			angleY += deltaY;
			xOrigin = -1;
		}
		else {// state = GLUT_DOWN
			xOrigin = x;
			yOrigin = y;
		}
	}
	if (button == 3 || button == 4) {
		if (state == GLUT_UP) return;
		if (button == 3) {
			radius += 0.3f;
		}
		else {
			radius -= 0.3f;
		}

	}
}

int main(int argc, char** argv) {

	//FluidSimulation fs = FluidSimulation(10, 10, 10, 0.1);
	//fs.run();
	
	
	Cell* grid = (Cell*)malloc(sizeof(Cell) * CELL_COUNT * FRAMES);
	int* cellCount = (int*)malloc(sizeof(int) * FRAMES);
	Particle* paticleData = (Particle*)malloc(sizeof(Particle) * particleCount * FRAMES);
	

	Solver fs = Solver(particleCount,40,40,40,0.25);
	fr = FluidRenderer(&fs, grid, cellCount, paticleData);

	fr.getCenter(&x, &y, &z);

	fs.run(0.05, FRAMES, paticleData, grid, cellCount);

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Fluid");


	//glEnable(GL_MULTISAMPLE);

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}



