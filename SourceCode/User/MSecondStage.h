#pragma once
#include "SceneManager.h"

/// <summary>
/// セカンドステージに向かう際のチュートリアル
/// </summary>
class MSecondStage : public  BattleScene {
private:
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
	///	導入部分の更新処理
	/// </summary>
	bool IntroUpdate()override;

	//クリア後カメラ
	void ResultCamera(int Timer);
	/// <summary>
	/// クリア更新処理
	/// </summary>
	bool ClearUpdate() override;
	/// <summary>
	/// ハチミツの処理
	/// </summary>
	void HoneyUpdate();

	void DrawLocal() override;
private:
	enum {
		kLeftNest = 0,
		kMiddleNest,
		kRightNest,
		kMaxNestNum
	};

	Actor* honey_[kMaxNestNum] = {};
	
	//ミッション
	std::unique_ptr<Sprite> mission_{};


	enum {
		kHoneyNumMax=4,
	};
	int nowOpenHoney = 0;
	//蜂蜜ゲット
	std::unique_ptr<Sprite>	honey_get_[kHoneyNumMax]{};

	PostEffect* postEffect = nullptr;
};

