#pragma once



#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

//#define TINYOBJLOADER_IMPLEMENTATION
//#include "tiny_obj_loader.h"


class Mesh{

	private:

		//glm::mat4 identity = glm::mat4(1.0f);
		float x = 0.7, y = 0, z = 0;
		float scale_x = 1, scale_y = 1, scale_z = 1;
		float theta = 90;
		float axis_x = 0, axis_y = 0, axis_z = 1;

	public:

		Mesh();



};