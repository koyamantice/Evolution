#pragma once
#include"Sprite.h"
#include <memory>
#include <Input.h>
#include <SceneChanger.h>
#include <array>

class PauseUI {
private:
	// DirectX::Çè»ó™
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

	enum class CameraSystem :int {
		REVERSE = -1,
		NOREVERSE = 1
	};
	const CameraSystem& GetReverseCamera() { return reverse_camera; }

	const bool& GetEndFlag() { return endflag; }
	void SetEndFlag(const bool& endflag) { this->endflag = endflag; }
	const int& GetBar() { return static_cast<int>(frameBar_); }
private:

	//ä÷êîÉ|ÉCÉìÉ^
	static void(PauseUI::* systemFuncTable[])();

	void OpenOptionMenu();
	void SelectOption();
	void OpenCheck2Title();
	void FinalCheck2Title();
	void OpenChangeManual();
	void ChangeManualSystem();
	void CloseOptionMenu();

	enum class SystemConfig : int {
	kOpenOptionMenu=0,
	kSelectOption,
	kOpenCheck2Title,
	kFinalCheck2Title,
	kOpenChangeManual,
	kChangeManualSystem,
	kCloseOptionMenu,
	};

	SystemConfig state_=SystemConfig::kOpenOptionMenu;

	float open_frame_ = 0;

	enum {
		Sheet,
		kPause,
		TitleBack,
		ZBack,
		Option,
		Bar,

		CameraOpt,
		Normal,
		Reverse,
		OptBack,
		CameraBar,

		kOptionConfig,
		kTitleOk,
		kTitleCancel,
		ConfigBar,

		Max
	};
	std::array<std::unique_ptr<Sprite>,Max> UI;
	
	std::array<XMFLOAT2, Max> sizes{};
	std::array<XMFLOAT2, Max> basesizes{};

	
	std::unique_ptr<SceneChanger> sceneChanger_ = nullptr;

	XMFLOAT2 move{};

	bool endflag = false;


	bool option_system = false;

	XMFLOAT2 bar_pos = {};
	static CameraSystem reverse_camera;

	float frame = 0;
	XMFLOAT2 pos={640,400};

	const XMFLOAT2 basePos = { 640,400 };
	


	enum class SelectFrame :int {
		Back2Title = 0,
		ChangeManualSystem,
		CloseOptionMenu,
		SelectFrameMax,
	};
	std::array<float, static_cast<int>(SelectFrame::SelectFrameMax)> bar_pos_ = 
	{ 330, 450 ,570 };

	SelectFrame frameBar_ = SelectFrame::Back2Title;

	enum class ManualFrame :int {
		NOREVERSEBUTTON=0,
		REVERSEBUTTON,
		BACKSELECT,
		ManualFrameMax
	};

	ManualFrame manualBar_ = ManualFrame::NOREVERSEBUTTON;
	std::array<XMFLOAT2, static_cast<int>(ManualFrame::ManualFrameMax)> manual_pos_ =
	{ XMFLOAT2(pos.x - 150,pos.y - 80),XMFLOAT2(pos.x + 150,pos.y - 80) , XMFLOAT2(pos.x,pos.y + 180) };

	enum class FinalCheckFrame :int {
		BACKTITLE=0,
		CANCELSELECT,
		FinalCheckFrameMax
	};

	FinalCheckFrame finalCheckBar_ = FinalCheckFrame::CANCELSELECT;
	std::array<XMFLOAT2, static_cast<int>(FinalCheckFrame::FinalCheckFrameMax)> final_pos_ =
	{ XMFLOAT2(pos.x - 150,pos.y +180),XMFLOAT2(pos.x + 150,pos.y +180) };

};
