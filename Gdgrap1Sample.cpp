#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include "DrawPentagon.h"

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

#include <iostream>
#include <string>

float z_mod = 0;
glm::mat4 identity = glm::mat4(1.0f);
float x = 0, y = 0, z = 0;
float scale_x = 5, scale_y = 5, scale_z = 1;
float theta = 90;
float axis_x = 0, axis_y = 1, axis_z = 0;


void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (!(action == GLFW_PRESS))
        return;

    switch (key) {
        case GLFW_KEY_A: theta--; break;
        case GLFW_KEY_D: theta++; break;
        case GLFW_KEY_W: scale_x += 0.1f; scale_y += 0.1f;  break;
        case GLFW_KEY_S: scale_x -= 0.1f; scale_y -= 0.1f;  break;
        case GLFW_KEY_Q: z_mod -= 0.3f;  break;
        case GLFW_KEY_E: z_mod += 0.3f;  scale_y -= 0.1f;  break;
    }
}


int main(void)
{

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float window_width = 720.f;
    float window_height = 720.f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_width, window_height, "Nico Tolentino", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*Put code below here*/
    
    gladLoadGL();

   // glViewport(0, 0, window_width, window_height);

    glfwSetKeyCallback(window, Key_Callback);

    std::fstream vertSrc("Shaders/shaders.vert");

    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();
    
    std::fstream fragSrc("Shaders/shaders.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);

    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    //get the EBO indices array
    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); // line responsible for VAO
    glGenBuffers(1, &VBO); // line responsible for VBO
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO); // assigns VAO currently being edited
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // assigns VBO currently being edited and attaches VBO to VAO
    
    glBufferData(GL_ARRAY_BUFFER, 
        sizeof(GL_FLOAT) * attributes.vertices.size(), //size in bytes
        attributes.vertices.data(), //array
        GL_STATIC_DRAW); // GL_STATIC_DRAW as the model doesn't move

    glVertexAttribPointer(
        0,
        3, //x,y,z
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //unsigned int xLoc = glGetUniformLocation(shaderProg, "x");
    //unsigned int yLoc = glGetUniformLocation(shaderProg, "y");

 
    /*glm::mat4 projectionMatrix = glm::ortho(
        -4.f, //left
        4.f, //right
        -4.f, //bot
        4.f, //top
        -1.f, //znear
        1.f //zfar
    );
    */

    
    /* Loop until the user closes the window */
    /*
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.f),//fov
        window_height/window_width,//aspect ratio
        0.1f, //znear != 0
        100.f //zfar
    );
  */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
       // z = z_mod;
        
        glm::mat4 transformation_matrix = glm::translate(
            identity,
            glm::vec3(x, y, z)
        );

        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        //theta += 0.025;
        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
        );

        //unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        //glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));
       
        //glUniform1f(xLoc, x_mod);

      
       // glUniform1f(yLoc, y_mod);

        /*put rendering stuff here*/
        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        //DrawPentagon::draw(0.5f)
        /* Swap front and back buffers */
        glfwSwapBuffers(window);


        /* Poll for and process events */
        glfwPollEvents();
    }

    /*Cleanup here*/
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}