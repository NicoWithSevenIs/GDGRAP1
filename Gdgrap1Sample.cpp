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
float x = 0.f, y = 0.f, z = -5.0f;
float scale_x = 5, scale_y = 5, scale_z = 1;
float theta = 90;
float axis_x = 0, axis_y = 1, axis_z = 0;
float zoom = 60.f;
float x_mod = 0.f;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS)
        return;

    switch (key) {
        case GLFW_KEY_W: y += 0.1f;  break;
        case GLFW_KEY_A: x -= 0.1f; break;
        case GLFW_KEY_S: y -= 0.1f;  break;
        case GLFW_KEY_D: x += 0.1f; break;
        
        
        case GLFW_KEY_Q: scale_x += 0.3f; scale_y += 0.3f; scale_z += 0.3f;  break;
        case GLFW_KEY_E: scale_x -= 0.3f; scale_y -= 0.3f; scale_z -= 0.3f;  break;

        case GLFW_KEY_LEFT: theta -= 15.f; break;
        case GLFW_KEY_RIGHT: theta += 15.f; break;

        case GLFW_KEY_Z: zoom -= 10.f; break;
        case GLFW_KEY_X: zoom += 10.f; break;
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

    glViewport(0, 0, window_width, window_height);

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

    glm::vec3 worldUP = glm::vec3(0, 1.f, 0);
    glm::vec3 center = glm::vec3(0, 5.f, 0);
    
 
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        
        glm::vec3 camera(x_mod, 0, 10.f);
      
        /*
        glm::mat4 camPos = glm::translate(glm::mat4(1.0f), camera * -1.0f);
       
        glm::vec3 F = glm::vec3(glm::normalize(center - camera));

        glm::vec3 R = glm::vec3(glm::cross(F, worldUP));

        glm::vec3 U = glm::normalize(glm::cross(R, F));

        //ruf

        std::vector<std::vector<float>> toMat = {
            {R.x, R.y, R.z},
            {U.x, U.y, U.z},
            {F.x, F.y, F.z}
        };
        glm::mat4 cameraOrientation = glm::mat4(1.f);

       
        /*
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cameraOrientation[j][i] = toMat[i][j];
            }
        }  
        
   
        cameraOrientation[0][0] = R.x;
        cameraOrientation[1][0] = R.y;
        cameraOrientation[2][0] = R.z;

        cameraOrientation[0][1] = U.x;
        cameraOrientation[1][1] = U.y;
        cameraOrientation[2][1] = U.z;

        cameraOrientation[0][2] = -F.x;
        cameraOrientation[1][2] = -F.y;
        cameraOrientation[2][2] = -F.z;

        glm::mat4 viewMatrix = cameraOrientation * camPos;
        
        */
        
        glm::mat4 viewMatrix = glm::lookAt(camera, center, worldUP);
        




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

        glm::mat4 projectionMatrix = glm::perspective(
            glm::radians(zoom),//fov
            window_height / window_width,//aspect ratio
            0.1f, //znear != 0
            100.f //zfar
        );

       
        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

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