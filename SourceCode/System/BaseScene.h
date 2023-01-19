#pragma once
class SceneManager;
#include "FPSManager.h"
#include "input.h"
#include "DirectXCommon.h"

#include "DebugCamera.h"
#include "LightGroup.h"

#include<string>
#include <fstream>
#include <iostream>

#include <memory>
#include <DirectXMath.h>

#include "FBXObject3d.h"
#include "Object3d.h"
#include "Sprite.h"

#include "ModelManager.h"
#include "ImageManager.h"
#include "AudioManager.h"

#include <SourceCode/Common/Easing.h>
#include <SourceCode/User/PauseUI.h>
#include <SceneChanger.h>
#include <ParticleEmitter.h>

/// <summary>
/// 基底シーン
/// </summary>
class BaseScene {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
protected:
	LightGroup* lightGroup = nullptr;
	DebugCamera* camera = { nullptr };


	bool pause = false;
	float frame = 0.0f;

	bool scene_first_change = true;
	bool intro_skip = false;
	bool skip_frame = 0.0f;
	std::unique_ptr<SceneChanger> scene_changer = nullptr;
	std::unique_ptr <ParticleEmitter> particleEmitter = nullptr;

public:
	virtual ~BaseScene()=default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon) = 0;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update(DirectXCommon* dxCommon) = 0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon) = 0;

	/// <summary>
	/// 共通の初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	void InitCommon(DirectXCommon* dxCommon);

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize()=0;
};

