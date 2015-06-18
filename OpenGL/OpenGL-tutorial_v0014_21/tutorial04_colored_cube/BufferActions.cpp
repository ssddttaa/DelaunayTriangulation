//
//  BufferActions.cpp
//  Tutorials
//
//  Created by Sadat Shaik on 6/15/15.
//
//

#include "BufferActions.h"


void BufferActions::addObjectToBuffer(GLfloat *vertexesToAdd, GLfloat *colorsToAdd, int numberOfVertices, vector<float> *vertex_buffer_pointer, vector<float> *color_buffer_pointer)
{
    for(int i = 0;i<numberOfVertices;i++)
    {
        vertex_buffer_pointer->push_back(vertexesToAdd[i]);
        color_buffer_pointer->push_back(colorsToAdd[i]);
    }
}

void BufferActions::addVec3ToBuffer(int * arrayOfVec3, vector<float> *buffer_pointer, vector<vec3> * allVec3s, int numberOfVectorsToAdd)
{
    for(int i = 0;i<numberOfVectorsToAdd;i++)
    {
        int whichPointToAdd = arrayOfVec3[i];
        vec3 tempVec(allVec3s->at(arrayOfVec3[whichPointToAdd]));
        buffer_pointer->push_back(tempVec[0]);
        buffer_pointer->push_back(tempVec[1]);
        buffer_pointer->push_back(tempVec[2]);
    }
}
