#include "render/shader.h"
#include "render/vertexbuffers.h"
#include "render/window.h"
#include "render/stb_image.h"
#define SHADER_DIR(a) "shaders/" a
#define TEXTURE_DIR(a) "assets/Textures/" a


const std::string embed_frag = 
    #include "../shaders/frag.frag"
;
const std::string embed_vert = 
    #include "../shaders/vert.vert"
;

struct Image {
    Shader *shader;
    VertexBuffers *vertexbuffers;
    short flip = -1;
    float scale = 1.0f;
    float displace_x = 0.0f;
    float displace_y = 0.0f;
    int tex_width = 0;
    int tex_height = 0;
    Image(char* image_path) {
     //rendering commands hee 
    float vertices[] = {
        // positions             // texture coords
        1.0f,  1.0f, /**/ 1.0f, 1.0f, // top right
        1.0f, -1.0f, /**/ 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f,/**/ 0.0f, 0.0f, // bottom left
        -1.0f,  1.0f,/**/ 0.0f, 1.0f // top left
    };
    unsigned int indices[] = {
        0,1,3, 
        1,2,3
    };
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
    //stbi_set_flip_vertically_on_load(false);
    int nrChannels;
    unsigned char* data = stbi_load(image_path,
                                    &tex_width, &tex_height, &nrChannels, STBI_rgb_alpha);
    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, 
                     GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;  
    }
    stbi_image_free(data);

    vertexbuffers = new VertexBuffers(vertices, 
        sizeof(vertices)/sizeof(vertices[0]),
        indices,
        sizeof(indices)/sizeof(indices[0])); 
        
    shader = new Shader (embed_vert.c_str(), embed_frag.c_str(), FromString); 
    }

    void apply_transformations(int screen_width, int screen_height) {
        std::cout << "tex_width = " << tex_width << "  screen_width" << screen_width << "\n";
        float fin_scale_x = ((float)tex_width/screen_width)*scale;
        float fin_scale_y = ((float)tex_height/screen_height)*scale;
        glUniform1i(glGetUniformLocation(this->shader->ID, "flip"), flip);
        glUniform1f(glGetUniformLocation(this->shader->ID, "scale_x"), fin_scale_x);
        glUniform1f(glGetUniformLocation(this->shader->ID, "scale_y"), fin_scale_y);
        glUniform1f(glGetUniformLocation(this->shader->ID, "displace_x"),displace_x);
        glUniform1f(glGetUniformLocation(this->shader->ID, "displace_y"),displace_y);
    }
    void render(GLFWwindow* main_window) {
        glClear(GL_COLOR_BUFFER_BIT);

        int screen_width, screen_height;
        glfwGetWindowSize(main_window ,&screen_width,&screen_height);
        apply_transformations(screen_width, screen_height);

        this->vertexbuffers->draw(this->shader->ID);
        // check and call events and swap the buffers 
        glfwSwapBuffers(main_window);
    }

    void free() {
        delete this->shader;
        delete this->vertexbuffers;
    }

};

void processInput(GLFWwindow *window,Image*);

int main(int argc, char** argv) {
    if(argc < 2) {
        std::cout << "No file selected\n";
        return 0;
    }
    GLFWwindow* main_window = create_new_window("Mathicians",800,600); 
    Image picture( argv[1] );

    while(!glfwWindowShouldClose(main_window)){
        // input
        glfwPollEvents();
        picture.render(main_window);
        processInput(main_window,&picture);
    }
    glfwTerminate();
    return 0;
}

bool flip_key_disable = false;
void processInput(GLFWwindow *window, Image *picture){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(!flip_key_disable && glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        picture->flip *= -1;
        flip_key_disable = true;
    }
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
        flip_key_disable = false;
    }

    if(glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        picture->scale += (picture->scale*0.1f);
    }

    if(glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        picture->scale -= (picture->scale*0.1f);
        if(picture->scale <= 0)
            picture->scale = 0.01f;
    }

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        picture->displace_y -= 0.1f;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        picture->displace_y += 0.1f;
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        picture->displace_x += 0.1f;
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        picture->displace_x -= 0.1f; 
    }
}
