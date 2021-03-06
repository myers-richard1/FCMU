#version 330 core

layout(location = 0) in vec3 vertex_pos;

out vec2 UV;

void main(){
    gl_Position.xyz = vertex_pos;
    UV.xy = (vertex_pos.xy +1) /2;
    gl_Position.w = 1.0;
}