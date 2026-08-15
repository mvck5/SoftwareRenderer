#include <cmath>
#include <algorithm>
#include "maths.h"

namespace Maths {
	Vec3::Vec3(double x, double y, double z) : 
		m_arr{ x,y,z } {
	}

	double Vec3::getMagnitude() const {
		return std::sqrt(m_arr[0] * m_arr[0] + m_arr[1] * m_arr[1] + m_arr[2] * m_arr[2]);
	}

	void Vec3::normalise() {
		double mag{ this->getMagnitude() };
		double factor{ 1 / mag };
		for (int i{ 0 };i < 3;i++) {
			m_arr[i] *= factor;
		}
	}

	double& Vec3::operator()(int index) {
		return m_arr[index];
	}

	const double& Vec3::operator()(int index) const {
		return m_arr[index];
	}

	Vec3 operator+(const Vec3& v1, const Vec3& v2) {
		return Vec3(v1(0) + v2(0), v1(1) + v2(1), v1(2) + v2(2));
	}

	Vec3 operator-(const Vec3& v1, const Vec3& v2) {
		return Vec3(v1(0) - v2(0), v1(1) - v2(1), v1(2) - v2(2));
	}

	Vec3 operator*(const Vec3& v1, const Vec3& v2) {
		return Vec3(v1(0) * v2(0), v1(1) * v2(1), v1(2) * v2(2));
	}

	Vec3 operator*(const Vec3& v, double scalar) {
		return Vec3(v(0) * scalar, v(1) * scalar, v(2) * scalar);
	}

	Vec3 operator*(double scalar, const Vec3& v) {
		return Vec3(v(0) * scalar, v(1) * scalar, v(2) * scalar);
	}

	Vec3 operator/(const Vec3& v1, const Vec3& v2) {
		return Vec3(v1(0) / v2(0), v1(1) / v2(1), v1(2) / v2(2));
	}

	Vec3 operator/(const Vec3& v, double scalar) {
		return Vec3(v(0) / scalar, v(1) / scalar, v(2) / scalar);
	}

	Vec3 operator/(double scalar, const Vec3& v) {
		return Vec3(v(0) / scalar, v(1) / scalar, v(2) / scalar);
	}

	double Vec3::getDot(const Vec3& v) {
		return m_arr[0] * v(0) + m_arr[1] * v(1) + m_arr[2] * v(2);
	}

	double Vec3::getAngleBetween(const Vec3& v) {
		return std::acos((this->getDot(v)) / (this->getMagnitude() * v.getMagnitude()));
	}

	Vec3 Vec3::getCross(const Vec3& v) {
		return Vec3(m_arr[1] * v(2) - m_arr[2] * v(1), m_arr[2] * v(0) - m_arr[0] * v(2), m_arr[0] * v(1) - m_arr[1] * v(0));
	}

	void Vec3::transform(Mat4& mat) {
		m_arr[0] = (mat(0, 0) * m_arr[0]) + (mat(0, 1) * m_arr[1]) + (mat(0, 2) * m_arr[2]) + mat(0,3);
		m_arr[1] = (mat(1, 0) * m_arr[0]) + (mat(1, 1) * m_arr[1]) + (mat(1, 2) * m_arr[2]) + mat(1, 3);
		m_arr[2] = (mat(2, 0) * m_arr[0]) + (mat(2, 1) * m_arr[1]) + (mat(2, 2) * m_arr[2]) + mat(2, 3);
	}

	void Vec3::project(Mat4& mat) {
		m_arr[0] = (mat(0, 0) * m_arr[0]) + (mat(0, 1) * m_arr[1]) + (mat(0, 2) * m_arr[2]) + mat(0, 3);
		m_arr[1] = (mat(1, 0) * m_arr[0]) + (mat(1, 1) * m_arr[1]) + (mat(1, 2) * m_arr[2]) + mat(1, 3);
		m_arr[2] = (mat(2, 0) * m_arr[0]) + (mat(2, 1) * m_arr[1]) + (mat(2, 2) * m_arr[2]) + mat(2, 3);
		
		double w = m_arr[0] * mat(3, 0) + m_arr[1] * mat(3, 1) + m_arr[2] * mat(3, 2) + mat(3,3);

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

	Mat4::Mat4(std::array<std::array<double, 4>, 4> arr) : 
		m_arr{arr} {
	}

	double& Mat4::operator()(int row, int column) {
		return m_arr[row][column];
	}
	const double& Mat4::operator()(int row, int column) const {
		return m_arr[row][column];
	}

	//done in this manner to ensure "unnamed" return value optimisation despite it's verbosity, as named rvo is not guranteed
	Mat4 operator+(const Mat4& v1, const Mat4& v2) { 
		return Mat4(std::array<std::array<double, 4>, 4>{
			v1(0,0) + v2(0,0), v1(0,1) + v2(0,1), v1(0,2) + v2(0,2), v1(0,3) + v2(0,3),
			v1(1,0) + v2(1,0), v1(1,1) + v2(1,1), v1(1,2) + v2(1,2), v1(1,3) + v2(1,3),
			v1(2,0) + v2(2,0), v1(2,1) + v2(2,1), v1(2,2) + v2(2,2), v1(2,3) + v2(2,3),
			v1(3,0) + v2(3,0), v1(3,1) + v2(3,1), v1(3,2) + v2(3,2), v1(3,3) + v2(3,3)});
	}

	//done in this manner to ensure "unnamed" return value optimisation despite it's verbosity, as named rvo is not guranteed
	Mat4 operator-(const Mat4& v1, const Mat4& v2) {
		return Mat4(std::array<std::array<double, 4>, 4>{
			v1(0, 0) - v2(0, 0), v1(0, 1) - v2(0, 1), v1(0, 2) - v2(0, 2), v1(0, 3) - v2(0, 3),
			v1(1, 0) - v2(1, 0), v1(1, 1) - v2(1, 1), v1(1, 2) - v2(1, 2), v1(1, 3) - v2(1, 3),
			v1(2, 0) - v2(2, 0), v1(2, 1) - v2(2, 1), v1(2, 2) - v2(2, 2), v1(2, 3) - v2(2, 3),
			v1(3, 0) - v2(3, 0), v1(3, 1) - v2(3, 1), v1(3, 2) - v2(3, 2), v1(3, 3) - v2(3, 3)});
	}

	//done in this manner to ensure "unnamed" return value optimisation despite it's verbosity, as named rvo is not guranteed
	Mat4 operator*(const Mat4& v, double scalar) {
		return Mat4(std::array<std::array<double, 4>, 4>{
			v(0,0) * scalar, v(0,1) * scalar, v(0,2) * scalar, v(0,3) * scalar,
			v(1,0) * scalar, v(1,1) * scalar, v(1,2) * scalar, v(1,3) * scalar,
			v(2,0) * scalar, v(2,1) * scalar, v(2,2) * scalar, v(2,3) * scalar,
			v(3,0) * scalar, v(3,1) * scalar, v(3,2) * scalar, v(3,3) * scalar});
	}

	//done in this manner to ensure "unnamed" return value optimisation despite it's verbosity, as named rvo is not guranteed
	Mat4 operator*(double scalar, const Mat4& v) {
		return Mat4(std::array<std::array<double, 4>, 4>{
			v(0, 0)* scalar, v(0, 1)* scalar, v(0, 2)* scalar, v(0, 3)* scalar,
			v(1, 0)* scalar, v(1, 1)* scalar, v(1, 2)* scalar, v(1, 3)* scalar,
			v(2, 0)* scalar, v(2, 1)* scalar, v(2, 2)* scalar, v(2, 3)* scalar,
			v(3, 0)* scalar, v(3, 1)* scalar, v(3, 2)* scalar, v(3, 3)* scalar});
	}

	//done in this manner to ensure "unnamed" return value optimisation despite it's verbosity, as named rvo is not guranteed
	Mat4 operator*(const Mat4& v1, const Mat4& v2) {
		return Mat4(std::array<std::array<double, 4>, 4>{
			v1(0,0) * v2(0,0) + v1(0,1) * v2(1,0) + v1(0,2) * v2(2,0),
			v1(0,0) * v2(0,1) + v1(0,1) * v2(1,1) + v1(0,2) * v2(2,1),
			v1(0,0) * v2(0,2) + v1(0,1) * v2(1,2) + v1(0,2) * v2(2,2),
			v1(0,0) * v2(0,3) + v1(0,1) * v2(1,3) + v1(0,2) * v2(2,3),
			
			v1(1,0) * v2(0,0) + v1(1,1) * v2(1,0) + v1(1,2) * v2(2,0),
			v1(1,0) * v2(0,1) + v1(1,1) * v2(1,1) + v1(1,2) * v2(2,1),
			v1(1,0) * v2(0,2) + v1(1,1) * v2(1,2) + v1(1,2) * v2(2,2),
			v1(1,0) * v2(0,3) + v1(1,1) * v2(1,3) + v1(1,2) * v2(2,3),
			
			v1(2,0) * v2(0,0) + v1(2,1) * v2(1,0) + v1(2,2) * v2(2,0),
			v1(2,0) * v2(0,1) + v1(2,1) * v2(1,1) + v1(2,2) * v2(2,1),
			v1(2,0) * v2(0,2) + v1(2,1) * v2(1,2) + v1(2,2) * v2(2,2),
			v1(2,0) * v2(0,3) + v1(2,1) * v2(1,3) + v1(2,2) * v2(2,3),
		});
	}

	void Mat4::transpose3() {
		std::swap(m_arr[0][1], m_arr[1][0]);
		std::swap(m_arr[0][2], m_arr[2][0]);
		std::swap(m_arr[1][2], m_arr[2][1]);
	}

	double Mat4::getDeterminant3() {
		return m_arr[0][0] * (m_arr[1][1] * m_arr[2][2] - m_arr[1][2] * m_arr[2][1])
			+  m_arr[0][1] * (m_arr[1][0] * m_arr[2][2] - m_arr[1][2] * m_arr[2][0])
			+  m_arr[0][2] * (m_arr[1][0] * m_arr[2][1] - m_arr[1][1] * m_arr[2][0]);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	Mat4 Mat4::getInverse() {
		return Mat4();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Mat4::rotate(double angle, Vec3 vec) { //ideally this should use quaternions but oh well

		vec.normalise();

		(*this) = (*this) * Mat4({
			(vec(0) * vec(0))* (1 - std::cos(angle)) + std::cos(angle),
			(vec(0) * vec(1))* (1 - std::cos(angle)) - (vec(2) * std::cos(angle)),
			(vec(0) * vec(2))* (1 - std::cos(angle)) + (vec(1) * std::cos(angle)),
			0,
			(vec(0) * vec(1)) * (1 - std::cos(angle)) + (vec(2) * std::cos(angle)),
			(vec(1) * vec(1)) * (1 - std::cos(angle)) + std::cos(angle),
			(vec(1) * vec(2)) * (1 - std::cos(angle)) - (vec(0) * std::cos(angle)),
			0,
			(vec(0) * vec(2)) * (1 - std::cos(angle)) - (vec(1) * std::cos(angle)),
			(vec(1) * vec(2)) * (1 - std::cos(angle)) + (vec(0) * std::cos(angle)),
			(vec(2) * vec(2)) * (1 - std::cos(angle)) + std::cos(angle),
			0,
			0,
			0,
			0,
			1
			});
	}

	void Mat4::scale(Vec3 vec) {
		(*this) = (*this) * Mat4({
			vec(0),0,0,0,
			0,vec(1),0,0,
			0,0,vec(2),0,
			0,0,0,1 
			});
	}

	void Mat4::translate(Vec3 vec) {
		(*this) = (*this) * Mat4({ 
			1,0,0,vec(0),
			0,1,0,vec(1),
			0,0,1,vec(2),
			0,0,0,1 
			});
	}

	void Mat4::project(double fov, double aspectRatio, double near, double far) { // the projection matrix contains a vast amount of 0 so can be optimised like this althought it's ugly

		double f = 1 / (std::tan(fov / 2));

		m_arr[0][0] = m_arr[0][0] * (f / aspectRatio);
		m_arr[0][1] = m_arr[0][1] * (f);
		m_arr[0][2] = m_arr[0][2] * ((near + far) / (near - far))  + m_arr[0][3] * (-1);
		m_arr[0][3] = m_arr[0][2] * ((2 * near * far) / (near-far));

		m_arr[1][0] = m_arr[1][0] * (f / aspectRatio);
		m_arr[1][1] = m_arr[1][1] * (f);
		m_arr[1][2] = m_arr[1][2] * ((near + far) / (near - far)) + m_arr[1][3] * (-1);
		m_arr[1][3] = m_arr[1][2] * ((2 * near * far) / (near - far));

		m_arr[2][0] = m_arr[2][0] * (f / aspectRatio) ;
		m_arr[2][1] = m_arr[2][1] * (f);
		m_arr[2][2] = m_arr[2][2] * ((near + far) / (near - far)) + m_arr[2][3] * (-1);
		m_arr[2][3] = m_arr[2][2] * ((2 * near * far) / (near - far));

		m_arr[3][0] = m_arr[3][0] * (f / aspectRatio);
		m_arr[3][1] = m_arr[3][1] * (f);
		m_arr[3][2] = m_arr[3][2] * ((near + far) / (near - far)) + m_arr[3][3] * (-1);
		m_arr[3][3] = m_arr[3][2] * ((2 * near * far) / (near - far));
	}

}