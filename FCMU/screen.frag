#version 330 core
out vec4 FragColor;

uniform sampler2D consoleTexture;

in vec2 UV;

void main()
{
    //FragColor = vec4(UV.x, UV.x, UV.x, 1.0f);
    FragColor = texture(consoleTexture, UV);
} 