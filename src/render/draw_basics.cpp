#include "draw_basics.h"
#include <iostream>

void draw_hello_rectangle(){
    int success;
    char infoLog[512];
    
    //0. Bind Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //1. Copy verticies array in a buffer for OpenGL to USE    
    float verticies[] = { 0.5, 0.5, 0.0,  //top right
                          0.5,-0.5, 0.0,  //bottom right
                         -0.5,-0.5, 0.0,  //bottom left
                         -0.5, 0.5, 0.0}; //top left

    unsigned int indices[] = {0, 1, 3,  //first triangle
                              1, 2, 3}; //second triangle

    unsigned int VBO;//Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //2. set vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //3. set shader program details
    const char* vertexShaderSource = "#version 460 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main(){\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog 
                                                                   << "\n";
    }
    
    const char* fragmentShaderSource = "#version 460 core\n"
        "out vec4 FragColor;\n"
        "void main(){\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1 , &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog 
                                                                     << "\n";
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog 
                                                             << "\n";

    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //4. Draw the Object
        //4.1 set Shader Program to to use
    glUseProgram(shaderProgram);
        //4.2 Bind vertex array object we want to use
    glBindVertexArray(VAO);
        //4.3 Use openGl function to draw to screen
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//draw wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//draw triangles
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //4.4 Unbind Vertex array object
    glBindVertexArray(0);
}

///////////////////////////////////////////////////////////////////////////////

struct basic_draw_vertex create_vertex(float x, float y, float z, float r, float g, float b){
    return {.x=x, .y=y, .z=z , .r=r, .g=g, .b=b};
}

struct Vertex_Buffers create_Vertex_Buffers(float verticies[], 
                                            unsigned int vertex_count, 
                                            unsigned int indices[],
                                            unsigned int index_count) {
    //0. Bind Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //1. Copy verticies & indicies array in a buffer for OpenGL to use
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_count*sizeof(verticies[0]), verticies, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count*sizeof(indices[0]), indices, GL_STATIC_DRAW);

    //2. set vertex attributes pointers
        //2.1 Position Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
                (void*)0);
    glEnableVertexAttribArray(0);
        //2.2 Color Attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float),
                (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    struct Vertex_Buffers vertex_buffers = {.VAO = VAO, 
                                            .VBO = VBO, 
                                            .EBO = EBO, 
                                            .index_count = index_count};
    return vertex_buffers;
}

unsigned int create_ShaderProgram(const char* VertexShaderFile, 
                                  const char* FragmentShaderFile) {
    int success;
    char infoLog[512];

    std::ifstream ifs_VertexShaderFile(VertexShaderFile);
    std::ifstream ifs_FragmentShaderFile(FragmentShaderFile);
    std::string content;
   
    //3. set shader program details
    const char* vertexShaderSource; 
    content.assign( (std::istreambuf_iterator<char>(ifs_VertexShaderFile) ),
                    (std::istreambuf_iterator<char>()    ) );
    vertexShaderSource = content.c_str();
    

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog 
                                                                   << "\n";
    }
    
    const char* fragmentShaderSource;
        content.assign( (std::istreambuf_iterator<char>(ifs_FragmentShaderFile) ),
                        (std::istreambuf_iterator<char>()    ) );
        fragmentShaderSource = content.c_str();
    
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1 , &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog 
                                                                     << "\n";
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog 
                                                             << "\n";

    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void draw_Vertex_Buffers(struct Vertex_Buffers vertex_buffers, 
              unsigned int shaderProgram) {
    //4. Draw the Object
        //4.1 set Shader Program to to use
    glUseProgram(shaderProgram);
        //4.2 Bind vertex array object we want to use
    glBindVertexArray(vertex_buffers.VAO);
        //4.3 Use openGl function to draw to screen
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//draw wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//draw triangles
    glDrawElements(GL_TRIANGLES, vertex_buffers.index_count,
                   GL_UNSIGNED_INT, 0);
        //4.4 Unbind Vertex array object
    glBindVertexArray(0);

}
