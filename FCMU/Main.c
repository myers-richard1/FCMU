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

GLFWwindow* window;

void processInput();
void render();
void load_rom(char *filename);

int main() {
	init_map();
	
	load_rom("output.bin");
	init_cpu();
	window = test();
	while (!glfwWindowShouldClose(window)) {
		cycle();
		processInput();
		draw();
	}
	glfwTerminate();
	return 0;
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

	string[fsize] = 0;
}