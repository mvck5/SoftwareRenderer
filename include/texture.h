#pragma once
#include <vector>
#include "maths.h"

//0x00   00   00     00
//alpha, red, green, blue

class Texture {
private:
	int m_height;
	int m_width;
	std::vector<std::uint32_t> m_data;
public:
	Texture(const char* filename);

	void load(const char* filename);
	std::uint32_t getTex(Maths::Vec2f point) const;

	int getHeight() const;
	int getWidth() const;
	const std::vector<std::uint32_t>& getData() const;
};