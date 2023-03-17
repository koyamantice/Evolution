#pragma once
#include <list>
#include <memory>
#include <Sprite.h>
#include <DirectXMath.h>

class ShineRain {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
public:
	ShineRain();
	~ShineRain();
	void Init();

	void Update();

	void Draw();
	struct Shine{
		std::unique_ptr<Sprite> sprite=nullptr;
	};


private:
	std::list<std::unique_ptr<Sprite>> shines = {};
	float vel = 0;
	XMFLOAT2 size = {};



};

