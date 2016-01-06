#include "Shader_Utils.h"

#include <GL/glew.h>

#include <stdio.h>
#include <string.h>

#include <fstream>
#include <string>
using namespace std;

// PrintOpenGLInfo() ////////////////////////////////////////////////////////////
//
//  Print the OpenGL version and attribute info to the screen
//
////////////////////////////////////////////////////////////////////////////////
void PrintOpenGLInfo() {
    GLint maxVertexAttribs = 0;
    glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs );
    
    GLint maxLights = 0;
    glGetIntegerv( GL_MAX_LIGHTS, &maxLights );
    
    GLint maxTextures = 0;
    glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures );
    
    fprintf( stdout, "[INFO]: /--------------------------------------------------------\\\n");
    fprintf( stdout, "[INFO]: | OpenGL Information                                     |\n");
    fprintf( stdout, "[INFO]: |--------------------------------------------------------|\n");
    fprintf( stdout, "[INFO]: |   OpenGL Version:  %35s |\n", glGetString(GL_VERSION) );
    fprintf( stdout, "[INFO]: |   OpenGL Renderer: %35s |\n", glGetString(GL_RENDERER) );
    fprintf( stdout, "[INFO]: |   OpenGL Vendor:   %35s |\n", glGetString(GL_VENDOR) );
    fprintf( stdout, "[INFO]: |   Shading Version: %35s |\n", glGetString(GL_SHADING_LANGUAGE_VERSION) );
    fprintf( stdout, "[INFO]: >--------------------------------------------------------<\n");
    fprintf( stdout, "[INFO]: |   Max # Lights:    %35d |\n", maxLights );
    fprintf( stdout, "[INFO]: |   Max # Textures:  %35d |\n", maxTextures );
    fprintf( stdout, "[INFO]: |   Max # Vertex Attributes:  %26d |\n", maxVertexAttribs );
    fprintf( stdout, "[INFO]: \\--------------------------------------------------------/\n\n");
}

// readTextFromFile() //////////////////////////////////////////////////////////
//
//  Reads in a text file as a single string. Used to aid in shader loading.
//
////////////////////////////////////////////////////////////////////////////////
bool readTextFromFile(string filename, char* &output){
    string buf = string("");
    string line;
    
    ifstream in(filename.c_str());
    if( !in.is_open() ) {
        fprintf( stderr, "[ERROR]: Could not open shader file \"%s\"\n", filename.c_str() );
        return false;
    }
    
    while( getline(in, line) ) {
        buf += line + "\n";
    }
    output = new char[buf.length()+1];
    strncpy(output, buf.c_str(), buf.length());
    output[buf.length()] = '\0';
    
    in.close();
    
    return true;
}

// printLog() //////////////////////////////////////////////////////////////////
//
//  Check for errors from compiling or linking a vertex/fragment/shader program
//	Prints to terminal
//
////////////////////////////////////////////////////////////////////////////////
void printLog( GLuint handle, string filename = "" ) {
    int infologLength = 0;
    int maxLength;
    
    /* check if the handle is to a vertex/fragment shader */
    if( glIsShader( handle ) ) {
        glGetShaderiv(  handle, GL_INFO_LOG_LENGTH, &maxLength );
    }
    /* check if the handle is to a shader program */
    else {
        glGetProgramiv( handle, GL_INFO_LOG_LENGTH, &maxLength );
    }
    
    /* create a buffer of designated length */
    char infoLog[maxLength];
    
    bool isShader;
    if( glIsShader( handle ) ) {
        /* get the info log for the vertex/fragment shader */
        glGetShaderInfoLog(  handle, maxLength, &infologLength, infoLog );
        
        isShader = true;
    } else {
        /* get the info log for the shader program */
        glGetProgramInfoLog( handle, maxLength, &infologLength, infoLog );
        
        isShader = false;
    }
    
    /* if the length of the log is greater than 0 */
    if( infologLength > 0 ) {
        /* print info to terminal */
        printf( "[INFO]: %s Handle %d (%s): %s\n", (isShader ? "Shader" : "Program"), handle, filename.c_str(), infoLog );
    } else {
        printf( "[INFO]: %s Handle %d (%s): No errors!\n", (isShader ? "Shader" : "Program"), handle, filename.c_str() );
    }
}

// compileShader() ///////////////////////////////////////////////////////////////
//
//  Compile a given shader program
//
////////////////////////////////////////////////////////////////////////////////
GLuint compileShader( string filename, GLenum shaderType ) {
    char *shaderString;
    
    /* read in each text file and store the contents in a string */
    if( !readTextFromFile( filename.c_str(), shaderString ) ) {
        fprintf( stderr, "[ERROR]: Could not compile %s Shader.\n", (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment") );
        return 0;
    }
    
    /* create a handle to our shader */
    GLuint shaderHandle = glCreateShader( shaderType );
    
    /* send the contents of each program to the GPU */
    glShaderSource( shaderHandle, 1, (const char**)&shaderString, NULL );
    
    /* we are good programmers so free up the memory used by each buffer */
    delete [] shaderString;
    
    /* compile each shader on the GPU */
    glCompileShader( shaderHandle );	printLog( shaderHandle, filename );
    
    return shaderHandle;
}

// createShaderProgram() ///////////////////////////////////////////////////////
//
//  Compile and Register our Vertex and Fragment Shaders
//
////////////////////////////////////////////////////////////////////////////////
GLuint createShaderProgram( char *vertexShaderFilename, char *fragmentShaderFilename, string programName = "" ) {
    
    /* compile each one of our shaders */
    GLuint vertexShaderHandle   = compileShader( vertexShaderFilename,   GL_VERTEX_SHADER   );
    GLuint fragmentShaderHandle = compileShader( fragmentShaderFilename, GL_FRAGMENT_SHADER );
    
    if( vertexShaderHandle == 0 || fragmentShaderHandle == 0 ) {
        fprintf( stderr, "[ERROR]: Could not link Shader Program.  Problem with vertex/fragment shader.\n");
        return 0;
    }
    
    /* get a handle to a shader program */
    GLuint shaderProgramHandle = glCreateProgram();
    
    /* attach the vertex, geometry, and fragment shaders to the shader program */
    glAttachShader( shaderProgramHandle, vertexShaderHandle   );
    glAttachShader( shaderProgramHandle, fragmentShaderHandle );	
    
    /* link all the programs together on the GPU */
    glLinkProgram( shaderProgramHandle );	printLog( shaderProgramHandle, programName );
    
    /* return handle */
    return shaderProgramHandle;
}
