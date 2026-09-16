#include "camera.h"


Camera::Camera(float fov, float aspectRatio, float near, float far) :
	m_view{ Maths::Mat4{} },
	m_fov{ fov },
	m_aspectRatio{ aspectRatio },
	m_near{ near },
	m_far{ far }{
}

void Camera::move(Maths::Vec3f vec) {
	m_view.translateLocal(vec);
}

void Camera::rotate(float angle, Maths::Vec3f vec) {
	m_view.rotate(angle, vec);
}

const Maths::Mat4& Camera::getView() {
	return m_view;
}

float Camera::getFov() {
	return m_fov;
}

float Camera::getAspectRatio() {
	return m_aspectRatio;
}

float Camera::getNear() {
	return m_near;
}

float Camera::getFar() {
	return m_far;
}

Maths::Vec3f Camera::getPosition() {
	return Maths::Vec3f{m_view(0,3),m_view(1,3),m_view(2,3) };
}