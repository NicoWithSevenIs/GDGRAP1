#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include "DrawPentagon.h"

#define TINYOBJLOADER_IMPLEMENTATION 
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <string>

float z_mod = 0;
glm::mat4 identity = glm::mat4(1.0f);
float x = 0.f, y = 0.f, z = -5.0f;
float scale_x = 1, scale_y =1, scale_z = 1;
float theta = 90;
float axis_x = 0, axis_y = 1, axis_z = 0;
float zoom = 60.f;
float x_mod = 0.f;

float cameraSpeed = 2.f; // adjust accordingly

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE) {
        return;
    }

   
    switch (key) {

    case GLFW_KEY_W:
        cameraPos += cameraSpeed * cameraFront;
        break;
    case GLFW_KEY_A:
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        break;
    case GLFW_KEY_S:
        cameraPos -= cameraSpeed * cameraFront;
        break;
    case GLFW_KEY_D:
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        break;

    case GLFW_KEY_F: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); break;
    case GLFW_KEY_Q: scale_x += 0.3f; scale_y += 0.3f; scale_z += 0.3f;  break;
    case GLFW_KEY_E: scale_x -= 0.3f; scale_y -= 0.3f; scale_z -= 0.3f;  break;

    case GLFW_KEY_LEFT: theta -= 15.f; break;
    case GLFW_KEY_RIGHT: theta += 15.f; break;

    case GLFW_KEY_Z: zoom -= 10.f; break;
    case GLFW_KEY_X: zoom += 10.f; break;
    }
}

bool firstMouse = true;
float lastX = 500, lastY = 300;
float yaw = -90, pitch = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{


    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
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

    //
    int img_width;
    int img_height;
    int colorChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes = stbi_load("3D/partenza.jpg",
        &img_width,
        &img_height,
        &colorChannels,
        0);

    ///


    glViewport(0, 0, window_width, window_height);

    glfwSetKeyCallback(window, Key_Callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

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

    std::string path = "3D/djSword.obj";
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


    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };

    std::vector<GLfloat> fullVertexData;
    //get the EBO indices array
    //std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        
        tinyobj::index_t vData = shapes[0].mesh.indices[i];
        /*mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );*/

        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)+1]);
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);

        fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]);
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);

        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);
    }

    GLuint VAO, VBO;
    //, EBO, VBO_UV;
    glGenVertexArrays(1, &VAO); // line responsible for VAO
    glGenBuffers(1, &VBO); // line responsible for VBO
    //glGenBuffers(1, &EBO);

    //glGenBuffers(1, &VBO_UV);



    glBindVertexArray(VAO); // assigns VAO currently being edited
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // assigns VBO currently being edited and attaches VBO to VAO
    
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        GL_DYNAMIC_DRAW
    );


    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*) 0
    );


    glEnableVertexAttribArray(0);

    GLintptr normalptr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*) normalptr
    );
    glEnableVertexAttribArray(1);


    GLintptr uvptr = 6 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)uvptr
    );
 
    glEnableVertexAttribArray(2);

    
    /*
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
    */
    //glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
    /*glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * sizeof(UV) / sizeof(UV[0]),
        &UV[0],
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(GL_FLOAT),
        (void*)0
    );

    glEnableVertexAttribArray(2);



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * fullVertexData.size(),
        fullVertexData.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);

    */

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width,
        img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        tex_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */

    //glm::vec3 worldUP = glm::vec3(0, 1.f, 0);
    //glm::vec3 center = glm::vec3(0, 5.f, 0);
    
 
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        
        //glm::vec3 camera(x_mod, 0, 10.f);
      
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
        
        //glm::mat4 viewMatrix = glm::lookAt(camera, center, worldUP);
        




       // z = z_mod;
        glm::mat4 transformation_matrix = glm::translate(
            identity,
            glm::vec3(x, y, z)
        );

        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        theta += 0.025;
        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
        );

        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(zoom), window_height / window_width, 0.1f, 100.f);

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));




        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));
       
        //glUniform1f(xLoc, x_mod);

        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

       // glUniform1f(yLoc, y_mod);

        /*put rendering stuff here*/
        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size()/8);
        //glDrawElements(GL_TRIANGLES, fullVertexData.size(), GL_UNSIGNED_INT, 0);
        //DrawPentagon::draw(0.5f)
        /* Swap front and back buffers */
        glfwSwapBuffers(window);


        /* Poll for and process events */
        glfwPollEvents();
    }

    /*Cleanup here*/
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteBuffers(1, &VBO_UV);
    glfwTerminate();
    return 0;
}