#include "Mesh.hpp"

Mesh::Mesh(std::string filename){

	this->translation = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
	this->rotation_axis = glm::vec3(0, 1, 0);
	this->theta = 0.0f;

	glGenVertexArrays(1, &this->obj["VAO"]); // line responsible for VAO
	glGenBuffers(1, &this->obj["VBO"]); // line responsible for VBO
	glGenBuffers(1, &this->obj["EBO"]);
	

	/******************/

	//Readingfor Mesh
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> material;
	std::string warning, error;

	tinyobj::attrib_t attributes;

	bool success = tinyobj::LoadObj(
		&attributes,
		&shapes,
		&material,
		&warning,
		&error,
		filename.c_str()
	);

	/******************/


	for (auto i : shapes[0].mesh.indices) {
		this->mesh_indices.push_back(i.vertex_index);
	}

	//Binding
	
	glBindVertexArray(this->obj["VAO"]); // assigns VAO currently being edited
	glBindBuffer(GL_ARRAY_BUFFER, this->obj["VBO"]); // assigns VBO currently being edited and attaches VBO to VAO

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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->obj["EBO"]);

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

Mesh::~Mesh() {
	for (auto i : obj) {
		glDeleteVertexArrays(1, &i.second);
	}
}

void Mesh::Draw(ShaderManager& shader) {

	glm::mat4 identity = glm::mat4(1.0f);

	glm::mat4 transformation_matrix = glm::translate(
		identity,
		this->translation
	);

	transformation_matrix = glm::scale(
		transformation_matrix,
		this->scale
	);

	transformation_matrix = glm::rotate(
		transformation_matrix,
		glm::radians(theta),
		glm::normalize(this->rotation_axis)
	);
	
	unsigned int transformLoc = glGetUniformLocation(shader.getShaderProg(), "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

	glUseProgram(shader.getShaderProg());

}

void Mesh::setTransformation(Transformation type, glm::vec3 vector) {
	switch (type) {
		case Transformation::Translate: 
			this->translation = vector;
			break;
		case Transformation::Scale: 
			this->scale = vector;
			break;
		case Transformation::Rotate: 
			this->rotation_axis = vector;
			break;
	}
}

void Mesh::setTheta(float theta) {
	this->theta = theta;
}

std::vector<GLuint> Mesh::getMeshIndices() { return this->mesh_indices; }

std::unordered_map<std::string, GLuint> Mesh::getObj() { return this->obj; }