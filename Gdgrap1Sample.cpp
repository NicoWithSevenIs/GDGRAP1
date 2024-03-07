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
#include <fstream>




int main(void)
{

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    float window_width = 720.f;
    float window_height = 720.f;

    window = glfwCreateWindow(window_width, window_height, "Nico Tolentino", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    gladLoadGL();


    int img_width;
    int img_height;
    int colorChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes = stbi_load("3D/partenza.jpg",
        &img_width,
        &img_height,
        &colorChannels,
        0);


    glViewport(0, 0, window_width, window_height);

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


    std::string path = "3D/quiz.obj";
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



    /*
        Yapping Stuff:

        The first thing I did was try to load the model into the file and was puzzled that it wasnt working.
        I thought hmm maybe yung color channels yung problem, but it wasn't. So I investigated the obj file
        in notepad and noticed that it was preeettty similar to our sword model. I compared both and oml it is the sword
        model but what's up with it? 
        
        I tried to loading the model in different model viewers but it seems that the model was broken-broken, this was the
        question I wanted to ask on discord but I thought hmm maybe this is part of quiz's problem. It is but yeah...

        I got sidetracked

        Anyway, one of the model viewers hinted that the model lacked uv coordinates.
        
        I went back to notepad and noticed that the lines on quiz.obj was less than djSword.obj. I was like hmm kinda sus
        so I compared it side by side and lo and behold, it's just djSword.obj minus the uv coordinates.

        Knowing what to do now, I studied how tinyobjloader.h loaded stuff and it's pretty much just file processing and stuffing
        the coordinates in the right vectors based on the identifiers at the start of each line then building the attribute struct with said vectors.

        I looked into how tinyobjloader.h implements vt processing and used that to build a vector of texcoords that the for loop that iterates
        over fullVertexData can use and substituted the texcoords loading there with the texcoords vector I made by copying the uv coords
        from djSword.obj, pasting them into a separate text file and processing it.  
    
    */

    std::vector<float> texCoords;

    std::fstream texCoordsSrc("3D/swordCoords.txt");
    std::stringstream texCoordsBuff;
    texCoordsBuff << texCoordsSrc.rdbuf();

    std::string texStrBuff;

    //based on tinyobjloader.h's loadObj implementation
    while (getline(texCoordsBuff, texStrBuff, ' ')) {
        //assumes that the file's formatting is correct
        if(texStrBuff == "vt")
            continue;
        texCoords.push_back(std::stof(texStrBuff));
    }

    std::vector<GLfloat> fullVertexData;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3]);
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)+1]);
        fullVertexData.push_back(attributes.vertices[vData.vertex_index * 3 + 2]);

        fullVertexData.push_back(attributes.normals[vData.normal_index * 3]);
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 1]);
        fullVertexData.push_back(attributes.normals[vData.normal_index * 3 + 2]);


        /* Quiz.obj has no texcoords.
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2]);
        fullVertexData.push_back(attributes.texcoords[vData.texcoord_index * 2 + 1]);
        */

        fullVertexData.push_back(texCoords[vData.texcoord_index * 2]);
        fullVertexData.push_back(texCoords[vData.texcoord_index * 2 + 1]);

        

    }


    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);
 
    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    
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

    
    
    //Directional Lighting with direction -1,-1,0
    glm::vec3 lightPos = glm::vec3(-1, -1, 0);
    //Green Color
    glm::vec3 lightColor = glm::vec3(0, 1, 0);

    float ambientStr = 0.2f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.1f;
    float specPhong = 16;


    float theta = 90;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        //FOV = 90
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.f), window_height / window_width, 0.1f, 100.f);

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));


        //Texture
        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);



        //Lighting (Directional)
        GLuint lightAddress = glGetUniformLocation(shaderProg, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress = glad_glGetUniformLocation(shaderProg, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        
       
        GLuint ambientStrAddress = glGetUniformLocation(shaderProg, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        GLuint ambientColorAddress = glGetUniformLocation(shaderProg, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));


        GLuint cameraPosAddress = glGetUniformLocation(shaderProg, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));

        GLuint specStrAddress = glGetUniformLocation(shaderProg, "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(shaderProg, "specPhong");
        glUniform1f(specPhongAddress, specPhong);
        

       
        //Centered 
        glm::mat4 transformation_matrix = glm::translate(
            glm::mat4(1.f),
            glm::vec3(0.f, 0.f, 0.f)
        );

        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(0.08f, 0.08f, 0.08f)
        );

        

        //Constantly rotates around y axis
        theta += 0.025;
        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(0.f, 1.f, 0.f))
        );

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));


        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 8);
        
       

        


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

