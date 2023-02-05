#pragma once
#include "Object3d.h"
#include "ParticleManager.h"


class Touch {
private:
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	enum FireColor{
		f_green = 0,
		f_blue,
	};
public:
	Touch(const XMFLOAT3& pos = { 0,0,0 }, const XMFLOAT3& rot = { 0,0,0 });
	~Touch();

	void Initialize(const XMFLOAT3& pos = {0,0,0}, const XMFLOAT3& rot = { 0,0,0 });
	
	void SetColor(const int color=0);

	void Update();

	void Draw();
private:
	std::unique_ptr<Object3d> touch_obj;

	std::unique_ptr<ParticleManager> fire;

	XMFLOAT4 s_color={1,1,1,1};
	XMFLOAT4 e_color={1,1,1,1};

	void FireAdd();
};