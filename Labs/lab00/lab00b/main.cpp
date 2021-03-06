/*
 *  CSCI 441, Computer Graphics, Fall 2015
 *
 *  Project: lab00b
 *  File: main.cpp
 *
 *	Author: Jeffrey Paone - Fall 2015
 *
 *  Description:
 *      Contains the code for a simple 2D OpenGL / GLUT example.
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
#include <math.h>

// global variables to keep track of window width and height.
// set to initial values for convenience, but we need variables
// for later on in case the window gets resized.
int windowWidth = 512, windowHeight = 512;

// global variable to keep track of the window id
int windowId;

//
//  void changeSize(int w, int h)
//
//      We will register this function as GLUT's reshape callback.
//   When the window is resized, the OS will tell GLUT and GLUT will tell
//   us by calling this function - GLUT will give us the new window width
//   and height as 'w' and 'h,' respectively - so save those to our global vars.
//
void changeSize( int w, int h ) {
    //save the new window width / height that GLUT informs us about
    windowWidth = w;
    windowHeight = h;

    // update the projection matrix based on the window size
    // the GL_PROJECTION matrix governs properties of the view;
    // i.e. what gets seen - use an Orthographic projection that ranges
    // from [0, windowWidth] in X and [0, windowHeight] in Y. (0,0) is the lower left.
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, windowWidth, 0, windowHeight );
    
    // update the viewport as well - tell OpenGL we want to render the whole window
    glViewport( 0, 0, windowWidth, windowHeight );

    // just as good practice, switch back to GL_MODELVIEW mode; that's the matrix
    // that we want to be changing with glTranslatef(), glScalef(), etc., so
    // just in case we forget to explicitly set it later...
    glMatrixMode( GL_MODELVIEW );
}

//
//  void renderScene()
//
//      We will register this function as GLUT's display callback.
//  This is where the magic happens - all rendering is done here.
//

void drawMyTriangle() {
	//Draw a triangle
	glColor3f(1, .8, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(-2.5, -2);
	glVertex2f(2.5, -2);
	glVertex2f(0, 2);
	glEnd();
}

void drawTriforce() {
	//Draw the first triangle
	glPushMatrix();
	glTranslatef(-2.5, -2, 0);
	drawMyTriangle();
	glPopMatrix();

	//Draw the second triangle
	glPushMatrix();
	glTranslatef(2.5, -2, 0);
	drawMyTriangle();
	glPopMatrix();

	//Draw the last triangle
	glPushMatrix();
	glTranslatef(0, 2, 0);
	drawMyTriangle();
	glPopMatrix();
}


void renderScene() {
    // clear whatever was drawn to the screen last time - 
    // set the clear color to black and then signal to clear the RGB buffer.
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT );

    // switch to GL_MODELVIEW, for when we start using glTranslatef(), glScalef(), etc..
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	glPushMatrix();
	glTranslatef(300, 300, 0);
	glScalef(5, 5, 0);
	glRotatef(90, 0, 0, 1);
	drawTriforce();
	glPopMatrix();

    // flush the OpenGL commands and make sure they get rendered!
    glFlush();
}


//
//  int main( int argc, char* argv[] )
//
int main( int argc, char* argv[] ) {
    // initialize GLUT... always need this call.
    glutInit( &argc, argv );

    // request a window with just an RGB frame buffer, nothing fancy...
    // place it at (50,50) on the screen and set its size to our constants from earlier
    glutInitDisplayMode( GLUT_RGB );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( windowWidth, windowHeight );
    
    // this actually creates our window. 
    // bet you thought the title was going to be a zelda reference, eh? #sworcery
    windowId = glutCreateWindow( "Triforce" );

    // register our reshape and display callbacks with GLUT - these functions are above
    glutDisplayFunc( renderScene );
    glutReshapeFunc( changeSize );

    // begin GLUT loop
    glutMainLoop();

    return 0;
}
