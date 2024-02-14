#pragma once

#include "../../Model/lib.hpp"
#include "../../Model/Model3D/Model3D.hpp"
#include "../../Model/ShaderManager/ShaderManager.hpp"
#include "../../Model/TransformHandler/TransformHandler.hpp"

namespace controllers {
	using namespace models;
	class RenderManager {

		private:
			GLFWwindow* window;
			float window_width;
			float window_height;
			std::string window_name;

		private:
			ShaderManager shader;

			//For models that use the same transforms
			std::vector<TransformHandler> Editors;

		public:
			RenderManager(std::string window_name, float window_width, float window_height);
		
		private:
			void initialize();
			TransformHandler* findTransformHandlerByName(std::string name);

		public:
			void run();
		
	};
};