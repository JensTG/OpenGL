#include <glad/glad.h>
#include <glfw3.h>

#include <VAO.h>
#include <shader.h>
#include <map.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

float speed = 1;

float turnHor = 0.0f;
float turnVer = 0.0f;
vec3 camera = vec3(0.0f, 0.0f, 6.0f);
float zoom = 1.0f;

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
	cout << endl;
	program.use();

	vector<VAO> cubes = readCollection("C:/VSC_PRO_B/OpenGL/resources/collections/proof.col");

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// -------------------- Rendering --------------------
	while (!glfwWindowShouldClose(window))
	{
		// Input:
		processInput(window);

		// Render:
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.use();

		// Creating a projection:
		mat4 proj = perspective(radians(45.0f), SCR_HEIGHT > 0 ? (float)(SCR_WIDTH / SCR_HEIGHT) : 1, 0.1f, 1000.0f);
		program.setMat4("proj", proj);

		// Moving the camera:
		mat4 view = mat4(1.0f);
		view = translate(view, camera * vec3(sin(turnHor), 0.0f, cos(turnHor)));
		view = rotate(view, -turnHor, vec3(0.0f, 1.0f, 0.0f));
		view = scale(view, vec3(zoom, zoom, zoom));
		program.setMat4("view", view);

		// Moving the model around:
		mat4 model = mat4(1.0f);
		program.setMat4("model", model);

		for (int i = 0; i < cubes.size(); i++)
		{
			cubes[i].bind();
			glDrawElements(GL_TRIANGLES, cubes[i].nInd, GL_UNSIGNED_INT, NULL);
		}


		// Bufferswap and polling:
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clear all previously allocated GLFW resources:
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	SCR_HEIGHT = height;
	SCR_WIDTH = width;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera -= 0.1f * vec3((float)sin(turnHor), 0.0f, (float)cos(turnHor));
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera += 0.1f * vec3((float)sin(turnHor), 0.0f, (float)cos(turnHor));
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		turnHor += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		turnHor -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.y += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.y -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
		zoom += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
		zoom -= 0.01f;
}