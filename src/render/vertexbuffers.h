#ifndef VERTEXBUFFERS_H
#define VERTEXBUFFERS_H

#include "../glad/glad.h"
class vertex {
    public:
        float x,y,z, r,g,b,a;
        vertex(float _x, float _y, float _z,
               float _r, float _b, float _g, float _a) { //constructor
            x=_x; y=_y; z=_z; r=_r; b=_b; g=_g; a=_a;
        }
};



class VertexBuffers {
    public:
        unsigned int VAO, VBO, EBO, index_count;
        VertexBuffers(float vertices[],  int vertex_count,
                      unsigned int indices[], int construct_index_count)     { //constructor
            //0. Bind Vertex Array Object
            unsigned int construct_VAO;
            glGenVertexArrays(1, &construct_VAO);
            glBindVertexArray(construct_VAO);

            //1. Copy verticies & indicies array in a buffer for OpenGL to use
            
            unsigned int construct_VBO;
            glGenBuffers(1, &construct_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, construct_VBO);
            glBufferData(GL_ARRAY_BUFFER,
                         vertex_count*sizeof(vertices[0]), vertices,
                         GL_STATIC_DRAW);

            unsigned int construct_EBO;
            glGenBuffers(1, &construct_EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, construct_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                         construct_index_count*sizeof(indices[0]), indices,
                         GL_STATIC_DRAW);

            //2. set vertex attributes pointers
                //2.1 Position Attributes
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                  (void*)0);
            glEnableVertexAttribArray(0);
                //2.2 Color Attributes
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                  (void*) (3*sizeof(float)));
            glEnableVertexAttribArray(1);
                //2.3 Texture Attributes
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                                  (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            

            VAO = construct_VAO;
            VBO = construct_VBO;
            EBO = construct_EBO;
            index_count = construct_index_count;
        }

        void draw(unsigned int shader) {
            //4. Draw the Object
                //4.1 set Shader Program to to use
            glUseProgram(shader);
                //4.2 Bind vertex array object we want to use
            glBindVertexArray(VAO);
                //4.3 Use openGl function to draw to screen
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//draw wireframe
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//draw triangles
            glDrawElements(GL_TRIANGLES, index_count,
                           GL_UNSIGNED_INT, 0);
                //4.4 Unbind Vertex array object
            glBindVertexArray(0);

        }

        void unbind() {
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);
        }

};


#endif
