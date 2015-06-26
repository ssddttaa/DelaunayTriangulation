//
//  CGALDelaunayTriangulation.h
//  Tutorials
//
//  Created by Sadat Shaik on 6/23/15.
//
//

#ifndef __Tutorials__CGALDelaunayTriangulation__
#define __Tutorials__CGALDelaunayTriangulation__

#include <stdio.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_3.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <list>
#include <vector>
#include "BufferActions.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_3<K>      Triangulation;
typedef Triangulation::Cell_handle    Cell_handle;
typedef Triangulation::Vertex_handle  Vertex_handle;
typedef Triangulation::Locate_type    Locate_type;
typedef Triangulation::Point          Point;

typedef CGAL::Triangulation_3<CGAL::Epick, CGAL::Default, CGAL::Default> Triangulation_3;

using namespace std;

class CGALDelaunay
{
    public:
        static void TriangulateUsingCGAL(vector<vector<float> > *nodeArrayPointer, vector<float>* bufferPointer, vector<float> *colorPointer, int* numberOfVertices, std::list<Point> *VerticesToTriangulate, std::list<Point> * VerticesToAdd, int* verticesAlreadyAdded,Triangulation* T, int* totalVertices);
};
#endif /* defined(__Tutorials__CGALDelaunayTriangulation__) */
