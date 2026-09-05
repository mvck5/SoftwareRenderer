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

    if (p.x > 0 && p.x < m_width && p.y > 0 && p.y < m_height) { //check if pixel is within canvas/screen/window
        //std::cout << "p.y: " << p.y << " p.x: " << p.x << "\n";
        if (m_zBuffer[p.y * m_width + p.x] > p.z) { //if z value of pixel is nearer than the value currently in the z buffer
            m_zBuffer[p.y * m_width + p.x] = p.z;
            m_frameBuffer[p.y * m_width + p.x] = colour;
        }
    }
}

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
        std::vector<float> positions{ Maths::Interpolate(point1.x, point2.x, p1(1) * m_height, p2(1) * m_height)};

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
        std::vector<float> positions{ Maths::Interpolate(point1.y, point2.y, p1(0) * m_width, p2(0) * m_width) };
        
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

        //we precalculate parts of z, as it reduces the amount of operations needed for baryocentric coordinate calculations
        //this should be done for all vertex attributes
        float tempZ1{ 1/p1(2) };
        float tempZ2{ 1/p2(2) - 1/p1(2) };
        float tempZ3{ 1/p3(2) - 1/p1(2) };
        
        //precalculate edge function and incrememnt it by a step, this means we only have to do
        //a singular addition instead of recalculating the edge function for each pixel
        //essentially we're just linearly interprolating and adding it up

        float preEdge1{ Maths::edgeFunction(p1, p2, static_cast<float>(xMin) + 0.5f, static_cast<float>(yMin) + 0.5f) };
        float xStepEdge1{ p2(1) - p1(1) };
        float yStepEdge1{ p1(0) - p2(0) };

        float preEdge2{ Maths::edgeFunction(p2, p3, static_cast<float>(xMin) + 0.5f, static_cast<float>(yMin) + 0.5f) };
        float xStepEdge2{ p3(1) - p2(1) };
        float yStepEdge2{ p2(0) - p3(0) };

        float preEdge3{ Maths::edgeFunction(p3, p1, static_cast<float>(xMin) + 0.5f, static_cast<float>(yMin) + 0.5f) };
        float xStepEdge3{ p1(1) - p3(1) };
        float yStepEdge3{ p3(0) - p1(0) };

        float tempEdge1;
        float tempEdge2;
        float tempEdge3;

        int yCounter{ 0 };


        for (int y{ yMin }; y <= yMax; y++) {

            edge1 = preEdge1 + yStepEdge1 * yCounter;
            edge2 = preEdge2 + yStepEdge2 * yCounter;
            edge3 = preEdge3 + yStepEdge3 * yCounter;
    
            for (int x{ xMin }; x <= xMax; x++) {

                //check if pixel is within the triangle
                edge1 += xStepEdge1;
                edge2 += xStepEdge2;
                edge3 += xStepEdge3;

                if (edge1 >= 0 && edge2 >= 0 && edge3 >= 0) {
                    //calculate barycentric coords
                    tempEdge1 = edge1 / area;
                    tempEdge2 = edge2 / area;
                    tempEdge3 = edge3 / area;
                    
                    //use barycentric coords to find z(can also be used for vertex attributes like colours,normals or texture maps)
                    //note that we calculate the inverse as projective transformations don't preserve distance

                    //z = 1 / ((1 / (p1(2))) * edge1 + (1 / (p2(2))) * edge2 + (1 / (p3(2))) * edge3); naive calculation
                    z = 1 / (tempZ1 + edge2 * tempZ2 + edge3 * tempZ3);

                    drawPixel(Maths::RasterPoint{ x,y,z }, colour);
                    
                }
                else if (edge1 < 0 && edge2 < 0 && edge3 < 0) {
                    //calculate barycentric coords
                    tempEdge1 = edge1 / area;
                    tempEdge2 = edge2 / area;
                    tempEdge3 = edge3 / area;

                    //use barycentric coords to find z(can also be used for vertex attributes like colours,normals or texture maps)
                    //note that we calculate the inverse as projective transformations don't preserve distance
                    
                    //z = 1 / ((1 / (p1(2))) * edge1 + (1 / (p2(2))) * edge2 + (1 / (p3(2))) * edge3); naive calculation
                    z = 1 / (tempZ1 + edge2 * tempZ2 + edge3 * tempZ3);

                    drawPixel(Maths::RasterPoint{ x,y,z }, colour);

                }

            }

            yCounter++;

        }
    }
}

void Rasteriser::drawObject(Object& object, bool wireframe) {
    const std::vector<std::array<int, 3>>& drawOrder{ object.getOrder() };

    Maths::Vec3f vert1;
    Maths::Vec3f vert2;
    Maths::Vec3f vert3;

    Maths::Mat4 total{};


    //we need thie inverse of the camera and as it has only undergone affine transformations it's quicker to find the inverse
    //then multiply the translation by the inverse
    Maths::Mat4 view = m_camera.getView();

    view.transpose3();

    view(0, 3) = -(view(0,0) * m_camera.getView()(0,3)
                 + view(0, 1) * m_camera.getView()(1, 3)
                 + view(0, 2) * m_camera.getView()(2, 3));  

    view(1, 3) = -(view(1, 0) * m_camera.getView()(0, 3)
                 + view(1, 1) * m_camera.getView()(1, 3)
                 + view(1, 2) * m_camera.getView()(2, 3));

    view(2, 3) = -(view(2, 0) * m_camera.getView()(0, 3)
                 + view(2, 1) * m_camera.getView()(1, 3)
                 + view(2, 2) * m_camera.getView()(2, 3));



    total = total * object.getModel(); //model transformation
    total = view * total; //view transformation
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

Camera& Rasteriser::getCamera() {
    return m_camera;
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