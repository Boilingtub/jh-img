#version 460 core
layout (location = 0) in vec3 aPos;

uniform vec4 ourPos; //Set variable in C++ code 

void main() {
    gl_Position = vec4(aPos.x * ourPos.x, aPos.y * ourPos.y, aPos.z * ourPos.z, 1.0);
}
