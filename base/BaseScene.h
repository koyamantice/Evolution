#pragma once

class SceneManager;
#include "DirectXCommon.h"
#include "DebugCamera.h"
#include "LightGroup.h"
#include<memory>
#include "Sprite.h"
#include "FBXObject3d.h"
#include "Object3d.h"
#include "Texture.h"
#include <DirectXMath.h>
#include "input.h"
#include "FPSManager.h"
#include<memory>

/// <summary>
/// シーン規定
/// </summary>
class BaseScene {
public:
	const bool& GetEnd() { return end; }
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
protected:
	Input* input = Input::GetInstance();
	LightGroup* lightGroup = nullptr;
	float ambientColor0[3] = { 1,1,1 };

	// 光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };
	int EffectNum = 0;
	float distanceY = 12;
	float distanceZ = 12;

	//始まる前の演出のための変数
	bool bossstart = false;
	int appearanceNumber = 0;
	int appearanceTimer = 0;
	XMFLOAT3 cameraPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 cameratargetPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Aftereyepos;
	XMFLOAT3 Aftertargetpos;
	float frame = 0.0f;
	int Interval = 0;
	bool end = false;
	int EndNumber = 0;
	int EndTimer = 0;
	bool gameover = false;
	int overNumber = 0;
	int overTimer = 0;
	float overframe = 0.0f;
	float nameframe = 0.0f;
	float clearframe = 0.0f;
	//スキップのための演出
	bool Skip = false;
	int SkipTimer = 0;
	//スプライトやテクスチャ
	Sprite* WhiteFilter = nullptr;
	Sprite* BlackFilter = nullptr;
	Sprite* bossName = nullptr;
	Sprite* SkipSprite = nullptr;
	Sprite* GameOverSprite = nullptr;
	Sprite* GameClearSprite = nullptr;
	XMFLOAT4 WhiteColor = { 1.0f,1.0f,1.0f,0.0f };
	XMFLOAT4 BlackColor = { 0.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 GameOverColor = { 1.0f,1.0f,1.0f,0.0f };
	XMFLOAT2 Skippos = { 640.0f,360.0f, };
	XMFLOAT2 namePos = { 0.0f,0.0f };
	XMFLOAT2 overPos = { 240.0f,140.0f };
	XMFLOAT2 clearSize = { 0.0f, 0.0f };
public:

	virtual ~BaseScene()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(DirectXCommon* dxCommon) =0;
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize()=0;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update(DirectXCommon* dxCommon)=0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon)=0;

};

