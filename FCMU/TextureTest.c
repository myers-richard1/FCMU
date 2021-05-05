#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TextureTest.h"
#include "shaderloader.h"
#include "Memory.h"

#include "Hardware.h"

GLuint vertShader;
GLuint fragShader;
GLuint mapProgram;
GLuint imageDataTexture;
GLuint defaultColorsTexture;
GLuint mapTexture;
GLuint paletteTexture;
GLuint vao;
GLuint vbo;

GLFWwindow* window;


GLFWwindow* test() {
	//regular init stuff
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create the actual window
	window = glfwCreateWindow(600, 600, "Lemon8", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return;

	//char* vertsrc = loadShader("rendertilemap.vert");
	//char* fragsrc = loadShader("rendertilemap.frag");
	//compile the shaders
	char* vertsrc = loadShader("test.vert");
	char* fragsrc = loadShader("test.frag");

	vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertsrc, NULL);
	glCompileShader(vertShader);
	int success;
	char infolog[512];
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertShader, 512, NULL, infolog);
		printf("%s\n", infolog);
	}
	else printf("Vert shader compiled\n");


	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragsrc, NULL);
	glCompileShader(fragShader);
	
	
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, infolog);
		printf("%s\n", infolog);
	}
	else printf("frag shader compiled\n");

	//free the shader text that was allocated
	free(vertsrc);
	free(fragsrc);
	//create the shader program and link the shaders
	mapProgram = glCreateProgram();
	glAttachShader(mapProgram, vertShader);
	glAttachShader(mapProgram, fragShader);
	glLinkProgram(mapProgram);

	glGetProgramiv(mapProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mapProgram, 512, NULL, infolog);
		printf("%s\n", infolog);
	}

	//set up the quad that represents the screen
	float vertices[] = {
		-1, -1, 0,
		-1, 1, 0,
		1, -1, 0,
		-1, 1, 0,
		1, 1, 0, 
		1, -1, 0
	};

	//create out vbo and vao
	vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//setup imageDataTexture (texture that holds the actual pixels of the tiles)
	glActiveTexture(GL_TEXTURE0);
	imageDataTexture;
	glGenTextures(1, &imageDataTexture);
	glBindTexture(GL_TEXTURE_2D, imageDataTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//fill the texture with zeroes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 4096, 1, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
	GLuint imageDataVariableLocation = glGetUniformLocation(mapProgram, "imageData");
	//gl use program is necessary before setting uniforms
	glUseProgram(mapProgram);
	glUniform1i(imageDataVariableLocation, 0);

	//setup defaultColorTexture (the texture that represents the colors that should be mapped to the tiles)
	glActiveTexture(GL_TEXTURE1);
	defaultColorsTexture;
	glGenTextures(1, &defaultColorsTexture);
	glBindTexture(GL_TEXTURE_2D, defaultColorsTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//fill the texture with zeroes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 256, 1, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
	GLuint defaultColorDataVariableLocation = glGetUniformLocation(mapProgram, "defaultColorData");
	glUniform1i(defaultColorDataVariableLocation, 1);

	//setup map texture (the actual tilemap for the background
	glActiveTexture(GL_TEXTURE2);
	mapTexture;
	glGenTextures(1, &mapTexture);
	glBindTexture(GL_TEXTURE_2D, mapTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//fill the texture with zeroes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 1024, 1, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
	GLuint mapVariableLocation = glGetUniformLocation(mapProgram, "mapData");
	glUniform1i(mapVariableLocation, 2);

	//setup palette texture (the texture that holds the rgb values for each color index in the color map)
	glActiveTexture(GL_TEXTURE3);
	paletteTexture;
	glGenTextures(1, &paletteTexture);
	glBindTexture(GL_TEXTURE_2D, paletteTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//fill with zeroes
	//it's 256 "elements" because each element is 3 numbers
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8UI, 256, 1, 0, GL_RGB_INTEGER, GL_UNSIGNED_BYTE, NULL);
	GLuint paletteVariableLocation = glGetUniformLocation(mapProgram, "paletteData");
	glUniform1i(paletteVariableLocation, 3);

	//return the window pointer
	return window;
}

void draw() {
	//clear the screen
	GLenum err;
	glViewport(0, 0, 600, 600);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw background 0
	glUseProgram(mapProgram);
	glActiveTexture(GL_TEXTURE0);
	//buffer the image data, the color indices, the tilemap, and the color palette into vram
	glBindTexture(GL_TEXTURE_2D, imageDataTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 4096, 1, GL_RED_INTEGER, GL_UNSIGNED_BYTE, &map[TILE_PIXEL_DATA_ADDR]);
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("1There was an error: %d\n", err);
	}
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, defaultColorsTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 1, GL_RED_INTEGER, GL_UNSIGNED_BYTE, &map[TILE_COLOR_DATA_ADDR]);
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("2There was an error: %d\n", err);
	}
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mapTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1024, 1, GL_RED_INTEGER, GL_UNSIGNED_BYTE, &map[BACKGROUND_MAP_ADDR]);
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("3There was an error: %d\n", err);
	}
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, paletteTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 1, GL_RGB_INTEGER, GL_UNSIGNED_BYTE, &map[PALETTE_DATA_ADDR]);
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("4There was an error: %d\n", err);
	}
	//bind the vao and draw (does this vao not need to be bound before buffering?)
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//swap buffers and get input
	glfwSwapBuffers(window);
	glfwPollEvents();
}