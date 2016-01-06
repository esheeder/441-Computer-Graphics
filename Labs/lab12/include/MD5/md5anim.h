/*
 * md5anim.c -- md5mesh model loader + animation
 * last modification: aug. 14, 2007
 *
 * Doom3's md5mesh viewer with animation.  Animation portion.
 * Dependences: md5model.h, md5mesh.c.
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

#ifndef _MD5_MD5ANIM_H_
#define _MD5_MD5ANIM_H_

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "MD5/md5model.h"

/* Joint info */
struct joint_info_t
{
    char name[64];
    int parent;
    int flags;
    int startIndex;
};

/* Base frame joint */
struct baseframe_joint_t
{
    vec3_t pos;
    quat4_t orient;
};

/**
 * More quaternion operations for skeletal animation.
 */

float Quat_dotProduct (const quat4_t qa, const quat4_t qb);

void Quat_slerp (const quat4_t qa, const quat4_t qb, float t, quat4_t out);

/**
 * Check if an animation can be used for a given model.  Model's
 * skeleton and animation's skeleton must match.
 */
int CheckAnimValidity (const struct md5_model_t *mdl, const struct md5_anim_t *anim);

/**
 * Build skeleton for a given frame data.
 */
static void BuildFrameSkeleton (const struct joint_info_t *jointInfos,
                    const struct baseframe_joint_t *baseFrame,
                    const float *animFrameData,
                    struct md5_joint_t *skelFrame,
                                int num_joints);

/**
 * Load an MD5 animation from file.
 */
int ReadMD5Anim (const char *filename, struct md5_anim_t *anim);

/**
 * Free resources allocated for the animation.
 */
void FreeAnim (struct md5_anim_t *anim);

/**
 * Smoothly interpolate two skeletons
 */
void InterpolateSkeletons (const struct md5_joint_t *skelA,
                      const struct md5_joint_t *skelB,
                      int num_joints, float interp,
                           struct md5_joint_t *out);

/**
 * Perform animation related computations.  Calculate the current and
 * next frames, given a delta time.
 */
void Animate (const struct md5_anim_t *anim, struct anim_info_t *animInfo, double dt);

#endif