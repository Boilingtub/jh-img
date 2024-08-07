#include "render/shader.h"
#include "render/vertexbuffers.h"
#include "Window/window.h"
#include "render/stb_image.h"
#include <cmath>
#define SHADER_DIR(a) "shaders/" a
#define TEXTURE_DIR(a) "assets/Textures/" a
void processInput(GLFWwindow *window);



int main(int argc, char** argv) {
    
    if(argc < 2) {
        std::cout << "No file selected\n";
        return 0;
    }

    GLFWwindow* main_window = create_new_window("Mathicians",800,600); 

    while(!glfwWindowShouldClose(main_window)){
        // input
        processInput(main_window);

        //rendering commands hee 

        float vertices[] = {
            // positions            // colors              // texture coords
            1.0f,  1.0f, 0.0f, /**/ 1.0f, 0.0f, 0.0f, /**/ 1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f, /**/ 0.0f, 1.0f, 0.0f, /**/ 1.0f, 0.0f, // bottom right
           -1.0f, -1.0f, 0.0f, /**/ 0.0f, 0.0f, 1.0f, /**/ 0.0f, 0.0f, // bottom left
           -1.0f,  1.0f, 0.0f, /**/ 1.0f, 1.0f, 0.0f, /**/ 0.0f, 1.0f // top left
};
        unsigned int indices[] = {
            0,1,3, 
            1,2,3
        };

        //BOOK-PG-63

        //Create texture
        unsigned int texture;
        glGenTextures(1 , &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        //set texture wraping/filtering oprions (on currently bound texture)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // set blending options
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // load and generate the texture 
        stbi_set_flip_vertically_on_load(true);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(argv[1],
                                        &width, &height, &nrChannels, STBI_rgb_alpha);
        if(data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, 
                         GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

        } else {
            std::cout << "Failed to load texture" << std::endl;  
        }
        stbi_image_free(data);


        VertexBuffers vertexbuffers(vertices, 
                                    sizeof(vertices)/sizeof(vertices[0]),
                                    indices,
                                    sizeof(indices)/sizeof(indices[0])); 

        const std::string embed_frag = 
            #include "../shaders/frag.frag"
        ;
        const std::string embed_vert = 
            #include "../shaders/vert.vert"
        ;

        Shader embededbasicShader(embed_vert.c_str(), embed_frag.c_str(), FromString);

        vertexbuffers.draw(embededbasicShader.ID);

        // check and call events and swap the buffers
        glfwSwapBuffers(main_window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
