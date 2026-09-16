#pragma once
#include "maths.h"


class Camera {
private:
	Maths::Mat4 m_view;
	float m_fov;
	float m_aspectRatio;
	float m_near;
	float m_far;
public:
	Camera(float fov = 1.570796f, float aspectRatio = 1.7777778f, float near = 1.0f, float far = 500.0f); //fov is in radians

	void move(Maths::Vec3f vec);
	void rotate(float angle, Maths::Vec3f vec); // angle is in radians

	const Maths::Mat4& getView();
	float getFov();
	float getAspectRatio();
	float getNear();
	float getFar();
	Maths::Vec3f getPosition();
};