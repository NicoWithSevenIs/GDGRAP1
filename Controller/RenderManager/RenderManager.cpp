#include "RenderManager.hpp"

using namespace controllers;

RenderManager::RenderManager(std::string window_name, float window_width, float window_height) {
	this->window_name = window_name;
	this->window_width = window_width;
	this->window_height = window_height;

	this->window = glfwCreateWindow(this->window_width, this->window_height, this->window_name.c_str(), NULL, NULL);

	if (this->window) {
		glfwMakeContextCurrent(window);
		gladLoadGL();
	}else glfwTerminate();
	
}

	

void RenderManager::initialize() {
	this->Editors.push_back(TransformHandler("Bunny"));
	
	this->Editors[0] += new Model3D("Bunny", "3D/Bunny.obj");
	
	this->shader.LoadShader("Shaders/shaders.vert", GL_VERTEX_SHADER);
	this->shader.LoadShader("Shaders/shaders.frag", GL_FRAGMENT_SHADER);
}

void RenderManager::run() {
	if (!this->window) {
		return;
	}

	this->initialize();

	while (!glfwWindowShouldClose(this->window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto i : this->Editors) {

			unsigned int transformLoc = glGetUniformLocation(shader.getShaderProg(), "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(i.getTransformMatrix()));
			glUseProgram(shader.getShaderProg());
			glBindVertexArray(Mesh.getBuffer()["VAO"]);
			glDrawElements(GL_TRIANGLES, obj.getMeshIndices().size(), GL_UNSIGNED_INT, 0);

		}

	
		

		


		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);


		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

}

TransformHandler* RenderManager::findTransformHandlerByName(std::string name) {
	for (auto i : this->Editors) {
		if (name == i.getName())
			return &i;
	}
	return NULL;
}