#pragma once

#include "../lib.hpp"
#include "../Enum/Transform.hpp"
#include "../Model3D/Model3D.hpp"

namespace models {

	class TransformHandler {

		private:
			std::string name;

		private:
			float theta;
			std::unordered_map<Transform, glm::vec3> transformations;
			std::unordered_map<std::string, Model3D*> to_transform;

		private:
			glm::mat4 transformation_matrix;

		public:
			TransformHandler(std::string name);
			void operator += (Model3D*);
			void operator -= (Model3D*);
	
		private:
			void applyTransformation();

		public:
			std::string getName();

			glm::mat4 getTransformMatrix();
			std::unordered_map<std::string, Model3D*> getModelsToTransform();

			void setTransformation(Transform type, glm::vec3 vector);
			void resetTransformations();

				
	};
};