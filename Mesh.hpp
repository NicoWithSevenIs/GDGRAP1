#pragma once

#include "ShaderManager.hpp"


enum Transformation {
	Translate,
	Scale,
	Rotate,
};

class Mesh{

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