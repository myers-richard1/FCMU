#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* init_renderer(); 
void render_console();
void render_screen();
void init_screen_buffers();
void init_console_buffers();
void resize_callback(GLFWwindow* window, int width, int height);