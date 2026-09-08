#include <cmath>
#include <algorithm>
#include "maths.h"



namespace Maths {

	Vec2f::Vec2f(float x, float y) : 
		m_arr{x,y} {
	}

	Vec2f& Vec2f::operator= (const Vec2f& vec) {
		m_arr[0] = vec(0);
		m_arr[1] = vec(1);

		return *this;
	}

	float& Vec2f::operator()(int index) {
		return m_arr[index];
	}

	const float& Vec2f::operator()(int index) const {
		return m_arr[index];
	}


	Vec3f::Vec3f(float x, float y, float z) :
		m_arr{ x,y,z } {
	}

	float Vec3f::getMagnitude() const {
		return std::sqrt(m_arr[0] * m_arr[0] + m_arr[1] * m_arr[1] + m_arr[2] * m_arr[2]);
	}

	void Vec3f::normalise() {
		float mag{ this->getMagnitude() };
		float factor{ 1 / mag };
		for (int i{ 0 };i < 3;i++) {
			m_arr[i] *= factor;
		}
	}

	float& Vec3f::operator()(int index) {
		return m_arr[index];
	}

	const float& Vec3f::operator()(int index) const {
		return m_arr[index];
	}

	Vec3f& Vec3f::operator= (const Vec3f& vec) {
		m_arr[0] = vec(0);
		m_arr[1] = vec(1);
		m_arr[2] = vec(2);

		return *this;
	}

	Vec3f operator+(const Vec3f& v1, const Vec3f& v2) {
		return Vec3f(v1(0) + v2(0), v1(1) + v2(1), v1(2) + v2(2));
	}

	Vec3f operator-(const Vec3f& v1, const Vec3f& v2) {
		return Vec3f(v1(0) - v2(0), v1(1) - v2(1), v1(2) - v2(2));
	}

	Vec3f operator*(const Vec3f& v1, const Vec3f& v2) {
		return Vec3f(v1(0) * v2(0), v1(1) * v2(1), v1(2) * v2(2));
	}

	Vec3f operator*(const Vec3f& v, float scalar) {
		return Vec3f(v(0) * scalar, v(1) * scalar, v(2) * scalar);
	}

	Vec3f operator*(float scalar, const Vec3f& v) {
		return Vec3f(v(0) * scalar, v(1) * scalar, v(2) * scalar);
	}

	Vec3f operator/(const Vec3f& v1, const Vec3f& v2) {
		return Vec3f(v1(0) / v2(0), v1(1) / v2(1), v1(2) / v2(2));
	}

	Vec3f operator/(const Vec3f& v, float scalar) {
		return Vec3f(v(0) / scalar, v(1) / scalar, v(2) / scalar);
	}

	Vec3f operator/(float scalar, const Vec3f& v) {
		return Vec3f(v(0) / scalar, v(1) / scalar, v(2) / scalar);
	}

	float Vec3f::getDot(Vec3f v) {
		return m_arr[0] * v(0) + m_arr[1] * v(1) + m_arr[2] * v(2);
	}

	float Vec3f::getAngleBetween(Vec3f v) {
		return std::acos((this->getDot(v)) / (this->getMagnitude() * v.getMagnitude()));
	}

	Vec3f Vec3f::getCross(Vec3f v) {
		return Vec3f(m_arr[1] * v(2) - m_arr[2] * v(1), m_arr[2] * v(0) - m_arr[0] * v(2), m_arr[0] * v(1) - m_arr[1] * v(0));
	}

	void Vec3f::transform(const Mat4& mat) {

		float x = m_arr[0];
		float y = m_arr[1];
		float z = m_arr[2];

		m_arr[0] = (mat(0, 0) * x) + (mat(0, 1) * y) + (mat(0, 2) * z) + mat(0,3);
		m_arr[1] = (mat(1, 0) * x) + (mat(1, 1) * y) + (mat(1, 2) * z) + mat(1, 3);
		m_arr[2] = (mat(2, 0) * x) + (mat(2, 1) * y) + (mat(2, 2) * z) + mat(2, 3);
	}

	void Vec3f::project(const Mat4& mat) {
		double w = m_arr[0] * mat(3, 0) + m_arr[1] * mat(3, 1) + m_arr[2] * mat(3, 2) + mat(3, 3);

		float x = m_arr[0];
		float y = m_arr[1];
		float z = m_arr[2];

		m_arr[0] = (mat(0, 0) * x) + (mat(0, 1) * y) + (mat(0, 2) * z) + mat(0, 3);
		m_arr[1] = (mat(1, 0) * x) + (mat(1, 1) * y) + (mat(1, 2) * z) + mat(1, 3);
		m_arr[2] = (mat(2, 0) * x) + (mat(2, 1) * y) + (mat(2, 2) * z) + mat(2, 3);

		m_arr[0] /= w;
		m_arr[1] /= w;
		m_arr[2] /= w;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mat4::Mat4() :
		m_arr{1,0,0,0,
		      0,1,0,0,
		      0,0,1,0,
		      0,0,0,1 } {
	}

	Mat4::Mat4(std::array<std::array<float, 4>, 4> arr) :
		m_arr{arr} {
	}

	float& Mat4::operator()(int row, int column) {
		return m_arr[row][column];
	}
	const float& Mat4::operator()(int row, int column) const {
		return m_arr[row][column];
	}

	Mat4& Mat4::operator= (const Mat4& vec) {
		for (int i{ 0 }; i < 4; i++) {
			for (int j{ 0 }; j < 4; j++) {
				m_arr[i][j] = vec(i, j);
			}
		}
		return *this;
	}

	//done in this manner to ensure "unnamed" return value optimisation despite it's verbosity, as named rvo is not guranteed
	Mat4 operator+(const Mat4& v1, const Mat4& v2) { 
		return Mat4(std::array<std::array<float, 4>, 4>{
			v1(0,0) + v2(0,0), v1(0,1) + v2(0,1), v1(0,2) + v2(0,2), v1(0,3) + v2(0,3),
			v1(1,0) + v2(1,0), v1(1,1) + v2(1,1), v1(1,2) + v2(1,2), v1(1,3) + v2(1,3),
			v1(2,0) + v2(2,0), v1(2,1) + v2(2,1), v1(2,2) + v2(2,2), v1(2,3) + v2(2,3),
			v1(3,0) + v2(3,0), v1(3,1) + v2(3,1), v1(3,2) + v2(3,2), v1(3,3) + v2(3,3)});
	}

	//done in this manner to ensure "unnamed" return value optimisation despite it's verbosity, as named rvo is not guranteed
	Mat4 operator-(const Mat4& v1, const Mat4& v2) {
		return Mat4(std::array<std::array<float, 4>, 4>{
			v1(0, 0) - v2(0, 0), v1(0, 1) - v2(0, 1), v1(0, 2) - v2(0, 2), v1(0, 3) - v2(0, 3),
			v1(1, 0) - v2(1, 0), v1(1, 1) - v2(1, 1), v1(1, 2) - v2(1, 2), v1(1, 3) - v2(1, 3),
			v1(2, 0) - v2(2, 0), v1(2, 1) - v2(2, 1), v1(2, 2) - v2(2, 2), v1(2, 3) - v2(2, 3),
			v1(3, 0) - v2(3, 0), v1(3, 1) - v2(3, 1), v1(3, 2) - v2(3, 2), v1(3, 3) - v2(3, 3)});
	}

	//done in this manner to ensure "unnamed" return value optimisation despite it's verbosity, as named rvo is not guranteed
	Mat4 operator*(const Mat4& v, float scalar) {
		return Mat4(std::array<std::array<float, 4>, 4>{
			v(0,0) * scalar, v(0,1) * scalar, v(0,2) * scalar, v(0,3) * scalar,
			v(1,0) * scalar, v(1,1) * scalar, v(1,2) * scalar, v(1,3) * scalar,
			v(2,0) * scalar, v(2,1) * scalar, v(2,2) * scalar, v(2,3) * scalar,
			v(3,0) * scalar, v(3,1) * scalar, v(3,2) * scalar, v(3,3) * scalar});
	}

	//done in this manner to ensure "unnamed" return value optimisation despite it's verbosity, as named rvo is not guranteed
	Mat4 operator*(float scalar, const Mat4& v) {
		return Mat4(std::array<std::array<float, 4>, 4>{
			v(0, 0)* scalar, v(0, 1)* scalar, v(0, 2)* scalar, v(0, 3)* scalar,
			v(1, 0)* scalar, v(1, 1)* scalar, v(1, 2)* scalar, v(1, 3)* scalar,
			v(2, 0)* scalar, v(2, 1)* scalar, v(2, 2)* scalar, v(2, 3)* scalar,
			v(3, 0)* scalar, v(3, 1)* scalar, v(3, 2)* scalar, v(3, 3)* scalar});
	}

	//done in this manner to ensure "unnamed" return value optimisation despite it's verbosity, as named rvo is not guranteed
	Mat4 operator*(const Mat4& v1, const Mat4& v2) {
		return Mat4(std::array<std::array<float, 4>, 4>{
			v1(0,0) * v2(0,0) + v1(0,1) * v2(1,0) + v1(0,2) * v2(2,0) + v1(0,3) * v2(3,0),
			v1(0,0) * v2(0,1) + v1(0,1) * v2(1,1) + v1(0,2) * v2(2,1) + v1(0,3) * v2(3,1),
			v1(0,0) * v2(0,2) + v1(0,1) * v2(1,2) + v1(0,2) * v2(2,2) + v1(0,3) * v2(3,2),
			v1(0,0) * v2(0,3) + v1(0,1) * v2(1,3) + v1(0,2) * v2(2,3) + v1(0,3) * v2(3,3),
			
			v1(1,0) * v2(0,0) + v1(1,1) * v2(1,0) + v1(1,2) * v2(2,0) + v1(1,3) * v2(3, 0),
			v1(1,0) * v2(0,1) + v1(1,1) * v2(1,1) + v1(1,2) * v2(2,1) + v1(1,3) * v2(3, 1),
			v1(1,0) * v2(0,2) + v1(1,1) * v2(1,2) + v1(1,2) * v2(2,2) + v1(1,3) * v2(3, 2),
			v1(1,0) * v2(0,3) + v1(1,1) * v2(1,3) + v1(1,2) * v2(2,3) + v1(1,3) * v2(3, 3),
			
			v1(2,0) * v2(0,0) + v1(2,1) * v2(1,0) + v1(2,2) * v2(2,0) + v1(2,3) * v2(3, 0),
			v1(2,0) * v2(0,1) + v1(2,1) * v2(1,1) + v1(2,2) * v2(2,1) + v1(2,3) * v2(3, 1),
			v1(2,0) * v2(0,2) + v1(2,1) * v2(1,2) + v1(2,2) * v2(2,2) + v1(2,3) * v2(3, 2),
			v1(2,0) * v2(0,3) + v1(2,1) * v2(1,3) + v1(2,2) * v2(2,3) + v1(2,3) * v2(3, 3),

			0,
			0,
			0,
			1
		});
	}


	

	void Mat4::transpose3() {
		std::swap(m_arr[0][1], m_arr[1][0]);
		std::swap(m_arr[0][2], m_arr[2][0]);
		std::swap(m_arr[1][2], m_arr[2][1]);
	}

	float Mat4::getDeterminant3() {
		return m_arr[0][0] * (m_arr[1][1] * m_arr[2][2] - m_arr[1][2] * m_arr[2][1])
			+  m_arr[0][1] * (m_arr[1][0] * m_arr[2][2] - m_arr[1][2] * m_arr[2][0])
			+  m_arr[0][2] * (m_arr[1][0] * m_arr[2][1] - m_arr[1][1] * m_arr[2][0]);
	}

	Mat4 Mat4::getInverse() { // we return the identity matrix if no inverse exists

		//Gauss-Jordan Matrix Inversion
		//we create an identity matrix, and apply the same elementary operations to it that we apply to
		//the original matrix which transforms it into an identity matrix to find it's inverse

		Mat4 copy = m_arr;


		Mat4 inverse{};
		float temp{};
		float invTemp{};
		int largestRowAbsolute{};
		
		//we attempt to row reduce each column one at a time
		//first step, check if pivot is non zero, if not swap with next row which is non zero
		for (int j{ 0 };j < 4;j++) { // change j to column and i to row
			if (copy(j,j) == 0.0f) {

				temp = std::abs(copy(0,j));
				largestRowAbsolute = 0;

				if (temp < std::abs(copy(1,j))) {
					temp = std::abs(copy(1,j));
					largestRowAbsolute = 1;
				}
				if (temp < std::abs(copy(2,j))) {
					temp = std::abs(copy(2,j));
					largestRowAbsolute = 2;
				}
				if (temp < std::abs(copy(3,j))) {
					temp = std::abs(copy(3,j));
					largestRowAbsolute = 3;
				}


				if (largestRowAbsolute == 0) {
					for (int i{ 0 }; i < 4; i++) {
						temp = copy(j,i);
						invTemp = inverse(j, i);

						copy(j,i) = copy(0,i);
						inverse(j, i) = inverse(0, i);

						copy(0,i) = temp;
						inverse(0, i) = invTemp;
					}

				}
				else if (largestRowAbsolute == 1) {
					for (int i{ 0 }; i < 4; i++) {
						temp = copy(j,i);
						invTemp = inverse(j,i);

						copy(j,i) = copy(1,i);
						inverse(j, i) = inverse(1, i);

						copy(1,i) = temp;
						inverse(1, i) = invTemp;
					}

				}
				else if (largestRowAbsolute == 2) {
					for (int i{ 0 }; i < 4; i++) {
						temp = copy(j, i);
						invTemp = inverse(j, i);

						copy(j,i) = copy(2,i);
						inverse(j, i) = inverse(2, i);

						copy(2,i) = temp;
						inverse(2, i) = invTemp;
					}
				}
				else if (largestRowAbsolute == 3) {
					for (int i{ 0 }; i < 4; i++) {
						temp = copy(j, i);
						invTemp = inverse(j, i);

						copy(j,i) = copy(3,i);
						inverse(j, i) = inverse(3, i);

						copy(3,i) = temp;
						inverse(3, i) = invTemp;
					}
				}
				else {
					return Mat4();
				}
			}
		}

		//step two, we eliminate the rows under the diagonal element
		//we multiply our "pivots" row by the negative value of the element we want to eliminate divided by itself
		//this makes it equal to the negative of the element we want to eliminate
		//we then add the rows together to eliminate the element
		//we can call such a value k, where k = m_arr[row][column] / m_arr[column][column] 
		//repeat for all elements we wish to eliminate

		float k{};
		for (int column{0}; column <3; column++) {
			for (int row{ column+1 }; row < 4; row++) {
				k = copy(row,column) / copy(column,column);
				for (int p{0}; p < 4; p++) {
					copy(row,p) -= k * copy(column,p);
					inverse(row, p) -= k * inverse(column, p);
				}
				copy(row,column) = 0.0f; //this is just for safety due to floating point imprecision
			}
		}


		//step 3, we scale our pivots to 0
		//we reuse k as a divisor

		for (int row{ 0 }; row < 4; row++) {
			k = copy(row,row);
			for (int column{ 0 }; column < 4; column++) {
				copy(row,column) /= k;
				inverse(row, column) /= k;
			}
		}


		//step 4, eliminate numbers above the diagonal
		//we reuse k again

		for (int row{ 0 }; row < 4; row++) {
			for (int column{ row + 1 }; column < 4; column++) {
				k = copy(row,column);
				for (int p{ 0 }; p < 4; p++) {
					copy(row,p) -= copy(column,p) * k;
					inverse(row, p) -= inverse(column, p) * k;
				}
				copy(row,column) = 0.0f; //this is just for safety due to floating point imprecision
			}
		}



		return inverse;
	}

	void Mat4::rotate(float angle, Vec3f vec) { //ideally this should use quaternions but oh well

		vec.normalise();

		float c = std::cos(angle);
		float s = std::sin(angle);
		
		if (std::abs(c) < 0.000001f) {
			c = 0.0f;
		}
		if (std::abs(s) < 0.000001f) {
			s = 0.0f;
		}

		float t = 1 - c;

		(*this) = (*this) * Mat4({
			(vec(0) * vec(0)) * (t) + c,
			(vec(0) * vec(1)) * (t) - (vec(2) * s),
			(vec(0) * vec(2)) * (t) + (vec(1) * s),
			0,

			(vec(0) * vec(1)) * (t) + (vec(2) * s),
			(vec(1) * vec(1)) * (t) + c,
			(vec(1) * vec(2)) * (t) - (vec(0) * s),
			0,

			(vec(0) * vec(2)) * (t) - (vec(1) * s),
			(vec(1) * vec(2)) * (t) + (vec(0) * s),
			(vec(2) * vec(2)) * (t) + c,
			0,

			0,
			0,
			0,
			1
			});

	}

	void Mat4::scale(Vec3f vec) {
		(*this) = (*this) * Mat4({
			vec(0),0,0,0,
			0,vec(1),0,0,
			0,0,vec(2),0,
			0,0,0,1 
			});
	}

	void Mat4::translateLocal(Vec3f vec) {
		(*this) = (*this) * Mat4({
			1,0,0,vec(0),
			0,1,0,vec(1),
			0,0,1,vec(2),
			0,0,0,1 
			});
	}

	void Mat4::translateWorld(Vec3f vec) {
		(*this) = Mat4({
			1,0,0,vec(0),
			0,1,0,vec(1),
			0,0,1,vec(2),
			0,0,0,1
			}) * (*this);
	}

	
	// the projection matrix doesn't use * as * is definied for affine transformations as an optimisation
	void Mat4::project(float fov, float aspectRatio, float near, float far) {

		float f = 1 / (std::tan(fov / 2));
		float depth = near - far;

		//it is more efficient to store 4 values and immediatley change the values in the matrix, rather than store the whole matrix
		std::array<float, 4> arr{m_arr[2][0], m_arr[2][1], m_arr[2][2], m_arr[2][3]};
	
		m_arr[0][0] = (f / aspectRatio) * m_arr[0][0];
		m_arr[0][1] = (f / aspectRatio) * m_arr[0][1];
		m_arr[0][2] = (f / aspectRatio) * m_arr[0][2];
		m_arr[0][3] = (f / aspectRatio) * m_arr[0][3];
	
		m_arr[1][0] = f * m_arr[1][0];
		m_arr[1][1] = f * m_arr[1][1];
		m_arr[1][2] = f * m_arr[1][2];
		m_arr[1][3] = f * m_arr[1][3];
	
		m_arr[2][0] = (near + far) / (depth) * m_arr[2][0] + (2 * near * far) / (depth) * m_arr[3][0];
		m_arr[2][1] = (near + far) / (depth) * m_arr[2][1] + (2 * near * far) / (depth) * m_arr[3][1];
		m_arr[2][2] = (near + far) / (depth) * m_arr[2][2] + (2 * near * far) / (depth) * m_arr[3][2];
		m_arr[2][3] = (near + far) / (depth) * m_arr[2][3] + (2 * near * far) / (depth) * m_arr[3][3];
	
		m_arr[3][0] = -1 * arr[0];
		m_arr[3][1] = -1 * arr[1];
		m_arr[3][2] = -1 * arr[2];
		m_arr[3][3] = -1 * arr[3];
	}

	float degreeToRadian(float angle) {
		return (angle / 180) * 3.14159265;
	}
	float radianToDegree(float angle) {
		return (angle / 3.14159265) * 180;
	}

	void swap(RasterPoint& p1, RasterPoint& p2) {

		RasterPoint temp{p1.x,p1.y,p1.z};

		p1.x = p2.x;
		p1.y = p2.y;
		p1.z = p2.z;

		p2.x = temp.x;
		p2.y = temp.y;
		p2.z = temp.z;
	}

	void swap(Vec3f& p1, Vec3f& p2) {

		Vec3f temp{p1(0),p1(1),p1(2)};

		p1(0) = p2(0);
		p1(1) = p2(1);
		p1(2) = p2(2);

		p2(0) = temp(0);
		p2(1) = temp(1);
		p2(2) = temp(2);
	}

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

	float edgeFunction(Vec3f p1, Vec3f p2, float x, float y){
		return (x - p1(0)) * (p2(1) - p1(1)) - (y - p1(1)) * (p2(0) - p1(0));
	}

	float edgeFunction(Vec3f p1, Vec3f p2, Vec3f p3) {
		return (p3(0) - p1(0)) * (p2(1) - p1(1)) - (p3(1) - p1(1)) * (p2(0) - p1(0));
	}

}