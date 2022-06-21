#pragma once
#include "DirectXCommon.h"
#include"SceneManager.h"
#include "WinApp.h"
#include "Input.h"
#include "AudioManager.h"
#include "Object3d.h"
#include "Sprite.h"
#include "AbstractSceneFactory.h"
#include "Texture.h"
#include"FPSManager.h"
#include"ModelManager.h"
#include "XorShift.h"
#include "ImageManager.h"

/// <summary>
/// フレームワーク
/// </summary>
class Framework {
public:
	/// <summary>
/// 実行
/// </summary>
	void Run();

	/// <summary>
/// 初期化
/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon);
	/// <summary>
/// 終了
/// </summary>
	virtual void Finalize();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(DirectXCommon* dxCommon);

	/// <summary>
/// 描画
/// </summary>
	virtual void Draw(DirectXCommon* dxCommon);//=0でこのクラスは継承のみ

	bool IsEndRequst() { return endResquest_; }

	void SetEnd(bool endResquest_) { this->endResquest_ = endResquest_; }
protected:
	bool endResquest_ = false;
	WinApp* winApp = nullptr;
	DirectXCommon* dxcommon = nullptr;

	Input* input = nullptr;
	AudioManager* audio = nullptr;
	FPSManager* fps = nullptr;
	XorShift* xorShift=nullptr;
	//DebugText* debugText = nullptr;
	AbstractSceneFactory* sceneFactory_ = nullptr;
	ImageManager* imageManager = nullptr;
	SceneManager* scaneMana = nullptr;
};