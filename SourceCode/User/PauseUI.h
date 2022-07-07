#pragma once
#include"Sprite.h"
#include <memory>

class PauseUI {
private:
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	PauseUI();
	~PauseUI();

	void Initialize();
	void Update();
	void Draw();
	void Reset();
private:
	enum {
		Sheet,
		Max
	};
	std::unique_ptr<Sprite> UI[Max];
	bool ease = false;
	float frame = 0;
	XMFLOAT2 pos={640,-360};
};
