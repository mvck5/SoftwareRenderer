#pragma once
#include <vector>
#include <array>
#include "maths.h"
#include "mesh.h"

//vertices and draw order are references so that we only have to have the memory allocated for an object once regardless of it's instances
//refer to instancing
class Object { 
private:
	const Mesh& m_mesh;
	Maths::Mat4 m_model;
public:
	Object(const Mesh& mesh);

	void move(Maths::Vec3f vec);
	void rotate(float angle, Maths::Vec3f vec); // angle is in radians
	void scale(Maths::Vec3f vec);

	const Mesh& getMesh();
	const Maths::Mat4& getModel();
};