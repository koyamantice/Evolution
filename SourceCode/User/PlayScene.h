#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include"PauseUI.h"
#include <SourceCode/FrameWork/collision/TouchableObject.h>

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
private:
	/// <summary>
	///	カメラ周り
	/// </summary>
	void CameraUpda();
	float angle = 0;
	const float PI=3.1452f;
	XMFLOAT2 distance={0,10};
	XMFLOAT2 dis = { 0,15 };
private:
	//メンバ変数
	enum {
		MPlayer,
		MEnemy,
		Chr_Max
	};

	std::unique_ptr<Actor> actor[Chr_Max]{};
	std::unique_ptr<TouchableObject> skydome{};
	std::unique_ptr<TouchableObject> ground{};
	std::unique_ptr<PauseUI> pauseUi{};
	



};

