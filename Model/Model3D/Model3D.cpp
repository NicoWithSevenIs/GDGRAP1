#include "Model3D.hpp"

using namespace models;

Model3D::Model3D(std::string name, std::string filename) {
	this->name = name;
	this->LoadMesh(filename);
}

Model3D::~Model3D() {
	for (auto i : buffers) {
		glDeleteVertexArrays(1, &i.second);
	}
}

void Model3D::LoadMesh(std::string filename)
{

	std::string warning = "";
	std::string error = "";

	bool success = tinyobj::LoadObj(
		&attributes,
		&shapes,
		&material,
		&warning,
		&error,
		filename.c_str()
	);

	if (!warning.empty())
		std::cout << "Warning: " << warning << std::endl;

	if (!error.empty()) {
		std::cout << "Error: " << warning << std::endl;
		return;
	}

	for (auto i : shapes[0].mesh.indices) {
		this->mesh_indices.push_back(i.vertex_index);
	}

	this->BindBuffers();
}

void Model3D::BindBuffers()
{

	glBindVertexArray(this->buffers["VAO"]); // assigns VAO currently being edited
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers["VBO"]); // assigns VBO currently being edited and attaches VBO to VAO

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(GL_FLOAT) * attributes.vertices.size(), //size in bytes
		attributes.vertices.data(), //array
		GL_STATIC_DRAW
	); // GL_STATIC_DRAW as the model doesn't move

	glVertexAttribPointer(
		0,
		3, //x,y,z
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float),
		(void*)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers["EBO"]);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLuint) * this->mesh_indices.size(),
		this->mesh_indices.data(),
		GL_STATIC_DRAW
	);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


std::string Model3D::getName() { return this->name; }
void Model3D::setName(std::string name) { this->name = name; }
std::vector<GLuint> Model3D::getMeshIndices() { return this->mesh_indices; }
std::unordered_map<std::string, GLuint> Model3D::getBuffers() { return this->buffers; }