/*
 * md5mesh.c -- md5mesh model loader + animation
 * last modification: aug. 14, 2007
 *
 * Doom3's md5mesh viewer with animation.  Mesh portion.
 * Dependences: md5model.h, md5anim.c.
 *
 * Copyright (c) 2005-2007 David HENRY
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * gcc -Wall -ansi -lGL -lGLU -lglut md5anim.c md5anim.c -o md5model
 */

#ifndef _MD5_MD5MESH_H_
#define _MD5_MD5MESH_H_ 1

#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#include <string>
using namespace std;

#include "MD5/md5model.h"

#define BUFFER_OFFSET(i) ((void*)(i))

/**
 * Basic quaternion operations.
 */

void Quat_computeW (quat4_t q);

void Quat_normalize (quat4_t q);

void Quat_multQuat (const quat4_t qa, const quat4_t qb, quat4_t out);

void Quat_multVec (const quat4_t q, const vec3_t v, quat4_t out);

void Quat_rotatePoint (const quat4_t q, const vec3_t in, vec3_t out);

GLuint loadTexture( string filename );

/**
 * Load an MD5 model from file.
 */
int ReadMD5Model (const char *filename, struct md5_model_t *mdl);

/**
 * Free resources allocated for the model.
 */
void FreeModel (struct md5_model_t *mdl);

/**
 * Prepare a mesh for drawing.  Compute mesh's final vertex positions
 * given a skeleton.  Put the vertices in vertex arrays.
 */
void PrepareMesh (const struct md5_mesh_t *mesh, const struct md5_joint_t *skeleton);

void DrawMesh ( const struct md5_mesh_t *mesh );

void AllocVertexArrays();

void FreeVertexArrays ();

/**
 * Draw the skeleton as lines and points (for joints).
 */
void DrawSkeleton (const struct md5_joint_t *skeleton, int num_joints);


#endif