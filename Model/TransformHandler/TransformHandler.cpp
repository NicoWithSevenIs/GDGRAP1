#include "TransformHandler.hpp"

using namespace models;

void TransformHandler::operator += (Model3D* to_add) {
	this->to_transform[to_add->getName()] = to_add;
}

void TransformHandler::operator -= (Model3D* to_remove) {
	this->to_transform.erase(to_remove->getName());
}


TransformHandler::TransformHandler(std::string name) {
	this->name = name;
	this->resetTransformations();
}


void TransformHandler::applyTransformation() {

	this->transformation_matrix = glm::translate(
		this->transformation_matrix,
		this->transformations[Transform::Translate]
	);

	this->transformation_matrix = glm::scale(
		this->transformation_matrix,
		this->transformations[Transform::Scale]
	);

	this->transformation_matrix = glm::rotate(
		this->transformation_matrix,
		glm::radians(this->theta),
		glm::normalize(this->transformations[Transform::Rotate])
	);

}

glm::mat4 TransformHandler::getTransformMatrix() {
	this->applyTransformation();
	return this->transformation_matrix;
}

std::unordered_map<std::string, Model3D*> TransformHandler::getModelsToTransform() {
	return this->to_transform;
}

void TransformHandler::setTransformation(Transform type, glm::vec3 vector) {
	this->transformations[type] = vector;
}

std::string TransformHandler::getName() { return this->name; }

void TransformHandler::resetTransformations() {
	this->transformation_matrix = glm::mat4(1.f);
	this->transformations[Translate] = glm::vec3(0, 0, 0);
	this->transformations[Scale] = glm::vec3(1, 1, 1);
	this->transformations[Rotate] = glm::vec3(0, 1, 0);
	this->theta = 0.0f;
}

