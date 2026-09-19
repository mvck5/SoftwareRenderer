#include "object.h"
#include <iostream>

Object::Object(const Mesh& mesh, const Texture& texture):
	m_mesh{ mesh }, m_model{ Maths::Mat4{} }, m_texture{ texture } {
}

void Object::move(Maths::Vec3f vec) {
	m_model.translateWorld(vec);
}

void Object::rotate(float angle, Maths::Vec3f vec) {
	m_model.rotate(angle, vec);
}

void Object::scale(Maths::Vec3f vec) {
	m_model.scale(vec);
}

const Mesh& Object::getMesh() {
	return m_mesh;
}

const Maths::Mat4& Object::getModel() {
	return m_model;
}

const Texture& Object::getTexture() {
	return m_texture;
}