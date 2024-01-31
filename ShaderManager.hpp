#pragma once

#ifndef DEPENDENCIES
#define DEPENDENCIES

#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>

#endif
class ShaderManager {
	private:
		GLuint shaderProg;
	public:
		ShaderManager();
	public:
		void LoadShader(std::string path, unsigned int shader_type);
		GLuint& getShaderProg();
};