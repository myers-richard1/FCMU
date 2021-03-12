#version 330 core
out vec4 FragColor;

in vec2 UV;

uniform usampler2D tileData;

void main()
{
    FragColor = vec4(UV.x, UV.y, 1, 1.0f);
    FragColor = texture2D(tileData, UV);
} 