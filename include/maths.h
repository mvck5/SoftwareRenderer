#pragma once
#include <array>
#include <vector>
#include "colour.h"

// note that the maths used here are simple and only for the use cases that are needed
// it is not extensive and not recommended for use other than this renderer

namespace Maths {

	class Mat4;

	struct Vec2f {
	private:
		std::array<float, 2> m_arr;
	public:
		Vec2f(float x = 0.0f, float y = 0.0f);

		float& operator() (int index);
		const float& operator() (int index) const;
		Vec2f& operator= (const Vec2f& vec);
	};

	class Vec3f {
	private:
		std::array<float, 3> m_arr;
	public:
		Vec3f(float x = 0.0f, float y = 0.0f, float z = 0.0f);

		float getMagnitude() const;
		void normalise();
		float getDot(Vec3f v);
		float getAngleBetween(Vec3f v); // angle in radians
		Vec3f getCross(Vec3f v);

		void transform(const Mat4& mat); // transforms the vect

		float& operator() (int index);
		const float& operator() (int index) const;
		Vec3f& operator= (const Vec3f& vec);

		friend Vec3f operator+(const Vec3f& v1, const Vec3f& v2);
		friend Vec3f operator-(const Vec3f& v1, const Vec3f& v2);
		friend Vec3f operator*(const Vec3f& v1, const Vec3f& v2);
		friend Vec3f operator*(const Vec3f& v, float scalar);
		friend Vec3f operator*(float scalar, const Vec3f& v);
		friend Vec3f operator/(const Vec3f& v1, const Vec3f& v2);
		friend Vec3f operator/(const Vec3f& v, float scalar);
		friend Vec3f operator/(float scalar, const Vec3f& v);
	};

	

	class Mat4 {
	private:
		std::array<std::array<float,4>, 4> m_arr;
	public:
		Mat4(); // identity matrix
		Mat4(std::array<std::array<float, 4>, 4> arr);


		void transpose3();
		float getDeterminant3();
		Mat4 getInverse();

		//affine transformations (does not change w)
		void rotate(float angle, Vec3f vec); // angle is in radians
		void scale(Vec3f vec);
		void translateLocal(Vec3f vec);
		void translateWorld(Vec3f vec);

		//projective transformations (does change w)
		void project(float fov, float aspectRatio, float near, float far);

		float& operator() (int row, int column);
		const float& operator() (int row, int column) const;
		Mat4& operator= (const Mat4& vec);

		friend Mat4 operator+(const Mat4& v1, const Mat4& v2);
		friend Mat4 operator-(const Mat4& v1, const Mat4& v2);
		friend Mat4 operator*(const Mat4& v1, const Mat4& v2); //only for affine transformations
		friend Mat4 operator*(const Mat4& v, float scalar);
		friend Mat4 operator*(float scalar, const Mat4& v);

	};

	struct RasterPoint {
		int x{};
		int y{};
		float z{};
	};

	struct Vertex {
		Vec3f position{};
		Vec3f normal{};
		Vec2f texCoord{};

		float invW;
	};


	float degreeToRadian(float angle);
	float radianToDegree(float angle);

	void swap(RasterPoint& p1, RasterPoint& p2);
	void swap(Vec3f& p1, Vec3f& p2);

	std::vector<float> Interpolate(int  i0, int i1, float d0, float d1);

	float edgeFunction(Vec3f p1, Vec3f p2, float x, float y);
	float edgeFunction(Vec3f p1, Vec3f p2, Vec3f p3);
	
}