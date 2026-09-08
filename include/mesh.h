#pragma once
#include "maths.h"
#include <string>
#include <array>

class Mesh{
private:
	std::vector<Maths::Vec3f> m_positions;
	std::vector<Maths::Vec3f> m_normals;
	std::vector<Maths::Vec2f> m_texCoords;
	std::vector<std::array<std::array<int,3>,3>> m_faces;
	//should also have texture data of some sort
public:
	Mesh(const char* filename); //ideally should take in all data needed

	void load(const char* filename); //should load in data from an obj file

	const std::vector<std::array<std::array<int, 3>, 3>>& getFaces() const;
	const std::vector<Maths::Vec3f>& getPositions() const;
	const std::vector<Maths::Vec3f>& getNormals() const;
	const std::vector<Maths::Vec2f>& getTexCoords() const;
};