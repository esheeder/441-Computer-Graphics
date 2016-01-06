/*
 *   MD5 Model Fragment Shader
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

varying vec2 vTexCoord;

/*****************************************/
/*********       Textures        *********/
/*****************************************/

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

void main(void) {
    /*****************************************/
    /********* Texture Calculations  *********/
    /*****************************************/
    
    vec2 texCoord = vec2(vTexCoord.s, 1.0-vTexCoord.t); // flip the Y coord
    vec4 diffuseColor = texture2D(diffuseMap, texCoord);

    /*****************************************/
    /******* Final Color Calculations ********/
    /*****************************************/
    
    gl_FragColor = diffuseColor;
}
