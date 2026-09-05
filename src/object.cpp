#include "object.h"
#include <iostream>

Object::Object(const std::vector<Maths::Vec3f>& vertices, const std::vector<std::array<int, 3>>& drawOrder):
	m_vertices{ vertices }, m_drawOrder{ drawOrder }, m_model{ Maths::Mat4{} } {
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


const std::vector<std::array<int, 3>>& Object::getOrder() {
	return m_drawOrder;
}

Maths::Vec3f Object::getVertex(int index) {
	return m_vertices[index];
}

const Maths::Mat4& Object::getModel() {
	return m_model;
}