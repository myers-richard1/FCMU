#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "Renderer.h"
#include "shaderloader.h"
#include "Memory.h"

GLFWwindow* window;

GLuint screen_VAO;

GLuint screen_VBO;

GLuint screen_VertexShader, screen_FragmentShader;
GLuint screen_program;

GLuint tileDataTexture;

const int consoleHorizontalResolution = 160;
const int consoleVerticalResolution = 160;

static const GLfloat quad_points[] = {
	-1, -1, 0,
	-1, 1, 0,
	1, -1, 0,
	-1, 1, 0,
	1, 1, 0,
	1, -1, 0
};

GLFWwindow* init_renderer() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(600, 600, "FCMU", NULL, NULL);
	if (window == NULL) {
		printf("Window could not be created, for whatever reason. Exiting :(");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	/*initialize glad*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("GLAD broke :(");
		return (GLFWwindow*)-1;
	}
	/*initialize our rendering viewport*/
	glViewport(0, 0, 600, 600);
	/*set up resize callback*/
	glfwSetFramebufferSizeCallback(window, resize_callback);

	//gen and bind the vao for the screen
	glGenVertexArrays(1, &screen_VAO);
	glBindVertexArray(screen_VAO);
	init_screen_buffers();

	return window;
}


void init_screen_buffers() {
	printf("initializing screen...\n");
	glGenBuffers(1, &screen_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, screen_VBO);

	const GLchar* vertexshadersrc = loadShader("screen.vert");
	const GLchar* fragshadersrc = loadShader("screen.frag");

	screen_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	screen_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	printf("Compiling shaders...\n");

	glShaderSource(screen_VertexShader, 1, &vertexshadersrc, NULL);

	printf("loaded vert source\n");
	glCompileShader(screen_VertexShader);

	printf("Vert compiled.\n");
	glShaderSource(screen_FragmentShader, 1, &fragshadersrc, NULL);
	glCompileShader(screen_FragmentShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(screen_FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(screen_FragmentShader, 512, NULL, infoLog);
		printf("%s\n", infoLog);
	}
	else {
		printf("vert shader compiled successfully\n");
	}
	printf("Creating program...\n");

	screen_program = glCreateProgram();
	glAttachShader(screen_program, screen_VertexShader);
	glAttachShader(screen_program, screen_FragmentShader);
	glLinkProgram(screen_program);

	printf("Finished setting up screen program\n");

	printf("binding\n");

	glBindBuffer(GL_ARRAY_BUFFER, screen_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_points), quad_points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	printf("attribute arrays\n");
	free((void*)vertexshadersrc);
	free((void*)fragshadersrc);

	glGenTextures(1, &tileDataTexture);
	glBindTexture(GL_TEXTURE_2D, tileDataTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	byte color = 1;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 1, 1, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, &color);
}

void render_screen() {
	printf("begin render\n");
	glUseProgram(screen_program);
	glViewport(0, 0, 600, 600);

	glClearColor(0, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, tileDataTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
