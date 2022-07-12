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
	//メンバ変数
	enum {
		MPlayer,
		MEnemy,
		Chr_Max
	};
	std::unique_ptr<Actor> actor[Chr_Max]{};
	std::unique_ptr<Object3d> skydome{};
	std::unique_ptr<TouchableObject> ground{};
	std::unique_ptr<PauseUI> pauseUi{};
	int SelectNumber = 0;
};

