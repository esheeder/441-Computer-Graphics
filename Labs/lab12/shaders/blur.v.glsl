/*
 *   Blur Vertex Shader
 *
 *   Nothing special, pass through everything
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

void main(void) {
    /*****************************************/
    /********* Vertex Calculations  **********/
    /*****************************************/
    
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    /*****************************************/
    /********* Texture Calculations  *********/
    /*****************************************/
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
}