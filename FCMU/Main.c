#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "Memory.h"
#include "cpu.h"
#include "Renderer.h"

void init();

GLFWwindow* window;

void processInput();
void render();
void load_rom(char *filename);

int main() {
	printf("Hello, graphics world!\n");
	window = init_renderer();
	init_map();
	load_rom("output.bin");
	printf("Map[0] = %x\n", map[0]);
	printf("Map[1] = %x\n", map[1]);
	init_cpu();
	//test_cpu();
	
	while (!glfwWindowShouldClose(window)) {
		cycle();
		processInput();
		render_console();
		render_screen();
		
		//getchar();
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