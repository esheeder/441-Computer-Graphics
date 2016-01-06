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
 *	input2.csv
 */

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>

	#include <ALUT/alut.h>  // OpenAL Headers
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/freeglut.h>
	#include <AL/alut.h>    // OpenAL Headers
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <cstdlib>



// C++ Libraries we'll use
#include <fstream>			// we'll use ifstream	
#include <string>			// for, well strings!
#include <vector>			// and vectors (the storage container, not directional)
#include <map>

//headers we've written
#include "Point.h"
#include "BezierSurface.h"
#include "Camera.h"
#include "notsure.h"
#include "Truck.h"
#include "SirVive.h"
#include "MogdinBaramud.h"
#include "Light.h"
#include "Material.h"
#include "CameraController.h"
#include "ArcballController.h"
#include "FreeCamController.h"
#include "FirstPersonController.h"
#include "Viewport.h"
#include "BezierCurve.h"

using namespace std;

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

static size_t windowWidth  = 640;
static size_t windowHeight = 480;

int timer = 0;
clock_t endTime;
double time1;
clock_t startTime;
double FPS = 0.0;
string sFPS = "";
int frames = 0;
const size_t NUM_HEROES = 4;
const int MAP_SIZE = 240;

//Create a place to store the heroes
Hero *heroes[NUM_HEROES];
float heroScaleFactor[NUM_HEROES] = { 1.0, 0.5, 0.5, 0.5 }; //Contains the factors to scale down each hero when we draw it

//Resolution variables for Bezier surfaces. Higher number = finer resolution
float bezierU = 90, bezierV = 90;
float fps;
const float WIND_TURBINE_HEIGHT = 20;



/////////////////////
GLfloat texpts[2][2][2] = { {{0.0, 0.0}, { 0.0, 1.0 }}, {{ 1.0, 0.0 }, { 1.0, 1.0 }}};


vector<Point> heroTrack;	//Stores the control points for the track
vector<Point> parametric;	//Stores the Bezier curve points for the hero traveling at equal parametric intervals
vector<Point> arcLength; //Stores the Bezier curve points for the hero traveling at equal arc length intervals
vector<BezierSurface> surfaces;	//Holds the surfaces loaded in from the input file

vector<Point> windTurbines; //Holds location and angle values for windmills (since windmills are all same height, y location stores angle of rotation instead of height)
vector<Point> blocks; //Holds location for the blocks. In this case, a y value of 0 means the block is drawn normally, and any other value means the block is drawn upside down

GLuint texture[7];

//Arrays storing look-up values of heights and angles from the Bezier Surface. Used for moving car up and down hills and tilting it properly.
float height[MAP_SIZE][MAP_SIZE];
float xAngle[MAP_SIZE][MAP_SIZE];
float zAngle[MAP_SIZE][MAP_SIZE];

float trackPointVal = 0.0f;

bool moggyMoving = false, movingForward = false, movingBackward = false, turningLeft = false, turningRight = false;

// The two camera views. mainCamera is always non-null,
// while firstPersonCamera can be null when there is no first-person view.
std::shared_ptr<CameraController> mainCamera;
std::shared_ptr<CameraController> firstPersonCamera;

// All the cameras available
std::shared_ptr<FreeCamController> freeCam;
std::vector<std::shared_ptr<ArcballController>> arcballCams;
std::vector<std::shared_ptr<FirstPersonController>> firstPersonCams;

// Viewports
std::shared_ptr<Viewport> mainViewport;

std::shared_ptr<Viewport> splitViewport;
std::weak_ptr<Viewport> leftViewport;
std::weak_ptr<Viewport> rightViewport;

BezierCurve heroCurve;

// An enum for the different main menu options
enum MenuOption
{
    QUIT = 0,
    FREE_CAM = 1
};

// Display list for the static portions of the scene
GLuint environmentDL;


#define NUM_BUFFERS 3
#define NUM_SOURCES 3

ALCdevice *device;
ALCcontext *context;
ALuint buffers[NUM_BUFFERS];
ALuint sources[NUM_SOURCES];

// getRand() ///////////////////////////////////////////////////////////////////
//
//  Simple helper function to return a random number between 0.0f and 1.0f.
//
////////////////////////////////////////////////////////////////////////////////
float getRand() {
   	return rand() / (float)RAND_MAX;
}

//Loads our textures into the texture[] array
//Some code taken from here:
//http://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture
//We couldn't get textures to work using glTexImage2D, even using the book's example, but using this example and gluBuild2DMipmaps, our texturing works
void loadTextures() {
	ifstream myfile;
	GLubyte questionmark[64][64][3];
	//Read the image file and store the data in an array
	myfile.open("textures/questionmark.ppm", std::ifstream::in);
	string line;
	int row = 0;
	int col = 0;
	int color = 0;
	int width = 64;
	while (getline(myfile, line))
	{
		questionmark[row][col][color] = (GLubyte) stoi(line);
		color++;
		if (color == 3) {
			col++;
			color = 0;
		}
		if (col == width) {
			row++;
			col = 0;
		}
	}
	myfile.close();

	//Step 2 - Generate texture handle with glGenTextures()
	glGenTextures(1, &texture[0]);
	//Step 3 - Set it to be active with glBindTexture()
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	//Step 4 - Set texture parameters
	//Color calculation - multiply texture color
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Min/mag filters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Step 5 - transfer image data to GPU
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, width, GL_RGB, GL_UNSIGNED_BYTE, questionmark);

	
	//Do it again for skybox top
	GLubyte skybox[512][512][3];

	myfile.open("textures/skybox/top.ppm", std::ifstream::in);
	row = 0;
	col = 0;
	color = 0;
	width = 512;
	while (getline(myfile, line)) {
		skybox[row][col][color] = (GLubyte)stoi(line);
		color++;
		if (color == 3) {
			col++;
			color = 0;
		}
		if (col == width) {
			row++;
			col = 0;
		}
	}
	myfile.close();

	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, width, GL_RGB, GL_UNSIGNED_BYTE, skybox);

	//Do it again for skybox left
	myfile.open("textures/skybox/left.ppm", std::ifstream::in);
	row = 0;
	col = 0;
	color = 0;
	width = 512;
	while (getline(myfile, line)) {
		skybox[row][col][color] = (GLubyte)stoi(line);
		color++;
		if (color == 3) {
			col++;
			color = 0;
		}
		if (col == width) {
			row++;
			col = 0;
		}
	}
	myfile.close();

	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, width, GL_RGB, GL_UNSIGNED_BYTE, skybox);

	//Do it again for skybox front
	myfile.open("textures/skybox/front.ppm", std::ifstream::in);
	row = 0;
	col = 0;
	color = 0;
	width = 512;
	while (getline(myfile, line)) {
		skybox[row][col][color] = (GLubyte)stoi(line);
		color++;
		if (color == 3) {
			col++;
			color = 0;
		}
		if (col == width) {
			row++;
			col = 0;
		}
	}
	myfile.close();

	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, width, GL_RGB, GL_UNSIGNED_BYTE, skybox);

	//Do it again for skybox right
	myfile.open("textures/skybox/right.ppm", std::ifstream::in);
	row = 0;
	col = 0;
	color = 0;
	width = 512;
	while (getline(myfile, line)) {
		skybox[row][col][color] = (GLubyte)stoi(line);
		color++;
		if (color == 3) {
			col++;
			color = 0;
		}
		if (col == width) {
			row++;
			col = 0;
		}
	}
	myfile.close();

	glGenTextures(1, &texture[4]);
	glBindTexture(GL_TEXTURE_2D, texture[4]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, width, GL_RGB, GL_UNSIGNED_BYTE, skybox);

	//Do it again for skybox back
	myfile.open("textures/skybox/back.ppm", std::ifstream::in);
	row = 0;
	col = 0;
	color = 0;
	width = 512;
	while (getline(myfile, line)) {
		skybox[row][col][color] = (GLubyte)stoi(line);
		color++;
		if (color == 3) {
			col++;
			color = 0;
		}
		if (col == width) {
			row++;
			col = 0;
		}
	}
	myfile.close();

	glGenTextures(1, &texture[5]);
	glBindTexture(GL_TEXTURE_2D, texture[5]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, width, GL_RGB, GL_UNSIGNED_BYTE, skybox);

	//Do it again for skybox bottom
	myfile.open("textures/skybox/bottom.ppm", std::ifstream::in);
	row = 0;
	col = 0;
	color = 0;
	width = 512;
	while (getline(myfile, line)) {
		skybox[row][col][color] = (GLubyte)stoi(line);
		color++;
		if (color == 3) {
			col++;
			color = 0;
		}
		if (col == width) {
			row++;
			col = 0;
		}
	}
	myfile.close();

	glGenTextures(1, &texture[6]);
	glBindTexture(GL_TEXTURE_2D, texture[6]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, width, GL_RGB, GL_UNSIGNED_BYTE, skybox);
}

//Draws a face of a ?-block
void drawface() {
	Material whiteMaterial(
		Color(1.0, 1.0, 1.0),
		Color(1.0, 1.0, 1.0),
		Color(1.0, 1.0, 1.0),
		1);
	whiteMaterial.apply();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glPushMatrix(); {
		glTranslatef(0, 0, 0);
		glRotatef(180, 1, 0, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0, -2.5, -2.5);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0, 2.5, -2.5);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 2.5, 2.5);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0, -2.5, 2.5);
		glEnd();
	} glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

//Draws a ?-block
void drawBlock() {

	glPushMatrix(); {
		glTranslatef(2.5, 0, 0);
		drawface();
	} glPopMatrix();

	glPushMatrix(); {
		glRotatef(90, 0, 1, 0);
		glTranslatef(2.5, 0, 0);
		drawface();
	} glPopMatrix();

	glPushMatrix(); {
		glRotatef(180, 0, 1, 0);
		glTranslatef(2.5, 0, 0);
		drawface();
	} glPopMatrix();

	glPushMatrix(); {
		glRotatef(270, 0, 1, 0);
		glTranslatef(2.5, 0, 0);
		drawface();
	} glPopMatrix();

	glPushMatrix(); {
		glRotatef(90, 0, 0, 1);
		glTranslatef(2.5, 0, 0);
	
		drawface();
	} glPopMatrix();

	glPushMatrix(); {
		glRotatef(90, 0, 0, 1);
		glTranslatef(-2.5, 0, 0);
		drawface();
	} glPopMatrix();

}

void drawSky() {
	Material whiteMaterial(
		Color(1.0, 1.0, 1.0),
		Color(1.0, 1.0, 1.0),
		Color(1.0, 1.0, 1.0),
		1);
	whiteMaterial.apply();
	glEnable(GL_TEXTURE_2D);
	glPushMatrix(); {
		glTranslatef(100, 0, 100);
		//Draw top part
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-256, 256, -256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-256, 256, 256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(256, 256, 256);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(256, 256, -256);
			glEnd();
		} glPopMatrix();
		//Draw left part
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(256, -256, -256);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(256, 256, -256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-256, 256, -256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-256, -256, -256);
			glEnd();
		} glPopMatrix();
		//Draw front part
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(256, -256, 256);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(256, 256, 256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(256, 256, -256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(256, -256, -256);
			glEnd();
		} glPopMatrix();
		//Draw right part
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-256, -256, 256);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-256, 256, 256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(256, 256, 256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(256, -256, 256);
			glEnd();
		} glPopMatrix();
		//Draw back part
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-256, -256, -256);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-256, 256, -256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-256, 256, 256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-256, -256, 256);
			glEnd();
		} glPopMatrix();
		//Draw bottom part
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		glPushMatrix(); {
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-256, -256, -256);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-256, -256, 256);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(256, -256, 256);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(256, -256, -256);
			glEnd();
		} glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	} glPopMatrix();
}

//Draws 3 spinning blades for each turbine
void drawTurbineBlades() {
	Material turbineMaterial(
		Color(0.7, 0.7, 0.7),
		Color(0.7, 0.7, 0.7),
		Color(0.70, 0.70, 0.70),
		0.25);
	turbineMaterial.apply();
	//Draw each blade at an angle
	glPushMatrix(); {
		glRotatef(timer*2, 0, 1, 0);
		glutSolidCone(0.2, 4, 20, 10);
	} glPopMatrix();
	glPushMatrix(); {
		glRotatef((timer + 120)*2, 0, 1, 0);
		glutSolidCone(0.2, 4, 20, 10);
	} glPopMatrix();
	glPushMatrix(); {
		glRotatef((timer - 120)*2, 0, 1, 0);
		glutSolidCone(0.2, 4, 20, 10);
	} glPopMatrix();
}

//Draws the base of the wind turbines
void drawWindTurbines() {
	for (unsigned int i = 0; i < windTurbines.size(); i++) {
		glPushMatrix(); {
			Material turbineMaterial(
				Color(0.7, 0.7, 0.7),
				Color(0.7, 0.7, 0.7),
				Color(0.70, 0.70, 0.70),
				0.25);
			turbineMaterial.apply();
			glTranslatef(windTurbines[i].getX(), height[int(windTurbines[i].getX())][int(windTurbines[i].getZ())], windTurbines[i].getZ());
			glRotatef(windTurbines[i].getY(), 0, 1, 0);

			//Draw the base
			glPushMatrix(); {
				glRotatef(-90, 1, 0, 0);
				gluCylinder(gluNewQuadric(), .8, 0.3, WIND_TURBINE_HEIGHT, 20, 10);
			} glPopMatrix();

			//Draw the cylinder on top
			glPushMatrix(); {
				glTranslatef(0, WIND_TURBINE_HEIGHT, -1);
				gluCylinder(gluNewQuadric(), 0.4, 0.4, 2, 20, 10);
			} glPopMatrix();
			Material coneMaterial(
				Color(0.68, 0.85, 0.90),
				Color(0.68, 0.85, 0.90),
				Color(0.68, 0.85, 0.90),
				0.25);
			coneMaterial.apply();
			//Draw the cone on top
			glPushMatrix(); {
				glTranslatef(0, WIND_TURBINE_HEIGHT, 1);
				glutSolidCone(0.4, 2, 20, 10);
			} glPopMatrix();

		} glPopMatrix();
	}
}


void positionListener(const Point& pos, const Point& dir, const Point& up = Point(0, 1, 0))
{
	ALfloat listenerPosition[] = { pos.getX(), pos.getY(), pos.getZ() };
	ALfloat listenerOrientation[] = { dir.getX(), dir.getY(), dir.getZ(),
                                      up.getX(), up.getY(), up.getZ() };

	alListenerfv(AL_POSITION, listenerPosition);
	alListenerfv(AL_ORIENTATION, listenerOrientation);
}


void positionSource(ALuint src, float posX, float posY, float posZ) {

	ALfloat srcPosition[] = { posX,posY,posZ };
	alSourcefv(src, AL_POSITION, srcPosition);
}

void positionSource(ALuint src, const Point& pos)
{
    positionSource(src, pos.getX(), pos.getY(), pos.getZ());
}


//Draws a Bezier Surface and populates our reference array with height values
//This function is called in Generate Environment, once for every surface loaded
void drawSurface(const BezierSurface& someSurface) {
	vector< vector <Point> > temp = someSurface.getAllPoints();

	for (unsigned int i = 0; i < temp.size() - 1; i++) {
		vector<Point> row = temp[i];
		for (unsigned int j = 0; j < row.size() - 1; j++) {

			//Calculate the normal for the quad
			//Get 2 vectors and store the values in point objects
            Point vec2 = row[j + 1] - row[j];
            Point vec1 = temp[i + 1][j] - row[j];

			//Compute cross product of vec1 x vec2
            Point cross = vec1.cross(vec2);
            cross.normalize();

			//Draw Quads
			glPushMatrix(); {
				glBegin(GL_QUADS); {

					Material(
						Color(0, 0, 0),
						Color(0.55, 0.55, 0.55),
						Color(0.7, 0.7, 0.7),
						0.6).apply();

					glNormal(cross);
                    glVertex(row[j]);
					glNormal(cross);
                    glVertex(row[j+1]);
					glNormal(cross);
                    glVertex(temp[i+1][j+1]);
					glNormal(cross);
                    glVertex(temp[i+1][j]);
					
				} glEnd();
			} glPopMatrix();
		}
	}
}

//Calculates the Bezier surfaces and stores the data
void calculateSurface(const BezierSurface& someSurface) {
	vector< vector <Point> > temp = someSurface.getAllPoints();

	for (unsigned int i = 0; i < temp.size() - 1; i++) {
		vector<Point> row = temp[i];
		for (unsigned int j = 0; j < row.size() - 1; j++) {

			//Populate lookup tables with height and angle information
			height[int(row[j].getX())][int(row[j].getZ())] = row[j].getY();

			//Calculate value to rotate around z-axis
			float deltaX = row[j + 1].getX() - row[j].getX();
			float deltaY = row[j + 1].getY() - row[j].getY();
			zAngle[int(row[j].getX())][int(row[j].getZ())] = atan(deltaY / deltaX) * 180 / 3.14;

			//Calculate value to rotate around x-axis. deltaX is technically deltaZ, but this saves us from creating an extra variable
			deltaX = temp[i + 1][j].getZ() - row[j].getZ();
			deltaY = temp[i + 1][j].getY() - row[j].getY();
			xAngle[int(row[j].getX())][int(row[j].getZ())] = -atan(deltaY / deltaX) * 180 / 3.14;
		}
	}
}

//If there are "holes" in the surface (i.e. height value is still -1000 after initialization), we need to fill them with data from a valid neighbor
//Note that this function doesn't fill holes on the i = MAP_SIZE and j = MAP_SIZE boundaries because this is not needed
void fillHoles() {
	int patched = 0;

	int holecount = 0;
	for (unsigned int i = 0; i < MAP_SIZE - 1; i++) {
		for (unsigned int j = 0; j < MAP_SIZE - 1; j++) {
			if (height[i][j] <= -900) {
				holecount++;
			}
		}
	}

	for (unsigned int i = 0; i < MAP_SIZE - 1; i++) {
		for (unsigned int j = 0; j < MAP_SIZE - 1; j++) {
			//Case for finding a hole
			if (height[i][j] <= -900) {
				//See if we are an non-boundary point
				if (i < MAP_SIZE - 1 && j < MAP_SIZE - 1) {
					//Try to copy data from northeast
					if (height[i + 1][j + 1] > -900) {
						height[i][j] = height[i + 1][j + 1];
						xAngle[i][j] = xAngle[i + 1][j + 1];
						zAngle[i][j] = zAngle[i + 1][j + 1];
						patched++;
					}
					//If that doesn't have data, try the east
					else if (height[i + 1][j] > -900) {
						height[i][j] = height[i + 1][j];
						xAngle[i][j] = xAngle[i + 1][j];
						zAngle[i][j] = zAngle[i + 1][j];
						patched++;
					}
					//Try north
					else if (height[i][j+1] > -900) {
						height[i][j] = height[i][j + 1];
						xAngle[i][j] = xAngle[i][j + 1];
						zAngle[i][j] = zAngle[i][j + 1];
						patched++;
					}
				}
				//West boundary point
				else if (i == 0) {
					cout << "found hole on west bound" << endl;
				}
				//Southern boundary point
				else if (j == 0) {
					cout << "found hole on south bound" << endl;
				}
			}
		}
	}
	holecount = 0;
	for (unsigned int i = 1; i < MAP_SIZE - 1; i++) {
		for (unsigned int j = 1; j < MAP_SIZE - 1; j++) {
			if (height[i][j] <= -900) {
				cout << "hole still remains at " << i << " " << j << endl;
				holecount++;
			}
			
		}
	}
}

//Draws the track the 2 heroes move on
//Draws 1 piece of track for every x points
void drawTrack() {

	//Draw control points
	/*for (unsigned int i = 0; i < heroTrack.size(); i++) {
		glPushMatrix(); {
			glTranslatef(heroTrack[i].getX(), heroTrack[i].getY(), heroTrack[i].getZ());
			glutSolidSphere(1, 10, 10);
		} glPopMatrix();
	}*/
	float transparency = 0.3;

	Material redMaterial(
		Color(1.0, 0.0, 0.0, transparency),
		Color(1.0, 0.0, 0.0, transparency),
		Color(1.0, 0.0, 0.0, transparency),
		0.25);

	Material orangeMatrial(
		Color(1.0, 0.27, 0.0, transparency),
		Color(1.0, 0.27, 0.0, transparency),
		Color(1.0, 0.27, 0.0, transparency),
		0.25);

	Material yellowMatrial(
		Color(1.0, 1.0, 0.0, transparency),
		Color(1.0, 1.0, 0.0, transparency),
		Color(1.0, 1.0, 0.0, transparency),
		0.25);

	Material greenMatrial(
		Color(0, 1.0, 0.0, transparency),
		Color(0, 1.0, 0.0, transparency),
		Color(0, 1.0, 0.0, transparency),
		0.25);

	Material blueMatrial(
		Color(0, 0.0, 1.0, transparency),
		Color(0, 0.0, 1.0, transparency),
		Color(0, 0.0, 1.0, transparency),
		0.25);

	Material indigoMatrial(
		Color(0.5, 0.0, 0.5, transparency),
		Color(0.5, 0.0, 0.5, transparency),
		Color(0.5, 0.0, 0.5, transparency),
		0.25);
	int x = 4;
	//Draws a "plank" for each "x" points in the track and is sized and oriented to match up to the point "x" points away
	//x acts as a reduction factor; x = 1 draws a plank for each point, x = 2 draws half as many points, x = 3 draws 1/3 as many points, etc.
	for (unsigned int i = 0; i < arcLength.size(); i+= x) {
		float distance, pitch, yaw;
		float roll = 0;
		if (i < arcLength.size() - x) {
			distance = (arcLength[i + x] - arcLength[i]).getMagnitude();
			pitch = atan2((arcLength[i + x] - arcLength[i]).getY(), sqrt(pow((arcLength[i + x] - arcLength[i]).getX(), 2) + pow((arcLength[i + x] - arcLength[i]).getZ(), 2))) * 180 / M_PI;
			yaw = -atan2((arcLength[i + x] - arcLength[i]).getZ(), (arcLength[i + x] - arcLength[i]).getX()) * 180 / M_PI;
			//Cheat for the loop-de-loop
			if ((yaw > 179 || yaw < -179) && (arcLength[i + x].getX() > 39) && (arcLength[i + x].getX() < 91) && (arcLength[i + x].getZ() > 9) && (arcLength[i + x].getZ() < 11)) roll = 180;
		} //Case for the final track piece connecting the end to the beginning
		else {
			distance = (arcLength[0] - arcLength[arcLength.size() - x]).getMagnitude();
			pitch = atan2((arcLength[0] - arcLength[arcLength.size() - x]).getY(), sqrt(pow((arcLength[0] - arcLength[arcLength.size() - x]).getX(), 2) + pow((arcLength[0] - arcLength[arcLength.size() - x]).getZ(), 2))) * 180 / M_PI;
			yaw = -atan2((arcLength[0] - arcLength[arcLength.size() - x]).getZ(), (arcLength[0] - arcLength[arcLength.size() - x]).getX()) * 180 / M_PI;
		}

		//Draw the rainbow pieces of each track
		for (int j = 0; j < 6; j++) {
			glPushMatrix(); {
				if (j % 6 == 5) redMaterial.apply();
				else if (j % 6 == 4) orangeMatrial.apply();
				else if (j % 6 == 3) yellowMatrial.apply();
				else if (j % 6 == 2) greenMatrial.apply();
				else if (j % 6 == 1) blueMatrial.apply();
				else if (j % 6 == 0) indigoMatrial.apply();
				glTranslatef(arcLength[i].getX(), arcLength[i].getY(), arcLength[i].getZ());
				glRotatef(yaw, 0, 1, 0);
				glRotatef(pitch, 0, 0, 1);
				glRotatef(roll, 1, 0, 0);
				glTranslatef(0, 0, 8 - float(j)*2);
				glScalef(distance + 0.05, 0.1, 2);
				glutSolidCube(1);
				//if (i == arcLength.size() - 2) glutSolidCube(10);
			} glPopMatrix();
		}
	}
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
    //Create a Display List & Call our Drawing Functions
	environmentDL = glGenLists(1);
	glNewList(environmentDL, GL_COMPILE);

	for (unsigned int i = 0; i < surfaces.size(); i++) {
		drawSurface(surfaces[i]);
	}
	fillHoles();
	drawWindTurbines();
	drawSky();
	drawTrack();
	glEndList();
}

// resizeWindow() //////////////////////////////////////////////////////////////
//
//  GLUT callback for window resizing. Resets GL_PROJECTION matrix and viewport.
//
////////////////////////////////////////////////////////////////////////////////
void resizeWindow(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    
    mainViewport->setWidth(w);
    mainViewport->setHeight(h);

    splitViewport->setWidth(w);
    splitViewport->setHeight(h);
}

/**
 * GLUT callback for mouse clicks.
 * Input is directly forwarded to the active camera, which then deals with it.
 */
void mouseCallback(int button, int state, int thisX, int thisY) {

    mainCamera->mouseButton(button, state, thisX, thisY);
	if (button == GLUT_RIGHT_BUTTON) {
		movingBackward = false;
		movingForward = false;
		turningLeft = false;
		turningRight = false;
	}
}

/**
 * GLUT callback for mouse movement.
 * Input is directly forwarded to the active camera, which then deals with it.
 */
void mouseMotion(int x, int y) {
    mainCamera->mouseMotion(x, y);
	glutPostRedisplay(); // redraw our scene from our new camera POV
}

// initScene() /////////////////////////////////////////////////////////////////
//
//  A basic scene initialization function; should be called once after the
//      OpenGL context has been created. Doesn't need to be called further.
//
////////////////////////////////////////////////////////////////////////////////
void initScene()  {
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	// Enable blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(Color(126 / 255., 192 / 255., 238 / 255.));

    std::shared_ptr<Light> light0 = Light::getLight(GL_LIGHT0);
    light0->enable();
    light0->setDiffuse(Color(0.6,0.6, 0.6));
    light0->setAmbient(Color(0.1, 0.1, 0.1));
    light0->setSpecular(Color(0.1, 0.1, 0.1));
	std::shared_ptr<Light> light3 = Light::getLight(GL_LIGHT3);
	light3->enable();
	light3->setDiffuse(Color(0.6, 0.6, 0.6));
	light3->setAmbient(Color(0.1, 0.1, 0.1));
	light3->setSpecular(Color(0.1, 0.1, 0.1));

    std::shared_ptr<Light> light1 = Light::getLight(GL_LIGHT1);
    light1->enable();
    light1->setDiffuse(Color(1, 1, 1, 1));
    light1->makeSpotLight(100, 12);
	
    std::shared_ptr<Light> light2 = Light::getLight(GL_LIGHT2);
    light2->enable();
    light2->setDiffuse(Color(1, 1, 1, 1));
    light2->makeSpotLight(100, 12);

	//Load textures
	loadTextures();

    srand( time(NULL) );	// seed our random number generator
    generateEnvironmentDL();
}

const string fpsToStr(double x) {
	stringstream ss;
	ss << "FPS: "<< x;
	return ss.str();
}

// renderScene() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for scene rendering. Sets up the modelview matrix, renders
//      a scene to the back buffer, and switches the back buffer with the
//      front buffer (what the user sees).
//
////////////////////////////////////////////////////////////////////////////////
void renderScene(std::shared_ptr<CameraController> camera)  {
	frames++;
	
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	

	glEnable(GL_LIGHTING);

    camera->look();
	
	Light::getLight(GL_LIGHT0)->setPosition(Point(0, 100, -100));
	Light::getLight(GL_LIGHT3)->setPosition(Point(0, 100, 100));





    // Position the listener and background music source at the position of the main camera.
    if (camera == mainCamera)
    {
		//take the frame rate every 10 frames so it doesn't change as frequently
		

        positionListener(camera->getCamera().getPosition(),
                         camera->getCamera().getDirection(),
                         camera->getCamera().getUp());
        positionSource(sources[0], camera->getCamera().getPosition());
		glDisable(GL_LIGHTING);
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 640, 0, 480);

		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glColor3f(1, 1, 0);
		glRasterPos2i(10, 460);  // move in 10 pixels from the left and bottom edges

		for (size_t i = 0; i < sFPS.length(); i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,sFPS[i]);
		}
		
		
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_LIGHTING);
		
    }


	positionSource(sources[1], heroes[1]->getPosition());

	if ((heroes[2]->getPosition() - heroes[1]->getPosition()).getMagnitude() < 5) {
        ALenum sourceState;
        alGetSourcei(sources[1], AL_SOURCE_STATE, &sourceState);
        if (sourceState != AL_PLAYING)
        {
            alSourcePlay(sources[1]);
        }

	}

	//Draw the heroes
	for (unsigned int i = 0; i < NUM_HEROES; i++) {
		glPushMatrix(); {
			glTranslate(heroes[i]->getPosition());
			if (i == 0) glTranslatef(0, 0.5, 0); //Eric's hero needs to be drawn so it is not in the ground
			//Rotate hero accordingly
			if (i == 0) {
				glRotatef(float(heroes[0]->getXAngle()), 1, 0, 0);
				glRotatef(float(heroes[0]->getZAngle()), 0, 0, 1);
				glRotatef(float(heroes[0]->getTheta()), 0, 1, 0);
			}
			else {
				glRotatef(float(heroes[i]->getTheta()), 0, 1, 0);
				glRotatef(float(heroes[i]->getZAngle()), 0, 0, 1);
				glRotatef(float(heroes[i]->getXAngle()), 1, 0, 0);
			}
			//Draw hero name floating above it
			glDisable(GL_LIGHTING);
			glPushMatrix(); {
				glColor3f(1, 0, 0);
				if (i == 0)	glTranslatef(0, 2, 6);
				if (i == 1) glTranslatef(0, 6, 8);
				if (i == 2) glTranslatef(0, 5, 4);
				if (i == 3) glTranslatef(0, 2, 10);
				glRotatef(90, 0, 1, 0);
				glScalef(0.02, 0.02, 0.02);
				for (char c : heroes[i]->getName()) {
					glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
				}
			} glPopMatrix();
			glEnable(GL_LIGHTING);

			//Scale to make it look fitting
			glScalef(float(heroScaleFactor[i]), float(heroScaleFactor[i]), float(heroScaleFactor[i]));
			//Draw hero and pet
			glPushMatrix(); {
				if (i == 1) glTranslatef(0, 0, 10);
				//if (i == 2) glTranslatef(0, 0, -10);
				heroes[i]->drawHero();
				heroes[i]->drawPet();
			}  glPopMatrix();
		} glPopMatrix();
	}

	//Draw the spinning turbine blades
	for (unsigned int i = 0; i < windTurbines.size(); i++) {
		glPushMatrix(); {
			glTranslatef(windTurbines[i].getX(), height[int(windTurbines[i].getX())][int(windTurbines[i].getZ())], windTurbines[i].getZ());
			glRotatef(windTurbines[i].getY(), 0, 1, 0);
			glTranslatef(0, WIND_TURBINE_HEIGHT, 1);
			glRotatef(-90, 1, 0, 0);
			drawTurbineBlades();
		} glPopMatrix();
	}

	//Draw the ?-blocks
	for (unsigned int i = 0; i < blocks.size(); i++) {
		glPushMatrix(); {
			glTranslate(blocks[i]);
			glRotatef(45, 1, 0, 0);
			glRotatef(45, 0, 0, 1);
			glRotatef(timer, 1, 1, -1);
			drawBlock();
		} glPopMatrix();
	}

	glCallList(environmentDL);
}

void renderViewports()
{
    glDrawBuffer( GL_BACK );

    if (firstPersonCamera)
    {
        splitViewport->display();
    }
    else
    {
        mainViewport->display();
		
			//when frame is finished rendering get end time
			endTime = clock();
            // cout << "End: " << endTime << ", Start: " << startTime << ", diff: " << endTime - startTime << ", CPS: " << CLOCKS_PER_SEC <<  endl;
			time1 = CLOCKS_PER_SEC / (double)(endTime - startTime);
			startTime = clock();
			FPS = time1;
			sFPS = fpsToStr(FPS);
		
    }
	
	
    //push the back buffer to the screen
    glutSwapBuffers();
}

//This function gets called once every time myTimer is called if Moggy is supposed to be moving
void moveMoggy() {
	//Calculate angle between traveler and Moggy
	float toRotate = (-atan2(heroes[0]->getZ() - heroes[3]->getZ(), heroes[0]->getX() - heroes[3]->getX()) * 180 / M_PI) - heroes[3]->getTheta();
	if (toRotate > 180) toRotate -= 360;
	if (toRotate < -180) toRotate += 360;
	//First, rotate until the helicopter is facing us
	if (toRotate > 0.001 || toRotate < -0.001) {
		//If we have less than 1.5 degrees left to rotate
		if (toRotate < 1.5 && toRotate > -1.5) {
			heroes[3]->changeTheta(toRotate);
			toRotate = 0;
		}
		else if (toRotate > 0) {
			heroes[3]->changeTheta(1.5);
			toRotate -= 1.5;
		}
		else {
			heroes[3]->changeTheta(-1.5);
			toRotate += 1.5;
		}
	}

	//If we are lined up with the target and not hovering over it, move 0.4 units toward the location we need to go to
	if (toRotate <= 0.001 && toRotate >= -0.001) {
		Point direction = Point(heroes[0]->getX() - heroes[3]->getX(), 10 + heroes[0]->getY() - heroes[3]->getY(), heroes[0]->getZ() - heroes[3]->getZ());
		float mag = sqrt(pow(direction.getX(), 2) + pow(direction.getY(), 2) + pow(direction.getZ(), 2));
		//See if we are within one unit
		if (mag <= 1) {
			heroes[3]->move(direction.getX(), direction.getY(), direction.getZ());
			moggyMoving = false;
		}
		else {
			heroes[3]->move(direction.getX() / (mag*5/2), direction.getY() / (mag * 5/2), direction.getZ() / (mag * 5/2));
		}
	}
}

// normalKeysDown() ////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback; gets called when the user presses a key.
//
////////////////////////////////////////////////////////////////////////////////
void normalKeysDown(unsigned char key, int x, int y) {
    // Convert all letter keys to lowercase for simplicity
    if (key >= 'A' && key <= 'Z')
    {
        key += 'a' - 'A';
    }
    mainCamera->keyboardInput(key, x, y);
    if (firstPersonCamera)
    {
        firstPersonCamera->keyboardInput(key, x, y);
    }

    if(key == 'q' || key == 27)
        exit(0);
	if (key == 'f')
        Light::getLight(GL_LIGHT1)->toggle();
	if (key == 'g')
        Light::getLight(GL_LIGHT2)->toggle();
    if (mainCamera != freeCam)
    {
        if (key == 'w') movingForward = true;
        if (key == 's') movingBackward = true;
        if (key == 'a') turningLeft = true;
        if (key == 'd') turningRight = true;
    }

	if (key == 'c' && moggyMoving == false) moggyMoving = true;

    glutPostRedisplay();		// redraw our scene from our new camera POV
}

void normalKeysUp(unsigned char key, int x, int y) {
	// Convert all letter keys to lowercase for simplicity
	if (key >= 'A' && key <= 'Z')
	{
		key += 'a' - 'A';
	}

	if (key == 'w') movingForward = false;
	if (key == 's') movingBackward = false;
	if (key == 'a') turningLeft = false;
	if (key == 'd') turningRight = false;

	glutPostRedisplay();		// redraw our scene from our new camera POV
}

void moveForward() {
	heroes[0]->setX(heroes[0]->getX() + 1 * cos(heroes[0]->getTheta() * M_PI / 180));
	heroes[0]->setZ(heroes[0]->getZ() - 1 * sin(heroes[0]->getTheta() * M_PI / 180));
	heroes[0]->setSpinAngle(heroes[0]->getSpinAngle() - 10);

	//Make sure we are in boundary
	if (heroes[0]->getX() > (MAP_SIZE - 2)) heroes[0]->setX(MAP_SIZE - 2.0f);
	if (heroes[0]->getX() < 2) heroes[0]->setX(2.0f);
	if (heroes[0]->getZ() > (MAP_SIZE - 2)) heroes[0]->setZ(float(MAP_SIZE) - 2.0f);
	if (heroes[0]->getZ() < 2) heroes[0]->setZ(2.0f);

	//Set data depending on x/z location
	heroes[0]->setY(height[int(heroes[0]->getX())][int(heroes[0]->getZ())]);
	heroes[0]->setXAngle(xAngle[int(heroes[0]->getX())][int(heroes[0]->getZ())]);
	heroes[0]->setZAngle(zAngle[int(heroes[0]->getX())][int(heroes[0]->getZ())]);
}

void moveBackward() {
	heroes[0]->setX(heroes[0]->getX() - 1 * cos(heroes[0]->getTheta() * M_PI / 180));
	heroes[0]->setZ(heroes[0]->getZ() + 1 * sin(heroes[0]->getTheta() * M_PI / 180));
	heroes[0]->setSpinAngle(heroes[0]->getSpinAngle() + 10);

	//Make sure we are in boundary
	if (heroes[0]->getX() > (MAP_SIZE - 2)) heroes[0]->setX(MAP_SIZE - 2.0f);
	if (heroes[0]->getX() < 2) heroes[0]->setX(2.0f);
	if (heroes[0]->getZ() > (MAP_SIZE - 2)) heroes[0]->setZ(float(MAP_SIZE) - 2.0f);
	if (heroes[0]->getZ() < 2) heroes[0]->setZ(2.0f);

	//Set data depending on x/z location
	heroes[0]->setY(height[int(heroes[0]->getX())][int(heroes[0]->getZ())]);
	heroes[0]->setXAngle(xAngle[int(heroes[0]->getX())][int(heroes[0]->getZ())]);
	heroes[0]->setZAngle(zAngle[int(heroes[0]->getX())][int(heroes[0]->getZ())]);
}

void turnLeft() {
	heroes[0]->setTheta(heroes[0]->getTheta() + 1.5);
}

void turnRight() {
	heroes[0]->setTheta(heroes[0]->getTheta() - 1.5);
}

void myTimer(int value) {


	//Move our hero based on input keys
	//Moving the hero in the timer function allows us to turn and move at the same time
	if (movingForward && !movingBackward) moveForward();
	if (!movingForward && movingBackward) moveBackward();
	if (turningLeft && !turningRight) turnLeft();
	if (!turningLeft && turningRight) turnRight();


	//Update position of the 2 heroes on the track
	unsigned int point = timer % parametric.size();
	//Parametric
    heroes[1]->setPosition(heroCurve.evaluateAt((float)point / parametric.size()));
	//Calculate how to rotate hero along curve tangent
	float deltaX, deltaY, deltaZ;
	if (point != parametric.size() - 1) {
		deltaX = parametric[point + 1].getX() - parametric[point].getX();
		deltaY = parametric[point + 1].getY() - parametric[point].getY();
		deltaZ = parametric[point + 1].getZ() - parametric[point].getZ();
	}
	else {
		deltaX = parametric[0].getX() - parametric[parametric.size() - 1].getX();
		deltaY = parametric[0].getY() - parametric[parametric.size() - 1].getY();
		deltaZ = parametric[0].getZ() - parametric[parametric.size() - 1].getZ();
	}
	float pitch = atan2(deltaY, sqrt(deltaX*deltaX + deltaZ*deltaZ)) * 180 / M_PI;
	float yaw = -atan2(deltaZ, deltaX) * 180 / M_PI;
	heroes[1]->setTheta(yaw);
	heroes[1]->setZAngle(pitch);

	//This is a cheat to make the loop-de-loop function properly.
	//Causes our hero to flip on its back when it is in the upper half of the loop-de-loop, but not anywhere else on the track.
	//It's hard-coded and bad, but it works and looks awesome!
	if ((yaw > 179 || yaw < -179) && (heroes[1]->getX() > 39) && (heroes[1]->getX() < 91) && (heroes[1]->getZ() > 9) && (heroes[1]->getZ() < 11)) heroes[1]->setXAngle(180);
	else heroes[1]->setXAngle(0);

	//Arc length
	point = timer % arcLength.size();
	heroes[2]->setX(arcLength[point].getX());
	heroes[2]->setY(arcLength[point].getY());
	heroes[2]->setZ(arcLength[point].getZ());
	if (point != arcLength.size() - 1) {
		deltaX = arcLength[point + 1].getX() - arcLength[point].getX();
		deltaY = arcLength[point + 1].getY() - arcLength[point].getY();
		deltaZ = arcLength[point + 1].getZ() - arcLength[point].getZ();
	}
	else {
		deltaX = arcLength[0].getX() - arcLength[arcLength.size() - 1].getX();
		deltaY = arcLength[0].getY() - arcLength[arcLength.size() - 1].getY();
		deltaZ = arcLength[0].getZ() - arcLength[arcLength.size() - 1].getZ();
	}
	float pitch2 = atan2(deltaY, sqrt(deltaX*deltaX + deltaZ*deltaZ)) * 180 / M_PI;
	float yaw2 = -atan2(deltaZ, deltaX) * 180 / M_PI;
	heroes[2]->setTheta(yaw2);
	heroes[2]->setZAngle(pitch2);

	if ((yaw2 > 179 || yaw2 < -179) && (heroes[2]->getX() > 39) && (heroes[2]->getX() < 91) && (heroes[2]->getZ() > 9) && (heroes[2]->getZ() < 11)) heroes[2]->setXAngle(180);
	else heroes[2]->setXAngle(0);

	if (moggyMoving) moveMoggy();
	for (size_t i = 0; i < NUM_HEROES; i++) {
		heroes[i]->incrementTimer();
	}
	timer++;

	// redraw our display
	glutPostRedisplay();
	// register a new timer callback
	glutTimerFunc(1000.0f / 60.0f, myTimer, 0);

	for (size_t i = 0; i < NUM_HEROES; i++) {
		heroes[i]->incrementTimer();
	}
	timer++;


}

void mainMenu(int value) {
	if (value == MenuOption::QUIT)
    {
        exit(0);
    }
    else if (value == MenuOption::FREE_CAM)
    {
        mainCamera = freeCam;
    }
}

//ArcBall Cam submenu
void arcballMenu(int value) {
    mainCamera = arcballCams[value];
}

//First Person Cam submenu
void firstPersonMenu(int value) {
    if (value == -1)
    {
        firstPersonCamera.reset();
    }
    else
    {
        firstPersonCamera = firstPersonCams[value];
    }
}

// createMenus() ///////////////////////////////////////////////////////////////
//
//  Handles creating a menu, adding menu entries, and attaching the menu to
//  a mouse button
//
////////////////////////////////////////////////////////////////////////////////
void createMenus() {

	int arcballSubmenu = glutCreateMenu(arcballMenu);
    for (size_t i = 0; i < NUM_HEROES; ++i)
    {
        glutAddMenuEntry(heroes[i]->getName().c_str(), i);
    }

	int firstPersonSubmenu = glutCreateMenu(firstPersonMenu);
    for (size_t i = 0; i < NUM_HEROES; ++i)
    {
        glutAddMenuEntry(heroes[i]->getName().c_str(), i);
    }
    glutAddMenuEntry("None", -1);

	glutCreateMenu(mainMenu);
	glutAddSubMenu("ArcBall Cam", arcballSubmenu);
	glutAddSubMenu("First Person Cam", firstPersonSubmenu);
	glutAddMenuEntry("Free Cam", MenuOption::FREE_CAM);
	glutAddMenuEntry("Quit", MenuOption::QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//Calculates the points for the parametric curve
void calculateParametric() {
    heroCurve = BezierCurve(heroTrack);
	int resolution = 100;
	for (unsigned int i = 0; i < heroTrack.size() - 1; i += 3) {
		for (float j = 0; j < resolution; j++) {
			float t = float(j / resolution);
			Point thePoint = (1 - t)*(1 - t)*(1 - t)*heroTrack[i] + 3 * (1 - t)*(1 - t)*t*heroTrack[i + 1] + 3 * (1 - t)*t*t*heroTrack[i + 2] + t*t*t*heroTrack[i + 3];

			parametric.push_back(thePoint);
		}
	}
}

//Calculates the points for the arc length curve
void calculateArc() {
	vector<float> distancetable;
    Point delta;
	distancetable.push_back(0.0f);
	//Populate our distance table
	for (unsigned int i = 0; i < parametric.size() - 1; i++) {
        Point delta = heroCurve.evaluateAt((float)(i + 1) / parametric.size()) -
                      heroCurve.evaluateAt((float)i / parametric.size());
		distancetable.push_back(distancetable.back() + delta.getMagnitude());
	}

    delta = heroCurve.evaluateAt(1 - 1.0 / parametric.size()) - heroCurve.evaluateAt(0);
	distancetable.push_back(distancetable.back() + delta.getMagnitude());

	float maxDistance = distancetable.back();


	//Populates the arcLength table with data one which point on the curve corresponds to which percentage of the total curve length
	for (unsigned int i = 0; i < parametric.size(); i++) {
		int P1, P2;
		float percentage = float(i) / float(parametric.size());
		float goal = maxDistance * percentage;

		//Look for the first instance in our distance table with a value greater than goal.
		//Our interpolation values will be between this point and the point before it, so mark these points
		for (unsigned int j = 0; j < parametric.size() - 1; j++) {
			if (distancetable[j] > goal) {
				P1 = j - 1;
				P2 = j;
				break;
			}
		}

		//Now do the linear interpolation to find what our t value should be
		float percent = (goal - distancetable[P1]) / (distancetable[P2] - distancetable[P1]);

		//Figure out which control points we want for our calculation and then calculate the point on the bezier curve using the standard bezier formula
		int curveNumber = P1 / 100;
		int point = P1 % 100;
		if (point < 0) point = 0;
		float t = (float(point) + percent)/100.0f;
		Point thePoint = (1 - t)*(1 - t)*(1 - t)*heroTrack[curveNumber*3] + 3 * (1 - t)*(1 - t)*t*heroTrack[curveNumber * 3 + 1] + 3 * (1 - t)*t*t*heroTrack[curveNumber * 3 + 2] + t*t*t*heroTrack[curveNumber * 3 + 3];
		arcLength.push_back(thePoint);
	}
}

//Reads an input file for information on surfaces, hero pet paths, hero paths, object placement, etc
bool loadInputWorld(char* filename) {
	//Determines what we should be doing with our lines
	//1 = surface, 2 = pet Bezier curve, 3 = track, 4 = wind turbine, 5 = block
	int mode = 0;

	ifstream infile(filename);

	//Check to see if the file exists
	if (infile) {

		string line;

		//variables used for Bezier surfaces
		Point somepoints[16];
		int j = 0;

		//variables used for pet curves
		vector<Point> petPoints;
		int hero = 0;

		//Read line by line
		while (getline(infile, line)) {
			//If we have a surface, create a surface object and add it to our vector
			if (line.substr(0, 7) == "surface") {
				mode = 1;
			}
			//If we have a pet, use those points for the Bezier curve for the corresponding hero
			else if (line.substr(0, 3) == "pet") {
				hero = atoi(line.c_str());
				mode = 2;
				petPoints.clear();
			}
			else if (line.substr(0, 5) == "track") {
				mode = 3;
			}
			else if (line.substr(0, 7) == "turbine") {
				mode = 4;
			}
			else if (line.substr(0, 5) == "block") {
				mode = 5;
			}

			//Case for finding data
			else {
				//Reads the line of data and stores the x, y, and z values to a point object
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

				Point thePoint = Point(x, y, z);

				//Depending on what mode we are in, do different things with the point
				//Case for Bezier surfaces
				if (mode == 1) {
					somepoints[j] = thePoint;
					j++;
					if (j == 16) {
						BezierSurface mySurface(somepoints, bezierU, bezierV);
						surfaces.push_back(mySurface);
						j = 0;
					}
				}
				//If it's a pet point, add the control point to the appropriate hero's pet path
				else if (mode == 2) {
					heroes[hero]->addControlPoint(thePoint);
				}
				else if (mode == 3) {
					heroTrack.push_back(thePoint);
				}
				else if (mode == 4) {
					windTurbines.push_back(thePoint);
				}
				else if (mode == 5) {
					blocks.push_back(thePoint);
				}
			}
		}
		infile.close();
	}
	else {
		return false;
	}
	calculateParametric();
	calculateArc();
	for (unsigned int i = 0; i < surfaces.size(); i++) {
		calculateSurface(surfaces[i]);
	}
	return true;
}

// initializeOpenAL() //////////////////////////////////////////////////////////
void initializeOpenAL(int argc, char *argv[]) {
	ALsizei size, freq;
	ALenum 	format;
	ALvoid 	*data;
	ALboolean loop;

	alutInit(&argc, argv);
	ALCdevice *device = alcOpenDevice(NULL);
	ALCcontext *context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);


	alGenBuffers(NUM_SOURCES, buffers);
	alGenSources(NUM_SOURCES, sources);


#ifdef __APPLE__
	alutLoadWAVFile((ALbyte*)"wavs/mario.wav", &format, &data, &size, &freq);
#else
	alutLoadWAVFile((ALbyte*)"wavs/mario.wav", &format, &data, &size, &freq, &loop);
#endif
	alBufferData(buffers[0], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alSourcei(sources[0], AL_BUFFER, buffers[0]);
	alSourcei(sources[0], AL_LOOPING, AL_TRUE);
    alSourcef(sources[0], AL_GAIN, 1);


    // Load and set up the beep sound
#ifdef __APPLE__
	alutLoadWAVFile((ALbyte*)"wavs/beepbeep.wav", &format, &data, &size, &freq);
#else
	alutLoadWAVFile((ALbyte*)"wavs/beepbeep.wav", &format, &data, &size, &freq, &loop);
#endif
	alBufferData(buffers[1], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	alSourcei(sources[1], AL_BUFFER, buffers[1]);
	alSourcei(sources[1], AL_LOOPING, AL_FALSE);
    alSourcef(sources[1], AL_GAIN, 1000);

	positionSource(sources[0], Point(0, 0, 0));
}

// cleanupOpenAL() /////////////////////////////////////////////////////////////
void cleanupOpenAL() {

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);


	alDeleteBuffers(NUM_BUFFERS, buffers);
	alDeleteSources(NUM_SOURCES, sources);
}

/**
 * Create the fullscreen and split screen viewports.
 */
void initializeViewports()
{
	
    mainViewport = make_shared<Viewport>(0, 0, windowWidth, windowHeight);
    mainViewport->setDisplayFunction([&](){
        renderScene(mainCamera);
    });

    splitViewport = make_shared<Viewport>(0, 0, windowWidth, windowHeight);
    auto leftAndRight = splitViewport->splitHorizontal(0.5);

    leftViewport = leftAndRight.first;
    rightViewport = leftAndRight.second;

    leftViewport.lock()->setDisplayFunction([&](){
        renderScene(mainCamera);
    });

    // TODO: First person camera doesn't work at the moment.
    // Figure out why.
    rightViewport.lock()->setDisplayFunction([&](){
        renderScene(firstPersonCamera);
    });


    
}

// main() //////////////////////////////////////////////////////////////////////
//
//  Program entry point. Does not process command line arguments.
//
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
	// create a double-buffered GLUT window at (50,50) with predefined windowsize
	glutInit(&argc, argv);
	initializeOpenAL(argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Guild Wars");
	
	//Load heroes into array
	heroes[0] = new NotSure(make_shared<Point>(50, 0, 50), 0.0f, 0.0f, 0.0f, 0.0f);
	heroes[1] = new Truck(make_shared<Point>(20, 0, 20), 0.0f, 0.0f, 0.0f, 0.0f);
	heroes[2] = new SirVive(make_shared<Point>(10, 0, 10), 0.0f, 0.0f, 0.0f, 0.0f);
	heroes[3] = new MogdinBaramud(make_shared<Point>(50, 0, 50), 0, 0, 0, 0);

    // Add arcball and first person cameras for each hero
    for (size_t i = 0; i < NUM_HEROES; ++i)
    {
        // Figure out rotation mode for first person camera. See FirstPersonController.h for details.
        RotationMode mode;
        if (i == 1 || i == 2)
        {
            mode = RotationMode::CURVE;
        }
        else
        {
            mode = RotationMode::SURFACE;
        }
        firstPersonCams.push_back(std::make_shared<FirstPersonController>(heroes[i], 5, mode));
        arcballCams.push_back(
            std::make_shared<ArcballController>(heroes[i]->getPositionPointer(),
                                                50, -M_PI / 3.0f, M_PI / 2.8f)
        );
    }

    freeCam = std::make_shared<FreeCamController>(Point(50, 50, 75), -M_PI / 3, M_PI / 2.8);
    mainCamera = freeCam;

    initializeViewports();

	//Set array values to large negative number so we can figure out where holes exist in our world and deal with them later
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			height[i][j] = -1000.0f;
		}
	}

	if (!loadInputWorld(argv[1])) {
		fprintf(stdout, "Error: Input file not found\n");
        exit(1);
	}

	// create our menu options and attach to mouse button
	createMenus();

	//create listener
	////////////////////////////////////////////////////////////////////////////////////////set to camera
    
	positionListener(mainCamera->getCamera().getPosition(),
                     mainCamera->getCamera().getDirection(),
                     mainCamera->getCamera().getUp());



	// register callback functions...
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutKeyboardFunc(normalKeysDown);
	glutDisplayFunc(renderViewports);
	glutKeyboardUpFunc(normalKeysUp);
	glutReshapeFunc(resizeWindow);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotion);
	
	glutTimerFunc(1000.0f / 60.0f, myTimer, 0);

	// do some basic OpenGL setup



	initScene();

	heroes[0]->calculatePetPath(200);

	glLineWidth(3.0f);

	//Set hero initial locations
	heroes[0]->setY(height[int(heroes[0]->getX())][int(heroes[0]->getZ())]);
	heroes[0]->setXAngle(xAngle[int(heroes[0]->getX())][int(heroes[0]->getZ())]);
	heroes[0]->setZAngle(zAngle[int(heroes[0]->getX())][int(heroes[0]->getZ())]);

	//Start heroes 1 and 2 on the curve
    heroes[1]->setPosition(heroCurve.evaluateAt(0));

    heroes[2]->setPosition(arcLength[0]);

	//Start flying hero as flying above main hero
    heroes[3]->setPosition(heroes[0]->getPosition() + Point(0, 10, 0));

	atexit(cleanupOpenAL);

	alSourcePlay(sources[0]);
    // and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
