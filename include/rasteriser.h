#pragma once
#include <vector>
#include "colour.h"

class Rasteriser {
private:
    int m_width;
    int m_height;
    int m_pitch;
    std::vector<std::uint32_t> m_pixels;
public:
    Rasteriser(int width, int height);

    void drawPixel(int x, int y, std::uint32_t colour);

    std::vector<std::uint32_t>& getBuffer();
    int getWidth();
    int getHeight();
    int getPitch();
};