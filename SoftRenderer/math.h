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
	float Length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	// 벡터를 정규화하는 함수
	void Normalize() {
		float norm = Length();
		if (norm != 0) {
			x /= norm;
			y /= norm;
			z /= norm;
		}
	}

	// 내적 계산 함수
	float Dot(const Vec3& other) const {
		return (x * other.x + y * other.y + z * other.z);
	}

	// 외적 계산 함수
	Vec3 Cross(const Vec3& other) const {
		return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
};

struct Mat4x4 {
	float m[4][4];

	Mat4x4() : m{ 0 } {}

	// 단위 행렬 만드는 함수
	void MakeIdentity() {
		// 0으로 채워진 새 객체를 나 자신에게 덮어씌움으로써 배열에 들어있을 수 있는 쓰레기 값을 모두 0으로 초기화
		*this = Mat4x4();
		for (int i = 0; i < 4; i++) {
			m[i][i] = 1;
		}
	}

	// 기존 벡터에 행렬을 곱해서 새로운 위치의 벡터를 생성하는 함수
	Vec3 MultiplyVector(const Vec3& other) const {
		float x = other.x * m[0][0] + other.y * m[1][0] + other.z * m[2][0] + 1 * m[3][0];
		float y = other.x * m[0][1] + other.y * m[1][1] + other.z * m[2][1] + 1 * m[3][1];
		float z = other.x * m[0][2] + other.y * m[1][2] + other.z * m[2][2] + 1 * m[3][2];
		return Vec3(x, y, z);
	}

	// X축 회전 함수
	static Mat4x4 MakeRotationX(float radian) {
		Mat4x4 matrix;
		matrix.MakeIdentity();
		matrix.m[1][1] = std::cos(radian);
		matrix.m[2][2] = std::cos(radian);
		matrix.m[1][2] = std::sin(radian);
		matrix.m[2][1] = -std::sin(radian);

		return matrix;
	}

	// Y축 회전 함수
	static Mat4x4 MakeRotationY(float radian) {
		Mat4x4 matrix;
		matrix.MakeIdentity();
		matrix.m[0][0] = std::cos(radian);
		matrix.m[2][2] = std::cos(radian);
		matrix.m[0][2] = -std::sin(radian);
		matrix.m[2][0] = std::sin(radian);

		return matrix;
	}

	// Z축 회전 함수
	static Mat4x4 MakeRotationZ(float radian) {
		Mat4x4 matrix;
		matrix.MakeIdentity();
		matrix.m[0][0] = std::cos(radian);
		matrix.m[1][1] = std::cos(radian);
		matrix.m[0][1] = std::sin(radian);
		matrix.m[1][0] = -std::sin(radian);

		return matrix;
	}

	// 크기 조절 함수
	static Mat4x4 MakeScale(float x, float y, float z) {
		Mat4x4 matrix;
		matrix.MakeIdentity();
		matrix.m[0][0] = x;
		matrix.m[1][1] = y;
		matrix.m[2][2] = z;

		return matrix;
	}

	// 이동 함수
	static Mat4x4 MakeTranslation(float x, float y, float z) {
		Mat4x4 matrix;
		matrix.MakeIdentity();
		matrix.m[3][0] = x;
		matrix.m[3][1] = y;
		matrix.m[3][2] = z;

		return matrix;
	}
};