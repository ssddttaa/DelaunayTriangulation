#include "DelaunayTriangulate.h"

DelaunayTriangulation::DelaunayTriangulation()
{
    //temporary set of coordinates used to copy the default cube coordiantes into the cube_vertex_data and cube_color_data buffers

    numberOfTetrahedra = 0; //Used for the tetgen tetrahedralization

    currentViewScale = -5.0;
    
    totalVertices = 0;//Variable used for the tetegen library
    
    //REMOVE WHEN USING ROS TO GET THE DATA
    
    
    //PARSE THROUGH NODE FILE AND GET ALL OF THE NODES
    int nodeArraySize;
    ParseClass::ParseNodeFile("firstThird.node", &nodeArray, &nodeArraySize);
    
    //MAIN TRIANGULATION BEGINS
    triangulate(false, &nodeArray, true, nodeArraySize);
    
    vector<vector<float> > secondThird;
    int numberOfAddedPoints;
    ParseClass::ParseNodeFile("secondThird.node", &secondThird, &numberOfAddedPoints);
    
    addPointsToTriangulation(&secondThird, numberOfAddedPoints);
    
    nowDraw = true;
    vector<vector<float> > ThirdThird;
    int numberOfAddedPointsLast;
    ParseClass::ParseNodeFile("thirdThird.node", &ThirdThird, &numberOfAddedPointsLast);
    
    addPointsToTriangulation(&ThirdThird, numberOfAddedPointsLast);
    
    /*for(int i = 0 ;i<(g_vertex_buffer_data.size()/3);i++)
    {
        cout<<"Point:("<<g_vertex_buffer_data.at((i*3))<<","<<g_vertex_buffer_data.at((i*3)+1)<<","<<g_vertex_buffer_data.at((i*3)+2)<<")"<<endl;
    }*/
}

//Add points to the triangulation dynamically. The code re-tetrahedralizes with the given points
void DelaunayTriangulation::addPointsToTriangulation(vector<vector<float> > * pointsToAdd, int numberOfPoints)
{
    //Calls the class that triangulates using the library CGAL
    CGALDelaunay::TriangulateUsingCGAL(pointsToAdd, &g_vertex_buffer_data, &g_color_buffer_data,&numberOfPoints,&VerticesToTriangulate, &VerticesToAdd,&numberOfPoints, &TriangulationOfPoints, &numVertices);
}

//Calls the different triangulation libraries. One calls the tetgen library, the other uses the CGAL library
void DelaunayTriangulation::triangulate(bool usingTetgen, vector<vector<float> >*nodeArray, bool pointsDynamicallyInserted, int nodeArraySize)
{
    if(usingTetgen)
    {
        triangulateUsingTetgen(nodeArray);
    }
    else
    {
        triangulateUsingCGAL(nodeArray, pointsDynamicallyInserted, nodeArraySize);
    }
}

//This function triangulates the points using the library CGAL
void DelaunayTriangulation::triangulateUsingCGAL(vector<vector<float> > *nodeArrayPointer, bool pointsDynamicallyInserted, int nodeArraySize)
{
    int VerticesAlreadyAdded(VerticesToTriangulate.size());
    CGALDelaunay::TriangulateUsingCGAL (nodeArrayPointer, &g_vertex_buffer_data, &g_color_buffer_data, &nodeArraySize, &VerticesToTriangulate, &VerticesToAdd, &VerticesAlreadyAdded, &TriangulationOfPoints, &numVertices);
}

//This function triangulates using the tetgen library.
void DelaunayTriangulation::triangulateUsingTetgen(vector<vector<float> >*nodeArray)
{
    DelaunayTriangulate(nodeArray);
    
    vertex_buffer_pointer = &g_vertex_buffer_data;
    vertex_color_pointer = &g_color_buffer_data;
    
    //createAllPoints(vertex_buffer_pointer, vertex_color_pointer);
    
    createAllTetrahedra();
}

//This function triangulates using tetgen
void DelaunayTriangulation::DelaunayTriangulate(vector<vector<float> > *nodeArray)
{
    in.firstnumber = 0;
    in.numberofpoints = numVertices;
    in.pointlist = new REAL[in.numberofpoints*3];
    
    for(int i = 0 ;i<nodeArray->size();i++)
    {
        vector<float> coordinateVec(nodeArray->at(i));
        in.pointlist[i*3] = coordinateVec[0];
        in.pointlist[(i*3)+1] = coordinateVec[1];
        in.pointlist[(i*3)+2] = coordinateVec[2];
    }
    
    
    string behaviorString = "";
    char * commands = (char*)behaviorString.c_str();
    
    
    
    tetrahedralize(commands, &in, &out);

    string filePrefix = "randomNodes";
    char* filePrefixChar= (char*)filePrefix.c_str();
    out.save_elements(filePrefixChar);
    
    ParseClass::ParseEdgeFile((filePrefix+".ele"), &tetrahedraArray, &numberOfTetrahedra);
}

//This function sets up the OpenGL window for displaying the contents of the tetrahedralization
//Creates the tetrahedron for the tetgen library
void DelaunayTriangulation::createAllTetrahedra()
{
    for(int i = 0; i< numberOfTetrahedra;i++)
    {
        vector<vector<float> > tetPoints;
        vector<float> tetPoint1(nodeArray.at((tetrahedraArray.at(i*4))));
        vector<float> tetPoint2(nodeArray.at((tetrahedraArray.at((i*4)+1))));
        vector<float> tetPoint3(nodeArray.at((tetrahedraArray.at((i*4)+2))));
        vector<float> tetPoint4(nodeArray.at((tetrahedraArray.at((i*4)+3))));
        
        tetPoints.push_back(tetPoint1);tetPoints.push_back(tetPoint2);tetPoints.push_back(tetPoint3);tetPoints.push_back(tetPoint4);
        
        int tetPointsToAdd[12] = {0,1,2,0,1,3,0,2,3,1,2,3};
        BufferActions::addVec3ToBuffer(tetPointsToAdd, vertex_buffer_pointer, &tetPoints, 12);
        BufferActions::addVec3ToBuffer(tetPointsToAdd, vertex_color_pointer, &tetPoints, 12);
        
    }
}