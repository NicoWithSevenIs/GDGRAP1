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
#include <algorithm>

#include <functional>
float z_mod = 0;
glm::mat4 identity = glm::mat4(1.0f);
float x = 0.f, y = 0.f, z = 0.0f;

float newZ = 5.0f;
float scale_x = 0.25f, scale_y = 0.25f, scale_z = 0.25f;

float theta = 90;
float axis_x = 0, axis_y = 1, axis_z = 0;

float fixed_theta = 90;
float axis_x2 = 0, axis_y2 = 0, axis_z2 = -1;

float zoom = 60.f;
float x_mod = 0.f;

float cameraSpeed = 1.f; // adjust accordingly

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

std::vector<glm::vec3> sizes = {
    glm::vec3(1.1f, 1.1f, 1.1f),
    glm::vec3(1.2f, 1.2f, 1.2f),
    glm::vec3(1.2f, 1.2f, 1.2f),
    glm::vec3(1.1f, 1.1f, 1.1f)

};

float lightX = 5.f;
float lightY = 4.f;
float lightZ = 0.f;

float ambientStr = 0.2f;


float cX = 1.0f;
float lX = 1.0f;
float qX = 1.0f;

//void runThrice(std::function<void()>)

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

    case GLFW_KEY_R:
        newZ += 3;
       break;
    case GLFW_KEY_T:
        newZ -= 3;
        break;

    case GLFW_KEY_I:
        lightZ += 1.f;
        std::cout << "{ " << lightX << "," << lightY << "," << lightZ << "," << " }" << std::endl;
        break;
    case GLFW_KEY_J:
        lightX-= 1.f;
        std::cout << "{ " << lightX << "," << lightY << "," << lightZ << "," << " }" << std::endl;
        break;
    case GLFW_KEY_K:
        lightZ -= 1.f;
        std::cout << "{ " << lightX << "," << lightY << "," << lightZ << "," << " }" << std::endl;
        break;
    case GLFW_KEY_L:
        lightX += 1.f;
        std::cout << "{ " << lightX << "," << lightY << "," << lightZ << "," << " }" << std::endl;
        break;

    case GLFW_KEY_U:
        lightY += 1.f;
        std::cout << "{ " << lightX << "," << lightY << "," << lightZ << "," << " }" << std::endl;
        break;
    case GLFW_KEY_O:
        lightY -= 1.f;
        std::cout << "{ " << lightX << "," << lightY << "," << lightZ << "," << " }" << std::endl;
        break;


    case GLFW_KEY_1:
        cX += 0.01f;
        break;
    case GLFW_KEY_2:
        cX = std::min(0.1f, cX - 0.01f);
        break;

    case GLFW_KEY_3:
        lX += 0.01f;
        break;
    case GLFW_KEY_4:
        lX = std::min(0.0001f, lX - 0.01f);
        break;


    case GLFW_KEY_5:
        qX += 0.001f;
        break;
    case GLFW_KEY_6:
        qX = std::min(0.00001f, qX - 0.001f);
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
    #pragma region peepee
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

    unsigned char* tex_bytes = stbi_load("3D/brickwall.jpg",
        &img_width,
        &img_height,
        &colorChannels,
        0);

    ///


    //
    int img_width2;
    int img_height2;
    int colorChannels2;


    stbi_set_flip_vertically_on_load(true);
    unsigned char* normal_bytes = stbi_load("3D/brickwall_normal.jpg",
        &img_width2,
        &img_height2,
        &colorChannels2,
        0);

    ///


   

    glViewport(0, 0, window_width, window_height);

    glfwSetKeyCallback(window, Key_Callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);


    //Skybox*******
    std::fstream skyboxVertSrc("Shaders/skybox.vert");
    std::stringstream skyboxVertBuff;
    skyboxVertBuff << skyboxVertSrc.rdbuf();

    std::string skyboxVertS = skyboxVertBuff.str();
    const char* sv = skyboxVertS.c_str();

    std::fstream skyboxFragSrc("Shaders/skybox.frag");
    std::stringstream skyboxFragBuff;
    skyboxFragBuff << skyboxFragSrc.rdbuf();

    std::string skyboxFragS = skyboxFragBuff.str();
    const char* sf = skyboxFragS.c_str();


    GLuint skyboxVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skyboxVertexShader, 1, &sv, NULL);
    glCompileShader(skyboxVertexShader);

    GLuint skyboxFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skyboxFragShader, 1, &sf, NULL);
    glCompileShader(skyboxFragShader);

    GLuint skyboxShaderProg = glCreateProgram();
    glAttachShader(skyboxShaderProg, skyboxVertexShader);
    glAttachShader(skyboxShaderProg, skyboxFragShader);

    glLinkProgram(skyboxShaderProg);
    glDeleteShader(skyboxVertexShader);
    glDeleteShader(skyboxFragShader);

    //*********************


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





    std::string path = "3D/plane.obj";
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


    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;


    for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];


        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[(vData1.vertex_index * 3) + 1],
            attributes.vertices[(vData1.vertex_index * 3) + 2]
        );

        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[(vData2.vertex_index * 3) + 1],
            attributes.vertices[(vData2.vertex_index * 3) + 2]
        );

        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[(vData3.vertex_index * 3) + 1],
            attributes.vertices[(vData3.vertex_index * 3) + 2]
        );

        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[(vData1.texcoord_index * 2)],
            attributes.texcoords[(vData1.texcoord_index * 2) + 1]
        );

        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[(vData2.texcoord_index * 2)],
            attributes.texcoords[(vData2.texcoord_index * 2) + 1]
        );

        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[(vData3.texcoord_index * 2)],
            attributes.texcoords[(vData3.texcoord_index * 2) + 1]
        );


        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;


        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;


        float r = 1.f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }



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

       
       fullVertexData.push_back(tangents[i].x);
       fullVertexData.push_back(tangents[i].y);
       fullVertexData.push_back(tangents[i].z);

       fullVertexData.push_back(bitangents[i].x);
       fullVertexData.push_back(bitangents[i].y);
       fullVertexData.push_back(bitangents[i].z);
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
        14 * sizeof(float),
        (void*) 0
    );


    glEnableVertexAttribArray(0);

    GLintptr normalptr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*) normalptr
    );
    glEnableVertexAttribArray(1);


    GLintptr uvptr = 6 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)uvptr
    );
 
    glEnableVertexAttribArray(2);

    GLintptr tangentptr = 8 * sizeof(float);
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)tangentptr
    );
    glEnableVertexAttribArray(3);

    GLintptr bitangentptr = 11 * sizeof(float);
    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)bitangentptr
    );
    glEnableVertexAttribArray(4);



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

    GLuint norm_tex;
    glGenTextures(1, &norm_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
    glBlendEquation(GL_SUBTRACT);


    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width2,
        img_height2,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        normal_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(normal_bytes);


    
    #pragma endregion peepee
 
  
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

 
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.25f;
    float specPhong = 16.f;

    float smallSpecStr = 0.1f;
    float bigSpecStr = 6.f;

    /*
              7--------6
             /|       /|
            4--------5 |
            | |      | |
            | 3------|-2
            |/       |/
            0--------1
        */
        //Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };


    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    //Faces of the skybox
    //This is to easily load it later 
    std::string facesSkybox[]{
        "Skybox/rainbow_rt.png", //Right 
        "Skybox/rainbow_lf.png", //Left 
        "Skybox/rainbow_up.png", //Up
        "Skybox/rainbow_dn.png", //Down
        "Skybox/rainbow_ft.png", //Front 
        "Skybox/rainbow_bk.png" //Back
    };

    unsigned int skyboxTex;

    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    

    for (unsigned int i = 0; i < 6; i++) {

        int w, h, skyChannel;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyChannel, 0);

        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                w,
                h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
            stbi_image_free(data);
        }
        else {
            std::cout << "Failed to load texture: " << facesSkybox[i] << std::endl;
        }

    }

    stbi_set_flip_vertically_on_load(true);





    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        #pragma region peepee2

       

        glm::vec3 lightPos = glm::vec3(lightX, lightY, lightZ);

        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(zoom), window_height / window_width, 0.1f, 100.f);


        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

       



        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        glUseProgram(skyboxShaderProg);

        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(viewMatrix));

        unsigned int skyboxViewLoc = glGetUniformLocation(skyboxShaderProg, "view");
        glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

        unsigned int skyboxProjectionLoc = glGetUniformLocation(skyboxShaderProg, "projection");
        glUniformMatrix4fv(skyboxProjectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


        glBindVertexArray(skyboxVAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glUseProgram(shaderProg);
       




        GLuint lightAddress = glGetUniformLocation(shaderProg, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress = glad_glGetUniformLocation(shaderProg, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        GLuint ambientStrAddress = glGetUniformLocation(shaderProg, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        GLuint ambientColorAddress = glGetUniformLocation(shaderProg, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint baseConstantAddress = glGetUniformLocation(shaderProg, "baseConstant");
        glUniform1f(baseConstantAddress, 1.0f);

        GLuint baseLinearAddress = glGetUniformLocation(shaderProg, "baseLinear");
        glUniform1f(baseLinearAddress, 0.027f);

        GLuint baseQuadraticAddress = glGetUniformLocation(shaderProg, "baseQuadratic");
        glUniform1f(baseQuadraticAddress, 0.0028);



        GLuint multConstantAddress = glGetUniformLocation(shaderProg, "multConstant");
        glUniform1f(multConstantAddress, cX);

        GLuint multLinearAddress = glGetUniformLocation(shaderProg, "multLinear");
        glUniform1f(multLinearAddress, lX);

        GLuint multQuadraticAdderess = glGetUniformLocation(shaderProg, "multQuadratic");
        glUniform1f(multQuadraticAdderess, qX);

        
        
        GLuint cameraPosAddress = glGetUniformLocation(shaderProg, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));

        GLuint specPhongAddress = glGetUniformLocation(shaderProg, "specPhong");
        glUniform1f(specPhongAddress, specPhong);




        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        glActiveTexture(GL_TEXTURE1);
        GLuint normLoc = glGetUniformLocation(shaderProg, "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(normLoc, 1);

        for (int i = 0; i < 5; i++) {
            
            

         
            glm::mat4 transformation_matrix = glm::translate(
                identity,
                glm::vec3(x+i*3, y, i%2 ? newZ : z)
            );

            transformation_matrix = glm::scale(
                transformation_matrix,
                sizes[i]
            );

            transformation_matrix = glm::rotate(
                transformation_matrix,
                glm::radians(fixed_theta),
                glm::normalize(glm::vec3(axis_x2, axis_y2, axis_z2))
            );

            theta += 0.01f;
            transformation_matrix = glm::rotate(
                transformation_matrix,
                glm::radians(theta),
                glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
            );


            if (i==4) {
                

               transformation_matrix = glm::translate(
                    identity,
                    lightPos
                );

                transformation_matrix = glm::scale(
                    transformation_matrix,
                    glm::vec3(1.f, 1.f, 1.f)
                );


            }


            

            unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

            specStr = bigSpecStr;

            GLuint specStrAddress = glGetUniformLocation(shaderProg, "specStr");
            glUniform1f(specStrAddress, specStr);

            glUseProgram(shaderProg);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 14);


        }

            
      
 
        //glDrawElements(GL_TRIANGLES, fullVertexData.size(), GL_UNSIGNED_INT, 0);
        //DrawPentagon::draw(0.5f)
        /* Swap front and back buffers */
        glfwSwapBuffers(window);


        /* Poll for and process events */
        glfwPollEvents();
    #pragma endregion peepee2
    }

    /*Cleanup here*/
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteBuffers(1, &VBO_UV);
    glfwTerminate();
    return 0;
}


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


/* Loop until the user closes the window */

    //glm::vec3 worldUP = glm::vec3(0, 1.f, 0);
    //glm::vec3 center = glm::vec3(0, 5.f, 0);

//glm::vec3 camera(x_mod, 0, 10.f);



/*
*
*
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