#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include"TitleText.h"
#include <thread>
#include "SceneChanger.h"
#include "BossLevelLoader.h"
#include "Singleton.h"

/// <summary>
/// タイトルシーン
/// </summary>

class TitleScene : public BaseScene {
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
private:

	/// <summary>
	/// テキスト関連更新処理
	/// </summary>
	void SelectTextUpdate();
	
	/// <summary>
	/// 入力関連更新処理
	/// </summary>
	void InputUpdate();
	/// <summary>
	/// カメラ更新処理
	/// </summary>
	void CameraUpdate();
private:
	//メンバ変数
	enum {
		BackScene,
		moveDebug,
		nomoveDebug,
		moveTitle,
		nomoveTitle,
		SpriteMax,
	};
	std::unique_ptr<Sprite> UI[SpriteMax];
	
	std::unique_ptr <TitleText> text;
	BossLevelLoader* bossLevelLoader_;

	const XMFLOAT2 kWord2DebugPos = { 930.0f,500.0f };
	const XMFLOAT2 kWord2TitlegPos = { 930.0f,360.0f };

	const XMFLOAT4 kVisivleColor = { 1,1,1,1 };
	const XMFLOAT4 kInVisivleColor = { 1,1,1,0 };


	enum Portal {
		Title=0,
		Debug,

	};

	int nextScene = Portal::Title;



	float sin_frame_ = 0.0f;
	float sin_add_ = 0.1f;



	std::string SceneName = "null";

	float circ_x=80;
	float circ_z=80;
	float circ_vec_x = 1;
	float circ_vec_z = 1;


	float rad_frame = 0;
	const float kRoundFrameMax = 1200;
	float rad = 0;
	float c_x=80;
	float c_y=50;
	float c_z= 0;

	std::list<float> eyes{};

	XMFLOAT3 s_camera = { 50,0,50 };
	XMFLOAT3 e_camera = {80,0,80};
};

