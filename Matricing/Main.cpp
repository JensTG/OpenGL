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
void processInput(GLFWwindow* window, float deltaTime);

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

float deltaTime = 0.0f;
float lastTime = 0.0f;

vec3 cPos = vec3(0.0f, 0.0f, 6.0f);
vec3 cFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cUp = vec3(0.0f, 1.0f, 0.0f);

float cYaw = -90.0f;
float cPitch = 0.0f;
float cRoll = 0.0f;

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

	Shader program("uniform", "uniform");
	cout << endl;
	program.use();

	// Establishing the model mat4:
	mat4 model = mat4(1.0f);
	program.setMat4("model", model);

	vector<VAO> cubes = readCollection("C:/VSC_PRO_B/OpenGL/resources/collections/proof.col");

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
		mat4 view = mat4(1.0f);
		view = lookAt(cPos, cPos + cFront, cUp);
		program.setMat4("view", view);

		for (int i = 0; i < cubes.size(); i++)
		{
			cubes[i].bind();
			cubes[i].applyTransform(program);
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

void processInput(GLFWwindow* window, float deltaTime) {
	float cSpeed = 1.0f * deltaTime;
	float cLook = 15.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cPos -= cFront * cSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cPos += cFront * cSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cPos += normalize(cross(cFront, cUp)) * cSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cPos -= normalize(cross(cFront, cUp)) * cSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cPos -= normalize(cross(cross(cFront, cUp), cFront)) * cSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cPos += normalize(cross(cross(cFront, cUp), cFront)) * cSpeed;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cPitch -= cLook;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cPitch += cLook;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cYaw += cLook;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cYaw -= cLook;

	cPitch = cPitch < -89.0f ? -89.0f : cPitch;
	cPitch = cPitch > 89.0f ? 89.0f : cPitch;

	cFront = vec3(cos(radians(cYaw)) * cos(radians(cPitch)), sin(radians(cPitch)), sin(radians(cYaw)) * cos(radians(cPitch)));
}