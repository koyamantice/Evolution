#pragma once
#include"Sprite.h"
#include <memory>
#include <Input.h>
#include <SceneChanger.h>

class PauseUI {
private:
	// DirectX::‚ðÈ—ª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	Input* input = Input::GetInstance();
public:
	PauseUI();
	~PauseUI();

	void Initialize();
	void Update();
	void Draw();
	void Reset();

	
	const bool& GetReverseCamera() { return reverse_camera; }

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


		CameraOpt,
		Normal,
		Reverse,
		OptBack,
		CameraBar,




		Max
	};
	std::unique_ptr<Sprite> UI[Max];
	std::unique_ptr<SceneChanger> scene_changer = nullptr;

	XMFLOAT2 move{};

	bool endflag = false;


	bool option_system = false;
	float SetPos = 250;

	XMFLOAT2 bar_pos = {};
	int nowBar = 0;
	int cameraNow = 0;		
	static bool reverse_camera;
	bool ease = true;

	float frame = 0;
	XMFLOAT2 pos={640,400};
	XMFLOAT2 size[Max]{  };
};
