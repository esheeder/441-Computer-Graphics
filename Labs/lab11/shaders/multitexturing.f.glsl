/*
 *   Multitextured Image Based Fragment Shader
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

// TODO 1: Fragment Shader Part I

uniform sampler2D grassTex;
uniform sampler2D dirtTex;
uniform sampler2D drynessMapTex;

void main() {
    /*****************************************/
    /********* Texture Calculations  *********/
    /*****************************************/
    // TODO 2: Fragment Shader Part II
	vec4 grass = texture2D(grassTex, gl_TexCoord[0].st);
	vec4 dirt = texture2D(dirtTex, gl_TexCoord[1].st);
	vec4 drynessMap = texture2D(drynessMapTex, gl_TexCoord[2].st);
    /*****************************************/
    /******* Final Color Calculations ********/
    /*****************************************/
    
    // TODO 3: Fragment Shader Part III
	gl_FragColor = mix(grass, dirt, drynessMap.r);
}
