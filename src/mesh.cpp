#include "mesh.h"
#include <iostream>
#include <fstream>
#include <filesystem>

Mesh::Mesh(const char* filename) {
	load(filename);
}

void Mesh::load(const char* filename) {

	std::filesystem::path path = std::filesystem::path(RESOURCES_PATH) / filename;

	std::ifstream file(path);

	std::string text;

	Maths::Vec3f tempPosition{};
	Maths::Vec2f tempTexCoord{};
	std::array<std::array<int, 3>, 3> tempFace{};
	int temp{};
	int location{};
	int location2{};


	while (std::getline(file, text)) {
		if (text[0] == 'v') {
			if (text[1] == 'n') {
				temp = 3;
				location = 0;
				for (int i{ 3 }; i < text.size(); i++) {
					if (text[i] == ' ') {
						tempPosition(location) = std::stof(text.substr(temp,i-temp));
						temp = i+1;
						location++;
					}
				}
				tempPosition(location) = std::stof(text.substr(temp));

				m_normals.push_back(tempPosition);
			}
			else if (text[1] == 't') {
				temp = 3;
				location = 0;
				for (int i{ 3 }; i < text.size(); i++) {
					if (text[i] == ' ') {
						tempTexCoord(location) = std::stof(text.substr(temp, i - temp));
						temp = i + 1;
						location++;
					}
				}

				tempTexCoord(location) = std::stof(text.substr(temp));

				m_texCoords.push_back(tempTexCoord);
			}
			else {
				temp = 2;
				location = 0;
				for (int i{ 2 }; i < text.size(); i++) {
					if (text[i] == ' ') {
						tempPosition(location) = std::stof(text.substr(temp, i - temp));
						temp = i+1;
						location++;
					}
				}
				tempPosition(location) = std::stof(text.substr(temp));

				m_positions.push_back(tempPosition);
			}
		}
		else if (text[0] == 'f') {
			temp = 2;
			location = 0;
			location2 = 0;
			for (int i{ 2 }; i < text.size(); i++) {
				if (text[i] == '/') {
					tempFace[location][location2] = std::stoi(text.substr(temp,i - temp));
					temp = i + 1;
					location2++;
				}
				else if (text[i] == ' ') {
					tempFace[location][location2] = std::stoi(text.substr(temp, i - temp));
					temp = i + 1;
					location2 = 0;
					location++;
				}
			
			}
			tempFace[location][location2] = std::stoi(text.substr(temp));

			m_faces.push_back(tempFace);
		}

	}
}

const std::vector<std::array<std::array<int, 3>, 3>>& Mesh::getFaces() const {
	return m_faces;
}

const std::vector<Maths::Vec3f>& Mesh::getPositions() const {
	return m_positions;
}

const std::vector<Maths::Vec3f>& Mesh::getNormals() const {
	return m_normals;
}

const std::vector<Maths::Vec2f>& Mesh::getTexCoords() const {
	return m_texCoords;
}