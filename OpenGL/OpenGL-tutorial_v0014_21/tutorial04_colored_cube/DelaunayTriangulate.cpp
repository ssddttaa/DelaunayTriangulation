#include "DelaunayTriangulate.h"

int main( void )
{
    //PARSE THROUGH NODE FILE AND GET ALL OF THE NODES
    int nodeArraySize;
    ParseClass::ParseNodeFile("firstThird.node", &nodeArray, &nodeArraySize);
    
    //RANDOMIZE SEED FOR WHEN GENERATING RANODM VALUES
    //generateRandomPoints(&nodeArray);
    
    setupWindow();
    //SET UP INPUT TETGEN

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
    //triangulateUsingTetgen(&nodeArray);
    return 0;
}

void addPointsToTriangulation(vector<vec3> * pointsToAdd, int numberOfPoints)
{
    CGALDelaunay::TriangulateUsingCGAL(pointsToAdd, &g_vertex_buffer_data, &g_color_buffer_data,&numberOfPoints,&VerticesToTriangulate, &VerticesToAdd,&numberOfPoints, &TriangulationOfPoints, &numVertices);
    setBuffers();
    if(nowDraw)
    {
        drawLoop(numVertices, false);
        cleanup();
    }
}

void generateRandomPoints(vector<vec3> *nodeArray)
{
    srand(time(NULL));
    for(int i = 0;i<numVertices;i++)
    {
        float xCoord = (rand()%150/100.0);
        float yCoord = (rand()%150/100.0);
        float zCoord = (rand()%150/100.0);
        vec3 tempVec(xCoord, yCoord, zCoord);
        nodeArray->push_back(tempVec);
    }
}

void triangulate(bool usingTetgen, vector<vec3>*nodeArray, bool pointsDynamicallyInserted, int nodeArraySize)
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
void triangulateUsingCGAL(vector<vec3> *nodeArrayPointer, bool pointsDynamicallyInserted, int nodeArraySize)
{
    /*
     void CGALDelaunay::TriangulateUsingCGAL(vector<glm::vec3> *nodeArrayPointer, vector<float>* bufferPointer, vector<float> *colorPointer, int* numberOfVertices, std::list<Point> *VerticesToTriangulate, std::list<Point> * VerticesToAdd, int verticesAlreadyAdded, Triangulation* T)
     */
    int VerticesAlreadyAdded(VerticesToTriangulate.size());
    CGALDelaunay::TriangulateUsingCGAL (nodeArrayPointer, &g_vertex_buffer_data, &g_color_buffer_data, &nodeArraySize, &VerticesToTriangulate, &VerticesToAdd, &VerticesAlreadyAdded, &TriangulationOfPoints, &numVertices);
    setBuffers();
    if(!pointsDynamicallyInserted)
    {
        drawLoop(numVertices, false);
    }
}
void triangulateUsingTetgen(vector<vec3>*nodeArray)
{
    DelaunayTriangulate(nodeArray);
    
    vertex_buffer_pointer = &g_vertex_buffer_data;
    vertex_color_pointer = &g_color_buffer_data;
    
    createAllPoints(vertex_buffer_pointer, vertex_color_pointer);
    
    createAllTetrahedra();
    
    setBuffers();
    
    drawLoop((numberOfTetrahedra*4) + totalVertices*3,true);
    
    cleanup();
}

void DelaunayTriangulate(vector<vec3> *nodeArray)
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

    //ParseClass::ParseFaceFile("convexhull.face", &faceArray, &numberOfTetrahedra);
    //out.save_edges(filePrefixChar);
    //out.save_nodes(filePrefixChar);
    
}

void setupWindow()
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
                                       glm::vec3(50,50,-50), // Camera is at (4,3,-3), in World Space
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

void createAllPoints(vector<float> *vertex_buffer_pointer, vector<float> *vertex_color_pointer)
{
    DrawObjects newCube;
    BufferActions newBufferAction;
    
    for(int i = 0;i<numVertices;i++)
    {
        vec3 tempVec = nodeArray.at(i);
        float xCoord(tempVec[0]);
        float yCoord(tempVec[1]);
        float zCoord(tempVec[2]);
        
        GLfloat new_cube_vertex_data[108];
        for(int j=0;j<108;j++)
        {
            new_cube_vertex_data[j] = cube_vertex_data[j];
        }
        GLfloat* vertexPointer;
        vertexPointer = new_cube_vertex_data;
        GLfloat* colorPointer;
        colorPointer = cube_color_data;
        
        g_cube_centers.push_back(xCoord);
        g_cube_centers.push_back(yCoord);
        g_cube_centers.push_back(zCoord);
        newCube.createCubeWithTranslation(xCoord, yCoord, zCoord,.000002f, vertexPointer, &totalVertices);
        newBufferAction.addObjectToBuffer(vertexPointer, colorPointer, 108, vertex_buffer_pointer, vertex_color_pointer);
    }
}

void createAllTetrahedra()
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

void setBuffers()
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

void drawLoop(int numberOfVertices, bool drawUsingTetgen)
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
                
                glBegin(GL_LINES);
                glVertex3f(tetPoint1[0],tetPoint1[1],tetPoint1[2]);
                glVertex3f(tetPoint2[0],tetPoint2[1],tetPoint2[2]);
                glEnd();
                glBegin(GL_LINES);
                glVertex3f(tetPoint1[0],tetPoint1[1],tetPoint1[2]);
                glVertex3f(tetPoint3[0],tetPoint3[1],tetPoint3[2]);
                glEnd();
                glBegin(GL_LINES);
                glVertex3f(tetPoint1[0],tetPoint1[1],tetPoint1[2]);
                glVertex3f(tetPoint4[0],tetPoint4[1],tetPoint4[2]);
                glEnd();
                glBegin(GL_LINES);
                glVertex3f(tetPoint2[0],tetPoint2[1],tetPoint2[2]);
                glVertex3f(tetPoint3[0],tetPoint3[1],tetPoint3[2]);
                glEnd();
                glBegin(GL_LINES);
                glVertex3f(tetPoint3[0],tetPoint3[1],tetPoint3[2]);
                glVertex3f(tetPoint4[0],tetPoint4[1],tetPoint4[2]);
                glEnd();
                glBegin(GL_LINES);
                glVertex3f(tetPoint2[0],tetPoint2[1],tetPoint2[2]);
                glVertex3f(tetPoint4[0],tetPoint4[1],tetPoint4[2]);
                glEnd();
                
            }
        }
        else
        {
            for(int i = 0; i< numberOfVertices/3;i++)
            {
                vec3 triPoint1(g_vertex_buffer_data.at(i*9),g_vertex_buffer_data.at((i*9)+1), g_vertex_buffer_data.at((i*9)+2));
                vec3 triPoint2(g_vertex_buffer_data.at((i*9)+3),g_vertex_buffer_data.at((i*9)+4), g_vertex_buffer_data.at((i*9)+5));
                vec3 triPoint3(g_vertex_buffer_data.at((i*9)+6),g_vertex_buffer_data.at((i*9)+7), g_vertex_buffer_data.at((i*9)+8));
                glBegin(GL_LINES);
                glVertex3f(triPoint1[0],triPoint1[1],triPoint1[2]);
                glVertex3f(triPoint2[0],triPoint2[1],triPoint2[2]);
                glEnd();
                glBegin(GL_LINES);
                glVertex3f(triPoint1[0],triPoint1[1],triPoint1[2]);
                glVertex3f(triPoint3[0],triPoint3[1],triPoint3[2]);
                glEnd();
                glBegin(GL_LINES);
                glVertex3f(triPoint2[0],triPoint2[1],triPoint2[2]);
                glVertex3f(triPoint3[0],triPoint3[1],triPoint3[2]);
                glEnd();
                
                //cout<<"Drawing Triangle#"<<i<<" of Vertices::("<<triPoint1[0]<<","<<triPoint1[1]<<","<<triPoint1[2]<<"),"<<"("<<triPoint2[0]<<","<<triPoint2[1]<<","<<triPoint2[2]<<")"<<",("<<triPoint3[0]<<","<<triPoint3[1]<<","<<triPoint3[2]<<")"<<endl;
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

void cleanup()
{
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(programID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

void setHullFaces(vector<vec3> *convexHullFaces)
{
    hullFaces = *convexHullFaces;
    
}