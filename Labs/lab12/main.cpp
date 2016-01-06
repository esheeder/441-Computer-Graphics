/*
 * Lab12
 *
 * Code based on the following:
 *    md5mesh.c -- md5mesh model loader + animation
 *    last modification: aug. 14, 2007
 *    Doom3's md5mesh viewer with animation.
 */

#include <GL/glew.h>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <SOIL/SOIL.h>

#include <fstream>
#include <string>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Point.h"
#include "Shader_Utils.h"
#include "MD5/md5model.h"
#include "MD5/md5mesh.h"
#include "MD5/md5anim.h"

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

int windowWidth = 512, windowHeight = 512;  // window dimensions

GLint leftMouseButton, rightMouseButton;    // status of the mouse buttons
int mouseX = 0, mouseY = 0;                 // last known X and Y of the mouse

Point cameraTPR;                            // spherical coordinates of camera mapped XYZ - TPR (theta phi radius)
Point cameraLookAt;                         // cpoint camera is looking at

GLuint groundTexHandle;                     // a handle to the textures in our OpenGL context
GLuint skyboxHandles[6];                    // all of our skybox handles

struct md5_model_t md5file;
struct md5_anim_t md5anim;

int animated = 0;

struct md5_joint_t *skeleton = NULL;
struct anim_info_t animInfo;

bool DISPLAY_WIREFRAME = false;
bool DISPLAY_SKELETON = false;

GLuint blurShaderProgramHandle;
GLuint framebufferSizeLoc, blurSizeLoc;
GLfloat BLUR_SIZE = 1;
GLuint framebufferHandle, renderbufferHandle, fboTexHandle;
GLuint framebufferWidth = 1024;
GLuint framebufferHeight = 1024;

// END GLOBAL VARIABLES ////////////////////////////////////////////////////////

//
// void computeArcballPosition(Point tpr, Point &xyz);
//
// This function updates the camera's position in cartesian coordinates based
//  on its position in spherical coordinates. The user passes in the current phi,
//  theta, and radius, as a single point (X value of point = phi, etc.)  as well
//  as a Point variable to hold the resulting X, Y, and Z values.
//
//  That Point gets filled with the camera's position in R3.
//
void computeArcballPosition( Point tpr, Point &xyz ) {
    xyz.setX( tpr.getZ() *  sinf( tpr.getX() )*sinf( tpr.getY() ) );
    xyz.setY( tpr.getZ() * -cosf( tpr.getY() ) );
    xyz.setZ( tpr.getZ() * -cosf( tpr.getX() )*sinf( tpr.getY() ) );
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
//  void mouseCallback(int button, int state, int thisX, int thisY)
//
//  GLUT callback for mouse clicks. We save the state of the mouse button
//      when this is called so that we can check the status of the mouse
//      buttons inside the motion callback (whether they are up or down).
//
void mouseCallback(int button, int state, int thisX, int thisY) {
    //update the left and right mouse button states, if applicable
    if(button == GLUT_LEFT_BUTTON)
        leftMouseButton = state;
    else if(button == GLUT_RIGHT_BUTTON)
        rightMouseButton = state;
    
    //and update the last seen X and Y coordinates of the mouse
    mouseX = thisX;
    mouseY = thisY;
}

//
//  void mouseMotion(int x, int y)
//
//  GLUT callback for mouse movement. We update the model's rotational angles
//      if the left mouse button is held down.
//
void mouseMotion(int x, int y) {
    //if the user is holding down the left mouse button, update the angles.
    if(leftMouseButton == GLUT_DOWN) {
        cameraTPR.setX( cameraTPR.getX() + (x - mouseX)*0.005 );
        cameraTPR.setY( cameraTPR.getY() + (y - mouseY)*0.005 );
        
        // make sure that phi stays within the range (0, M_PI)
        if(cameraTPR.getY() <= 0)
            cameraTPR.setY( 0+0.001 );
        if(cameraTPR.getY() >= M_PI)
            cameraTPR.setY( M_PI-0.001 );
        
        //cameraDirX/Y/Z will just get updated at the start of every
        //render frame, so don't worry about updating them here.
    } else if(rightMouseButton == GLUT_DOWN) {
        //for the right mouse button, just determine how much the mouse has moved total.
        //not the best "zoom" behavior -- if X change is positive and Y change is negative,
        //(along the X = Y line), nothing will happen!! but it's ok if you zoom by
        //moving left<-->right or up<-->down, which works for most people i think.
        double totalChangeSq = (x - mouseX) + (y - mouseY);
        cameraTPR.setZ( cameraTPR.getZ() + totalChangeSq*0.1 );
        
        //limit the camera radius to some reasonable values so the user can't get lost
        if(cameraTPR.getZ() < 1.0)
            cameraTPR.setZ( 1.0 );
        if(cameraTPR.getZ() > 290.0)
            cameraTPR.setZ( 290.0 );
    }
    
    mouseX = x;
    mouseY = y;
}

void init (const char *filename, const char *animfile)
{
    /* Initialize OpenGL context */
    glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
    
    //tell OpenGL not to use the material system; just use whatever we pass with glColor()
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    float lightCol[4] = { 1, 1, 1, 1};
    float ambientCol[4] = {0.1, 0.1, 0.1, 1.0};
    glLightfv( GL_LIGHT0,GL_DIFFUSE,lightCol );
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable( GL_DEPTH_TEST );
    
    //tells OpenGL to blend colors across triangles. Once lighting is
    //enabled, this means that objects will appear smoother - if your object
    //is rounded or has a smooth surface, this is probably a good idea;
    //if your object has a blocky surface, you probably want to disable this.
    glShadeModel(GL_SMOOTH);
    
    // give our camera a pretty starting point
    cameraTPR = Point(2.0, 1.57, 200.0);
    cameraLookAt = Point(0,0,0);
    
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    /* Load MD5 model file */
    if (!ReadMD5Model (filename, &md5file))
        exit (EXIT_FAILURE);
    
    AllocVertexArrays ();
    
    /* Load MD5 animation file */
    if (animfile)
    {
        if (!ReadMD5Anim (animfile, &md5anim))
        {
            FreeAnim (&md5anim);
        }
        else
        {
            animInfo.curr_frame = 0;
            animInfo.next_frame = 1;
            
            animInfo.last_time = 0;
            animInfo.max_time = 1.0 / md5anim.frameRate;
            
            /* Allocate memory for animated skeleton */
            skeleton = (struct md5_joint_t *)
            malloc (sizeof (struct md5_joint_t) * md5anim.num_joints);
            
            animated = 1;
        }
    }
    
    if (!animated) {
        printf ("[.md5anim]: no animation loaded.\n");
    }
    
    printf("\n");
    
    // TODO #6 - Setup Framebuffer
	// 1) create the FBO
	glGenFramebuffers(1, &framebufferHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferHandle);

	// 2) create a renderbuffer (so we have a z-buffer) and attach it.
	// give the FBO a depth buffer (via a render buffer object -- kind of obtuse
	// so that when it gets rendered to, it can properly perform hidden surface
	// removal!
	glGenRenderbuffers(1, &renderbufferHandle);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbufferHandle);
	glRenderbufferStorage(GL_RENDERBUFFER,
		GL_DEPTH_COMPONENT,
		framebufferWidth,
		framebufferHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER,
		renderbufferHandle);

	// 3) create a texture, initialize it, and attach it to the FBO. (we render to
	// this.)
	// to hold the RGBA for our FBO, create a texture. 
	glGenTextures(1, &fboTexHandle);
	glBindTexture(GL_TEXTURE_2D, fboTexHandle);

	// SET UP ALL of the texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// initialize it with nothing! no image! just tell it to hold that much space.
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		framebufferWidth,
		framebufferHeight,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		NULL);

	// and finally, attach that texture to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		fboTexHandle,
		0);

	// check for any errors
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer initialized completely!\n");
	else
		printf("Framebuffer FAILED TO INITIALIZE COMPLETELY.\n");
}

void reshape (int w, int h)
{
    if (h == 0)
        h = 1;
    
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (45.0, w/(GLdouble)h, 0.1, 1000.0);
    
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}

//
//  void drawSkyboxPanel( ...params... )
//
//  Draws a single textured quad.  This became a bit more complex than intended, but it
//      allows for a generic definition.  We specify the two in-plane dimensions (dim1
//      and dim2) to determine our four corners.  These corners are centered around
//      point and are size distance away.  The quad is oriented with the provided
//      normal.  The provided texture is applied to the quad and can be flipped
//      along either axis if needed.
//
void drawSkyboxPanel( GLuint texHandle, Point point, Vector dim1, Vector dim2, Vector normal, int flippingFactor, int flippingFactor2, int size ) {
    // we're cheating by making our quads intersect at the edges to try and remove the joint line we sometimes see
    Point p1 = point + dim1*-(size+1) + dim2*-(size+1);  // LL corner
    Point p2 = point + dim1*(size+1) + dim2*-(size+1);   // LR corner
    Point p3 = point + dim1*(size+1) + dim2*(size+1);    // UR corner
    Point p4 = point + dim1*-(size+1) + dim2*(size+1);   // UL corner
    
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texHandle );
    glPushMatrix(); {
        glColor4f(1,1,1,1);
        glBegin( GL_QUADS ); {
            glTexCoord2f( 0, 0);
            normal.glNormal();
            p1.glVertex();
            
            glTexCoord2f( 1*flippingFactor, 0);
            normal.glNormal();
            p2.glVertex();
            
            glTexCoord2f( 1*flippingFactor, 1*flippingFactor2);
            normal.glNormal();
            p3.glVertex();
            
            glTexCoord2f( 0, 1*flippingFactor2);
            normal.glNormal();
            p4.glVertex();
        }; glEnd();
    }; glPopMatrix();
    glDisable( GL_TEXTURE_2D );
}

//
//  void drawSkybox( int size )
//
//  Draws the six planes of our sky box.  Texturing each and having them oriented the correct way.
//
void drawSkybox( int size ) {
    glUseProgram( 0 );
    glDisable( GL_LIGHTING );
    glColor4f(1,1,1,1);
    //                 texHandle             Point                  dim1            dim2            Normal         flip   size          panel
    drawSkyboxPanel( skyboxHandles[0], Point(   -1,     0, -size), Vector(1,0,0), Vector(0,1,0), Vector( 0, 0, 1), -1,  1, size );     // back
    drawSkyboxPanel( skyboxHandles[1], Point( size,     0,     1), Vector(0,0,1), Vector(0,1,0), Vector(-1, 0, 0), -1,  1, size );     // right
    drawSkyboxPanel( skyboxHandles[2], Point(    1,     0,  size), Vector(1,0,0), Vector(0,1,0), Vector( 0, 0,-1),  1,  1, size );     // front
    drawSkyboxPanel( skyboxHandles[3], Point(-size,     0,    -1), Vector(0,0,1), Vector(0,1,0), Vector( 1, 0, 0),  1,  1, size );     // left
    drawSkyboxPanel( skyboxHandles[5], Point(    0,  size,     0), Vector(1,0,0), Vector(0,0,1), Vector( 0,-1, 0),  1, -1, size );     // top
    drawSkyboxPanel(  groundTexHandle, Point(    0, -size,     0), Vector(1,0,0), Vector(0,0,1), Vector( 0, 1, 0),  1,  1, size );     // bottom
    //                 texHandle             Point                  dim1            dim2            Normal         flip   size          panel
    
    glEnable( GL_LIGHTING );
}

void display () {
    int i;
    
    static double curent_time = 0;
    static double last_time = 0;
    
    last_time = curent_time;
    curent_time = (double)glutGet (GLUT_ELAPSED_TIME) / 1000.0;
    
    // compute where on the sphere our camera is located
    Point cameraPosition;
    computeArcballPosition(cameraTPR, cameraPosition);
    
    // now compute where our sphere is located relative to the look at point
    Point eyePosition = cameraPosition + (cameraLookAt - Point(0,0,0));
    
    // TODO #7: Bind Framebuffer and Render full scene to framebuffer texture
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferHandle);
	glPushAttrib(GL_VIEWPORT_BIT); {
		glViewport(0, 0, framebufferWidth, framebufferHeight);
		// render our scene as desired
		// set camera, lights, materials, draw objects, etc.
		gluLookAt(eyePosition, cameraLookAt, Vector(0, 1, 0));
		float lPosition[4] = { 100, 100, 100, 1 };
		glLightfv(GL_LIGHT0, GL_POSITION, lPosition);

		// make sure that OpenGL has finished rendering everything...
		glFlush();
		glFinish();
		// and clean up. detatch the framebuffer so we render to the screen again.
		
	// the first pass is now inside of that texture! (fboTexHandle!)
	}; glPopAttrib();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/* clear color and depth buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // update the modelview matrix based on the camera's position
        glMatrixMode( GL_MODELVIEW );         // make sure we aren't changing the projection matrix!
        glLoadIdentity();
        gluLookAt( eyePosition, cameraLookAt, Vector(0,1,0) );
        
        // this is some code to enable a default light for the scene;
        float lPosition[4] = { 100, 100, 100, 1 };
        glLightfv( GL_LIGHT0, GL_POSITION, lPosition );
        
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        drawSkybox( 300 );
        
        if (animated)
        {
            /* Calculate current and next frames */
            Animate (&md5anim, &animInfo, curent_time - last_time);
            
            /* Interpolate skeletons between two frames */
            InterpolateSkeletons (md5anim.skelFrames[animInfo.curr_frame],
                                  md5anim.skelFrames[animInfo.next_frame],
                                  md5anim.num_joints,
                                  animInfo.last_time * md5anim.frameRate,
                                  skeleton);
        }
        else
        {
            /* No animation, use bind-pose skeleton */
            skeleton = md5file.baseSkel;
        }
        
        glPushMatrix(); {
            glTranslatef (0.0f, -50.0f, 0.0f);
            
            // draw our ground
            glPushMatrix(); {
                glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
                float squareSize = 100;
                glEnable( GL_TEXTURE_2D );
                glBindTexture( GL_TEXTURE_2D, groundTexHandle );
                glBegin( GL_QUADS ); {
                    glNormal3f( 0, 1, 0 ); glTexCoord2f( 0, 0 ); glVertex3f( -squareSize, 0, -squareSize );
                    glNormal3f( 0, 1, 0 ); glTexCoord2f( 0, 1 ); glVertex3f( -squareSize, 0,  squareSize );
                    glNormal3f( 0, 1, 0 ); glTexCoord2f( 1, 1 ); glVertex3f(  squareSize, 0,  squareSize );
                    glNormal3f( 0, 1, 0 ); glTexCoord2f( 1, 0 ); glVertex3f(  squareSize, 0, -squareSize );
                }; glEnd();
            }; glPopMatrix();
            
            glRotatef (-90.0f, 1.0, 0.0, 0.0);
            
            /* Draw skeleton */
            if( DISPLAY_SKELETON ) {
                DrawSkeleton (skeleton, md5file.num_joints);
            }
            
            glColor3f (1.0f, 1.0f, 1.0f);
            
            if( DISPLAY_WIREFRAME ) {
                glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
                glLineWidth( 2.0f );
            } else {
                glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
                glLineWidth( 1.0f );
            }
            
            /* Draw each mesh of the model */
            for (i = 0; i < md5file.num_meshes; ++i)
            {
                md5_mesh_t mesh = md5file.meshes[i];
                
                PrepareMesh (&mesh, skeleton);
                
                DrawMesh( &mesh );
            }
            
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        }; glPopMatrix();
        
    // TODO #8: Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // TODO #9: Render texture to a 2D quad filling the scene
	/* render our texture to a 2D quad covering the entire window */
	// reset the projection matrix for our quad...
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); {
		glLoadIdentity();
		gluOrtho2D(-1, 1, -1, 1);

		//and the modelview...
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, fboTexHandle);
		glUseProgram(blurShaderProgramHandle);
		glBegin(GL_QUADS); {
			glTexCoord2f(0, 0); glVertex2f(-1, -1);
			glTexCoord2f(1, 0); glVertex2f(1, -1);
			glTexCoord2f(1, 1); glVertex2f(1, 1);
			glTexCoord2f(0, 1); glVertex2f(-1, 1);
		}; glEnd();
		glUseProgram(0);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}; glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glutSwapBuffers();
    // TODO #10: Apply blur shader
    
    glutSwapBuffers();
}

void
keyboard (unsigned char key, int x, int y)
{
    /* Escape */
    if (key == 27 || key == 'q' || key == 'Q')
        exit (0);
    else if (key == 'm' || key == 'M')
        DISPLAY_WIREFRAME = !DISPLAY_WIREFRAME;
    else if (key == 'b' || key == 'B')
        DISPLAY_SKELETON = !DISPLAY_SKELETON;
    
    // move camera forward - shrink the radius (remember Z = Radius for spherical point)
    else if(key == 'w' || key == 'W')
        cameraTPR.setZ( cameraTPR.getZ() - 1.0f );
    
    // move camera backward - increase the radius
    else if(key == 's' || key == 'S')
        cameraTPR.setZ( cameraTPR.getZ() + 1.0f );
    
    else if(key == '+') {
        BLUR_SIZE += 1;
    }
    else if(key == '-') {
        BLUR_SIZE -= 1;
        if( BLUR_SIZE < 1 ) BLUR_SIZE = 1;
    }
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
    glutPostRedisplay();
    
    glutTimerFunc((unsigned int)(1000.0 / 60.0), myTimer, 0);
}

void cleanup() {
    FreeModel (&md5file);
    FreeAnim (&md5anim);
    
    if (animated && skeleton)
    {
        free (skeleton);
        skeleton = NULL;
    }
    
    FreeVertexArrays ();
}

//
//  loadAndRegisterTexture( string, textureParams )
//
//  Loads and registers the texture using SOIL.  Then sets our texture parameters for it.  The
//  texture parameters can be specified, or use defaults.
//
GLuint loadAndRegisterTexture( string textureFileName,
                              GLint minFilter = GL_LINEAR_MIPMAP_LINEAR,
                              GLint magFilter = GL_LINEAR,
                              GLint wrapS = GL_REPEAT,
                              GLint wrapT = GL_REPEAT,
                              GLint envMode = GL_MODULATE ) {
    GLuint texHandle = SOIL_load_OGL_texture( textureFileName.c_str(),
                                             SOIL_LOAD_AUTO,
                                             SOIL_CREATE_NEW_ID,
                                             SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT );
    
    if( texHandle == 0 ) {
        printf( "[ERROR]: Could not load texture \"%s\"\n", textureFileName.c_str() );
    } else {
        glBindTexture(   GL_TEXTURE_2D,  texHandle                        );
        glTexParameteri( GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, minFilter );
        glTexParameteri( GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, magFilter );
        glTexParameteri( GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S,     wrapS     );
        glTexParameteri( GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T,     wrapT     );
        glTexEnvi(       GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,   envMode   );
    }
    
    return texHandle;
}

int main( int argc, char *argv[] ) {
    if (argc < 2) {
        fprintf (stderr, "usage: %s <filename.md5mesh> [<filename.md5anim>]\n", argv[0]);
        return 0;
    }
    
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (640, 480);
    glutCreateWindow ("Lab12 - MD5 Model w/ FBO & VBO");
    
    /* initialize GLEW */
    GLenum glewResult = glewInit();
    /* check for an error */
    if( glewResult != GLEW_OK ) {
        printf( "[ERROR]: Error initalizing GLEW\n");
        return 0;
    }
    /* make sure OpenGL 2.0 is supported */
    if( !glewIsSupported( "GL_VERSION_2_0" ) ) {
        printf( "[ERROR]: System does not support OpenGL 2.0 and GLSL 1.20\n" );
        return 0;
    }
    
    PrintOpenGLInfo();
    
    printf( "[INFO]: System supports OpenGL2.0 and GLSL 1.20!\n\n" );
    
    atexit (cleanup);
    init (argv[1], (argc > 2) ? argv[2] : NULL);
    printf( "[INFO]: OpenGL Scene set up\n" );
    
    groundTexHandle = loadAndRegisterTexture( "textures/metal.jpg" );
    printf( "[INFO]: ground texture read in and registered\n" );
    
    // and get handles for our full skybox
    printf( "[INFO]: registering skybox..." );
    fflush( stdout );
    skyboxHandles[0] = loadAndRegisterTexture( "textures/skybox/necros_hell_back.JPG"   );   printf( "." ); fflush( stdout );
    skyboxHandles[1] = loadAndRegisterTexture( "textures/skybox/necros_hell_right.JPG"  );   printf( "." ); fflush( stdout );
    skyboxHandles[2] = loadAndRegisterTexture( "textures/skybox/necros_hell_front.JPG"  );   printf( "." ); fflush( stdout );
    skyboxHandles[3] = loadAndRegisterTexture( "textures/skybox/necros_hell_left.JPG"   );   printf( "." ); fflush( stdout );
    skyboxHandles[5] = loadAndRegisterTexture( "textures/skybox/necros_hell_top.JPG"    );   printf( "." ); fflush( stdout );
    printf( "skybox textures read in and registered!\n\n" );
    
    blurShaderProgramHandle = createShaderProgram( "shaders/blur.v.glsl", "shaders/blur.f.glsl", "Blur Shader Program" );
    framebufferSizeLoc = glGetUniformLocation( blurShaderProgramHandle, "framebufferSize" );
    blurSizeLoc = glGetUniformLocation( blurShaderProgramHandle, "blurSize" );
    
    glutReshapeFunc (reshape);
    glutDisplayFunc (display);
    glutKeyboardFunc (keyboard);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);
    glutTimerFunc((unsigned int)(1000.0 / 60.0), myTimer, 0);
    
    glutMainLoop ();
    
    return 0;
}
