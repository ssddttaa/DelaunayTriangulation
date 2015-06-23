//
//  ParseClass.cpp
//  Tutorials
//
//  Created by Sadat Shaik on 6/16/15.
//
//

#include "ParseClass.h"

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

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

using namespace glm;

void ParseClass::ParseEdgeFile(string fileName, vector<float> *nodeArray, int* numberOfVertices)
{
    ifstream fileHandle;
    fileHandle.open(fileName);
    string currentLine = "";
    bool pastHeader = false;
    while(getline(fileHandle, currentLine))
    {
        istringstream iss(currentLine);
        if(!pastHeader)
        {
            iss>>*numberOfVertices;
            pastHeader = true;
        }
        else
        {
            
            float nextNumber;
            iss>>nextNumber; //Throw away first value, which is the id of hte current point.
            
            iss>>nextNumber; //Keep the second number, as that is x coordinate
            nodeArray->push_back(nextNumber);
            
            iss>>nextNumber; //Keep the third number, as that is y coordinate
            nodeArray->push_back(nextNumber);
            
            iss>>nextNumber; //Keep the fourth number, as that is z coordinate
            nodeArray->push_back(nextNumber);
            
            //Since tetrahedra have four vertices, you also have to get the fifth value to get all vertices.
            
            iss>>nextNumber; //Keep the fifth number, as that is z coordinate
            nodeArray->push_back(nextNumber);
        }
    }
}

void ParseClass::ParseNodeFile(string fileName, vector<vec3> *nodeArray, int* numberOfVertices)
{
    ifstream fileHandle;
    fileHandle.open(fileName);
    string currentLine = "";
    bool pastHeader = false;
    while(getline(fileHandle, currentLine))
    {
        istringstream iss(currentLine);
        if(!pastHeader)
        {
            iss>>*numberOfVertices;
            pastHeader = true;
        }
        else
        {
            
            float nextNumber, xCoordinate = 0.0, yCoordinate = 0.0, zCoordinate = 0.0;
            iss>>nextNumber; //Throw away first value, which is the id of hte current point.
            
            iss>>xCoordinate;
            iss>>yCoordinate;
            iss>>zCoordinate;
            
            vec3 coordinate(xCoordinate, yCoordinate, zCoordinate);
            
            nodeArray->push_back(coordinate);
        }
    }
}


void ParseClass::ParseMeshFile(string meshFile, vector<int> *facetArray, int* numberOfFacets)
{
    ifstream fileHandle;
    fileHandle.open(meshFile);
    string currentLine = "";
    bool pastHeader = false;
    getline(fileHandle, currentLine);
    while(getline(fileHandle, currentLine))
    {
        istringstream iss(currentLine);
        if(!pastHeader)
        {
            iss>>*numberOfFacets;
            pastHeader = true;
        }
        else
        {
            
            float nextNumber;
            vec3 tempFacet;
            iss>>nextNumber; //Throw away first value, which is the id of hte current point.
            
            iss>>nextNumber; //Keep the second number, as that is x coordinate
            tempFacet[0] = nextNumber;
            
            iss>>nextNumber; //Keep the third number, as that is y coordinate
            tempFacet[1] = nextNumber;
            
            iss>>nextNumber; //Keep the fourth number, as that is z coordinate
            tempFacet[2] = nextNumber;
        }
    }
}


void ParseClass::ParseFaceFile(string faceFileName, vector<vec3> *nodeArray, int* numberOfVertices)
{
    ifstream fileHandle;
    fileHandle.open(faceFileName);
    string currentLine = "";
    while(getline(fileHandle, currentLine))
    {
        istringstream iss(currentLine);
        float nextNumber;
        
        vec3 tempVec;
        iss>>nextNumber; //Keep the second number, as that is x coordinate
        
        tempVec[0] = nextNumber;
        
        iss>>nextNumber; //Keep the third number, as that is y coordinate
        tempVec[1] = nextNumber;
        
        iss>>nextNumber; //Keep the fourth number, as that is z coordinate
        tempVec[2] = nextNumber;
        
        nodeArray->push_back(tempVec);
    }
}
