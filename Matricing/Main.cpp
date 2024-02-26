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
	VAO cube("C:/VSC_PRO_B/OpenGL/resources/shapes/cube");
	program.use();

	unsigned int texture;
	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);
	int width; int height; int nrChannels;
	unsigned char* data = stbi_load("C:/VSC_PRO_B/OpenGL/resources/textures/red_brick_wall.jpg", &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

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
		// Moving the camera:
		mat4 view = mat4(1.0f);
		view = translate(view, vec3(0.0f, 0.0f, -15.0f));

		program.setMat4("proj", proj);
		program.setMat4("view", view);

		for (int i = 0; i < 6; i++)
		{
			// Moving the model around:
			mat4 model = mat4(1.0f);
			model = translate(model, vec3(i - 3, (float)i / 2, -5.0f));
			model = rotate(model, (float)glfwGetTime() * radians(20.0f), vec3(1.0f, 0.0f, 0.0f));
			model = scale(model, vec3(1.0f, i + 1, 1.0f));

			program.setMat4("model", model);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			glDrawElements(GL_TRIANGLES, cube.nInd, GL_UNSIGNED_INT, NULL);
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
		speed += 0.001f * speed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		speed -= 0.001f * speed;
}