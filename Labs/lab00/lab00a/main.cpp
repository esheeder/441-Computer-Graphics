/*
 *  CSCI 441, Computer Graphics, Fall 2015
 *
 *  Project: lab00a
 *  File: main.cpp
 *
 *	Author: Jeffrey Paone - Fall 2015
 *
 *  Description:
 *      Contains the code for a simple 2D OpenGL / GLUT example.
 *
 */

#ifdef __APPLE__				// if compiling on Mac OS
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#else							// if compiling on Linux or Windows OS
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif

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
void renderScene() {
    // clear whatever was drawn to the screen last time - 
    // set the clear color to black and then signal to clear the RGB buffer.
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT );

    // switch to GL_MODELVIEW, for when we start using glTranslatef(), glScalef(), etc..
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	//Draw first triangle
	glColor3f(1, 0, 0);
	glBegin( GL_TRIANGLES );
	glVertex2f(200, 100);
	glVertex2f(100, 100);
	glVertex2f(100, 125);
	glEnd();
   
	//Draw quadrilateral
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex2f(300, 345);
	glVertex2f(300, 315);
	glVertex2f(250, 205);
	glVertex2f(250, 235);
	glEnd();
    
	//Draw second triangle
	glColor3f(0.3, 0.25, 0.7);
	glBegin(GL_TRIANGLES);
	glVertex2f(100, 125);
	glVertex2f(200, 125);
	glVertex2f(200, 100);
	glEnd();

	//Draw a line
	glColor3f(0.5, 0.5, 1);
	glBegin(GL_LINES);
	glVertex2f(300, 100);
	glVertex2f(400, 125);
	glEnd();

	//Draw quadrilateral
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex2f(200, 345);
	glVertex2f(250, 235);
	glVertex2f(250, 205);
	glVertex2f(200, 315);
	glEnd();

	//Draw quadrilateral
	glColor3f(0.8, 0.2, 0.5);
	glBegin(GL_QUADS);
	glVertex2f(200, 345);
	glVertex2f(200, 225);
	glVertex2f(160, 235);
	glVertex2f(160, 335);
	glEnd();

	//Draw a line strip
	glColor3f(0.3, 0.6, 0.9);
	glBegin(GL_LINE_STRIP);
	glVertex2f(400, 125);
	glVertex2f(300, 125);
	glVertex2f(300, 100);
	glVertex2f(400, 100);
	glEnd();

	//Draw a triangle fan
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.2, 0.2, 0.2);
	glVertex2f(320, 335);
	glColor3f(0.4, 0.4, 0.4);
	glVertex2f(360, 335);
	glColor3f(0.6, 0.6, 0.6);
	glVertex2f(360, 345);
	glColor3f(0.8, 0.8, 0.8);
	glVertex2f(300, 345);
	glColor3f(1, 1, 1);
	glVertex2f(300, 335);
	glEnd();

	//Draw a triangle fan
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.2, 0.8, 0.5);
	glVertex2f(300, 125);
	glColor3f(0.6, 0.3, 0.7);
	glVertex2f(340, 125);
	glColor3f(0.8, 0.1, 0.6);
	glVertex2f(300, 185);
	glColor3f(0.25, 0.35, 0.55);
	glVertex2f(340, 225);
	glColor3f(.6, .4, .1);
	glVertex2f(300, 265);
	glColor3f(.1, 1, .1);
	glVertex2f(340, 335);
	glColor3f(1, .1, .1);
	glVertex2f(300, 335);
	glEnd();

	//Draw a line
	glColor3f(0.6, 0.2, 0);
	glBegin(GL_LINES);
	glVertex2f(300, 125);
	glVertex2f(400, 100);
	glEnd();

	//Draw a line Loop
	glColor3f(0.3, 0.3, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(200, 125);
	glVertex2f(200, 225);
	glVertex2f(160, 235);
	glVertex2f(160, 125);
	glEnd();

	//Draw quadrilateral
	glColor3f(1, 0, 1);
	glBegin(GL_QUADS);
	glVertex2f(160, 335);
	glVertex2f(200, 345);
	glVertex2f(140, 345);
	glVertex2f(140, 335);
	glEnd();

    // flush the OpenGL commands and make sure they get rendered!
    glFlush();
}

int main( int argc, char *argv[] ) {
	
	// initialize GLUT... always need this call.
	glutInit( &argc, argv );
	
	// request a window with just an RGB frame buffer, nothing fancy...
	// place it at (50,50) on the screen and set its size to our constants from earlier
	glutInitDisplayMode( GLUT_RGB );
	glutInitWindowPosition( 50, 50 );
	glutInitWindowSize( windowWidth, windowHeight );
	
	// this actually creates our window giving it title "Lab 00"
	windowId = glutCreateWindow( "Lab 00" );
	
	// tell OpenGL to use a Flat shading model - make all of our shapes 
	// a uniform color
	glShadeModel( GL_SMOOTH );
	
	// register our reshape and display callbacks with GLUT - these functions are above
	glutDisplayFunc( renderScene );
	glutReshapeFunc( changeSize );
	
	// begin GLUT loop
	glutMainLoop();
	
	return 0;
}