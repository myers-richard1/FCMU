#version 330 core
out vec4 FragColor;

in vec2 UV;
in vec2 pixel;

uniform sampler2D tileData;

void main()
{



    //get the byte
    vec4 pixelData = texelFetch(tileData, ivec2(0,0), 0).rgba;

    if (pixelData.b == 1) FragColor = vec4(1,1,1,1);
    else if (pixelData.r == 0) FragColor = vec4(0,1,0,1);
    else FragColor = vec4(0,0,0,1);


} 