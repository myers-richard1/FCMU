#version 330 core

layout(location = 0) in vec3 vertex_pos;

out vec2 UV;
out vec2 pixel;

void main(){
    gl_Position.xyz = vertex_pos;
    gl_Position.w = 1.0;
    UV.xy = (vertex_pos.xy +1) /2;
    pixel = UV.xy * 160;
}