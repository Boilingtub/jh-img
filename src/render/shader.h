#ifndef SHADER_H
#define SHADER_H

#include "../glad/glad.h" //glad for OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum ShaderSource {
    FromFile,
    FromString,
};

class Shader {
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath, ShaderSource src) {//constructor
            const char* vertexShaderCode;
            const char* fragmentShaderCode; 
            if(src == FromFile) {
                //1. retrieve vertex/fragment shader source code form files
                std::string vertexCode;
                std::string fragmentCode;
                std::ifstream vertexShaderFile;
                std::ifstream fragmentShaderFile;

                //ensure ifstream objects can throw exceptions
                vertexShaderFile.exceptions(std::ifstream::failbit |
                                        std::ifstream::badbit);
                fragmentShaderFile.exceptions(std::ifstream::failbit |
                                        std::ifstream::badbit);
                try {
                    //open files
                    vertexShaderFile.open(vertexPath);
                    fragmentShaderFile.open(fragmentPath);

                    std::stringstream vertexShaderStream, fragmentShaderStream;
                    // read files buffer contents into streams
                    vertexShaderStream << vertexShaderFile.rdbuf();
                    fragmentShaderStream << fragmentShaderFile.rdbuf();
                    //close file handlers
                    vertexShaderFile.close();
                    fragmentShaderFile.close();
                    //convert stream into string
                    vertexCode = vertexShaderStream.str();
                    fragmentCode = fragmentShaderStream.str();
                }
                catch(std::ifstream::failure e) {
                    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" 
                            << std::endl;
                }

                vertexShaderCode = vertexCode.c_str();
                fragmentShaderCode = fragmentCode.c_str();
            } 
            else if(src == FromString) {
                vertexShaderCode = vertexPath;
                fragmentShaderCode = fragmentPath;
            }
            else {
                std::cout << "Invalid Shader source Type";
            }

            // 2. compile shaders 
            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vertexShaderCode, NULL);
            glCompileShader(vertex);
            // print compile errors if any
            glGetShaderiv(vertex,GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                    infoLog << std::endl;
            }
           
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
            glCompileShader(fragment);
            glGetShaderiv(fragment,GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
                    infoLog << std::endl;
            }
            
            //shader program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            //print linking errors if any
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" <<
                    infoLog << std::endl;
            }

            //delete shadersl they are linked and no longere necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use() { //activate / use shader
            glUseProgram(ID);
        }
        //utility set values for uniforms
        void setBool(const std::string &name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }

        void setInt(const std::string &name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        void setFloat(const std::string &name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }

};
#endif
