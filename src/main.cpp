#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"

constexpr int ERROR_LOAD_SHADER_PROGRAM = -101;

float offset = 0.0f;

GLfloat points[] = {
    0.0f, 0.5f + offset, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

int WindowSizeX = 800;
int WindowSizeY = 460;

static void glfwWindowSizeCallback(GLFWwindow *window, const int width, const int height) {
    WindowSizeX = width;
    WindowSizeY = height;
    printf("Resized Window\nX: %d\nY: %d\n", WindowSizeX, WindowSizeY);
    printf("Resized In Pixels: \nX: %d\nY: %d\n", WindowSizeX - width, WindowSizeY - height);

    glViewport(0, 0, WindowSizeX, WindowSizeY);
}

static void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        } else {
            std::cout << key << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "failed load glad" << std::endl;
        return -1;
    }


    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(WindowSizeX, WindowSizeY, "Battle City", nullptr, nullptr);
    if (!window) {
        std::cout << "failed init window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "failed load glad" << std::endl;
        return -1;
    }

    std::cout << "OpenGL VERSION: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    glClearColor(0.f, 255.f, 0.f, 0.0f);

    {
        ResourceManager resource_manager(argv[0]);
        auto shader = resource_manager.loadShader(
            "triangle",
            "res/shaders/vertex.glsl",
            "res/shaders/fragment.glsl");

        if (!shader) {
            std::cerr << "Can't load shader program!" << std::endl;
            return ERROR_LOAD_SHADER_PROGRAM;
        }

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shader->use();

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
