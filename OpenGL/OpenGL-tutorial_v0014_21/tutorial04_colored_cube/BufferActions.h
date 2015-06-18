//
//  BufferActions.h
//  Tutorials
//
//  Created by Sadat Shaik on 6/15/15.
//
//

#ifndef __Tutorials__BufferActions__
#define __Tutorials__BufferActions__


// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "DrawObjects.h"

using namespace glm;
using namespace std;

#include <common/shader.hpp>

#include <iostream>

#include <vector>

#include "BufferActions.h"


class BufferActions
{
    public:
        static void addObjectToBuffer(GLfloat * bufferToAdd, GLfloat *colorsToAdd, int numberOfVertices, vector<float> *vertex_buffer_pointer, vector<float> *color_buffer_pointer);
        static void addVec3ToBuffer(int * arrayOfVec3,vector<float> *buffer_pointer, vector<vec3> * allVec3s, int numberOfVectorsToAdd);
};

#endif /* defined(__Tutorials__BufferActions__) */