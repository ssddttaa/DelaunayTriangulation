#include "DelaunayTriangulate.h"

DelaunayTriangulation::DelaunayTriangulation()
{
    GLfloat temp_vertex_data[] = {
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
    //temporary set of coordinates used to copy the default cube coordiantes into the cube_vertex_data and cube_color_data buffers

    numberOfTetrahedra = 0; //Used for the tetgen tetrahedralization
    memcpy(cube_vertex_data, temp_vertex_data, sizeof(temp_vertex_data));//Copies over the contents of the temp cube
    memcpy(cube_color_data, temp_vertex_data, sizeof(temp_vertex_data));//Copies over the content of the temp cube

    currentViewScale = .0000000000000000001;
    
    totalVertices = 0;//Variable used for the tetegen library
    
    //REMOVE WHEN USING ROS TO GET THE DATA
    
    
    //PARSE THROUGH NODE FILE AND GET ALL OF THE NODES
    int nodeArraySize;
    ParseClass::ParseNodeFile("firstThird.node", &nodeArray, &nodeArraySize);
    
    //SET UP OPENGL WINDOW
    setupWindow();
    
    //MAIN TRIANGULATION BEGINS
    triangulate(false, &nodeArray, true, nodeArraySize);
    
    vector<vec3> secondThird;
    int numberOfAddedPoints;
    ParseClass::ParseNodeFile("secondThird.node", &secondThird, &numberOfAddedPoints);
    
    addPointsToTriangulation(&secondThird, numberOfAddedPoints);
    
    nowDraw = true;
    vector<vec3> ThirdThird;
    int numberOfAddedPointsLast;
    ParseClass::ParseNodeFile("thirdThird.node", &ThirdThird, &numberOfAddedPointsLast);
    
    addPointsToTriangulation(&ThirdThird, numberOfAddedPointsLast);
}

//Add points to the triangulation dynamically. The code re-tetrahedralizes with the given points
void DelaunayTriangulation::addPointsToTriangulation(vector<vec3> * pointsToAdd, int numberOfPoints)
{
    //Calls the class that triangulates using the library CGAL
    CGALDelaunay::TriangulateUsingCGAL(pointsToAdd, &g_vertex_buffer_data, &g_color_buffer_data,&numberOfPoints,&VerticesToTriangulate, &VerticesToAdd,&numberOfPoints, &TriangulationOfPoints, &numVertices);
    
    //Populates the buffers with the vertex information and color information
    setBuffers();
    
    //REMOVE WHEN USING DATA FROM ROS, THIS IS ONLY NEEDED TEMPORARILY
    if(nowDraw)
    {
        drawLoop(numVertices, false); //Start the draw loop that draws the tetrahedralization every second
        cleanup(); // Clean up the buffers that contain the vertex and color information
    }
}

//Calls the different triangulation libraries. One calls the tetgen library, the other uses the CGAL library
void DelaunayTriangulation::triangulate(bool usingTetgen, vector<vec3>*nodeArray, bool pointsDynamicallyInserted, int nodeArraySize)
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
void DelaunayTriangulation::triangulateUsingCGAL(vector<vec3> *nodeArrayPointer, bool pointsDynamicallyInserted, int nodeArraySize)
{
    int VerticesAlreadyAdded(VerticesToTriangulate.size());
    CGALDelaunay::TriangulateUsingCGAL (nodeArrayPointer, &g_vertex_buffer_data, &g_color_buffer_data, &nodeArraySize, &VerticesToTriangulate, &VerticesToAdd, &VerticesAlreadyAdded, &TriangulationOfPoints, &numVertices);
    setBuffers();
    if(!pointsDynamicallyInserted)
    {
        drawLoop(numVertices, false);
    }
}

//This function triangulates using the tetgen library.
void DelaunayTriangulation::triangulateUsingTetgen(vector<vec3>*nodeArray)
{
    DelaunayTriangulate(nodeArray);
    
    vertex_buffer_pointer = &g_vertex_buffer_data;
    vertex_color_pointer = &g_color_buffer_data;
    
    //createAllPoints(vertex_buffer_pointer, vertex_color_pointer);
    
    createAllTetrahedra();
    
    setBuffers();
    
    drawLoop((numberOfTetrahedra*4) + totalVertices*3,true);
    
    cleanup();
}

//This function triangulates using tetgen
void DelaunayTriangulation::DelaunayTriangulate(vector<vec3> *nodeArray)
{
    in.firstnumber = 0;
    in.numberofpoints = numVertices;
    in.pointlist = new REAL[in.numberofpoints*3];
    
    for(int i = 0 ;i<nodeArray->size();i++)
    {
        vec3 coordinateVec(nodeArray->at(i));
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
void DelaunayTriangulation::setupWindow()
{
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        //return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        //return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        //return -1;
    }
    
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Dark blue background
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    
    // Create and compile our GLSL program from the shaders
    
    programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );
    
    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    
    // Get a handle for our buffers
    vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    vertexColorID = glGetAttribLocation(programID, "vertexColor");
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    Projection = glm::perspective(90.0f, 4.0f / 3.0f, 0.1f, 300.0f);
    // Camera matrix
    View       = glm::lookAt(
                                       glm::vec3(currentViewScale,currentViewScale,-currentViewScale), // Camera is at (4,3,-3), in World Space
                                       glm::vec3(0,0,0), // and looks at the origin
                                       glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                       );
    // Model matrix : an identity matrix (model will be at the origin)
    Model      = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
    
    // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
}

//Creates the tetrahedron for the tetgen library
void DelaunayTriangulation::createAllTetrahedra()
{
    for(int i = 0; i< numberOfTetrahedra;i++)
    {
        vector<vec3> tetPoints;
        vec3 tetPoint1(nodeArray.at((tetrahedraArray.at(i*4))));
        vec3 tetPoint2(nodeArray.at((tetrahedraArray.at((i*4)+1))));
        vec3 tetPoint3(nodeArray.at((tetrahedraArray.at((i*4)+2))));
        vec3 tetPoint4(nodeArray.at((tetrahedraArray.at((i*4)+3))));
        
        tetPoints.push_back(tetPoint1);tetPoints.push_back(tetPoint2);tetPoints.push_back(tetPoint3);tetPoints.push_back(tetPoint4);
        
        int tetPointsToAdd[12] = {0,1,2,0,1,3,0,2,3,1,2,3};
        BufferActions::addVec3ToBuffer(tetPointsToAdd, vertex_buffer_pointer, &tetPoints, 12);
        BufferActions::addVec3ToBuffer(tetPointsToAdd, vertex_color_pointer, &tetPoints, 12);
        
    }
}

//Sets the buffers for the vertices and for the colors
void DelaunayTriangulation::setBuffers()
{
    vertexBufferFloat = new float[g_vertex_buffer_data.size()]();
    colorBufferFloat = new float[g_color_buffer_data.size()]();
    
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    
    for(int i = 0 ;i<g_vertex_buffer_data.size();i++)
    {
        vertexBufferFloat[i] = g_vertex_buffer_data.at(i);
        colorBufferFloat[i] = g_color_buffer_data.at(i);
    }
    glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size()*4, vertexBufferFloat, GL_STATIC_DRAW);
    
    
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size()*4, colorBufferFloat, GL_STATIC_DRAW);
}

//Start the loop that draws the triangulation each second
void DelaunayTriangulation::drawLoop(int numberOfVertices, bool drawUsingTetgen)
{
    do{
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use our shader
        glUseProgram(programID);
        
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                              vertexPosition_modelspaceID, // The attribute we want to configure
                              3,                           // size
                              GL_FLOAT,                    // type
                              GL_FALSE,                    // normalized?
                              0,                           // stride
                              (void*)0                     // array buffer offset
                              );
        
        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(vertexColorID);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                              vertexColorID,               // The attribute we want to configure
                              3,                           // size
                              GL_FLOAT,                    // type
                              GL_FALSE,                    // normalized?
                              0,                           // stride
                              (void*)0                     // array buffer offset
                              );
        
        // Draw the triangleS !
        glDrawArrays(GL_TRIANGLES, 0, numberOfVertices); // 12*3 indices starting at 0 -> 12 triangles
        
        glLineWidth(2.5);
        glColor3f(1.0, 0.0, 0.0);
        
        if(drawUsingTetgen)
        {
            
            for(int i = 0; i< numberOfTetrahedra;i++)
            {
                vector<vec3> tetPoints;
                vec3 tetPoint1(nodeArray.at((tetrahedraArray.at(i*4))));
                vec3 tetPoint2(nodeArray.at((tetrahedraArray.at((i*4)+1))));
                vec3 tetPoint3(nodeArray.at((tetrahedraArray.at((i*4)+2))));
                vec3 tetPoint4(nodeArray.at((tetrahedraArray.at((i*4)+3))));
                
                int drawOrder[12] = {0,1,0,2,0,3,1,2,2,3,1,3};
                
                DrawLines(drawOrder, &tetPoints, 6);
            }
        }
        else
        {
            for(int i = 0; i< numberOfVertices/3;i++)
            {
                vector<vec3> triangleCoords;
                vec3 triPoint1(g_vertex_buffer_data.at(i*9),g_vertex_buffer_data.at((i*9)+1), g_vertex_buffer_data.at((i*9)+2));
                vec3 triPoint2(g_vertex_buffer_data.at((i*9)+3),g_vertex_buffer_data.at((i*9)+4), g_vertex_buffer_data.at((i*9)+5));
                vec3 triPoint3(g_vertex_buffer_data.at((i*9)+6),g_vertex_buffer_data.at((i*9)+7), g_vertex_buffer_data.at((i*9)+8));
                
                triangleCoords.push_back(triPoint1);
                triangleCoords.push_back(triPoint2);
                triangleCoords.push_back(triPoint3);
                
                for(int j = 0;j<3;j++)
                {
                    vec3 tempVec(triangleCoords.at(j));
                    for(int k =0; k<3;k++)
                    {
                        if(abs(tempVec[k])>currentViewScale)
                        {
                            currentViewScale = abs(tempVec[k]);
                            View       = glm::lookAt(
                                                     glm::vec3(currentViewScale,currentViewScale,-currentViewScale), // Camera is at (4,3,-3), in World Space
                                                     glm::vec3(0,0,0), // and looks at the origin
                                                     glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                                     );
                            MVP        = Projection * View * Model;
                        }
                    }
                }
                
                int drawOrder[6] = {0,1,0,2,1,2};
                DrawLines(drawOrder, &triangleCoords, 3);
            }
        }
        
        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexColorID);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    //INSERT EVENT HANDLER HERE
}

void DelaunayTriangulation::DrawLines(int * drawOrder, vector<vec3>* vertices, int linesToDraw)
{
    for(int i = 0; i<linesToDraw; i++)
    {
        vec3 currentVec1(vertices->at(drawOrder[i*2]));
        vec3 currentVec2(vertices->at(drawOrder[(i*2)+1]));
        
        glBegin(GL_LINES);
        glVertex3f(currentVec1[0],currentVec1[1],currentVec1[2]);
        glVertex3f(currentVec2[0],currentVec2[1],currentVec2[2]);
        glEnd();
    }
}

//This cleans up the buffers for memory saving purposes.
void DelaunayTriangulation::cleanup()
{
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(programID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}
