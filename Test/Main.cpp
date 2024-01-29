#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>

int height = 600;
int width = 800;

float vertices[2][9] = { 
	{
		0.0f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f
	},
	{
		0.0f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	}
};

//unsigned int indices[] = {  // note that we start from 0!
//	0, 1, 3,   // first triangle
//	1, 2, 3    // second triangle
//};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSources[2] = {
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f, 0.0f, 0.0f, 1.0f);\n"
"}\0",
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.0f, 0.0f, 0.8f, 1.0f);\n"
"}\0"
};

int  success;
char infoLog[512];

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(width, height, "Sandbox", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed window-creation" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Configurations:
	glClearColor(0.29f, 0.0f, 0.51f, 1.0f);

	// Linking vertex attributes and VAO:
	GLuint VAOs[2];
	glGenVertexArrays(2, VAOs);
	GLuint VBOs[2];
	glGenBuffers(2, VBOs);
	/*unsigned int EBO;
	glGenBuffers(1, &EBO);*/

	unsigned int fragmentShaders[2];
	unsigned int shaderPrograms[2];
	shaderPrograms[0], shaderPrograms[1] = glCreateProgram();

	// Vertex shader:
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	for (int i = 0; i < 2; i++) {
		glBindVertexArray(VAOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Fragment shaders:
		fragmentShaders[i] = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaders[i], 1, &fragmentShaderSources[i], NULL);
		glCompileShader(fragmentShaders[i]);

		// Programs:
		glAttachShader(shaderPrograms[i], vertexShader);
		glAttachShader(shaderPrograms[i], fragmentShaders[i]);
		glLinkProgram(shaderPrograms[i]);

		glGetProgramiv(shaderPrograms[i], GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderPrograms[i], 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
			return -1;
		}

		glDeleteShader(fragmentShaders[i]);
	}
	glDeleteShader(vertexShader);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	while (!glfwWindowShouldClose(window)) {
		// ----------- Input -----------
		processInput(window);

		// ----------- Render -----------
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < 2; i++) {
			glUseProgram(shaderPrograms[i]);
			glBindVertexArray(VAOs[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		glBindVertexArray(0);

		// ----------- Events and bufferswapping -----------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}