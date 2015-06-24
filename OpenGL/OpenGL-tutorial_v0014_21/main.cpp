//
//  main.cpp
//  DelaunayTriangualtionCGal
//
//  Created by Sadat Shaik on 6/23/15.
//  Copyright (c) 2015 Sadat Shaik. All rights reserved.
//

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_3.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <list>
#include <vector>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_3<K>      Triangulation;
typedef Triangulation::Cell_handle    Cell_handle;
typedef Triangulation::Vertex_handle  Vertex_handle;
typedef Triangulation::Locate_type    Locate_type;
typedef Triangulation::Point          Point;

typedef CGAL::Triangulation_3<CGAL::Epick, CGAL::Default, CGAL::Default> Triangulation_3;
int main()
{
    // construction from a list of points :
    std::list<Point> L;
    L.push_front(Point(0.2,0.6,0.6));
    L.push_front(Point(1.67,0.7,0.9));
    L.push_front(Point(0.1,5.5,0.6));
    Triangulation T(L.begin(), L.end());
    Triangulation::size_type n = T.number_of_vertices();
    // insertion from a vector :
    std::vector<Point> V(3);
    V[0] = Point(0.3,0.5,.17);
    V[1] = Point(1.1,1.2,1.3);
    V[2] = Point(2.4,2.6,2.6);
    n = n + T.insert(V.begin(), V.end());
    assert( n == 6 );       // 6 points have been inserted
    assert( T.is_valid() ); // checking validity of T
    Locate_type lt;
    int li, lj;
    Point p(0,0,0);
    Cell_handle c = T.locate(p, lt, li, lj);
    // p is the vertex of c of index li :
    //assert( lt == Triangulation::VERTEX );
    assert( c->vertex(li)->point() == p );
    Vertex_handle v = c->vertex( (li+1)&3 );
    // v is another vertex of
    Cell_handle nc = c->neighbor(li);
    // nc = neighbor of c opposite to the vertex associated with p
    // nc must have vertex v :
    int nli;
    assert( nc->has_vertex( v, nli ) );
    // nli is the index of v in nc
    std::ofstream oFileT("output",std::ios::out|std::ios::trunc);
    // writing file output;
    oFileT << T;
    Triangulation T1;
    std::ifstream iFileT("output",std::ios::in);
    // reading file output;
    iFileT >> T1;
    int count = 0;
    for( Triangulation::Finite_facets_iterator fit = T.finite_facets_begin(); fit != T.finite_facets_end(); ++fit){
        K::Triangle_3 triangle = T.triangle(*fit);
        Point vertex = triangle.vertex(0);
        std::cout<<"x coordinate:"<<vertex.x()<<"y coordinate"<<vertex.y()<<"z coordinate"<<vertex.z()<<std::endl;
        
        vertex = triangle.vertex(1);
        std::cout<<"x coordinate:"<<vertex.x()<<"y coordinate"<<vertex.y()<<"z coordinate"<<vertex.z()<<std::endl;

        
        vertex = triangle.vertex(2);
        std::cout<<"x coordinate:"<<vertex.x()<<"y coordinate"<<vertex.y()<<"z coordinate"<<vertex.z()<<std::endl;
    }
    
    
    assert( T1.is_valid() );
    assert( T1.number_of_vertices() == T.number_of_vertices() );
    assert( T1.number_of_cells() == T.number_of_cells() );
    return 0;
}
