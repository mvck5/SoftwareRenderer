#pragma once
#include <vector>
#include "colour.h"

class Renderer {
private:
    int m_width;
    int m_height;
    int m_pitch;
    std::vector<std::uint32_t> m_pixels;
public:
    Renderer(int width, int height);
    void putPixel(int x, int y, std::uint32_t colour);
    std::vector<std::uint32_t>& getBuffer();
    int getWidth();
    int getHeight();
    int getPitch();
};