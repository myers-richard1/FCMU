#version 330 core
out vec4 FragColor;

in vec3 uv;

in float pixelX, pixelY;

uniform usampler2D imageData;
uniform usampler2D defaultColorData;
uniform usampler2D mapData;
uniform usampler2D paletteData;

void main()
{

    int row = int(pixelY) % 8;
    int column = int(pixelX) % 8;
    int byteIndex = row * 2;
    if (column > 3) byteIndex++;

    uint byteValue = texelFetch(imageData, ivec2(byteIndex, 0), 0).r;

    uint columnInByte = uint(column);
    if (columnInByte > uint(3)) columnInByte = columnInByte - uint(4);
    uint shiftAmount = uint(3) - columnInByte;
    shiftAmount *= uint(2);
    uint bits = byteValue >> shiftAmount;
    bits = bits & uint(3);
    

    FragColor = vec4(1,1,0,1);
    if (byteValue == uint(0xff))
        FragColor = vec4(uv.x, uv.y, 1, 1.0f);

    if (bits == uint(3)) FragColor = vec4(0,0,0,1);
    else if (bits == uint(0)) FragColor = vec4(1,1,1,1);

    uint colorByte = texelFetch(defaultColorData, ivec2(0,0),0).r;
    if (colorByte != 0u) FragColor = vec4(1,0,0,1);
} 
