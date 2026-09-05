#pragma once
#include <vector>
#include "colour.h"
#include "maths.h"
#include "object.h"
#include "camera.h"


class Rasteriser { //buffers are vectors in case we want to resize the window
private:
    int m_width;
    int m_height;
    int m_pitch;
    std::vector<std::uint32_t> m_frameBuffer; //framebuffer
    std::vector<float> m_zBuffer; //z buffer, holds the nearest z value for each pixel
    Camera m_camera;
public:
    Rasteriser(int width, int height, Camera camera = Camera());

    void drawPixel(Maths::RasterPoint p, std::uint32_t colour);
    void drawLine(Maths::Vec3f p1, Maths::Vec3f p2, std::uint32_t colour = Colours::white);
    void drawTriangle(Maths::Vec3f p1, Maths::Vec3f p2, Maths::Vec3f p3, bool wireframe = false, std::uint32_t colour = Colours::white);
    void drawObject(Object& object, bool wireframe = false);

    std::vector<std::uint32_t>& getFrameBuffer();
    std::vector<float>& getZBuffer();
    Camera& getCamera();
    int getWidth();
    int getHeight();
    int getPitch();
};