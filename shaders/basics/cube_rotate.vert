#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec3 Theta;
out vec3 vertexColor;

vec3 rotate_y_axis(vec3 pos);
vec3 rotate_x_axis(vec3 pos);
vec3 rotate_z_axis(vec3 pos);

vec3 rotate_y_axis(vec3 pos) {
    vec3 D;
    D.x = pos.x * cos(Theta.y) - pos.z * sin(Theta.y);
    D.y = pos.y;
    D.x = pos.x * sin(Theta.y) + pos.z * cos(Theta.y);
    return D;
}

vec3 rotate_x_axis(vec3 pos) {
    vec3 D;
    D.x = pos.x;
    D.y = pos.y * cos(Theta.x) - pos.z * sin(Theta.x);
    D.z = pos.y * sin(Theta.x) + pos.z * cos(Theta.x);
    return D;
}

vec3 rotate_z_axis(vec3 pos) {
    vec3 D;
    D.x = pos.x * cos(Theta.z) - pos.y * sin(Theta.z);
    D.y = pos.x * sin(Theta.z) + pos.y * cos(Theta.z);
    D.z = pos.z;
    return D;
}


void main() {
    vec3 x_rotated_pos = rotate_x_axis(aPos);
    vec3 xy_rotated_pos = rotate_y_axis(x_rotated_pos);
    vec3 xyz_roteted_pos = rotate_z_axis(xy_rotated_pos);
    vertexColor = aColor;
    gl_Position = vec4(xyz_roteted_pos.x , xyz_roteted_pos.y, xyz_roteted_pos.z, 1.0);
}
