#pragma once
#include <vector>
#include <array>
#include "maths.h"

//vertices and draw order are references so that we only have to have the memory allocated for an object once regardless of it's instances
//refer to instancing
class Object { 
private:
	const std::vector<Maths::Vec3f>& m_vertices;
	const std::vector<std::array<int, 3>>& m_drawOrder;
	Maths::Mat4 m_model;
public:
	Object(const std::vector<Maths::Vec3f>& vertices, const std::vector<std::array<int, 3>>& drawOrder);

	void move(Maths::Vec3f vec);
	void rotate(float angle, Maths::Vec3f vec); // angle is in radians
	void scale(Maths::Vec3f vec);
	
	const std::vector<std::array<int, 3>>& getOrder();
	Maths::Vec3f getVertex(int index);
	const Maths::Mat4& getModel();
};