#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TextureTest.h"
#include "shaderloader.h"
#include "Memory.h"


void test() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Test", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return;

	char* vertsrc = loadShader("test.vert");
	char* fragsrc = loadShader("test.frag");

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
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


	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragsrc, NULL);
	glCompileShader(fragShader);
	
	
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, infolog);
		printf("%s\n", infolog);
	}
	else printf("frag shader compiled\n");

	GLuint mapProgram = glCreateProgram();
	glAttachShader(mapProgram, vertShader);
	glAttachShader(mapProgram, fragShader);
	glLinkProgram(mapProgram);

	glGetProgramiv(mapProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(mapProgram, 512, NULL, infolog);
		printf("%s\n", infolog);
	}

	float vertices[] = {
		-1, -1, 0,
		-1, 1, 0,
		1, -1, 0,
		-1, 1, 0,
		1, 1, 0, 
		1, -1, 0
	};

	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//setup imageDataTexture
	glActiveTexture(GL_TEXTURE0);
	GLuint imageDataTexure;
	glGenTextures(1, &imageDataTexure);
	glBindTexture(GL_TEXTURE_2D, imageDataTexure);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 4096, 1, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, &map[0xa500]);
	GLuint imageDataVariableLocation = glGetUniformLocation(mapProgram, "imageData");
	//gl use program is necessary before setting uniforms
	glUseProgram(mapProgram);
	glUniform1i(imageDataVariableLocation, 0);

	//setup defaultColorTexture
	glActiveTexture(GL_TEXTURE1);
	GLuint defaultColorsTexture;
	glGenTextures(1, &defaultColorsTexture);
	glBindTexture(GL_TEXTURE_2D, defaultColorsTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 100, 1, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, &map[0xb500]);
	GLuint defaultColorDataVariableLocation = glGetUniformLocation(mapProgram, "defaultColorData");
	glUniform1i(defaultColorDataVariableLocation, 1);

	//setup map texture
	glActiveTexture(GL_TEXTURE2);
	GLuint mapTexture;
	glGenTextures(1, &mapTexture);
	glBindTexture(GL_TEXTURE_2D, mapTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 1024, 1, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, &map[0xb600]);
	GLuint mapVariableLocation = glGetUniformLocation(mapProgram, "mapData");
	glUniform1i(mapVariableLocation, 2);

	//setup palette texture
	glActiveTexture(GL_TEXTURE3);
	GLuint paletteTexture;
	glGenTextures(1, &paletteTexture);
	glBindTexture(GL_TEXTURE_2D, paletteTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8UI, 256, 1, 0, GL_RGB_INTEGER, GL_UNSIGNED_BYTE, &map[0xd400]);
	GLuint paletteVariableLocation = glGetUniformLocation(mapProgram, "paletteData");
	glUniform1i(paletteVariableLocation, 3);

	
	glViewport(0, 0, 800, 600);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//okay.... when you actually UPDATE the texture again, use TEXSUBIMAGE not TEXIMAGE. that's what the smart guys on SO said
		glUseProgram(mapProgram);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, imageDataTexure);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, defaultColorsTexture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, paletteTexture);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return;
}