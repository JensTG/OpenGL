#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <shader.h>
#include <VAO.h>
#include <renderfuncs.h>

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

vector<float> vertices =
{
	-0.5f,  0.5f,  0.0f,
	 0.5f, -0.5f,  0.0f,
	-0.5f, -0.5f,  0.0f,
	 0.5f,  0.5f,  0.0f
};
vector<unsigned int> indices =
{
	0, 1, 2,
	3, 1, 0
};
float speed = 1;
float rot = 0;
double prevTime = 0;

vector<VAO> vaos;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Matricing", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load function pointers:
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader program("uniform", "ourColor");
	VAO vao(vertices, indices);

	program.use();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// -------------------- Rendering --------------------
	while (!glfwWindowShouldClose(window))
	{
		// Input:
		processInput(window);

		// Render:
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		program.use();
		vao.bind();

		rot += (float)((glfwGetTime() - prevTime) * speed);
		prevTime = glfwGetTime();
		mat4 trans = mat4(1.0f);
		trans = rotate(trans, rot, vec3(0.3f, 0.5f, 0.2f));
		unsigned int transLoc = glGetUniformLocation(program.ID, "transform");
		glUniformMatrix4fv(transLoc, 1, GL_FALSE, value_ptr(trans));

		glDrawElements(GL_TRIANGLES, vao.indices.size(), GL_UNSIGNED_INT, NULL);

		// Bufferswap and polling:
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clear all previously allocated GLFW resources:
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		speed += 0.01f * speed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		speed -= 0.01f * speed;
}