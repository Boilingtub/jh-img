#ifndef INCLUDE_BASICS_H
#define INCLUDE_BASICS_H

#include "../Window/window.h"
#include <fstream>

struct basic_draw_vertex{
    float x,y,z;
    float r,g,b;
};

struct Vertex_Buffers{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int index_count;
};

void draw_hello_rectangle();

struct basic_draw_vertex create_vertex(float x, float y, float z, float r, float g, float b);

struct Vertex_Buffers create_Vertex_Buffers(float verticies[],
                                            unsigned int vertex_count,
                                            unsigned int indicies[],
                                            unsigned int index_count);
unsigned int create_ShaderProgram(const char* VertexShaderFile,
                                  const char* FragmentShaderFile);
void draw_Vertex_Buffers(struct Vertex_Buffers vertex_buffers , unsigned int shaderProgram);

#endif
