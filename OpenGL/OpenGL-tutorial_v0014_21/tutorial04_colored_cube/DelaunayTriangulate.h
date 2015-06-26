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

using namespace std;

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
            
            std::vector<float> objects_vertex_buffers;
            std::vector<float> objects_color_buffers;
            
            vector<float> g_vertex_buffer_data;
            vector<float> g_color_buffer_data;
            vector<float> g_cube_centers;
            
            vector<float> tetrahedraArray;
            
            vector<vector<float> > faceArray;
            int numberOfTetrahedra = 0;
            
            vector<vector<float> > hullFaces;
    
            vector<vector<float> > nodeArray;
            int numVertices;
            
            int totalVertices = 0;
            tetgenio in, out;
            
            vector<float> *vertex_buffer_pointer;
            vector<float> *vertex_color_pointer;
    
            vector<float> viewOffset;
    
            std::list<Point> VerticesToTriangulate;
            
            std::list<Point> VerticesToAdd;
            
            Triangulation TriangulationOfPoints;
    
            vector<float> *vertexBufferFloat;
            vector<float> *colorBufferFloat;
    
            float currentViewScale;
    
            bool nowDraw = false;

            DelaunayTriangulation();
            void generateRandomPoints(vector<vector<float> > *nodeArray);
            void DelaunayTriangulate(vector<vector<float> > *nodeArray);
            void createAllPoints(vector<float> *vertex_buffer_pointer, vector<float> *vertex_color_pointer);
            void createAllTetrahedra();
            void triangulateUsingTetgen(vector<vector<float> >*nodeArray);
            void triangulateUsingCGAL(vector<vector<float> > *nodeArrayPointer, bool pointsDynamicallyInserted, int nodeArraySize);
            void triangulate(bool usingTetgen, vector<vector<float> >*nodeArray, bool pointsDynamicallyInserted, int nodeArraySize);
            void addPointsToTriangulation(vector<vector<float> > * pointsToAdd, int numberOfPoints);
};

#endif
