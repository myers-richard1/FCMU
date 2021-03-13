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
    //calculate which row and column of the tile image this pixel should use
    int row = int(pixelY) % 8;
    int column = int(pixelX) % 8;

    //calculate which index into the map we should be looking at
    int tileIndexInMapX = int(pixelX) / 8;
    int tileIndexInMapY = int(pixelY) / 8;

    int tileIndex = (tileIndexInMapY * 32) + tileIndexInMapX;
    //get the tile ID from the map
    uint tileID = texelFetch(mapData, ivec2(tileIndex, 0), 0).r;

    //find start of tile image 
    int byteIndex = 16*int(tileID);
    //move into tile image to find which byte we're reading from
    byteIndex += row * 2;
    if (column > 3) byteIndex++;
    //get the byte that contains the pixel data we need to decode
    uint byteValue = texelFetch(imageData, ivec2(byteIndex, 0), 0).r;

    //calculate which of the 4 possible "columns" (pair of bits) this pixel is in
    uint columnInByte = uint(column);
    if (columnInByte > uint(3)) columnInByte = columnInByte - uint(4);
    //calculate how much to shift by to move the relevant bits to the very right
    uint shiftAmount = uint(3) - columnInByte;
    shiftAmount *= uint(2);
    uint bits = byteValue >> shiftAmount;
    //mask out everything but the lowest 2 bits
    bits = bits & uint(3);
    //bits now contains the color id number (0-3) of this pixel. we plug this ID into the palette to get the actual color
    //but first we need to find out what palette we're using
    uint paletteID = texelFetch(defaultColorData, ivec2(tileID, 0), 0).r;
    //since there's four colors in each palette, we need to select the color from the palette
    uint colorOffset = paletteID * 4u;
    //now we get the palette from the palette data by adding the bits to the offset to select one of the four
    uvec3 palette = texelFetch(paletteData, ivec2(colorOffset + bits, 0), 0).rgb;

    FragColor = vec4(palette.r,palette.g,palette.b,1);

    //if (bits == uint(3)) FragColor = vec4(0,0,0,1);
    //else if (bits == uint(0)) FragColor = vec4(1,1,1,1);
} 
