#version 460

layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec3 vert_color;

out vec3 color;

void main() {
    color = vert_color;
    gl_Position = vec4(vert_pos, 1.0);
}