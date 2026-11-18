#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Renderer/ShaderProgram.h"

float offset = 0.0f;

GLfloat points[] = {
    0.0f, 0.5f+offset, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

const char *vertexShader =
        "#version 460\n"
        "layout(location = 0) in vec3 vert_pos;"
        "layout(location = 1) in vec3 vert_color;"
        "out vec3 color;"

        "void main() {"
        "    color = vert_color;"
        "    gl_Position = vec4(vert_pos, 1.0);"
        "};";

const char *fragmentShader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "    frag_color = vec4(color, 1.0);"
        "}";


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

int main() {
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

    Renderer::ShaderProgram shaderProgram(vertexShader, fragmentShader);
    if (!shaderProgram.isCompiled()) {
        std::cerr << "Can't create shader program!" << std::endl;
        return -1;
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

        shaderProgram.use();

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
