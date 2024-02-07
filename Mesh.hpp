#pragma once


#include <unordered_map>
#include <vector>

#include "ShaderManager.hpp"


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

enum Transformation {
	Translate,
	Scale,
	Rotate,
};

class Mesh{

	private:

	private:

		glm::vec3 translation;
		glm::vec3 scale;
		glm::vec3 rotation_axis;

		std::unordered_map<std::string, GLuint> obj;
		std::vector<GLuint> mesh_indices;

		float theta;

	public:

		Mesh(std::string filename);
		~Mesh();

	public:
		void Draw(ShaderManager& shader);

	public:
		std::vector<GLuint> getMeshIndices();
		std::unordered_map<std::string, GLuint> getObj();
		void setTransformation(Transformation type, glm::vec3 vector);		
		void setTheta(float theta);

};