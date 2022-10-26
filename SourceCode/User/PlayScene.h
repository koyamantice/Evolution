#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include "PauseUI.h"
#include <SourceCode/FrameWork/collision/TouchableObject.h>
#include "Gauge.h"
#include <PostEffect.h>
#include <SourceCode/Obj/2d/MiniMap.h>

/// <summary>
/// タイトルシーン
/// </summary>

class PlayScene : public BaseScene {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update(DirectXCommon* dxCommon) override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	void ResetCamera();
private:
	/// <summary>
	///	カメラ周り
	/// </summary>
	void CameraUpda();
	float angle = 0;
	float firstangle = 0;	
	float endangle = 0;

	XMFLOAT2 firstdis{};
	float angleframe = 0;
	bool Reset = false;
	const float PI=3.1452f;
	XMFLOAT2 distance={0,0};
	XMFLOAT2 dis = { 16,7 };
	bool clear = false;
	float Cframe = 0.0f;
	XMFLOAT2 clearPos = { 0,-720 };
private:
	Actor* player_shadow = nullptr;
	Actor* enemy_shadow = nullptr;
	Actor* crystal_shadow = nullptr;
	ActorComponent* PlayerComp = nullptr;
	std::unique_ptr<Sprite> Clear{};
	std::unique_ptr<Sprite>	Vignette{};
	std::unique_ptr<Gauge>	Demo;
	std::unique_ptr<Object3d> skydome{};
	std::unique_ptr<TouchableObject> ground{};
	std::unique_ptr<PauseUI> pauseUi{};	
	PostEffect* postEffect = nullptr;
	MiniMap* miniMap = nullptr;
};

