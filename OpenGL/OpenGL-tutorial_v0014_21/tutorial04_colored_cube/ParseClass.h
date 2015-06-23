//
//  ParseClass.h
//  Tutorials
//
//  Created by Sadat Shaik on 6/16/15.
//
//

#ifndef __Tutorials__ParseClass__
#define __Tutorials__ParseClass__

#include <stdio.h>
#include <string>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
using namespace std;
using namespace glm;
class ParseClass
{
    public:
        static void ParseEdgeFile(string nodeFile, vector<float> *nodeArray, int* numberOfVertices);
        static void ParseNodeFile(string nodeFile, vector<vec3> *nodeArray, int* numberOfVertices);
        static void ParseMeshFile(string meshFile, vector<int> *facetArray, int* numberOfFacets);
        static void ParseFaceFile(string faceFileName, vector<vec3> *nodeArray, int* numberOfVertices);
};

#endif /* defined(__Tutorials__ParseClass__) */
