#pragma once

#include "lib.hpp"


class ShaderManager {
	private:
		GLuint shaderProg;
	public:
		ShaderManager();
	public:
		void LoadShader(std::string path, unsigned int shader_type);
		GLuint& getShaderProg();
};