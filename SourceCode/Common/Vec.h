#pragma once

#ifndef VEC_H
#define VEC_H

#include<DirectXMath.h>

template<typename T>
struct Vec2
{
	T x = 0;
	T y = 0;

	Vec2() {};
	Vec2(T X, T Y) :x(X), y(Y) {};
	float Length()const {
		return sqrt(pow(x, 2) + pow(y, 2));
	};
	float Distance(const Vec2& To)const {
		return sqrt(pow(To.x - x, 2) + pow(To.y - y, 2));
	};
	Vec2<float> GetNormal()const {
		float len = Length();
		if (len == 0.0f)return Vec2<float>(0.0f, 0.0f);
		return Vec2<float>(x / len, y / len);
	};
	void Normalize() {
		float len = Length();
		x /= len;
		y /= len;
	};
	DirectX::XMFLOAT2 ConvertXMFLOAT2() {
		return DirectX::XMFLOAT2(x, y);
	};
	Vec2<int>Int() { return Vec2<int>(x, y); }
	Vec2<float>Float() { return Vec2<float>((float)x, (float)y); }

#pragma region オペレーター演算子
	Vec2 operator-() const {
		return Vec2(-x, -y);
	}
	Vec2 operator+(const Vec2& rhs) const {
		return Vec2(x + rhs.x, y + rhs.y);
	};
	Vec2 operator-(const Vec2& rhs)const {
		return Vec2(x - rhs.x, y - rhs.y);
	};
	Vec2 operator*(const Vec2& rhs)const {
		return Vec2(x * rhs.x, y * rhs.y);
	};
	Vec2 operator*(const float rhs)const {
		return Vec2(x * rhs, y * rhs);
	};
	Vec2 operator/(const Vec2& rhs)const {
		return Vec2(x / rhs.x, y / rhs.y);
	};
	Vec2 operator/(const float rhs)const {
		return Vec2(x / rhs, y / rhs);
	};
	Vec2 operator%(const Vec2& rhs) const {
		return Vec2(fmodf(x, rhs.x), fmodf(y, rhs.y));
	};
	void operator=(const Vec2& rhs) {
		x = rhs.x;
		y = rhs.y;
	};
	bool operator==(const Vec2& rhs)const {
		return (x == rhs.x && y == rhs.y);
	};
	bool operator!=(const Vec2& rhs)const {
		return !(*this == rhs);
	};
	void operator+=(const Vec2& rhs) {
		x += rhs.x;
		y += rhs.y;
	};
	void operator-=(const Vec2& rhs) {
		x -= rhs.x;
		y -= rhs.y;
	};
	void operator*=(const Vec2& rhs) {
		x *= rhs.x;
		y *= rhs.y;
	};
	void operator/=(const Vec2& rhs) {
		x /= rhs.x;
		y /= rhs.y;
	};
	void operator%=(const Vec2& rhs) {
		x = fmodf(x, rhs.x);
		y = fmodf(y, rhs.y);
	};
#pragma endregion
};

template<typename T>
struct Vec3
{
	T x = 0;
	T y = 0;
	T z = 0;

	Vec3() {};
	Vec3(T X, T Y, T Z) :x(X), y(Y), z(Z) {};
	float Length()const {
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	};
	float Distance(const Vec3& To)const {
		return sqrt(pow(To.x - x, 2) + pow(To.y - y, 2) + pow(To.z - z, 2));
	};
	Vec3 GetNormal()const {
		float len = Length();
		if (len == 0.0f)return Vec3(0, 0, 0);
		return Vec3(x / len, y / len, z / len);
	};
	void Normalize() {
		float len = Length();
		x /= len;
		y /= len;
		z /= len;
	};
	DirectX::XMFLOAT3 ConvertXMFLOAT3() {
		return DirectX::XMFLOAT3(x, y, z);
	};
	//角度


#pragma region オペレーター演算子
	Vec3 operator-()const {
		return Vec3(-x, -y, -z);
	}
	Vec3 operator+(const Vec3& rhs)const {
		return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
	};
	Vec3 operator-(const Vec3& rhs)const {
		return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
	};
	Vec3 operator*(const Vec3& rhs) const {
		return Vec3(x * rhs.x, y * rhs.y, z * rhs.z);
	};
	Vec3 operator*(const float rhs)const {
		return Vec3(x * rhs, y * rhs, z * rhs);
	};
	Vec3 operator/(const Vec3& rhs)const {
		return Vec3(x / rhs.x, y / rhs.y, z / rhs.z);
	};
	Vec3 operator/(const float rhs)const {
		return Vec3(x / rhs, y / rhs, z / rhs);
	};
	Vec3 operator%(const Vec3& rhs)const {
		return Vec3(fmodf(x, rhs.x), fmodf(y, rhs.y), fmodf(z, rhs.z));
	};
	void operator=(const Vec3& rhs) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	};
	bool operator==(const Vec3& rhs)const {
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	};
	bool operator!=(const Vec3& rhs)const {
		return !(*this == rhs);
	};
	void operator+=(const Vec3& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	};
	void operator+=(const float rhs) {
		x += rhs;
		y += rhs;
		z += rhs;
	};
	void operator-=(const Vec3& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
	};
	void operator-=(const float rhs) {
		x -= rhs;
		y -= rhs;
		z -= rhs;
	};
	void operator*=(const Vec3& rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
	};
	void operator*=(const float rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
	};
	void operator/=(const Vec3& rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
	};
	void operator/=(const float rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
	};
	void operator%=(const Vec3& rhs) {
		x = fmodf(x, rhs.x);
		y = fmodf(y, rhs.y);
		z = fmodf(z, rhs.z);
	};
#pragma endregion
};

static float Dot(const Vec3<float>& a, const Vec3<float>& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static Vec3<float>Cross(const Vec3<float>&a, const Vec3<float>&b)
{
	return Vec3<float>(
		a.y * b.z - b.y * a.z,
		a.z * b.x - b.z * a.x,
		a.x * b.y - b.x * a.y);
}
template<typename T>
struct Vec4
{
	T x = 0;
	T y = 0;
	T z = 0;
	T w = 0;

	Vec4() {}
	Vec4(T X, T Y, T Z, T W) :x(X), y(Y), z(Z), w(W) {}

	DirectX::XMFLOAT4 ConvertXMFLOAT4() {
		return DirectX::XMFLOAT3(x, y, z, w);
	}

#pragma region オペレーター演算子
	Vec4 operator-() {
		return Vec4(-x, -y, -z, -w);
	}
	Vec4 operator+(const Vec4& rhs) {
		return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}
	Vec4 operator-(const Vec4& rhs) {
		return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}
	Vec4 operator*(const Vec4& rhs) {
		return Vec4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
	}
	Vec4 operator*(const float rhs) {
		return Vec4(x * rhs, y * rhs, z * rhs, w * rhs);
	}
	Vec4 operator/(const Vec4& rhs) {
		return Vec4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
	}
	Vec4 operator%(const Vec4& rhs) {
		return Vec4(fmodf(x, rhs.x), fmodf(y, rhs.y), fmodf(z, rhs.z), fmodf(w, rhs.w));
	}
	void operator=(const Vec4& rhs) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
	}
	bool operator==(const Vec4& rhs) {
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
	}
	bool operator!=(const Vec4& rhs)const {
		return !(*this == rhs);
	};
	void operator+=(const Vec4& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
	}
	void operator-=(const Vec4& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
	}
	void operator*=(const Vec4& rhs) {
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
	}
	void operator/=(const Vec4& rhs) {
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
	}
	void operator%=(const Vec4& rhs) {
		x = fmodf(x, rhs.x);
		y = fmodf(y, rhs.y);
		z = fmodf(z, rhs.z);
		w = fmodf(w, rhs.w);
	}
	T& operator[](int idx) {
		return *(&x + idx);
	}
#pragma endregion
};

#endif
