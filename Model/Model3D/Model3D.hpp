#pragma once

#include "../lib.hpp"

namespace models {
	class Model3D {
		
		private:
			std::string name;
		private:
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> material;
			tinyobj::attrib_t attributes;

		private:
			std::unordered_map<std::string, GLuint> buffers;
			std::vector<GLuint> mesh_indices;

		public:
			Model3D(std::string name, std::string filename);
			~Model3D();

		private:
			void LoadMesh(std::string filename);
			void BindBuffers();

		public:
			std::string getName();
			void setName(std::string name);

			std::vector<GLuint> getMeshIndices();
			std::unordered_map<std::string, GLuint> getBuffers();



	};
};