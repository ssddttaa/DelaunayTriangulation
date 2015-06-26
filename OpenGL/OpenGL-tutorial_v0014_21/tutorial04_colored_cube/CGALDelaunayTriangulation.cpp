//
//  CGALDelaunayTriangulation.cpp
//  Tutorials
//
//  Created by Sadat Shaik on 6/23/15.
//
//

#include "CGALDelaunayTriangulation.h"

void CGALDelaunay::TriangulateUsingCGAL(vector<vector<float> > *nodeArrayPointer, vector<float>* bufferPointer, vector<float> *colorPointer, int* numberOfVertices, std::list<Point> *VerticesToTriangulate, std::list<Point> * VerticesToAdd, int* verticesAlreadyAdded, Triangulation* T, int* totalVertices)
{
    bool isFirstTriangulation = false;
    // construction from a list of points :
    if(*verticesAlreadyAdded == 0)
    {
        isFirstTriangulation = true;
        for(int i = 0;i<*numberOfVertices;i++)
        {
            vector<float> tempVec;
            tempVec = nodeArrayPointer->at(i);
            VerticesToTriangulate->push_front(Point(tempVec[0],tempVec[1],tempVec[2]));
        }
        Triangulation TempTriangulation(VerticesToTriangulate->begin(), VerticesToTriangulate->end());
        *T = TempTriangulation;
    }
    else
    {
        for(int i = 0;i<*numberOfVertices;i++)
        {
            vector<float> tempVec;
            tempVec = nodeArrayPointer->at(i);
            T->insert(Point(tempVec[0], tempVec[1], tempVec[2]));
        }
    }

    
    
    int count = 0;
    
    int vec3Order[] = {0,1,2};
    
    vector<float> emptyVector;
    *bufferPointer = emptyVector;
    *colorPointer = emptyVector;
    for( Triangulation::Finite_facets_iterator fit = T->finite_facets_begin(); fit != T->finite_facets_end(); ++fit){
        K::Triangle_3 triangle = T->triangle(*fit);
        
        Point vertex = triangle.vertex(0);
        
        vector<float> tempVec1;
        tempVec1.push_back(vertex.x());
        tempVec1.push_back(vertex.y());
        tempVec1.push_back(vertex.z());
        
        vertex = triangle.vertex(1);
        vector<float> tempVec2;
        tempVec2.push_back(vertex.x());
        tempVec2.push_back(vertex.y());
        tempVec2.push_back(vertex.z());
        
        vertex = triangle.vertex(2);
        vector<float> tempVec3;
        tempVec3.push_back(vertex.x());
        tempVec3.push_back(vertex.y());
        tempVec3.push_back(vertex.z());
        
        vector<vector<float> > TriangleVec;
        TriangleVec.push_back(tempVec1);
        TriangleVec.push_back(tempVec2);
        TriangleVec.push_back(tempVec3);
        
        
        BufferActions::addVec3ToBuffer(vec3Order, bufferPointer, &TriangleVec, 3);
        BufferActions::addVec3ToBuffer(vec3Order, colorPointer, &TriangleVec, 3);
        
        count += 3;
    }
    *totalVertices= count;
}
