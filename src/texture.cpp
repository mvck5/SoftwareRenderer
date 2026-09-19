#include "texture.h"
#include <iostream>
#include <filesystem>
#include "stb_image.h"

Texture::Texture(const char* filename) {
	load(filename);
}

void Texture::load(const char* filename) {

	std::filesystem::path path = std::filesystem::path(RESOURCES_PATH) / filename;

	int nrChannels;
	unsigned char* data = stbi_load(path.generic_string().c_str(), &m_width, &m_height, &nrChannels, 0);

	m_data.resize(m_width * m_height);

	//first pixel points to the top left so may need to change ordering

	//N = #comp     components
	//       1           grey
	//       2           grey, alpha
	//       3           red, green, blue
	//       4           red, green, blue, alpha

	//we want format:    alpha, red, green, blue

	std::uint32_t x{};

	if (nrChannels == 3) {
		for (int i{ 0 }; i < m_width * m_height; i++) {
			x = 0;

			x |= (std::uint32_t)255               << 24; // 255 as we want a solid alpha value
			x |= (std::uint32_t)data[(i * 3)    ] << 16;
			x |= (std::uint32_t)data[(i * 3) + 1] << 8;
			x |= (std::uint32_t)data[(i * 3) + 2];

			m_data[i] = x;
		}
	}
	else if (nrChannels == 4) {
		for (int i{ 0 }; i < m_width * m_height; i++) {
			x = 0;

			x |= (std::uint32_t)data[(i * 3) + 3] << 24;
			x |= (std::uint32_t)data[(i * 3)    ] << 16;
			x |= (std::uint32_t)data[(i * 3) + 1] << 8;
			x |= (std::uint32_t)data[(i * 3) + 2];

			m_data[i] = x;
		}
	}

	stbi_image_free(data);
}

std::uint32_t Texture::getTex(Maths::Vec2f point) const {

	int x = static_cast<int>(point(0) * (m_width - 1));
	int y = static_cast<int>(point(1) * (m_height - 1));

	return m_data[y * m_width + x];
}

int Texture::getHeight() const {
	return m_height;
}

int Texture::getWidth() const {
	return m_width;
}

const std::vector<std::uint32_t>& Texture::getData() const {
	return m_data;
}