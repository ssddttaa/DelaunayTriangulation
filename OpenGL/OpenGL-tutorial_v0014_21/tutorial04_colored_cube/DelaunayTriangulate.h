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
GLFWwindow* window;

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

#include "ParseClass.h"

#include "CGALDelaunayTriangulation.h"

#include <unistd.h>

std::vector<GLfloat> objects_vertex_buffers;
std::vector<GLfloat> objects_color_buffers;

vector<float> g_vertex_buffer_data;
vector<float> g_color_buffer_data;
vector<float> g_cube_centers;

vector<float> tetrahedraArray;

vector<vec3> faceArray;
int numberOfTetrahedra = 0;

vector<vec3> hullFaces;

GLfloat cube_vertex_data[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

//RAINBOW CUBE
GLfloat cube_color_data[] = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

vector<vec3> nodeArray;
int numVertices = 200;

int totalVertices = 0;
tetgenio in, out;

GLuint programID;

// Get a handle for our "MVP" uniform
GLuint MatrixID;

// Get a handle for our buffers
GLuint vertexPosition_modelspaceID;
GLuint vertexColorID;
// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Projection;
// Camera matrix
glm::mat4 View;
// Model matrix : an identity matrix (model will be at the origin)
glm::mat4 Model;
// Our ModelViewProjection : multiplication of our 3 matrices
glm::mat4 MVP;

vector<float> *vertex_buffer_pointer;
vector<float> *vertex_color_pointer;

std::list<Point> VerticesToTriangulate;

std::list<Point> VerticesToAdd;

Triangulation TriangulationOfPoints;

void generateRandomPoints(vector<vec3> *nodeArray);

void DelaunayTriangulate(vector<vec3> *nodeArray);

void setupWindow();

void createAllPoints(vector<float> *vertex_buffer_pointer, vector<float> *vertex_color_pointer);

void createAllTetrahedra();

void setBuffers();

void drawLoop(int numberOfVertices, bool drawUsingTetgen);

void cleanup();

void triangulateUsingTetgen(vector<vec3>*nodeArray);

void triangulateUsingCGAL(vector<vec3> *nodeArrayPointer, bool pointsDynamicallyInserted, int nodeArraySize);

void triangulate(bool usingTetgen, vector<vec3>*nodeArray, bool pointsDynamicallyInserted, int nodeArraySize);

void addPointsToTriangulation(vector<vec3> * pointsToAdd, int numberOfPoints);

GLuint vertexbuffer;
GLfloat *vertexBufferFloat;
GLfloat *colorBufferFloat;
GLuint colorbuffer;

bool nowDraw = false;

#endif
