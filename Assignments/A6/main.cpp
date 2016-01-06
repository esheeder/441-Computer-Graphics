/*
 *  CSCI 441, Computer Graphics, Fall 2015
 *
 *  Project: Assignment 6
 *  File: main.cpp
 *
 *  Description:
 *      This file contains the implementation of a simple 3D environment, with 
 *  a user-controllable object that is followed by an arcball camera.
 *
 *      Of particular use, it uses the Object class to load in a simple WaveFront 
 *  object file and render it to the world.
 *
 *  Author:
 *		Eric Sheeder
 *  
 *  Notes:
 *
 */

////////////////////////////////////THIS IS THE RIGHT MAIN

#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/glew.h>
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <SOIL/SOIL.h>  // Simple OpenGL Image Library

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Object.h"
#include "Particle.h"
#include "ParticleSystem.h"

using namespace std;

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

//global variables to keep track of window width and height.
//set to initial values for convenience, but we need variables
//for later on in case the window gets resized.
int windowWidth = 512, windowHeight = 512;

GLint leftMouseButton = GLUT_UP, rightMouseButton = GLUT_UP;    //status of the mouse buttons
int mouseX = 0, mouseY = 0;                 //last known X and Y of the mouse

float cameraTheta, cameraPhi, cameraRadius; //camera position in spherical coordinates
float cameraX, cameraY, cameraZ;            //camera position in cartesian coordinates
float objX, objY, objZ;                     //object position in world coordinates

float lPosition[4] = { 10, 10, 10, 1 };		//light position in world coordinates

enum LightType { POINT_LIGHT, DIRECTIONAL_LIGHT, SPOT_LIGHT };		// our types of lights
LightType lightType = POINT_LIGHT;				// current light type

Object *obj;                                // actual object

GLuint environmentDL;            // display list for the sky

GLuint texture[8];
GLuint shaderProgramHandle;
float wheelAngle = 0;

vector<ParticleSystem> fountains;

// END GLOBAL VARIABLES ///////////////////////////////////////////////////////

//
// void computeArcballPosition(float theta, float phi, float radius,
//                              float &x, float &y, float &z);
//
// This function updates the camera's position in cartesian coordinates based 
//  on its position in spherical coordinates. The user passes in the current phi,
//  theta, and radius, as well as variables to hold the resulting X, Y, and Z values.
//  Those variables for X, Y, and Z values get filled with the camera's position in R3.
//
void computeArcballPosition(float theta, float phi, float radius,
                            float &x, float &y, float &z) {
    x = radius * sinf(theta)*sinf(phi);
    y = radius * -cosf(phi);
    z = radius * -cosf(theta)*sinf(phi);
}

//
//  void drawGrid()
//
//      Simple function to draw a grid in the X-Z plane using GL_LINES.
//
void drawGrid() {

	glDisable(GL_LIGHTING);
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

	glEnable(GL_LIGHTING);
}

//Draws the skybox
void drawSky() {
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix(); {
		//Draw top part
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-256, 256, 256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-256, 256, -256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(256, 256, -256);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(256, 256, 256);
			glEnd();
		} glPopMatrix();
		//Draw left part
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(256, 256, -256);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(256, -256, -256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-256, -256, -256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-256, 256, -256);
			glEnd();
		} glPopMatrix();
		//Draw front part
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(256, 256, 256);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(256, -256, 256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(256, -256, -256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(256, 256, -256);
			glEnd();
		} glPopMatrix();
		//Draw right part
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-256, 256, 256);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-256, -256, 256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(256, -256, 256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(256, 256, 256);
			glEnd();
		} glPopMatrix();
		//Draw back part
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-256, 256, -256);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-256, -256, -256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-256, -256, 256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-256, 256, 256);
			glEnd();
		} glPopMatrix();
		//Draw bottom part
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-256, -256, 256);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-256, -256, -256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(256, -256, -256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(256, -256, 256);
			glEnd();
		} glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	} glPopMatrix();
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

void generateEnvironmentDL() {
	//Create a Display List & Call our Drawing Functions
	environmentDL = glGenLists(1);
	glNewList(environmentDL, GL_COMPILE);
	drawSky();
	glEndList();
}

void loadTextures() {
	texture[0] = SOIL_load_OGL_texture("textures/peashooter.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	texture[1] = SOIL_load_OGL_texture("textures/skybox/top.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS	| SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	texture[2] = SOIL_load_OGL_texture("textures/skybox/left.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	texture[3] = SOIL_load_OGL_texture("textures/skybox/front.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	texture[4] = SOIL_load_OGL_texture("textures/skybox/right.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	texture[5] = SOIL_load_OGL_texture("textures/skybox/back.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	texture[6] = SOIL_load_OGL_texture("textures/skybox/bottom.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
	//5.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//6.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

//
//  void resizeWindow(int w, int h)
//
//      We will register this function as GLUT's reshape callback.
//   When the window is resized, the OS will tell GLUT and GLUT will tell
//   us by calling this function - GLUT will give us the new window width
//   and height as 'w' and 'h,' respectively - so save those to our global vars.
//
void resizeWindow(int w, int h) {
    float aspectRatio = w / (float)h;

    windowWidth = w;
    windowHeight = h;

    //update the viewport to fill the window
    glViewport(0, 0, w, h);

    //update the projection matrix with the new window properties
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,aspectRatio,0.1,100000);
}

//
//  void initScene()
//
//  A basic scene initialization function; should be called once after the
//      OpenGL context has been created. Doesn't need to be called further.
//      Just sets up a few function calls so that our main() is cleaner.
//
void initScene() {
	// enable depth testing 
    glEnable(GL_DEPTH_TEST);

	// enable lighting
	glEnable(GL_LIGHTING);
	// enable light 0
    glEnable(GL_LIGHT0);

	// set the values for each component of lighting
    float diffuseLightCol[4] = { 1.0, 1.0, 1.0, 1 };	// white light
	float specularLightCol[4] = { 1.0, 1.0, 1.0, 1 };	// white light
    float ambientCol[4] = { 1, 1, 1, 1.0 };		// set this to be very low
	//float ambientCol[4] = { 0.1, 0.1, 0.1, 1.0 };		// set this to be very low
	// and now set each value
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLightCol );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularLightCol );
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
	
	// by default, OpenGL has a little bit of ambient light throughout the scene {0.2}
	// override this value to set the ambient scene light to black
	float black[4] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, black );
	
    //tells OpenGL to blend colors across triangles. Once lighting is
    //enabled, this means that objects will appear smoother - if your object
    //is rounded or has a smooth surface, this is probably a good idea;
    //if your object has a blocky surface, you probably want to disable this.
    glShadeModel(GL_SMOOTH);
	generateEnvironmentDL();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//
//  void renderScene()
//
//  GLUT callback for scene rendering. Sets up the modelview matrix, renders
//      a teapot to the back buffer, and switches the back buffer with the
//      front buffer (what the user sees).
//
void renderScene(void) {
	//clear the render buffer
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawBuffer( GL_BACK );

    //update the modelview matrix based on the camera's position
    glMatrixMode(GL_MODELVIEW);             //make sure we aren't changing the projection matrix!
    glLoadIdentity();

    gluLookAt(cameraX+objX, cameraY+objY, cameraZ+objZ,     //camera is located at (x,y,z)
                objX, objY, objZ,                           //camera is looking at (0,0,0)
                0.0f, 1.0f, 0.0f);                          //up vector is (0,1,0) (positive Y)
	
	// position our light
	glLightfv( GL_LIGHT0, GL_POSITION, lPosition );		
	
	// specify each of our material component colors
	GLfloat diffCol[4] = { 0.2, 0.3, 0.6 };					// a nice blue color for diffuse 
	GLfloat specCol[4] = { 1.0, 1.0, 0.0 };					// yellow specular highlights
	GLfloat ambCol[4] = { 0.8, 0.8, 0.8 };					// this value can be large because the final value
															// of our ambient light will be this * lightAmbient


	// and now set them for the front and back faces
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffCol );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specCol );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 96.0 );		// as well as the shininess - this value ranges between 0 & 90
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambCol );

	//Draw the particles
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	for (int i = 0; i < fountains.size(); i++) {
		fountains.at(i).drawParticles(cameraX, cameraZ);
	}
	//glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

    glPushMatrix(); {
        glTranslatef( objX, objY, objZ);
		drawHero();
    }; glPopMatrix();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix(); {
		glTranslatef(5, 0, 5);
		glUseProgram(shaderProgramHandle);
		obj->draw();
		glUseProgram(0);
	}; glPopMatrix();
	glDisable(GL_TEXTURE_2D);



    //move the grid down a touch so that it doesn't overlap with the axes
    glPushMatrix();
        glTranslatef(0,-1,0);
        drawGrid();
    glPopMatrix();



	glCallList(environmentDL);

    //push the back buffer to the screen
    glutSwapBuffers();
}

void registerTransparentOpenGLTexture(unsigned char *imageData, unsigned char *imageMask, int texWidth, int texHeight, GLuint &texHandle) {
	// combine the 'mask' array with the image data array into an RGBA array.
	unsigned char *fullData = new unsigned char[texWidth*texHeight * 4];
	for (int j = 0; j < texHeight; j++) {
		for (int i = 0; i < texWidth; i++) {
			fullData[(j*texWidth + i) * 4 + 0] = imageData[(j*texWidth + i) * 3 + 0];	// R
			fullData[(j*texWidth + i) * 4 + 1] = imageData[(j*texWidth + i) * 3 + 1];	// G
			fullData[(j*texWidth + i) * 4 + 2] = imageData[(j*texWidth + i) * 3 + 2];	// B
			fullData[(j*texWidth + i) * 4 + 3] = imageMask[(j*texWidth + i) * 3 + 0];	// A
		}
	}

	// first off, get a real texture handle.
	glGenTextures(1, &texHandle);

	// make sure that future texture functions affect this handle
	glBindTexture(GL_TEXTURE_2D, texHandle);

	// set this texture's color to be multiplied by the surface colors --
	//  GL_MODULATE instead of GL_REPLACE allows us to take advantage of OpenGL lighting
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// set the minification and magnification functions to be linear; not perfect
	//  but much better than nearest-texel (GL_NEAREST).
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set the texture to repeat in S and T -- though it doesn't matter here
	//  because our texture coordinates are always in [0,0] to [1,1].
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// actually transfer the texture to the GPU and use MipMaps!!
	/*    gluBuild2DMipMaps(
	GL_TEXTURE_2D,              //still working with 2D textures, obv.
	GL_RGBA,                    //we're going to provide OpenGL with R, G, B, and A components...
	texWidth,                   //...of this width...
	texHeight,                  //...and this height.
	GL_RGBA,                    //and store it, internally, as RGBA (OpenGL will convert to floats between 0.0 and 1.0f)
	GL_UNSIGNED_BYTE,           //this is the format our data is in, and finally,
	fullData);                  //there's the data!
	*/
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texWidth, texHeight, GL_RGBA, GL_UNSIGNED_BYTE, fullData);

	// clean up our memory on the CPU side since it's already been transfered to the GPU
	delete fullData;
}


//
//  void normalKeys(unsigned char key, int x, int y)
//
//      GLUT keyboard callback for regular characters.
//
void normalKeys(unsigned char key, int x, int y) {
    //kindly quit if the user requests!
    if(key == 'q' || key == 'Q')
        exit(0);
	
	switch( key ) {
		case 'd':
		case 'D':
			objX += 0.1f;
			break;
			
		case 'a':
		case 'A':
			objX -= 0.1f;
			break;
			
		case 'w':
		case 'W':
			objZ -= 0.1f;
			break;
			
		case 's':
		case 'S':
			objZ += 0.1f;
			break;
			
		case 'l':
		case 'L':
			// change our lighting type
			if( lightType == POINT_LIGHT ) {		// if a point light
				// change to directional light
				lightType = DIRECTIONAL_LIGHT;
				
				// set the direction of the light
				lPosition[0] = 3.0;
				lPosition[1] = -4.0;
				lPosition[2] = -5.0;
				lPosition[3] = 0.0;	// note this is 0
				
				// we mostly need to set the pos[3] "w" value to 0.0 to signify it is a vector
				// the position will be set in the renderScene() method
			} else if( lightType == DIRECTIONAL_LIGHT ) {	// if a directional light
				// change to spot light
				lightType = SPOT_LIGHT;
				
				// set the position of the light
				lPosition[0] = -4.0;
				lPosition[1] = 2.0;
				lPosition[2] = -3.0;
				lPosition[3] = 1.0;	// note this is 1
				// the position will be set in the renderScene() method
				
				// set the direction of the light
				GLfloat spotDirection[4] = { 4.0, -2.0, 3.0, 0.0 };
				glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection );
				
				// set the angle of our cone in degrees
				glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 30 );	// the cone will be double this value so 60 degrees
															// acceptable values are between 0 and 90
															// or the special value of 180
				// set the exponent of our falloff
				glLightf( GL_LIGHT0, GL_SPOT_EXPONENT, 30 );// acceptable values are between 0 and 128
			} else if( lightType == SPOT_LIGHT ) {	// if a spot light
				// change to point light
				lightType = POINT_LIGHT;
				
				// set the position of the light
				lPosition[0] = 10.0;
				lPosition[1] = 10.0;
				lPosition[2] = 10.0;
				lPosition[3] = 1.0;	// note this is 1
				
				// return the angle of our cone back to 180 to signify back to point light
				// 180 is the default value
				glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 180 );
				// return the falloff to 0 which is the default value
				glLightf( GL_LIGHT0, GL_SPOT_EXPONENT, 0 );
			}
			break;
	}
}

//
//  void mouseCallback(int button, int state, int thisX, int thisY)
//
//  GLUT callback for mouse clicks. We save the state of the mouse button
//      when this is called so that we can check the status of the mouse
//      buttons inside the motion callback (whether they are up or down).
//
void mouseCallback(int button, int state, int thisX, int thisY) {
	//update the left and right mouse button states, if applicable
    if(button == GLUT_LEFT_BUTTON) {
        leftMouseButton = state;
    } else if(button == GLUT_RIGHT_BUTTON) {
        rightMouseButton = state;
	}
    
    //and update the last seen X and Y coordinates of the mouse
    mouseX = thisX;
    mouseY = thisY;
}

//
//  void mouseMotion(int x, int y)
//
//  GLUT callback for mouse movement. We update cameraPhi, cameraTheta, and/or
//      cameraRadius based on how much the user has moved the mouse in the
//      X or Y directions (in screen space) and whether they have held down
//      the left or right mouse buttons. If the user hasn't held down any
//      buttons, the function just updates the last seen mouse X and Y coords.
//
void mouseMotion(int x, int y)
{
    //mouse is moving; if left button is held down...
    if(leftMouseButton == GLUT_DOWN ) {
		
		//update theta and phi! 
		cameraTheta += (x - mouseX)*0.005;
		cameraPhi   += (y - mouseY)*0.005;
		
		// we don't care if theta goes out of bounds; it'll just loop around.
		// phi, though.. it'll flip out if it flips over top. keep it in (0, M_PI)
		if(cameraPhi <= 0)
			cameraPhi = 0+0.001;
		if(cameraPhi >= M_PI)
			cameraPhi = M_PI-0.001;
        
		//update camera (x,y,z) based on (radius,theta,phi)
		computeArcballPosition(cameraTheta, cameraPhi, cameraRadius,
							   cameraX, cameraY, cameraZ);
	}
	else if( rightMouseButton == GLUT_DOWN ) {
		//for the right mouse button, just determine how much the mouse has moved total.
		//not the best "zoom" behavior -- if X change is positive and Y change is negative,
		//(along the X = Y line), nothing will happen!! but it's ok if you zoom by
		//moving left<-->right or up<-->down, which works for most people i think.
		double totalChangeSq = (x - mouseX) + (y - mouseY);
		cameraRadius += totalChangeSq*0.01;
		
		//limit the camera radius to some reasonable values so the user can't get lost
		if(cameraRadius < 2.0) 
			cameraRadius = 2.0;
		if(cameraRadius > 50.0) 
			cameraRadius = 50.0;
		
		//update camera (x,y,z) based on (radius,theta,phi)
		computeArcballPosition(cameraTheta, cameraPhi, cameraRadius,
							   cameraX, cameraY, cameraZ);
	}
	
    //and save the current mouseX and mouseY.
    mouseX = x;
    mouseY = y;
}

//
// void myTimer(int value)
//
//  We have to take an integer as input per GLUT's timer function prototype;
//  but we don't use that value so just ignore it. We'll register this function
//  once at the start of the program, and then every time it gets called it
//  will perpetually re-register itself and tell GLUT that the screen needs
//  be redrawn. yes, I said "needs be."
//
void myTimer(int value) {
	for (int i = 0; i < fountains.size(); i++) {
		fountains.at(i).updateParticles(cameraX, cameraY, cameraZ);
	}

    glutPostRedisplay();

    glutTimerFunc((unsigned int)(1000.0 / 60.0), myTimer, 0);
}


void readInputFile(char* filename) {
	ifstream infile(filename);

	//Check to see if the file exists
	if (infile) {
		string line;

		//Read line by line
		while (getline(infile, line)) {
			//Reads the line of data and stores the x, y, and z values to a point object
			float x, y, z, angle, minVel, maxVel, minLife, maxLife, spawnRate;
			istringstream ss(line);
			string data;

			int i = 0;
			while (getline(ss, data, ',')) {
				if (i == 0) x = atof(data.c_str());
				if (i == 1) y = atof(data.c_str());
				if (i == 2) z = atof(data.c_str());
				if (i == 3) angle = atof(data.c_str());
				if (i == 4) minVel = atof(data.c_str());
				if (i == 5) maxVel = atof(data.c_str());
				if (i == 6) minLife = atof(data.c_str());
				if (i == 7) maxLife = atof(data.c_str());
				if (i == 8) spawnRate = atof(data.c_str());
				i++;
			}
			fountains.push_back(ParticleSystem(Point(x, y, z), angle, minVel, maxVel, minLife, maxLife, spawnRate));
		}
	}
	infile.close();
}

//
//  int main(int argc, char **argv)
//
//      No offense but you should probably know what the main function does.
//      argc is the number of arguments to the program (program name included),
//      and argv is an array of strings; argv[i] is the ith command line
//      argument, where 0 <= i <= argc
//
int main(int argc, char **argv) {
    if( argc != 2 ) {
        fprintf( stderr, "Usage: ./A6.exe <input.csv> \n" );
        return -1;
    }

    //create a double-buffered GLUT window at (50,50) with predefined windowsize
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("Wizardry");

    //give the camera a 'pretty' starting point!
    cameraRadius = 12.0f;
    cameraTheta = 2.80;
    cameraPhi = 2.0;

    objX = objY = objZ = 0.0f;
    
    obj = new Object("peashooter.obj");
	
    computeArcballPosition(cameraTheta, cameraPhi, cameraRadius,
						   cameraX, cameraY, cameraZ);
	
	readInputFile(argv[1]);

    //register callback functions...
    glutKeyboardFunc(normalKeys);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(resizeWindow);
	glutMouseFunc( mouseCallback );
	glutMotionFunc( mouseMotion );

	//Loads skybox texture and blue texture
	loadTextures();

	////// LOAD IN OUR TRANSPARENT TEXTURES //////////////////////////////////////////////////
	// some relevant variables for the texture files that we won't need later...so don't make them global
	int rgbWidth, rgbHeight, alphaWidth, alphaHeight, channels;
	unsigned char *treeTexData, *treeMaskData;

	treeTexData = SOIL_load_image("textures/treeDiff.png",
		&rgbWidth, &rgbHeight, &channels,
		SOIL_LOAD_AUTO);
	treeMaskData = SOIL_load_image("textures/treeOpa.jpg",
		&alphaWidth, &alphaHeight, &channels,
		SOIL_LOAD_AUTO);
	registerTransparentOpenGLTexture(treeTexData, treeMaskData, rgbWidth, rgbHeight, texture[7]);

    //do some basic OpenGL setup to initialize our lights
    initScene();
	
	// register our timer function
    glutTimerFunc((unsigned int)(1000.0 / 60.0), myTimer, 0);

	//Shader stuff
	GLenum err = glewInit();
	GLuint vertexSH = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentSH = glCreateShader(GL_FRAGMENT_SHADER);

	string vertexSource = "void main() {gl_Position = gl_ProjectionMatrix *gl_ModelViewMatrix *gl_Vertex;}";
	string fragmentSource = "void main() {gl_FragColor = gl_Color;}";
	const char * fragmentString = fragmentSource.c_str();
	const char * vertexString = vertexSource.c_str();

	glShaderSource(vertexSH, 1, &vertexString, NULL);
	glShaderSource(fragmentSH, 1, &fragmentString, NULL);

	glCompileShader(vertexSH);
	glCompileShader(fragmentSH);

	shaderProgramHandle = glCreateProgram();

	glAttachShader(shaderProgramHandle, vertexSH);
	glAttachShader(shaderProgramHandle, fragmentSH);

	glLinkProgram(shaderProgramHandle);

    //and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
