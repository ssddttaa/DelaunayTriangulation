//
//  DelaunayTriangulate.h
//  Tutorials
//
//  Created by Sadat Shaik on 6/22/15.
//
//

#ifndef Tutorials_DelaunayTriangulate_h
#define Tutorials_DelaunayTriangulate_h
// Include standard headers
#include "tetgen.h"

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

#include <cstring>

#include <vector>

#include "BufferActions.h"

#include "ParseClass.h"

#include "CGALDelaunayTriangulation.h"

#include <unistd.h>

#include <stdio.h>

#include <stdlib.h>
class DelaunayTriangulation
{
        public:
            
            std::vector<GLfloat> objects_vertex_buffers;
            std::vector<GLfloat> objects_color_buffers;
            
            vector<float> g_vertex_buffer_data;
            vector<float> g_color_buffer_data;
            vector<float> g_cube_centers;
            
            vector<float> tetrahedraArray;
            
            vector<vec3> faceArray;
            int numberOfTetrahedra = 0;
            
            vector<vec3> hullFaces;
            
            vector<vec3> nodeArray;
            int numVertices;
            
            int totalVertices = 0;
            tetgenio in, out;
            
            vector<float> *vertex_buffer_pointer;
            vector<float> *vertex_color_pointer;
    
            vec3 viewOffset;
    
            std::list<Point> VerticesToTriangulate;
            
            std::list<Point> VerticesToAdd;
            
            Triangulation TriangulationOfPoints;
            
            GLuint vertexbuffer;
            GLfloat *vertexBufferFloat;
            GLfloat *colorBufferFloat;
            GLuint colorbuffer;
    
            float currentViewScale;
    
            bool nowDraw = false;

            DelaunayTriangulation();
            void generateRandomPoints(vector<vec3> *nodeArray);
            void DelaunayTriangulate(vector<vec3> *nodeArray);
            void createAllPoints(vector<float> *vertex_buffer_pointer, vector<float> *vertex_color_pointer);
            void createAllTetrahedra();
            void triangulateUsingTetgen(vector<vec3>*nodeArray);
            void triangulateUsingCGAL(vector<vec3> *nodeArrayPointer, bool pointsDynamicallyInserted, int nodeArraySize);
            void triangulate(bool usingTetgen, vector<vec3>*nodeArray, bool pointsDynamicallyInserted, int nodeArraySize);
            void addPointsToTriangulation(vector<vec3> * pointsToAdd, int numberOfPoints);
};

#endif
