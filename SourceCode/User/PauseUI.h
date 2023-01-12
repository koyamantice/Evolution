#pragma once
#include"Sprite.h"
#include <memory>

class PauseUI {
private:
	// DirectX::‚ðÈ—ª
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

	const bool& GetEndFlag() { return endflag; }
	void SetEndFlag(const bool& endflag) { this->endflag = endflag; }
	const int& GetBar() { return nowBar; }
private:
	void FirstOpen();
	void MoveSelect();
	void OptionSystem();

	enum {
		Sheet,
		kPause,
		TitleBack,
		Option,
		ZBack,
		Bar,







		Max
	};
	std::unique_ptr<Sprite> UI[Max];

	XMFLOAT2 move{};

	bool endflag = false;


	bool option_system = false;
	float SetPos = 250;
	int nowBar = 0;
	bool ease = false;
	float frame = 0;
	XMFLOAT2 pos={640,400};
	XMFLOAT2 size[Max]{  };
};
