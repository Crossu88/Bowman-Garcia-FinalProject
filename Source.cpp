#include <iostream>

#include<GL/glew.h>
#include<GLFW/glfw3.h>
//#include<GLFW/glfw3.h>

void error_callback(int error, const char* desc);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
	
	if (!glfwInit()) {
		// print that there was an error
		printf("GLFW Initialization: Failed!\n");
	}

	printf("GLFW Initialization: OK!\n");

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	GLFWwindow* window = glfwCreateWindow(640, 400, "My Window", NULL, NULL);
	if (!window) {
		// Window or Context creation failed
		printf("Window or Context Creation: Failed!\n");
	}

	glfwMakeContextCurrent(window);

	// Bind key callbacks
	glfwSetKeyCallback(window, key_callback);

	// Initialize Glew
	if (glewInit()) {
		printf("GLEW Initialization: Failed!\n");
	}

	glfwSwapInterval(1);

	GLuint Program;
	GLuint vao[1];

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vShaderSrc = "#version 460 \n void main() { gl_position = vec4(0.0, 0.0, 0.0, 1.0); }";
	const char* fShaderSrc = "#version 460 \n out vec4 color; void main() { color = vec4(0.0, 0.3, 1.0, 1.0); }";

	glShaderSource(vShader, 1, &vShaderSrc, NULL);
	glShaderSource(fShader, 1, &fShaderSrc, NULL);

	glCompileShader(vShader);
	glCompileShader(fShader);

	Program = glCreateProgram();
	glAttachShader(Program, vShader);
	glAttachShader(Program, fShader);

	glLinkProgram(Program);

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	// Display loop
	while (!glfwWindowShouldClose(window)) {
		// display loop runs

		glClearColor(1.0f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glPointSize(100.0f);
		glDrawArrays(GL_POINT, 0, 1);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void error_callback(int err, const char* desc) {
	printf("GLFW Error: %s\n", desc);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}