/*
 *  CSCI 441, Computer Graphics, Fall 2015
 *
 *  Project: Assignment 4
 *  File: main.cpp
 *
 *	Author: Eric Sheeder - Fall 2015
 *
 *  Description:
 *      Drives the hero around with a pet now
 *
 */

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <cstdlib>

// C++ Libraries we'll use
#include <fstream>			// we'll use ifstream	
#include <string>			// for, well strings!
#include <vector>			// and vectors (the storage container, not directional)

//headers we've written
#include "Point.h"

using namespace std;

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

static size_t windowWidth  = 640;
static size_t windowHeight = 480;
static float aspectRatio;

GLint leftMouseButton, rightMouseButton;    // status of the mouse buttons
int mouseX = 0, mouseY = 0;                 // last known X and Y of the mouse

float cameraX, cameraY, cameraZ;            // camera position in cartesian coordinates
float cameraTheta, cameraPhi;               // camera DIRECTION in spherical coordinates
float dirX, dirY, dirZ;                     // camera DIRECTION in cartesian coordinates
float heroX, heroY, heroZ;
float heroTheta;
float radius = 30;
float wheelAngle = 0;
bool drawCurve = true;
bool drawCage = true;

//Counts from 0 - 99
float countingTimer = 0;

//Keeps track of which curve we are drawing the pet on
int curveNumber = 0;

vector<Point> controlPoints;
float trackPointVal = 0.0f;

GLuint environmentDL;            // display list for the 'city'


// getRand() ///////////////////////////////////////////////////////////////////
//
//  Simple helper function to return a random number between 0.0f and 1.0f.
//
////////////////////////////////////////////////////////////////////////////////
float getRand() {
   	return rand() / (float)RAND_MAX;
}

// drawGrid() //////////////////////////////////////////////////////////////////
//
//  Function to draw a grid in the XZ-Plane using OpenGL 2D Primitives (GL_LINES)
//
////////////////////////////////////////////////////////////////////////////////
void drawGrid() {
    /*
     *	We will get to why we need to do this when we talk about lighting,
     *	but for now whenever we want to draw something with an OpenGL
     *	Primitive - like a line, quad, point - we need to disable lighting
     *	and then reenable it for use with the GLUT 3D Primitives.
     */
    glDisable( GL_LIGHTING );

    /** TODO #3: DRAW A GRID IN THE XZ-PLANE USING GL_LINES **/
	for (int i = -50; i <= 50; i++) {
		glColor3f(1, 1, 1);
		glBegin(GL_LINES);
		glVertex3f(-50, 0, i);
		glVertex3f(50, 0, i);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(i, 0, -50);
		glVertex3f(i, 0, 50);
		glEnd();
	};


    /*
     *	As noted above, we are done drawing with OpenGL Primitives, so we
     *	must turn lighting back on.
     */
    glEnable( GL_LIGHTING );
}

// drawCity() //////////////////////////////////////////////////////////////////
//
//  Function to draw a random city using GLUT 3D Primitives
//
////////////////////////////////////////////////////////////////////////////////
void drawCity() {
	for (int i = -50; i <= 50; i++) {
		for (int j = -50; j <= 50; j++) {
			if ((i % 2 == 0) && (j % 2 == 0) && (getRand() < (float)0.1)) {
				glPushMatrix(); {
					glColor3f(getRand(), getRand(), getRand());
					float height = getRand() * 10;
					if (height < (float)1.0) height = 1.0;
					glTranslatef(i, height/2, j);
					glScalef(1, height, 1);
					glutSolidCube(1);
				}
				glPopMatrix();
			}
		}
	}
			
}

//Draws a square wheel that rotates
void drawWheel() {
	glRotatef(wheelAngle, 0, 0, 1);
	glScalef(1, 1, 0.2);
	glutSolidCube(1);
}

//Draws square wheels for the car that rotate as it moves
void drawWheels() {
	glColor3f(.2, .3, .7);

	//Draw a wheel
	glPushMatrix(); {
		glTranslatef(2, 0, 2);
		drawWheel();
	}
	glPopMatrix();

	//Draw a wheel
	glPushMatrix(); {
		glTranslatef(-2, 0, 2);
		drawWheel();
	}
	glPopMatrix();

	//Draw a wheel
	glPushMatrix(); {
		glTranslatef(2, 0, -2);
		drawWheel();
	}
	glPopMatrix();

	//Draw a wheel
	glPushMatrix(); {
		glTranslatef(-2, 0, -2);
		drawWheel();
	}
	glPopMatrix();
}

//Draws the vehicle for the hero
void drawHero() {

	drawWheels();

	//Draw the body
	glPushMatrix(); {
		glColor3f(.5, 1, .2);
		glScalef(4, .5, 4);
		glutSolidCube(1);
	}
	glPopMatrix();

	//Draw a badass teapot at the front for style points
	glPushMatrix(); {
		glColor3f(1, 0, 0);
		glTranslatef(1.5, 1, 0);
		glutSolidTeapot(1);
	}
	glPopMatrix();
}

// generateEnvironmentDL() /////////////////////////////////////////////////////
//
//  This function creates a display list with the code to draw a simple 
//      environment for the user to navigate through.
//
//  And yes, it uses a global variable for the display list.
//  I know, I know! Kids: don't try this at home. There's something to be said
//      for object-oriented programming after all.
//
////////////////////////////////////////////////////////////////////////////////
void generateEnvironmentDL() {
    // TODO #1 Create a Display List & Call our Drawing Functions
	environmentDL = glGenLists(1);
	glNewList(environmentDL, GL_COMPILE);

	drawCity();
	drawGrid();

	glEndList();
}

// recomputeOrientation() //////////////////////////////////////////////////////
//
// This function updates the camera's position in cartesian coordinates based 
//  on its position in spherical coordinates. Should be called every time 
//  cameraTheta, cameraPhi, or cameraRadius is updated. 
//
////////////////////////////////////////////////////////////////////////////////
void recomputeOrientation() {

	cameraX = radius*sin(cameraTheta)*sin(cameraPhi) + heroX;
	cameraY = radius*cos(cameraPhi) + heroY;
	cameraZ = radius*-cos(cameraTheta)*sin(cameraPhi) + heroZ;

}

// resizeWindow() //////////////////////////////////////////////////////////////
//
//  GLUT callback for window resizing. Resets GL_PROJECTION matrix and viewport.
//
////////////////////////////////////////////////////////////////////////////////
void resizeWindow(int w, int h) {
    aspectRatio = w / (float)h;

    windowWidth = w;
    windowHeight = h;

    //update the viewport to fill the window
    glViewport(0, 0, w, h);

    //update the projection matrix with the new window properties
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,aspectRatio,0.1,100000);
}

// mouseCallback() /////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse clicks. We save the state of the mouse button
//      when this is called so that we can check the status of the mouse
//      buttons inside the motion callback (whether they are up or down).
//
////////////////////////////////////////////////////////////////////////////////
void mouseCallback(int button, int state, int thisX, int thisY) {
    // update the left mouse button states, if applicable
	if (button == GLUT_LEFT_BUTTON) {
		leftMouseButton = state;
		mouseX = thisX;
		mouseY = thisY;
	}
}

// mouseMotion() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for mouswe movement. We update cameraPhi, cameraTheta, and/or
//      cameraRadius based on how much the user has moved the mouse in the
//      X or Y directions (in screen space) and whether they have held down
//      the left or right mouse buttons. If the user hasn't held down any
//      buttons, the function just updates the last seen mouse X and Y coords.
//
////////////////////////////////////////////////////////////////////////////////
void mouseMotion(int x, int y) {
	//Checks if the mouse is being pressed so we can rotate the camera
    if(leftMouseButton == GLUT_DOWN && glutGetModifiers() != GLUT_ACTIVE_CTRL) {
		cameraTheta += (mouseX - x)*0.005;
		cameraPhi -= (mouseY - y)*0.005;
		if (cameraPhi < 0) cameraPhi = 0.01;
		if (cameraPhi > M_PI) cameraPhi = M_PI - 0.01;
		mouseX = x;
		mouseY = y;
	}
	//If mouse is clicked and ctrl is pressed, change the camera zoom instead
	else {
		radius += (mouseX - x)*0.05;
		radius -= (mouseY - y)*0.05;
		mouseX = x;
		mouseY = y;
		if (radius < 3) radius = 3;
		if (radius > 100) radius = 100;
	}
	recomputeOrientation(); // update camera (x,y,z) based on (radius,theta,phi)
	glutPostRedisplay(); // redraw our scene from our new camera POV
}

//Draws green spheres on the control points
void drawControlPoints() {
	for (int i = 0; i < controlPoints.size(); i++) {
		//Draw a green circle at each point
		glPushMatrix(); {
			glColor3f(0, 1, 0);
			glTranslatef(controlPoints[i].getX(), controlPoints[i].getY(), controlPoints[i].getZ());
			glutSolidSphere(0.5, 10, 10);
		} glPopMatrix();
	}
}

//Draws the cage of the points
void drawTheCage() {
	glLineWidth(3.0f);
	glColor3f(1, 1, 0);
	for (int i = 0; i < controlPoints.size() - 1; i++) {
		//Draw a yellow line between the points
		glBegin(GL_LINES);
		glVertex3f(controlPoints[i].getX(), controlPoints[i].getY(), controlPoints[i].getZ());
		glVertex3f(controlPoints[i + 1].getX(), controlPoints[i + 1].getY(), controlPoints[i + 1].getZ());
		glEnd();
	}
}

// evaluateBezierCurve() ////////////////////////////////////////////////////////
//
// Computes a location along a Bezier Curve. 
//
////////////////////////////////////////////////////////////////////////////////
Point evaluateBezierCurve(Point p0, Point p1, Point p2, Point p3, float t) {
	//Just use the formula
	Point thePoint = (1 - t)*(1 - t)*(1 - t)*p0 + 3 * (1 - t)*(1 - t)*t*p1 + 3 * (1 - t)*t*t*p2 + t*t*t*p3;
	return thePoint;
}

// renderBezierCurve() //////////////////////////////////////////////////////////
//
// Responsible for drawing a Bezier Curve as defined by four control points.
//  Breaks the curve into n segments as specified by the resolution. 
//
////////////////////////////////////////////////////////////////////////////////
void renderBezierCurve(Point p0, Point p1, Point p2, Point p3, int resolution) {
	//For each curve, we will draw resolution # of little lines
	for (float i = 0; i < resolution; i++) {
		glBegin(GL_LINES);
		Point point1 = evaluateBezierCurve(p0, p1, p2, p3, i / resolution);
		Point point2 = evaluateBezierCurve(p0, p1, p2, p3, (i + 1) / resolution);
		glVertex3f(point1.getX(), point1.getY(), point1.getZ());
		glVertex3f(point2.getX(), point2.getY(), point2.getZ());
		glEnd();
	}
}

//Draws the Bezier curve
void drawTheCurve() {
	glLineWidth(3.0f);
	glColor3f(0, 0, 1);
	for (int i = 0; i < controlPoints.size() - 1; i += 3) {
		//Draw the curve, using 4 points at a time
		renderBezierCurve(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], 10);
	}
}

//Draws spinning teapots around the pet
void drawSpinningTeapots() {
	//Draw first teapot
	glPushMatrix(); {
		glColor3f(0, 1, 0);
		glTranslatef(1, 1, 0);
		glRotatef(3.6*countingTimer, 0, 1, 0);
		glScalef(0.2, 0.2, 0.2);
		glutSolidTeapot(1);
	}
	//Draw second teapot
	glPushMatrix(); {
		glColor3f(0, 0, 1);
		glTranslatef(-1, 1, 0);
		glRotatef(3.6*countingTimer, 1, 0, 0);
		glScalef(0.2, 0.2, 0.2);
		glutSolidTeapot(1);
	}
	//Draw third teapot
	glPushMatrix(); {
		glColor3f(0, 1, 1);
		glTranslatef(-1, -1, 0);
		glRotatef(3.6*countingTimer, 1, 1, 0);
		glScalef(0.2, 0.2, 0.2);
		glutSolidTeapot(1);
	}
	//Draw fourth teapot
	glPushMatrix(); {
		glColor3f(0, .4, .8);
		glTranslatef(1, -1, 0);
		glRotatef(3.6*countingTimer, 0, 1, 1);
		glScalef(0.2, 0.2, 0.2);
		glutSolidTeapot(1);
	}
}

//Draws the pet floating around the hero
void drawPet() {

	glPushMatrix(); {
		//Center the pet on the Curve
		Point petLocation = evaluateBezierCurve(controlPoints[curveNumber], controlPoints[curveNumber + 1], controlPoints[curveNumber + 2], controlPoints[curveNumber + 3], countingTimer / 200);
		glTranslatef(petLocation.getX(), petLocation.getY(), petLocation.getZ());

		//Draw the spinning teapots around him
		drawSpinningTeapots();

		//Draw his body!
		glColor3f(1, .4, .7);
		glutSolidSphere(0.5, 10, 10);
	} glPopMatrix();
}

// initScene() /////////////////////////////////////////////////////////////////
//
//  A basic scene initialization function; should be called once after the
//      OpenGL context has been created. Doesn't need to be called further.
//
////////////////////////////////////////////////////////////////////////////////
void initScene()  {
    glEnable(GL_DEPTH_TEST);

    //******************************************************************
    // this is some code to enable a default light for the scene;
    // feel free to play around with this, but we won't talk about
    // lighting in OpenGL for another couple of weeks yet.
    float lightCol[4] = { 1, 1, 1, 1};
    float ambientCol[4] = { 0.0, 0.0, 0.0, 1.0 };
    float lPosition[4] = { 10, 10, 10, 1 };
    glLightfv( GL_LIGHT0, GL_POSITION,lPosition );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,lightCol );
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
	
    // tell OpenGL not to use the material system; just use whatever we 
    // pass with glColor*()
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    //******************************************************************

    glShadeModel(GL_FLAT);

    srand( time(NULL) );	// seed our random number generator
    generateEnvironmentDL();
}

// renderScene() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for scene rendering. Sets up the modelview matrix, renders
//      a scene to the back buffer, and switches the back buffer with the
//      front buffer (what the user sees).
//
////////////////////////////////////////////////////////////////////////////////
void renderScene(void)  {
    //clear the render buffer
    glDrawBuffer( GL_BACK );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //update the modelview matrix based on the camera's position
    glMatrixMode(GL_MODELVIEW);              //make sure we aren't changing the projection matrix!
    glLoadIdentity();

    gluLookAt( cameraX, cameraY, cameraZ,     
                heroX,  heroY,  heroZ,   	// camera is looking at hero
                0,  1,  0);     // up vector is (0,1,0) (positive Y)


	glCallList(environmentDL);

	//Draw the hero based on where he should be with his proper orientation
	glPushMatrix(); {
		glTranslatef(heroX, 0.5, heroZ);
		glRotatef(heroTheta, 0, 1, 0);
		if (drawCage) drawControlPoints();
		if (drawCage) drawTheCage();
		if (drawCurve) drawTheCurve();
		drawHero();
		drawPet();
	} glPopMatrix();


    //push the back buffer to the screen
    glutSwapBuffers();
}


// normalKeysDown() ////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback; gets called when the user presses a key.
//
////////////////////////////////////////////////////////////////////////////////
void normalKeysDown(unsigned char key, int x, int y) {
    if(key == 'q' || key == 'Q' || key == 27)
        exit(0);

	if (key == 'w') {
		heroX += 1*cos(heroTheta * M_PI / 180);
		heroZ -= 1*sin(heroTheta * M_PI / 180);
		wheelAngle -= 10;
	}
	if (key == 's') {
		heroX -= 1 * cos(heroTheta * M_PI / 180);
		heroZ += 1 * sin(heroTheta * M_PI / 180);
		wheelAngle += 10;
	}
	if (heroX > 48) heroX = 48;
	if (heroX < -48) heroX = -48;
	if (heroZ > 48) heroZ = 48;
	if (heroZ < -48) heroZ = -48;
	if (key == 'a') heroTheta += 1.5;
	if (key == 'd') heroTheta -= 1.5;
	recomputeOrientation();
    glutPostRedisplay();		// redraw our scene from our new camera POV
}

void myTimer(int value) {
	// redraw our display
	glutPostRedisplay();
	// register a new timer callback
	glutTimerFunc(1000.0f / 60.0f, myTimer, 0);
	countingTimer += 1;
	if (countingTimer >= 200) {
		countingTimer = 0;
		curveNumber += 3;
		//Reset to beginning of curve if we reached the end
		if (curveNumber + 1 >= controlPoints.size()) curveNumber = 0;
	}
}

void myMenu(int value) {
	// handle our menu options
	if (value == 0) drawCage = !drawCage;
	if (value == 1) drawCurve = !drawCurve;
	if (value == 2) exit(0);
}

// createMenus() ///////////////////////////////////////////////////////////////
//
//  Handles creating a menu, adding menu entries, and attaching the menu to
//  a mouse button
//
////////////////////////////////////////////////////////////////////////////////
void createMenus() {
	// TODO #01: Create a Simple Menu
	glutCreateMenu(myMenu);
	glutAddMenuEntry("Display/Hide Control Cage", 0);
	glutAddMenuEntry("Display/Hide Bezier Curve", 1);
	glutAddMenuEntry("Quit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// loadControlPoints() /////////////////////////////////////////////////////////
//
//  Load our control points from file and store them in a global variable.
//
////////////////////////////////////////////////////////////////////////////////
bool loadControlPoints(char* filename) {
	// TODO #04: read in control points from file.  Make sure the file can be
	// opened and handle it appropriately.
	ifstream infile(filename);
	//Check to see if the file exists
	if (infile) {
		string line;
		//Read line by line
		while (getline(infile, line)) {
			//Make sure the line has commas so we have a point
			if (line.find(",") != string::npos) {
				float x, y, z;
				istringstream ss(line);
				string data;

				int i = 0;
				while (getline(ss, data, ',')) {
					if (i == 0) x = atof(data.c_str());
					if (i == 1) y = atof(data.c_str());
					if (i == 2) z = atof(data.c_str());
					i++;
				}
				//Add the point to the vector
				controlPoints.push_back(Point(x, y, z));
			}
		}
		infile.close();
	}

	else {
		return false;
	}
	return true;
}

// main() //////////////////////////////////////////////////////////////////////
//
//  Program entry point. Does not process command line arguments.
//
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    // create a double-buffered GLUT window at (50,50) with predefined windowsize
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("Teapot Destruction Derby with PET");

	if (!loadControlPoints(argv[1])) {
		fprintf(stdout, "Error: Input file not found\n");
	}

    // give the camera a scenic starting point.
    cameraX = 60;
    cameraY = 40;
    cameraZ = 30;
	heroX = 0;
	heroY = 0;
	heroZ = 0;
	heroTheta = 0;
    cameraTheta = -M_PI / 3.0f;
    cameraPhi = M_PI / 2.5f;
    recomputeOrientation();

	// create our menu options and attach to mouse button
	createMenus();

    // register callback functions...
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    glutKeyboardFunc(normalKeysDown);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(resizeWindow);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);
	glutTimerFunc(1000.0f / 60.0f, myTimer, 0);

    // do some basic OpenGL setup
    initScene();

    // and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
