/*
 *   MD5 Model Vertex Shader
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

varying vec2 vTexCoord;

uniform sampler2D heightMap;

void main(void) {
    /*****************************************/
    /********* Vertex Calculations  **********/
    /*****************************************/
    
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    /*****************************************/
    /********* Texture Calculations  *********/
    /*****************************************/
    
    vTexCoord = gl_MultiTexCoord0.st;
}
