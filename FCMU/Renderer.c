#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "shaderloader.h"

GLFWwindow* window;
GLuint renderTextureBuffer; 
GLint renderTexture;
GLenum drawBuffers[1];

GLuint global_VAO;
GLuint screen_VBO; 
GLuint console_VBO;

GLuint screen_VertexShader, screen_FragmentShader;
GLuint screen_program;

static const GLfloat points[] = {
	-1, -1, 0,
	1, -1, 0,
	0, 1, 0
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
		return -1;
	}
	/*initialize our rendering viewport*/
	glViewport(0, 0, 600, 600);
	/*set up resize callback*/
	glfwSetFramebufferSizeCallback(window, resize_callback);

	//gen the vao
	glGenVertexArrays(1, &global_VAO);
	glBindVertexArray(global_VAO);

	
	init_screen_buffers();
	
	
	return window;
}

void init_console_buffers(){
	glGenBuffers(1, &console_VBO);

	//generate the framebuffer for the render texture
	renderTextureBuffer = 0;
	glGenFramebuffers(1, &renderTextureBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, renderTextureBuffer);

	//create a texture to render to 
	glGenTextures(1, &renderTexture);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	//empty image(?)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 160, 160, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	//suggested by learnopengl
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//skipping depth buffer. dont see a point for it
	//set rendertexture as the color attachment 0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);

	//set the list of draw buffers
	drawBuffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Something wrong with the render texture");
}

void init_screen_buffers() {
	printf("initializing screen...\n");
	glGenBuffers(1, &screen_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, screen_VBO);

	char* vertexshadersrc = loadShader("screen.vert");
	char* fragshadersrc = loadShader("screen.frag");

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
	glGetShaderiv(screen_VertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(screen_VertexShader, 512, NULL, infoLog);
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
}

void render_console() {
	glBindFramebuffer(GL_FRAMEBUFFER, renderTextureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, console_VBO);
	glViewport(0, 0, 160, 160);
}

void render_screen() {
	printf("begin render\n");
	glUseProgram(screen_program);
	

	printf("binding\n");
	
	
	glBindBuffer(GL_ARRAY_BUFFER, screen_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	printf("attribute arrays\n");

	glClearColor(0, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}