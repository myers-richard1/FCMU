#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "Memory.h"
#include "cpu.h"

void init();
void resize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void render();
void load_rom(char *filename);

int main() {
	printf("Hello, graphics world!\n");
	/**initialize glfw**/
	init();
	init_map();
	load_rom("output.bin");
	printf("Map[0] = %x\n", map[0]);
	printf("Map[1] = %x\n", map[1]);
	init_cpu();
	//test_cpu();
	GLFWwindow* window = glfwCreateWindow(800, 600, "FCMU", NULL, NULL);
	if (window == NULL) {
		printf("Window could not be created, for whatever reason. Exiting :(");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	/*initialize glad*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("GLAD broke :(");
		return -1;
	}
	/*initialize our rendering viewport*/
	glViewport(0, 0, 800, 600);
	/*set up resize callback*/
	glfwSetFramebufferSizeCallback(window, resize_callback);
	while (!glfwWindowShouldClose(window)) {
		cycle();
		processInput(window);
		render();
		glfwSwapBuffers(window);
		glfwPollEvents();
		//getchar();
	}
	glfwTerminate();
	return 0;
}

void init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void render() {
	glClearColor(0, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
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