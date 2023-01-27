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
	void HoneyUpdate();
private:
	enum {
		kLeftNest = 0,
		kMiddleNest,
		kRightNest,
		kMaxNestNum
	};

	Actor* honey_[kMaxNestNum] = {};

	std::unique_ptr<Sprite> Clear{};
	std::unique_ptr<Sprite> Over{};

	PostEffect* postEffect = nullptr;
};

