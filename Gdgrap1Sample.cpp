#define TINYOBJLOADER_IMPLEMENTATION

#include "Mesh.hpp"
#include "ShaderManager.hpp"

int main(void)
{

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(600, 600, "Nico Tolentino", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*Put code below here*/
    gladLoadGL();

    ShaderManager shader = ShaderManager();
    shader.LoadShader("Shaders/shaders.vert", GL_VERTEX_SHADER);
    shader.LoadShader("Shaders/shaders.frag", GL_FRAGMENT_SHADER);
    glLinkProgram(shader.getShaderProg());


    Mesh obj("3D/bunny.obj");
    
 
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        obj.Draw(shader);

        glUseProgram(shader.getShaderProg());

        glBindVertexArray(obj.getObj()["VAO"]);
        glDrawElements(GL_TRIANGLES, obj.getMeshIndices().size(), GL_UNSIGNED_INT, 0);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);


        /* Poll for and process events */
        glfwPollEvents();
    }

    /*Cleanup here*/

   
    glfwTerminate();
    return 0;
}