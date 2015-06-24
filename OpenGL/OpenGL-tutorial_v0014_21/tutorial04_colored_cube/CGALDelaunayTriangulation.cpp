//
//  CGALDelaunayTriangulation.cpp
//  Tutorials
//
//  Created by Sadat Shaik on 6/23/15.
//
//

#include "CGALDelaunayTriangulation.h"

void CGALDelaunay::TriangulateUsingCGAL(vector<glm::vec3> *nodeArrayPointer, vector<float>* bufferPointer, vector<float> *colorPointer, int* numberOfVertices, std::list<Point> *VerticesToTriangulate, std::list<Point> * VerticesToAdd, int* verticesAlreadyAdded, Triangulation* T, int* totalVertices)
{
    /*
     CGALDelaunay::TriangulateUsingCGAL(pointsToAdd, &g_vertex_buffer_data, &g_color_buffer_data,&numberOfPoints,&VerticesToTriangulate, &VerticesToAdd, VerticesToTriangulate.size(), TriangulationOfPoints);
     */
    bool isFirstTriangulation = false;
    // construction from a list of points :
    if(*verticesAlreadyAdded == 0)
    {
        isFirstTriangulation = true;
        for(int i = 0;i<*numberOfVertices;i++)
        {
            vec3 tempVec;
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
            vec3 tempVec;
            tempVec = nodeArrayPointer->at(i);
            T->insert(Point(tempVec[0], tempVec[1], tempVec[2]));
            //cout<<"Inserting:("<<tempVec[0]<<","<<tempVec[1]<<","<<tempVec[2]<<")"<<endl;
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
        
        vec3 tempVec1(vertex.x(),vertex.y(), vertex.z());
        
        vertex = triangle.vertex(1);
        vec3 tempVec2(vertex.x(),vertex.y(), vertex.z());
        
        vertex = triangle.vertex(2);
        vec3 tempVec3(vertex.x(),vertex.y(), vertex.z());
        
        vector<vec3> TriangleVec;
        TriangleVec.push_back(tempVec1);
        TriangleVec.push_back(tempVec2);
        TriangleVec.push_back(tempVec3);
        
        
        BufferActions::addVec3ToBuffer(vec3Order, bufferPointer, &TriangleVec, 3);
        BufferActions::addVec3ToBuffer(vec3Order, colorPointer, &TriangleVec, 3);
        
        count += 3;
    }
    *totalVertices= count;
}
