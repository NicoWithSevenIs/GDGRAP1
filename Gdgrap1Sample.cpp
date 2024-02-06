#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

//#define TINYOBJLOADER_IMPLEMENTATION 
//#include "tiny_obj_loader.h"

#include <iostream>
#include <string>

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
        /*
        for (int i = 1; i <= 3; i++) {

            glm::mat4 transformation_matrix = glm::rotate(
                identity,
                glm::radians(theta +  120 * i),
                glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
            );

            transformation_matrix = glm::scale(
                transformation_matrix,
                glm::vec3(scale_x, scale_y, scale_z)
            );

            theta += 0.025;

            transformation_matrix = glm::translate(
                transformation_matrix,
                glm::vec3(x, y, z)
            );

            unsigned int transformLoc = glGetUniformLocation(shader.getShaderProg(), "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

           
        }
        */

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