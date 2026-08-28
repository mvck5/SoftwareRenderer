#include <array>
#include <algorithm>
#include "rasteriser.h"
#include "cmath"

#include <iostream>

Rasteriser::Rasteriser(int width, int height, Camera camera) :
    m_width{ width }, m_height{ height }, m_pitch{ width * static_cast<int>(sizeof(std::uint32_t)) },
    m_frameBuffer{ std::vector<std::uint32_t>(width * height) }, m_zBuffer{ std::vector<float>(width * height) }, m_camera{ camera } {
}

void Rasteriser::drawPixel(Maths::RasterPoint p, std::uint32_t colour) {

    p.y = m_height - 1 - p.y; //this is to set y to be at the bottom left instead of the top left

    if (p.x >= 0 && p.x <= m_width && p.y >= 0 && p.y <= m_height) { //check if pixel is within canvas/screen/window
        if (m_zBuffer[p.y * m_width + p.x] > p.z) { //if z value of pixel is nearer than the value currently in the z buffer
            m_zBuffer[p.y * m_width + p.x] = p.z;
            m_frameBuffer[p.y * m_width + p.x] = colour;
        }
    }
}




//////////////////////////////////////////////////////////////////////////////// make into math function

std::vector<float> Interpolate(int  i0, int i1, float d0, float d1) {
    std::vector<float> vec;
    if (i0 == i1) {
        vec.push_back(d0);
        return vec;
    }
    float gradient = (d1 - d0) / (i1 - i0);
    vec.reserve(i1 - i0);
    float d = d0;
    for (int i{ i0 }; i < i1; i++) {
        vec.push_back(d);
        d = d + gradient;
    }

    return vec;
}

////////////////////////////////////////////////////////////////////////////////




void Rasteriser::drawLine(Maths::Vec3f p1, Maths::Vec3f p2, std::uint32_t colour) {

    Maths::RasterPoint point1{ static_cast<int>(p1(0) * m_width),static_cast<int>(p1(1) * m_height),p1(2) };
    Maths::RasterPoint point2{ static_cast<int>(p2(0) * m_width),static_cast<int>(p2(1) * m_height),p2(2) };
    
    //still need to calculate and interpolate z value

    if (std::abs(p2(0) - p1(0)) > std::abs(p2(1) - p1(1))) {
        //Line is more horizontal than vertical
        if (p1(0) > p2(0)) {
            Maths::swap(p1, p2); 
            Maths::swap(point1, point2);
        }
        std::vector<float> positions{ Interpolate(point1.x, point2.x, p1(1) * m_height, p2(1) * m_height)};

        float zGradient{(point2.z-point1.z) / positions.size()}; //same as changing per x coord increase

        for (int i{ 0 }; i < (point2.x - point1.x); i++) {
            this->drawPixel(Maths::RasterPoint{ i + point1.x, static_cast<int>(positions[i]), point1.z + zGradient * i }, colour);
        }
    }
    else {
        //Line is more vertical than horizontal
        if (p1(1) > p2(1)) {
            Maths::swap(p1, p2); 
            Maths::swap(point1, point2);
        }
        std::vector<float> positions{ Interpolate(point1.y, point2.y, p1(0) * m_width, p2(0) * m_width) };
        
        float zGradient{ (point2.z - point1.z) / positions.size() }; //same as changing per x coord increase

        for (int i{ 0 }; i < (point2.y - point1.y); i++) {
            this->drawPixel(Maths::RasterPoint{ static_cast<int>(positions[i]), i + point1.y, point1.z + zGradient * i }, colour);
        }
    }
    
}

void Rasteriser::drawTriangle(Maths::Vec3f p1, Maths::Vec3f p2, Maths::Vec3f p3, bool wireframe, std::uint32_t colour) {
    
    if (wireframe) {
        this->drawLine(p1, p2, colour);
        this->drawLine(p1, p3, colour);
        this->drawLine(p2, p3, colour);
    }
    else {

        std::array<int, 6> arr{ 
            static_cast<int>(p1(0) * m_width), 
            static_cast<int>(p2(0) * m_width),
            static_cast<int>(p3(0) * m_width),
            static_cast<int>(p1(1) * m_height),
            static_cast<int>(p2(1) * m_height),
            static_cast<int>(p3(1) * m_height)};

        int xMin{ arr[0] };
        int xMax{ arr[0] };
        int yMin{ arr[3] };
        int yMax{ arr[3] };

        for (std::size_t i{1};i<3 ;i++) {
            if (arr[i] < xMin) {
                xMin = arr[i];
            }
            else if (arr[i] > xMax) {
                xMax = arr[i];
            }
        }
        for (std::size_t i{ 4 };i < 6;i++) {
            if (arr[i] < yMin) {
                yMin = arr[i];
            }
            else if (arr[i] > yMax) {
                yMax = arr[i];
            }
        }

        p1(0) = p1(0) * m_width;
        p1(1) = p1(1) * m_height;
        p2(0) = p2(0) * m_width;
        p2(1) = p2(1) * m_height;
        p3(0) = p3(0) * m_width;
        p3(1) = p3(1) * m_height;

        float edge1,edge2,edge3;
        float area{ Maths::edgeFunction(p1,p2,p3)};

        float z;

        for (int y{ yMin }; y <= yMax; y++) {
            for (int x{ xMin }; x <= xMax; x++) {

                //check if pixel is within the triangle
                edge1 = Maths::edgeFunction(p1, p2, static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);
                edge2 = Maths::edgeFunction(p2, p3, static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);
                edge3 = Maths::edgeFunction(p3, p1, static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);

                if (edge1 >= 0 && edge2 >= 0 && edge3 >= 0) {
                    //calculate barycentric coords
                    edge1 /= area;
                    edge2 /= area;
                    edge3 /= area;
                    
                    //use barycentric coords to find z(can also be used for vertex attributes like colours,normals or texture maps)
                    //note that we calculate the inverse as projective transformations don't preserve distance
                    z = 1 / ((1 / (p1(2))) * edge1 + (1 / (p2(2))) * edge2 + (1 / (p3(2))) * edge3);

                    drawPixel(Maths::RasterPoint{ x,y,z }, colour);
                    
                }
                else if (edge1 <= 0 && edge2 <= 0 && edge3 <= 0) {
                    //calculate barycentric coords
                    edge1 /= area;
                    edge2 /= area;
                    edge3 /= area;

                    //use barycentric coords to find z(can also be used for vertex attributes like colours,normals or texture maps)
                    //note that we calculate the inverse as projective transformations don't preserve distance
                    z = 1 / ((1 / (p1(2))) * edge1 + (1 / (p2(2))) * edge2 + (1 / (p3(2))) * edge3);

                    drawPixel(Maths::RasterPoint{ x,y,z }, colour);

                }
            }
        }
    }
}

void Rasteriser::drawObject(Object& object, bool wireframe) {
    const std::vector<std::array<int, 3>>& drawOrder{ object.getOrder() };

    Maths::Vec3f vert1;
    Maths::Vec3f vert2;
    Maths::Vec3f vert3;

    Maths::Mat4 total{};

    total = total * object.getModel(); //model transformation
    total = total * m_camera.getView(); //view transformation
    total.project(m_camera.getFov(), m_camera.getAspectRatio(), m_camera.getNear(), m_camera.getFar()); //projection transformation

    for (std::size_t i{ 0 }; i < drawOrder.size(); i++) {
        vert1 = object.getVertex(drawOrder[i][0]);
        vert2 = object.getVertex(drawOrder[i][1]);
        vert3 = object.getVertex(drawOrder[i][2]);

        vert1.project(total); //project here means that's the transformation is projective / not affine
        vert2.project(total);
        vert3.project(total);

        this->drawTriangle( //we change the vertices from -1,1 to 0-1 coords before passing them
            Maths::Vec3f{ (vert1(0) + 1) / 2, (vert1(1) + 1) / 2, vert1(2) },
            Maths::Vec3f{ (vert2(0) + 1) / 2, (vert2(1) + 1) / 2, vert2(2) },
            Maths::Vec3f{ (vert3(0) + 1) / 2, (vert3(1) + 1) / 2, vert3(2) },
            wireframe);
    }
}

std::vector<std::uint32_t>& Rasteriser::getFrameBuffer() {
    return m_frameBuffer;
}

std::vector<float>& Rasteriser::getZBuffer() {
    return m_zBuffer;
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