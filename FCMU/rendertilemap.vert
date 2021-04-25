#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 uv;

out float pixelX, pixelY;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    uv = (aPos + 1) /2;
    uv.y = 1-uv.y;
    pixelX = uv.x * 160;
    pixelY = uv.y * 160;
}