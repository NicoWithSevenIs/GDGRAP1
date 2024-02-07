#include "ShaderManager.hpp"

ShaderManager::ShaderManager():shaderProg(glCreateProgram()) {}

void ShaderManager::LoadShader(std::string path, unsigned int shader_type) {

    std::fstream shaderSrc(path);

    std::stringstream shaderBuff;
    shaderBuff << shaderSrc.rdbuf();

    std::string shaderS = shaderBuff.str();
    const char* s = shaderS.c_str();

    //Initializing Shaders
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &s, NULL);
    glCompileShader(shader);

    glAttachShader(this->shaderProg, shader);

}

GLuint& ShaderManager::getShaderProg() { return this->shaderProg; }
