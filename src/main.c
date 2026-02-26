#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
    app->game->should_rerender = true;

    DEBUG_PRINTF("WINDOW RESIZE width=%u, height=%u", width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
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

    glfwGetFramebufferSize(window, &initial_width, &initial_height);
    DEBUG_PRINTF("Initial window size width=%u, height=%u", initial_width, initial_height);

    Game game = create_game(10, 20);
    UIBoard ui_board = {0};

    App app = {
        .viewport_width = initial_width,
        .viewport_height = initial_height,
        .game = &game,
        .renderer = NULL,
        .ui_board = &ui_board,
    };

    glfwSetWindowUserPointer(window, &app);
    glfwSetWindowPos(window, 900, 100);
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);

    update_board_dimensions(&app);

    if (! gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    Renderer renderer = create_renderer(&game);
    app.renderer = &renderer;

    // Wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (! glfwWindowShouldClose(window)) {
        // Handle inputs
        process_input(window);
        glfwPollEvents();

        calculateFPS(&app);

        if (game.state == GAME_PLAYING) {
            handle_tetromino_horizontal_movement(window, &game);
            handle_tetromino_vertical_movement(window, &game);
            handle_tetromino_rotation(window, &game);
        }

        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (game.should_rerender) {
            generate_ui_board_vertex_data(&renderer, &game, &ui_board);
            generate_pieces_vertex_data(&renderer, &game, &ui_board);

            game.should_rerender = false;
        }
        
        glUseProgram(renderer.shader_program);
        glUniform2f(renderer.screen_size_loc, (float) app.viewport_width, (float) app.viewport_height);

        draw_ui_board(&renderer);
        draw_pieces(&renderer);

        /*
        glBindVertexArray(board_vao);
        glDrawElements(GL_TRIANGLES, ui_board_vertex_data.elements_count, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(pieces_vao);
        glDrawElements(GL_TRIANGLES, pieces_vertex_data.elements_count, GL_UNSIGNED_INT, 0);
        */

        glBindVertexArray(0);

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // TODO: Cleanup renderer
    /*
    glDeleteVertexArrays(1, &board_vao);
    glDeleteBuffers(1, &board_vbo);
    glDeleteBuffers(1, &board_ebo);

    glDeleteProgram(renderer.shader_program);
    glfwTerminate();
    */

    return 0;
}
