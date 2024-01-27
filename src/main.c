#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdlib.h>

#define DEBUG

#include "aids.h"


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}

unsigned int compile_vertex_shader(const char *path) {
    Kyle shader_source = kyle_from_file(path);

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    int shader_lengths[] = { shader_source.length };
    glShaderSource(vertex_shader, 1, &shader_source.data, shader_lengths);
    glCompileShader(vertex_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (! success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
        exit(1);
    }

    DEBUG_PRINT("VERTEX SHADER COMPILED SUCCESSFULLY %s", "");

    return vertex_shader;
}

unsigned int compile_fragment_shader(const char *path) {
    Kyle shader_source = kyle_from_file(path);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    int shader_lengths[] = { shader_source.length };
    glShaderSource(fragment_shader, 1, &shader_source.data, shader_lengths);
    glCompileShader(fragment_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (! success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAG::COMPILATION_FAILED\n%s\n", infoLog);
        exit(1);
    }

    DEBUG_PRINT("FRAGMENT SHADER COMPILED SUCCESSFULLY %s", "");

    return fragment_shader;
}

unsigned int create_shader_program(unsigned int shaders[], int len) {
    unsigned int shader_program = glCreateProgram();

    for (int i = 0; i < len; i += 1) {
        glAttachShader(shader_program, shaders[i]);
    }

    glLinkProgram(shader_program);

    int success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

    if (! success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINK_ERROR\n%s\n", infoLog);
        exit(1);
    }

    DEBUG_PRINT("SHADER PROGRAM CREATED SUCCESSFULLY %s", "");

    return shader_program;
}

int main() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*
    const char *foo = glfwGetVersionString();
    printf("HELLO: %s\n", foo);
    fflush(stdout);
    */
    

    GLFWwindow *window = glfwCreateWindow(
        800,
        600,
        "Rustriss",
        NULL,
        NULL
    );

    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);

    unsigned int vertex_shader = compile_vertex_shader("./shaders/main.vert");
    unsigned int fragment_shader = compile_fragment_shader("./shaders/main.frag");

    unsigned int shaders[] = { vertex_shader, fragment_shader };

    unsigned int shader_program = create_shader_program(
        shaders,
        sizeof(shaders) / sizeof(unsigned int)
    );

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // BIND VAO
    glBindVertexArray(VAO);

    // BIND Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void *) 0
    );
    glEnableVertexAttribArray(0);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Unbind VAO
    glBindVertexArray(0);


    // TODO: Maybe it needs to be moved into the loop?
    glUseProgram(shader_program);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (! glfwWindowShouldClose(window)) {
        // Handle inputs
        process_input(window);


        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);
    glfwTerminate();

    return 0;
}
