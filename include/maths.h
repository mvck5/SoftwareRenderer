#pragma once
#include <array>

namespace Maths {
	class Mat4;

	class Vec3 {
	private:
		std::array<double, 3> m_arr;
	public:
		Vec3(double x = 0, double y = 0, double z = 0);

		double getMagnitude() const;
		void normalise();
		double getDot(const Vec3& v);
		double getAngleBetween(const Vec3& v); //radians
		Vec3 getCross(const Vec3& v);

		void transform(Mat4& mat);

		double& operator() (int index);
		const double& operator() (int index) const;

		friend Vec3 operator+(const Vec3& v1, const Vec3& v2);
		friend Vec3 operator-(const Vec3& v1, const Vec3& v2);
		friend Vec3 operator*(const Vec3& v1, const Vec3& v2);
		friend Vec3 operator*(const Vec3& v, double scalar);
		friend Vec3 operator*(double scalar, const Vec3& v);
		friend Vec3 operator/(const Vec3& v1, const Vec3& v2);
		friend Vec3 operator/(const Vec3& v, double scalar);
		friend Vec3 operator/(double scalar, const Vec3& v);
	};

	class Mat4 {
	private:
		std::array<std::array<double,4>, 4> m_arr;
	public:
		Mat4(); // identity matrix
		Mat4(std::array<std::array<double, 4>, 4> arr);

		void transpose();
		void transposeKeepTranslation();
		double getDeterminant3();
		Mat4 getInverse();

		void rotate(double angle, Vec3 vec);
		void scale(Vec3 vec);
		void translate(Vec3 vec);

		double& operator() (int row, int column);
		const double& operator() (int row, int column) const;

		friend Mat4 operator+(const Mat4& v1, const Mat4& v2);
		friend Mat4 operator-(const Mat4& v1, const Mat4& v2);
		friend Mat4 operator*(const Mat4& v1, const Mat4& v2);
		friend Mat4 operator*(const Mat4& v, double scalar);
		friend Mat4 operator*(double scalar, const Mat4& v);

	};
}

////////////////////////////                                                  ////////////////////////////
//////////////////////////// model, view, projection matrices need to be done ////////////////////////////
////////////////////////////                                                  ////////////////////////////