#pragma once
#include "DirectXCommon.h"
#include "SceneManager.h"
#include "WinApp.h"
#include "Input.h"
#include "AudioManager.h"
#include "Object3d.h"
#include "Sprite.h"
#include "AbstractSceneFactory.h"
#include "Object2d.h"
#include "FPSManager.h"
#include "ModelManager.h"
#include "XorShift.h"
#include "ImageManager.h"
#include "PostEffect.h"

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

	void SetEnd(bool endResquest) { this->endResquest_ = endResquest; }
protected:
	bool endResquest_ = false;
	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;

	Input* input_ = Input::GetInstance();
	FPSManager* fpsManager_ = FPSManager::GetInstance();
	XorShift* xorShift_ = XorShift::GetInstance();
	ImageManager* imageManager_ = ImageManager::GetIns();
	SceneManager* sceneManager_ = nullptr;
};