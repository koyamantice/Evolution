#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include"TitleText.h"
#include <thread>
#include "SceneChanger.h"

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

	void Heavy();


	void CameraUpdate();
private:
	//メンバ変数
	enum {
		BackScene,
		kkskydome,
		Button,
		moveTitle,
		nomoveTitle,
		moveDebug,
		nomoveDebug,
		SpriteMax,
	};
	std::unique_ptr<Sprite> UI[SpriteMax];
	TitleText* text;
	enum Portal {
		Title=0,
		Debug,

	};

	int nextScene = Portal::Title;

	XMFLOAT2 SetPos[3]={ { 640.0f,300.0f } ,{ 640.0f,360.0f } ,{ 640.0f,460.0f } };
	
	float moveText[2]{};
	int moveTitleNow = 1;
	int moveDebugNow = 2;

	std::string SceneName = "null";
	enum {
		UnLoad = 0,
		Loading,
		LoadFinish
	};
	static int LoadState;
	//static 画面 = ゲーム画面;


	float circ_x=80;
	float circ_z=80;
	float circ_vec_x = 1;
	float circ_vec_z = 1;


	float rad_frame = 0;
	float rad = 0;
	float c_x=80;
	float c_y=50;
	float c_z= 0;

	std::list<float> eyes{};

	XMFLOAT3 e_camera = {80,0,80};
	XMFLOAT3 s_camera = {50,0,80};
};

