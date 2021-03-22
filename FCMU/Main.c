#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "Memory.h"
#include "cpu.h"
#include "Renderer.h"
#include "TextureTest.h"

void init();
void process_framerate();

GLFWwindow* window;

void processInput();
void render();
void load_rom(char *filename);

double currentTime, previousTime;
int frameCount;

int main() {
	init_map();
	load_rom("output.bin");
	init_cpu();
	window = test();
	previousTime = glfwGetTime();
	frameCount = 0;
	int cyclesPerFrame = (4000 / 60);
	while (!glfwWindowShouldClose(window)) {
		process_framerate();
		for (int i = 0; i < cyclesPerFrame; i++)
			cycle();
		processInput();
		draw();
	}
	glfwTerminate();
	return 0;
}


void process_framerate() {
	double currentTime = glfwGetTime();
	frameCount++;
	if (currentTime - previousTime >= 1) {
		printf("FPS: %d\n", frameCount);
		frameCount = 0;
		previousTime = currentTime;
	}
}

void processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}


void load_rom(char* filename) {
#pragma warning (disable : 4996)
	FILE* f = fopen(filename, "rb");
	if (f == NULL) printf("File null\n");
	else printf("File not null\n");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* string = malloc(fsize + 1);
	fread(string, 1, fsize, f);
	fclose(f);

	memcpy(map, string, fsize);
	free(string);
	string[fsize] = 0;
}