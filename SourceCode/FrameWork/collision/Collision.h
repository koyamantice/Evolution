#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "CollisionPrimitive.h"
class Collision {
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	//当たり判定関数
	//円と円
	static bool CircleCollision(const float X1, const float Y1, const float R1, const float X2, const float Y2, const float R2);
	//球と球
	static bool SphereCollision(const float X1, const float Y1, const float Z1, const float R1, const float X2, const float Y2, const float Z2,const float R2);
	//球と球
	static bool SphereCollision2(const XMFLOAT3& pos, const float R1, const XMFLOAT3& pos2, const float R2);
	//球と球2
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
	//平面と球
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, XMVECTOR* inter = nullptr);
	//点と三角形
	static void ClodestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest);
	//球と三角形
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
	//レイと平面
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, XMVECTOR* inter = nullptr);
	//レイと三角形
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, XMVECTOR* inter = nullptr);
	//レイと球
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, XMVECTOR* inter = nullptr);
};