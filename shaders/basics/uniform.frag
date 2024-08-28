#version 460 core
out vec4 FragColor;

uniform vec4 ourColor; //Set variable in C++ code

void main() {
    FragColor = ourColor;
}
