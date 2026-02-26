#pragma once
#include <cmath>

struct Vec3 {
	float x, y, z;

	Vec3() : x(0), y(0), z(0) {}
	Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	Vec3 operator+(const Vec3& other) const {
		return Vec3(x + other.x, y + other.y, z + other.z);
	}

	Vec3 operator-(const Vec3& other) const {
		return Vec3(x - other.x, y - other.y, z - other.z);
	}

	Vec3 operator*(const float other) const {
		return Vec3(x * other, y * other, z * other);
	}

	// 벡터의 길이를 구하는 함수
	float length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	// 벡터를 정규화하는 함수
	void normalize() {
		float norm = length();
		if (norm != 0) {
			x /= norm;
			y /= norm;
			z /= norm;
		}
	}

	// 내적 계산 함수
	float dot(const Vec3& other) const {
		return (x * other.x + y * other.y + z * other.z);
	}

	// 외적 계산 함수
	Vec3 cross(const Vec3& other) const {
		return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
};

struct Mat4x4 {
	float m[4][4];

	Mat4x4() : m{ 0 } {}

	// 단위 행렬 만드는 함수
	void makeIdentity() {
		// 0으로 채워진 새 객체를 나 자신에게 덮어씌움으로써 배열에 들어있을 수 있는 쓰레기 값을 모두 0으로 초기화
		*this = Mat4x4();
		for (int i = 0; i < 4; i++) {
			m[i][i] = 1;
		}
	}

	// 행렬과 벡터를 곱하는 함수
	Vec3 multiplyVector(const Vec3& other) const {
		float x = other.x * m[0][0] + other.y * m[1][0] + other.z * m[2][0] + 1 * m[3][0];
		float y = other.x * m[0][1] + other.y * m[1][1] + other.z * m[2][1] + 1 * m[3][1];
		float z = other.x * m[0][2] + other.y * m[1][2] + other.z * m[2][2] + 1 * m[3][2];
		return Vec3(x, y, z);
	}
};