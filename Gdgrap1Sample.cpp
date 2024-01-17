#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "DrawPentagon.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(720, 720, "Nico Tolentino", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*Put code below here*/
    
    gladLoadGL();

    GLfloat vertices[]{
        // x,    y,    z 
          0.f, 0.5f, 0.f,
          -0.5f, -0.5f, 0.f,
          0.5f,   -0.5f, 0.f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO); // line responsible for VAO
    glGenBuffers(1, &VBO); // line responsible for VBO

    glBindVertexArray(VAO); // assigns VAO currently being edited
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // assigns VBO currently being edited and attaches VBO to VAO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // sizeof(vertices) -> size of array in bytes, GL_STATIC_DRAW as the model doesn't move

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /*put rendering stuff here*/
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //DrawPentagon::draw(0.5f);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);


        /* Poll for and process events */
        glfwPollEvents();
    }

    /*Cleanup here*/
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}