//
//  DrawObjects.h
//  Tutorials
//
//  Created by Sadat Shaik on 6/14/15.
//
//

#ifndef __Tutorials__DrawObjects__
#define __Tutorials__DrawObjects__



#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

class DrawObjects
{
    public:
        GLfloat cube_vertex_buffer_data[108];
        GLfloat cube_color_buffer_data[108];
        void draw();
        static void createCubeWithTranslation(float xOffset, float yOffset, float zOffset, float scale, GLfloat *vertex_buffer, int* totalVertices);
};


#endif /* defined(__Tutorials__DrawObjects__) */