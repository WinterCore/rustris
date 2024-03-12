#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define DEBUG

#include "game.h"
#include "aids.h"
#include "draw.h"


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    App *app = glfwGetWindowUserPointer(window);
    glViewport(0, 0, width, height);

    app->viewport_width = width;
    app->viewport_height = height;
    update_board_dimensions(app);

    DEBUG_PRINT("WINDOW RESIZE width=%u, height=%u | square width = %u", width, height, app->ui_board.square_width);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}

uint32_t compile_vertex_shader(const char *path) {
    Kyle shader_source = kyle_from_file(path);

    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);

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

uint32_t compile_fragment_shader(const char *path) {
    Kyle shader_source = kyle_from_file(path);

    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

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

uint32_t create_shader_program(unsigned int shaders[], int len) {
    uint32_t shader_program = glCreateProgram();

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /*
    Tetromino tetro = TETROMINOS[TETRO_Z];
    
    printf("HELLO: %d\n", tetro.vertices_count);
    fflush(stdout);
    */
    
    uint8_t square_width = 0;

    /*
    for (uint32_t i = 0; i < tetro.vertices_count; i += 2) {
        // x
        tetro.vertices[i + 0] = (2.0f / (float) BOARD_WIDTH) * tetro.vertices[i + 0] - 1;

        // y
        tetro.vertices[i + 1] = (2.0f / (float) BOARD_HEIGHT) * tetro.vertices[i + 1] - 1;

        printf("X: %f, Y: %f\n", tetro.vertices[i + 0], tetro.vertices[i + 1]);
    }
    */
    


    /*
    const char *foo = glfwGetVersionString();
    printf("HELLO: %s\n", foo);
    fflush(stdout);
    */
    

    GLFWwindow *window = glfwCreateWindow(
        800,
        600,
        "Rustris",
        NULL,
        NULL
    );

    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    int initial_width, initial_height;

    glfwGetWindowSize(window, &initial_width, &initial_height);

    App app = {
        .viewport_width = initial_width,
        .viewport_height = initial_height,
        .game = create_game(10, 20),
        .ui_board = {
            .padding_x = 0,
            .padding_y = 20,
            .square_width = 0,
            .square_height = 0,
        },
    };
    update_board_dimensions(&app);

    glfwSetWindowUserPointer(window, &app);
    glfwSetWindowPos(window, 900, 100);
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);

    if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }


    uint32_t vertex_shader = compile_vertex_shader("./shaders/main.vert");
    uint32_t fragment_shader = compile_fragment_shader("./shaders/main.frag");

    uint32_t shaders[] = { vertex_shader, fragment_shader };

    uint32_t shader_program = create_shader_program(
        shaders,
        sizeof(shaders) / sizeof(uint32_t)
    );

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    uint32_t VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // BIND VAO
    glBindVertexArray(VAO);

    UIBoardVertexData ui_board_vertex_data = generate_ui_board_vertex_data(&app);
    DEBUG_PRINT("HI %u", ui_board_vertex_data.elements_size);

    // BIND Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, ui_board_vertex_data.vertex_data_size * sizeof(float), ui_board_vertex_data.vertex_data, GL_STATIC_DRAW);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ui_board_vertex_data.elements_size * sizeof(uint32_t), ui_board_vertex_data.elements_data, GL_STATIC_DRAW);


    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void *) 0
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(
        1,
        1,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void *) (2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind VAO
    glBindVertexArray(0);

    glUseProgram(shader_program);


    // Wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (! glfwWindowShouldClose(window)) {
        // Handle inputs
        process_input(window);


        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        /*
        glDrawArrays(GL_TRIANGLES, 3, 3);
        */
        glDrawElements(GL_TRIANGLES, ui_board_vertex_data.elements_size, GL_UNSIGNED_INT, 0);


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
