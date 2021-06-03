#include <stdio.h>
#include <stdlib.h>

#include "shaderloader.h"

char* loadShader(char* filename) {
#pragma warning (disable : 4996)
    char* toReturn = NULL;
    FILE* shaderFile = fopen(filename, "r");
    if (shaderFile) {
        fseek(shaderFile, 0, SEEK_END);
        long filesize = ftell(shaderFile);
        rewind(shaderFile);

        toReturn = calloc(filesize + 1, sizeof(char));

        if (toReturn) {
            fread(toReturn, sizeof(char), filesize, shaderFile);
            printf("Read shader: \n");
            printf("%s\n", toReturn);
        }
    }
    fclose(shaderFile);
    return toReturn;
}
