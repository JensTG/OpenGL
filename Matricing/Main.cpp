#include <glad/glad.h>
#include <glfw3.h>

#include <VAO.h>
#include <shader.h>
#include <map.h>
#include <camera.h>
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
void processInput(GLFWwindow* window, float deltaTime);

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

float deltaTime = 0.0f;
float lastTime = 0.0f;

camera cam(15.0f, 1.0f);
vector<VAO> points;
bool keysHeld[GLFW_KEY_LAST + 1];

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

	// Establishing the model mat4:
	mat4 model = mat4(1.0f);
	program.setMat4("model", model);

	glEnable(GL_DEPTH_TEST);

	// -------------------- Rendering --------------------
	while (!glfwWindowShouldClose(window))
	{
		// Input:
		float currentTime = glfwGetTime();
		deltaTime = lastTime - currentTime;
		lastTime = currentTime;
		processInput(window, deltaTime);
		
		// Render:
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.use();

		// Creating a projection:
		mat4 proj = perspective(radians(45.0f), SCR_HEIGHT > 0 ? (float)(SCR_WIDTH / SCR_HEIGHT) : 1, 0.1f, 1000.0f);
		program.setMat4("proj", proj);

		// Moving the camera:
		program.setMat4("view", cam.calculate());

		for (int i = 0; i < points.size(); i++)
		{
			points[i].bind();
			points[i].applyTransform(program);
			glDrawElements(GL_TRIANGLES, points[i].nInd, GL_UNSIGNED_INT, NULL);
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

void processInput(GLFWwindow* window, float deltaTime) {
	float speedMul = 1.0f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speedMul = 2.0f;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (!keysHeld[GLFW_KEY_SPACE])
		{
			points.push_back(VAO("C:/VSC_PRO_B/OpenGL/resources/shapes/cube", scale(rotate(translate(mat4(1.0f), cam.cPos), acos(dot(vec3(0, 0, 1), cam.cFront)), normalize(cam.cFront)), vec3(0.1f))));
			keysHeld[GLFW_KEY_SPACE] = true;
		}
	}
	else keysHeld[GLFW_KEY_SPACE] = false;

	cam.takeKeyLook(window, deltaTime * speedMul);
	cam.takeMovement(window, deltaTime * speedMul);
}