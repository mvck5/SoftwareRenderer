#include "rasteriser.h"


Rasteriser::Rasteriser(int width, int height) :
    m_width{ width }, m_height{ height }, m_pitch{ width * static_cast<int>(sizeof(std::uint32_t)) }, m_pixels{ std::vector<std::uint32_t>(width * height) } {
}

void Rasteriser::drawPixel(int x, int y, std::uint32_t colour) {
    if (x < 0 || x >= m_width ||
        y < 0 || y >= m_height)
    {
        return;
    }

    m_pixels[y * m_width + x] = colour;
}

std::vector<std::uint32_t>& Rasteriser::getBuffer() {
    return m_pixels;
}

int Rasteriser::getWidth() {
    return m_width;
}
int Rasteriser::getHeight() {
    return m_height;
}

int Rasteriser::getPitch() {
    return m_pitch;
}