#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <shader.h>
#include <vector>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, int* count);
float degToRad(float deg);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    // -------------------- Setup --------------------
    Shader program("C:\\VSC_PRO_B\\OpenGL\\resources\\shaders\\colorVertex.txt", "C:\\VSC_PRO_B\\OpenGL\\resources\\shaders\\ourColorFragment.txt");
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    int count = 3;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // -------------------- Rendering --------------------
    while (!glfwWindowShouldClose(window))
    {
        // Input:
        processInput(window, &count);

        // Render:
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        count = (int)(sin(glfwGetTime()) * 10) + 13;

        vector<float> vertices;
        vector<unsigned int> indices;

        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        for (unsigned int i = 1; i <= count; i++)
        {
            float rad = degToRad((360 / count) * i);
            vertices.push_back(cosf(rad));
            vertices.push_back(sinf(rad));
            vertices.push_back(0.0f);

            indices.push_back(i);
            indices.push_back(0);
            indices.push_back(i + 1);
        }
        indices.pop_back();
        indices.push_back(1);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), (void*)vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), (void*)indices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        program.use();
        glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_INT, NULL);

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

void processInput(GLFWwindow* window, int* count) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float degToRad(float deg)
{
    return deg * (3.1415f / 180);
}
