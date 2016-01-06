/*
 *   Multitextured Image Based Vertex Shader
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

void main() {
    /*****************************************/
    /********* Vertex Calculations  **********/
    /*****************************************/
    
    // TODO: Vertex Shader Part I
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    /*****************************************/
    /********* Texture Calculations  *********/
    /*****************************************/
    
    // TODO: Vertex Shader Part II
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	gl_TexCoord[2] = gl_MultiTexCoord2;
}
